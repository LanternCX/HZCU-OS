# POSIX 信号量

## 实验目的和要求

1. 理解进程与线程的区别；
2. 使用 POSIX 信号量，实现线程的互斥和同步控制。

## 实验内容

1. 使用 P OSIX 信号量，实现线程的互斥和同步控制；
2. 实现读者优先的读者-写者问题；
3. 实现有限缓冲的生产者-消费者问题。

## 实验步骤

### (一) 基本概念

Linux 提供两种信号量：内核信号量（由内核控制路径使用）和用户态进程使用的信号量。用户态信号量又分为 POSIX 信号量和 SYSTEM V 信号量（将在实验九介绍）。POSIX 信号量（semaphore）是一种用于提供不同进程间或一个给定进程的不同线程间同步手段的原语。POSIX 信号量又分为有名信号量和无名信号量。 有名信号量， 其值保存在文件中，用于线程，也可以用于相关或无关进程间的同步；无名信号量， 其值保存在内存中，用于线程或相关进程间的同步。

本次实验主要介绍 POSIX 无名信号量的使用。无名信号量必须是多个进程（线程）的共享变量，无名信号量要保护的变量也必须是多个进程（线程）的共享变量。

### (二) 本实验涉及的系统调用

#### 1. 信号量初始化

`int sem_init(sem_t *sem,int pshared,unsigned int value);`

参数说明：

`sem_t *sem`：  信号量变量；

`int pshared`：  指明信号量的类型。不为 0 时此信号量在相关进程间共享，否则只能为当前进程的所有线程共享；

`unsigned int value`：该参数指定信号量的初始值。

返回值：

成功时返回 0；错误时，返回 -1，并把 errno 设置为合适的值。

#### 2. 信号量的减 1 操作

`int sem_wait(sem_t *sem);`

函数说明：

等待信号量，如果信号量的值大于 0，将信号量的值减 1，立即返回。如果信号量的值为 0，则线程阻塞。相当于 P 操作。

返回值：

成功返回 0，失败返回-1。

#### 3. 信号量的加 1 操作

`int sem_post(sem_t *sem);`

函数说明：

释放信号量，让信号量的值加 1。若此时有 sem_wait 正在阻塞则唤醒。相当于 V 操作。

#### 4. 获取信号量的值，一般只用来调试

`int sem_getvalue(sem_t *sem);`

### (三) 并发线程的互斥控制

#### 1. 线程互斥例题

生产围棋的工人不小心把相等数量的黑子和白子混装于一个箱子里， 现要用自动分拣系统把黑子和白子分开，该系统由两个并发执行的进程组成，功能如下：(1)进程 A 专门拣黑子，进程 B 专门拣白子；(2)每个进程每次只拣一个子，当一个进程在拣子时不允许另一个进程去拣子。试用 PV 操作实现两者的同步。

用 PV 操作解决互斥的方法：

1. 确定临界资源，找到临界区；
2. 设置互斥信号量，初始值为 1；
3. 在临界区前 P 操作，临界区后 V 操作。

```c
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int number; // 被保护的全局变量
sem_t sem_id;

void* thread_white_fun(void *arg)
{
 int i;
 for(i = 0;i < 3;i++)
 {
  sem_wait(&sem_id);
  printf("w%d:thread_white have the semaphore\n",i);
  number++;usleep(100);
  printf("w%d:white number = %d\n",i,number);
  sem_post(&sem_id);
 }
}

void* thread_black_fun(void *arg)
{
 int i;
 for(i = 0;i < 3;i++)
 {
  sem_wait(&sem_id);
  printf("b%d:thread_black have the semaphore \n",i);
  number--;usleep(100);
  printf("b%d:black number = %d\n",i,number);
  sem_post(&sem_id);
 }
}

int main(int argc,char *argv[])
{
 number = 0;
 pthread_t id1, id2;
 sem_init(&sem_id, 0, 1);
 pthread_create(&id1,NULL,thread_white_fun, NULL);
 pthread_create(&id2,NULL,thread_black_fun, NULL);
 pthread_join(id1,NULL);
 pthread_join(id2,NULL);
 printf("main,,,\n");
 return 0;
}
```

