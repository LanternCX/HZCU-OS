#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	int i;
	for (i = 0; i < 2; i++)
	{
		fork();
		printf("$\n");
		//printf("$");
	}
	//wait(0);
	//wait(0);
	return 0;
}
