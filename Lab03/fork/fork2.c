#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	char *message;
	int n;

	printf("fork program starting\n");
	pid = fork();
	switch (pid) {
		case -1:
			perror("fork failed");
			return 0;
		case 0:
			message = "This is the child";
			n = 5;
			break;
		default:
			message = "This is the parent";
			n = 3;
			break;
	}
	srand((unsigned)time(NULL));
	for (; n > 0; n--) {
		puts(message);
		usleep(rand() % 10 + 1);
	}
	return 0;
}
