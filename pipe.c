#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// takes the output of the first process and makes it the input of the second process
int main(int argc, char *argv[])
{
	if (argc == 1) { // for zero arguments, exit with einval
		errno = EINVAL;
		exit(errno);
	} else if (argc == 2) { // for one argument, just execute the first command
		errno = execlp(argv[1], argv[1], NULL);
		if (errno == -1) {
			exit(errno);
		}
		return 0;
	}
}
