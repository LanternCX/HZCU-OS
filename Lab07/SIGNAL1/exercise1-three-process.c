#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t parent_got_int = 0;
static volatile sig_atomic_t child_got_signal = 0;
static volatile sig_atomic_t grandson_got_signal = 0;
static pid_t grandson_pid = -1;

static void parent_handler(int sig)
{
    parent_got_int = 1;
}

static void child_handler(int sig)
{
    child_got_signal = 1;
}

static void grandson_handler(int sig)
{
    grandson_got_signal = 1;
}

int main(void)
{
    int ready_pipe[2];
    pid_t child_pid;
    char ready;

    if (pipe(ready_pipe) == -1) {
        perror("pipe");
        return 1;
    }

    signal(SIGINT, parent_handler);

    child_pid = fork();
    if (child_pid < 0) {
        perror("fork child");
        return 1;
    }

    if (child_pid == 0) {
        close(ready_pipe[0]);
        signal(SIGUSR1, child_handler);

        grandson_pid = fork();
        if (grandson_pid < 0) {
            perror("fork grandson");
            _exit(1);
        }

        if (grandson_pid == 0) {
            signal(SIGUSR2, grandson_handler);
            write(ready_pipe[1], "R", 1);
            close(ready_pipe[1]);

            while (!grandson_got_signal) {
                pause();
            }

            printf("Grandson process1 is killed by son!\n");
            fflush(stdout);
            _exit(0);
        }

        close(ready_pipe[1]);

        while (!child_got_signal) {
            pause();
        }

        kill(grandson_pid, SIGUSR2);
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
    printf("Press Ctrl+C to start signal communication.\n");
    fflush(stdout);

    while (!parent_got_int) {
        pause();
    }

    kill(child_pid, SIGUSR1);
    waitpid(child_pid, NULL, 0);
    printf("Parent process is killed!\n");

    return 0;
}
