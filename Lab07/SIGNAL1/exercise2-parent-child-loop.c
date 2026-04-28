#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ROUNDS 6

static volatile sig_atomic_t parent_got_signal = 0;
static volatile sig_atomic_t child_got_signal = 0;
static volatile sig_atomic_t stop_child = 0;
static volatile sig_atomic_t parent_count = 0;
static volatile sig_atomic_t child_count = 0;
static pid_t parent_pid = -1;
static pid_t child_pid = -1;

static void parent_handler(int sig)
{
    parent_got_signal = 1;
}

static void child_handler(int sig)
{
    child_got_signal = 1;
}

static void child_stop_handler(int sig)
{
    stop_child = 1;
}

int main(void)
{
    parent_pid = getpid();
    signal(SIGUSR1, parent_handler);

    child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        return 1;
    }

    if (child_pid == 0) {
        parent_pid = getppid();
        signal(SIGUSR1, child_handler);
        signal(SIGTERM, child_stop_handler);

        while (!stop_child) {
            while (!child_got_signal && !stop_child) {
                pause();
            }
            if (stop_child) {
                break;
            }

            child_got_signal = 0;
            child_count++;
            printf("child caught signal #%d\n", (int)child_count);
            fflush(stdout);
            kill(parent_pid, SIGUSR1);
        }

        _exit(0);
    }

    printf("Parent pid: %d\n", getpid());
    printf("Child pid: %d\n", child_pid);
    fflush(stdout);

    for (int i = 1; i <= MAX_ROUNDS; i++) {
        parent_got_signal = 0;
        kill(child_pid, SIGUSR1);

        while (!parent_got_signal) {
            pause();
        }

        parent_count++;
        printf("parent caught signal #%d\n", (int)parent_count);
        fflush(stdout);
        sleep(1);
    }

    kill(child_pid, SIGTERM);
    waitpid(child_pid, NULL, 0);
    return 0;
}
