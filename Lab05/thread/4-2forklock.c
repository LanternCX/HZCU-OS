#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int gnum = 0;

int main()
{
	pid_t pid;
	int i;
	pid = fork();

	if (pid == 0)
	{
		printf("I am child process, my PID is %d\n",getpid());
	      for( i=0; i<3; i++ ){
		    gnum++; usleep (100); gnum++;
		    printf (" chile process add 2 to num:%d\n",gnum);
		    //usleep(100);
   		 }
  		exit (0);
	}
	else 
	{
		printf("I am parent process, my PID is %d\n",getpid());
        	for( i=0; i<4; i++ )  {
   		    gnum++; usleep (100);gnum++; usleep (100);gnum++;
   		    printf ("parent process add 3 to num:%d\n",gnum);
    		    //usleep(100);
     		}
  		exit (0);
	}
}
