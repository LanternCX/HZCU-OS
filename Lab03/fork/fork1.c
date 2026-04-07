#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	int pid;

	pid = fork();
	printf("pid= %d\n", pid);
	//printf("pid= %d\n", getpid());
	//printf("process pid= %d return : %d\n", getpid(), pid);
	sleep(1);
	return 0;
}