编译链接通过后，多次运行例程，观察进程并发执行结果，并思考下述问题：：

1. 了解信号量的初始化操作、P 操作和 V 操作；
2. 注销红色代码，保留 sleep 语句，体会如何使用信号量对共享资源的互斥访问进行管理。

#### 2. 线程同步例题

本例有四个线程。 两个Read 线程从文件中读取数据， 两个Handle 线程处理读取出来的数据。要求先读取数据，后处理数据，两组线程是同步关系。

用 PV 操作解决同步的方法：

1. 确定先后动作；
2. 设置同步信号量，初始值为 0；
3. 先动作后 V 操作，后动作前 P 操作。

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define MAXSTACK 100
int stack[MAXSTACK];
sem_t sem;

//从文件 1.dat 读取数据，每读一次，信号量加一
void ReadData1(void){
 int val;
 FILE *fp=fopen("1.dat","r");
 while(!feof(fp)){
  fscanf(fp,"%d %d",&stack[0],&stack[1]);
  sem_getvalue(&sem,&val);
  printf("ReadData1 stack[0] =%d stack[1]=%d sem.val = %d\n", stack[0], stack[1], val);
  sem_post(&sem);
 }
 fclose(fp);
}

//从文件 2.dat 读取数据
void ReadData2(void){
 int val;
 FILE *fp=fopen("2.dat","r");
 while(!feof(fp)){
  fscanf(fp,"%d %d",&stack[0],&stack[1]);
  sem_getvalue(&sem,&val);
  printf("ReadData2 stack[0] =%d stack[1]=%d sem.val = %d\n", stack[0], stack[1], val);

  sem_post(&sem);
 }
 fclose(fp);
}

//阻塞等待缓冲区有数据，读取数据后，释放空间，继续等待
void HandleData1(void){
 int val;
 while(1){
  sem_wait(&sem);
  sem_getvalue(&sem,&val);
  printf("Plus:%d+%d=%d sem.val = %d\n", stack[0], stack[1], stack[0] + stack[1],val);
  }
}

void HandleData2(void){
 int val;
 while(1){
  sem_wait(&sem);
  sem_getvalue(&sem,&val);
  printf("Multiply:%d*%d=%d sem.val = %d\n",stack[0],stack[1], stack[0]*stack[1], val);
  }
}
int main(void){
 pthread_t t1,t2,t3,t4;
 sem_init(&sem,0,0);
 pthread_create(&t1,NULL,(void *)HandleData1,NULL);
 pthread_create(&t2,NULL,(void *)HandleData2,NULL);
 pthread_create(&t3,NULL,(void *)ReadData1,NULL);
 pthread_create(&t4,NULL,(void *)ReadData2,NULL);
 pthread_join(t1,NULL);
}
```

编译链接通过后，多次运行例程，观察进程并发执行结果，并思考下述问题：：

1. 理解信号量的初始化操作、P 操作和 V 操作；
2. 分析出四个线程的前驱后继关系；
3. 若要求 ReadData1 读出的数据仅由 HandleData1 处理，ReadData2 读出的数据仅由 HandleData2 处理，请修改例程。

#### 3. 捡棋子题中，如果还要求进程 A 和进程 B 交替拣子，则为进程同步问题，修改代码并调试运行。

```c
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int number; // 被保护的全局变量
sem_t sem_id1, sem_id2;

