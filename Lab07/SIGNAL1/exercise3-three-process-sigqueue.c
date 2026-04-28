#define _POSIX_C_SOURCE 199309L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef __linux__
int sigqueue(pid_t pid, int sig, const union sigval value);
#endif

static volatile sig_atomic_t parent_got_int = 0;
static volatile sig_atomic_t child_got_signal = 0;
static volatile sig_atomic_t grandson_got_signal = 0;
static volatile sig_atomic_t child_value = 0;
static volatile sig_atomic_t grandson_value = 0;
static pid_t grandson_pid = -1;

static void parent_handler(int sig, siginfo_t *info, void *context)
{
    parent_got_int = 1;
}

static void child_handler(int sig, siginfo_t *info, void *context)
{
    child_value = info->si_value.sival_int;
    child_got_signal = 1;
}

static void grandson_handler(int sig, siginfo_t *info, void *context)
{
    grandson_value = info->si_value.sival_int;
    grandson_got_signal = 1;
}

static void install_handler(int signo, void (*handler)(int, siginfo_t *, void *))
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handler;

    if (sigaction(signo, &act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

int main(void)
{
    int ready_pipe[2];
    pid_t child_pid;
    union sigval value;
    char ready;

    if (pipe(ready_pipe) == -1) {
        perror("pipe");
        return 1;
    }

    install_handler(SIGINT, parent_handler);

    child_pid = fork();
    if (child_pid < 0) {
        perror("fork child");
        return 1;
    }

    if (child_pid == 0) {
        close(ready_pipe[0]);
        install_handler(SIGUSR1, child_handler);

        grandson_pid = fork();
        if (grandson_pid < 0) {
            perror("fork grandson");
            _exit(1);
        }

        if (grandson_pid == 0) {
            install_handler(SIGUSR2, grandson_handler);
            write(ready_pipe[1], "R", 1);
            close(ready_pipe[1]);

            while (!grandson_got_signal) {
                pause();
            }

            printf("Grandson received value: %d\n", (int)grandson_value);
            printf("Grandson process1 is killed by son!\n");
            fflush(stdout);
            _exit(0);
        }

        close(ready_pipe[1]);

        while (!child_got_signal) {
            pause();
        }

        printf("Child received value: %d\n", (int)child_value);
        value.sival_int = (int)child_value + 1;
        sigqueue(grandson_pid, SIGUSR2, value);

        waitpid(grandson_pid, NULL, 0);
        printf("Child process is killed by parent!\n");
        fflush(stdout);
        _exit(0);
    }

    close(ready_pipe[1]);
    read(ready_pipe[0], &ready, 1);
    close(ready_pipe[0]);

    printf("Parent pid: %d\n", getpid());
    printf("Child pid: %d\n", child_pid);
    printf("Press Ctrl+C to start sigqueue communication.\n");
    fflush(stdout);

    while (!parent_got_int) {
        pause();
    }

    value.sival_int = 100;
    sigqueue(child_pid, SIGUSR1, value);

    waitpid(child_pid, NULL, 0);
    printf("Parent process is killed!\n");

    return 0;
}
