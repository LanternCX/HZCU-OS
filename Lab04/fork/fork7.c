#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	int p1;
	int i;

	while ((p1=fork())==-1);             /*父进程创建第一个进程，直到成功*/
	if(p1==0)                         /*0返回给子进程 1*/
		{
			/*子进程的处理过程*/ 
			for(i=0;i<3;i++)
			{
				printf("son process: pid = %d,ppid = %d\n",getpid(),getppid());
				system("/bin/ps");
				sleep(1);
			}
			exit(0);
		}
	else   
		{                             /*正数返回给父进程(子进程号)*/
			
			for(i=0;i<3;i++)
			{
				printf("parent process: pid = %d,ppid = %d\n",getpid(),getppid());
				system("/bin/ls");
				sleep(1);
			}
			wait(0);
	}
	return 0;
}
