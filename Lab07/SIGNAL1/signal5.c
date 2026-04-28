#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
static int alarm_fired = 0;            //闹钟未设置
//模拟闹钟
void ding(int sig)
{
	alarm_fired = 1;               //设置闹钟
}

int main()
{
	int pid;

	printf("alarm application starting\n");
	fflush(stdout);
	if((pid = fork( )) == 0) 
	{     //子进程5秒后发送信号SIGALRM给父进程
		sleep(3);
		kill(getppid(), SIGALRM);
		exit(0);
	}
//父进程安排好捕捉到SIGALRM信号后执行ding函数
	printf("waiting for alarm to go off\n");
	fflush(stdout);
	(void) signal(SIGALRM, ding); 
	pause();           //挂起父进程，直到有一个信号出现
	//wait(0);
	if (alarm_fired)
		printf("Ding!\n");
	printf("done\n");
}
