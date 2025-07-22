#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	// takes the output of the first process and makes it the input of the second process
	if (argc == 1) { // for zero arguments, exit with einval
		errno = EINVAL;


	}
}
