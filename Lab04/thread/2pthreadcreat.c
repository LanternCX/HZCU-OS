//"hello world"���̳߳���
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *p_msg(void *m);

int main(void)
{
	pthread_t t1 , t2;
	
	pthread_create(&t1,NULL,p_msg,(void * )"hello ");
	pthread_create(&t2,NULL,p_msg,(void * )"world\n");
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return 0;
}

void *p_msg(void *m)
{
	int i;

	for(i = 0 ; i < 5 ; i ++)
	{
		printf("%s", (char *)m);
		fflush(stdout);//����ļ����������ļ���д��ʽ��ʱ������������д���ļ� 
		sleep(1);
	}
	return NULL;
}
