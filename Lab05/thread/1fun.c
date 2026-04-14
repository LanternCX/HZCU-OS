//"hello world"单线程程序
#include <stdio.h>
#include <unistd.h>

const int NUM = 5;

void p_msg(char *m);

int main(void)
{
	p_msg("hello");
	p_msg("world\n");
	return 0;
}

void p_msg(char *m)
{
	int i;
	for(i = 0 ; i < NUM ; i ++)
	{
		printf("%s",m);
		fflush(stdout);
		sleep(1);
	}
}