void* thread_white_fun(void *arg)
{
 int i;

 for(i = 0;i < 3;i++)
 {
  sem_wait(&sem_id1);
  printf("thread_white have the semaphore\n");
  number++;
  printf("number = %d\n",number);
  sem_post(&sem_id2);
  //sleep(1);
 }
}
void* thread_black_fun(void *arg)
{
 int i;

 for(i = 0;i < 3;i++)
  {
   sem_wait(&sem_id2);
   printf("thread_black have the semaphore \n");
   number--;
   printf("number = %d\n",number);
   sem_post(&sem_id1);
   //sleep(1);
  }
}
int main(int argc,char *argv[])
{
 number = 0;
 pthread_t id1, id2;
 sem_init(&sem_id1, 0, 1); // 空闲的
 sem_init(&sem_id2, 0, 0); // 忙的
 pthread_create(&id1,NULL,thread_white_fun, NULL);

 pthread_create(&id2,NULL,thread_black_fun, NULL);
 pthread_join(id1,NULL);
 pthread_join(id2,NULL);
 printf("main,,,\n");
 return 0;
}
```

编译链接通过后，多次运行例程，观察进程并发执行结果，并思考下述问题：：

1. 理解信号量的初始化操作、P 操作和 V 操作；
2. 修改信号量的初始值，观察线程同步的顺序变化。

#### 4. 用 Posix 信号量解决“写者优先的读者-写者问题”

使用信号量解决写者优先的读者-写者问题的方法如下：

互斥信号量：wsem =1，x = 1；

共享变量：readcount = 0；

Reader process Writer process

```text
while (true) {
  P(x);
     readcount = readcount + 1;
     if (readcount==1) P(wsem);
  V(x);
  READ file;
  P(x);
     readcount = readcount - 1;
     if (readcount==0) V(wsem);
  V(x);
}
while (true) {
  P(wsem);
    WRITE file;
  V(wsem);
}
```

代码如下：

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>

#define READERCOUNT  5     //读者数
#define WRITERCOUNT  3     //写者数
#define PAPERSIZE  26       //文件长度

static char paper[PAPERSIZE];        // 文件

unsigned short int write_index = 0;    //写者需要写入的位置
char ch = 'A';                      //写者需要写入的字母
sem_t rmutex,wmutex;
int nReader = 0;

void *reader(void *args)    //读者线程
{
    int number =*((int *)args);
    int i;
    for(i = 0;i <10;i++){
        sem_wait(&rmutex);
        if (nReader == 0)                //如果是第一个读者, 则锁定 wmutex
            sem_wait(&wmutex);
        ++ nReader;
        sem_post(&rmutex);

        //开始读
        printf("## reader %d was reading...\n", number);
        printf("text: %s\n", paper);sleep(1);
        printf("   reader %d end reading... \n\n", number);

        sem_wait(&rmutex);
        -- nReader;
        if (nReader == 0)                //如果是最后一个读者, 则解锁 wmutex
            sem_post(&wmutex);
        sem_post(&rmutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *writer(void *args)    //写者线程
{
    int number =*((int *)args);
    int i;
    for(i = 0;i <9;i++){
        sem_wait(&wmutex);    //获取写锁

        printf("++ writer %d was writing...%c\n", number,ch);    //start writing
        paper[write_index] = ch;  sleep(1);
        write_index = (write_index+1)%26;
        ch = ch+1;
        if (ch > 'Z')  ch = 'A';
        printf("   writer %d end writing... \n\n", number);       //end writing

        sem_post(&wmutex);    //释放写锁
        sleep(1);
 }
    pthread_exit(NULL);
}

int main()
{
 int i;
 int rThdNum[READERCOUNT], wThdNum[WRITERCOUNT];
 pthread_t wthread[WRITERCOUNT], rthread[READERCOUNT];

 sem_init(&rmutex, 0, 1);
 sem_init(&wmutex, 0, 1);

 for (i = 0; i < READERCOUNT; i++)
   rThdNum[i] = i;
 for (i = 0; i < WRITERCOUNT; i++)
   wThdNum[i] = i;

   for ( i = 0; i < READERCOUNT; ++i)
       pthread_create(&rthread[i], NULL, reader,(void *)&rThdNum[i]);
   for (i = 0; i <WRITERCOUNT; ++i)
        pthread_create(&wthread[i], NULL, writer,(void *)&wThdNum[i]);

   for (i = 0; i < READERCOUNT; ++i)
        pthread_join(rthread[i], NULL);
 for (i = 0; i < WRITERCOUNT; ++i)
        pthread_join(wthread[i], NULL);
 }
```

