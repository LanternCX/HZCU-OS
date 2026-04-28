#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t parent_interrupted = 0;
volatile sig_atomic_t child1_stopped = 0;
volatile sig_atomic_t child2_stopped = 0;

void parent_stop(int sig)
{
    parent_interrupted = 1;
}

void child1_stop(int sig)
{
    child1_stopped = 1;
}

void child2_stop(int sig)
{
    child2_stopped = 1;
}

int main(void)
{
    pid_t p1, p2;

    signal(SIGINT, parent_stop);

    p1 = fork();
    if (p1 < 0) {
        perror("fork p1");
        exit(1);
    }

    if (p1 == 0) {
        signal(SIGUSR1, child1_stop);
        while (!child1_stopped) {
            pause();
        }
        printf("P1 is killed by parent 1\n");
        fflush(stdout);
        sleep(1);
        printf("P1 is killed by parent 2\n");
        fflush(stdout);
        exit(0);
    }

    p2 = fork();
    if (p2 < 0) {
        perror("fork p2");
        kill(p1, SIGKILL);
        waitpid(p1, NULL, 0);
        exit(1);
    }

    if (p2 == 0) {
        signal(SIGUSR2, child2_stop);
        while (!child2_stopped) {
            pause();
        }
        printf("P2 is killed by parent 1\n");
        fflush(stdout);
        sleep(1);
        printf("P2 is killed by parent 2\n");
        fflush(stdout);
        exit(0);
    }

    printf("Parent pid: %d\n", getpid());
    printf("Child p1 pid: %d\n", p1);
    printf("Child p2 pid: %d\n", p2);
    printf("Press Ctrl+C to send signals to children.\n");
    fflush(stdout);

    while (!parent_interrupted) {
        pause();
    }

    kill(p1, SIGUSR1);
    kill(p2, SIGUSR2);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    printf("parents is killed \n");
    return 0;
}
