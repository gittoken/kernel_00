#include <sys/types.h>

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
	
int main(void)
{
	pid_t pid;

	pid = fork();
	if (pid == (pid_t)-1)
	{
		perror("Failed to work");
		return EXIT_FAILURE;
	}

	if (pid == 0)
	{         /* child code */
		printf("I am child\n");
	}
	else 
	{
		printf("I am parent\n");
		if (waitpid(pid, NULL, 0) == -1)
			perror("waitpid[2]");
	}

    return 0;
}
