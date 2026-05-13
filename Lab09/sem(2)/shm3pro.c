#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define TEXT_SZ 2048
#define SEM_READ 0
#define SEM_WRITE 1

struct shared_use_st {
    int written_by_you;
    char some_text[TEXT_SZ];
};

static int sem_id = -1;
static int shmid = -1;

static void semaphore_p(int sem_num)
{
    struct sembuf sem_b;

    sem_b.sem_num = sem_num;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semop P failed\n");
        exit(EXIT_FAILURE);
    }
}

static void semaphore_v(int sem_num)
{
    struct sembuf sem_b;

    sem_b.sem_num = sem_num;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semop V failed\n");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    char buffer[BUFSIZ];

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    sem_id = semget((key_t)5678, 2, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        fprintf(stderr, "semget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    shared_stuff = (struct shared_use_st *)shared_memory;

    while (running) {
        semaphore_p(SEM_WRITE);

        printf("Enter some text: ");
        fflush(stdout);
        fgets(buffer, BUFSIZ, stdin);

        strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
        shared_stuff->some_text[TEXT_SZ - 1] = '\0';
        shared_stuff->written_by_you = 1;

        semaphore_v(SEM_READ);

        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