编译链接通过后，多次运行例程，观察进程并发执行结果，并思考下述问题：：

1. 理解信号量的初始化操作、P 操作和 V 操作；
2. 观察线程并发，理解读者优先。

#### 5. 用 POSIX 信号量解决生产者/消费者问题

使用信号量解决有限缓冲区生产者/消费者问题的方法如下：

同步信号量：EMPTY=k；FULL = 0；

互斥信号量：MUTEX =1；

生产者 消费者

```text
while (true)
{
produce(x);
P（EMPTY）；
P（MUTEX）；
append(x,Q);
V（MUTEX）；
V（FULL）；
}
while (true)
{
P（FULL）；
P（MUTEX）；
take(x,Q);
V（MUTEX）；
V（EMPTY）；
consume(x);
}
```

代码如下：

```c
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>

#define NBUFF      5      // 槽位的个数

int nitems;  // 条目的个数

int buff[NBUFF];
sem_t mutex,empty,full;

void *produce(void *arg);
void *consume(void *arg);

int main(int argc,char *argv[])
{
 pthread_t   tid_produce,tid_consume;
 if(argc != 2)
 {
  printf("usage: prodcons <#itmes>");
  exit(0);
 }

 nitems = atoi(argv[1]);  // 获取条目数目
 //创建信号量
 sem_init(&mutex,0,1);
 sem_init(&empty,0,NBUFF);
 sem_init(&full,0,0);

 pthread_setconcurrency(2); //设置并发级别
 pthread_create(&tid_produce,NULL,produce,NULL); //生产者线程
 pthread_create(&tid_consume,NULL,consume,NULL); //消费者线程
 pthread_join(tid_produce,NULL);
 pthread_join(tid_consume,NULL);
 exit(0);
}

void *produce(void *arg)
{
 int i;
 printf("\nproduce is called.\n");
 for(i=0;i<nitems;i++)
 {
  sem_wait(&empty); //判断是否有空槽，有的将其减少 1
  sem_wait(&mutex);  //锁住槽位，对于多个生产者的时有必要，单个生产者没必要
  printf("\nproduced a new item %d:",i);
  scanf("%d",&buff[i%NBUFF]);
  sleep(1);
  sem_post(&mutex);  //释放锁
  sem_post(&full);  //缓冲区中条目数加 1
 }
 return NULL;
}

void *consume(void *arg)
{
 int   i;
 printf("\nconsumer is called.\n");
 for(i=0;i<nitems;i++)
 {
  sem_wait(&full);  //判断缓冲区中是否有条目，有的话将条目数减少 1
  sem_wait(&mutex);  //锁住缓冲区，对多个消费者有必要，对单个消费者没必要
  if(buff[i % NBUFF] != i)
  printf("\nremoved %d item %d.",i,buff[i% NBUFF]);
  sleep(1);
  sem_post(&mutex);  //释放锁
  sem_post(&empty); //将缓冲区中的空槽数目加 1

 }
 return NULL;
}
```

编译链接通过后，多次运行例程，观察进程并发执行结果，并思考下述问题：：

1. 理解信号量的初始化操作、P 操作和 V 操作；
2. 观察线程并发，理解生产者消费者在信号量控制下，实现供需平衡。

### (四) 编程题：理解前述例程后，按要求完成程序编写。

1. 运行课后习题 4.9， 可发现输出结果有误。 请使用POSIX 无名信号量，修改代码，保证程序能输出正确结果。
2. 理解课后习题 5.4，编写完整的程序，使用 POSIX 线程实现并发，使用 POSIX 信号量实现并发线程互斥运行。
3. 请结合生产者-消费者例程，修改代码，实现下述变形的生产者/消费者问题的：有四个线程 A、B、C、D。线程 A 通过一个缓冲区不断地向线程 B、C、D 发送信息，A 每向缓冲区送入一个信息后，必须等线程 B、C、D 都取走后才可以发送下一个信息，B、C、D 对 A 送入的每一信息各取一次，试用 P、V 操作实现它们之间的正确通信。
