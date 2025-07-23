#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void setup_child(int i, int argc, int prev_pipe[2], int curr_pipe[2], char *argv[]) {
    // Redirect input from prev_pipe if not the first program
    if (i > 1) {
        dup2(prev_pipe[0], 0);
        close(prev_pipe[0]);
    }
    // Redirect output to curr_pipe if not the last program
    if (i < argc - 1) {
        dup2(curr_pipe[1], 1);
        close(curr_pipe[1]);
    }
    // Cleanup pipes
    close(curr_pipe[0]);
    close(curr_pipe[1]);
    if (i > 1) {
        close(prev_pipe[1]);
    }
    // Execute program
    if (execlp(argv[i], argv[i], NULL) == -1) {
        exit(errno);
    }
}

int handle_parent(int curr_pipe[2], int prev_pipe[2], int i, int argc) {
    int status;
    wait(&status);
    if (WEXITSTATUS(status) != 0) {
        return WEXITSTATUS(status);
    }
    close(curr_pipe[1]);

    if (i < argc - 1) {
        prev_pipe[0] = curr_pipe[0];
        prev_pipe[1] = curr_pipe[1];
    } else {
        close(prev_pipe[0]);
        close(prev_pipe[1]);
        close(curr_pipe[0]);
        close(curr_pipe[1]);
    }
    return 0;
}

// takes the output of the first process and makes it the input of the second process
int main(int argc, char *argv[])
{
	if (argc < 2) { // for zero arguments, exit with einval
		errno = EINVAL;
		exit(errno);
	} else if (argc == 2) { // for one argument, just execute the first command
		errno = execlp(argv[1], argv[1], NULL);
		if (errno == -1) {
			exit(errno);
		}
		return 0;
    } else { //2+ args, pipe
        int curr_pipe[2];
        int prev_pipe[2];

        
        pipe(curr_pipe);
        int pid = fork();
        if (pid < 0) {
            exit(errno);
        }
        if (pid == 0) {
            setup_child(1, argc, prev_pipe, curr_pipe, argv);
            return 0;
        } else {
            int result = handle_parent(curr_pipe, prev_pipe, 1, argc);
            if (result != 0) return result;
        }
        

        for (int i = 2; i < argc - 1; i++) {
            pipe(curr_pipe);
            pid = fork();
            if (pid < 0) {
                exit(errno);
            }
            if (pid == 0) {
                setup_child(i, argc, prev_pipe, curr_pipe, argv);
                return 0;
            } else {
                int result = handle_parent(curr_pipe, prev_pipe, i, argc);
                if (result != 0) return result;
            }
        }

        pid = fork();
        if (pid < 0) {
            exit(errno);
        }
        if (pid == 0) {
            setup_child(argc - 1, argc, prev_pipe, curr_pipe, argv);
            return 0;
        } else {
            int result = handle_parent(curr_pipe, prev_pipe, argc - 1, argc);
            if (result != 0) return result;
        }
        }
    return 0;
}