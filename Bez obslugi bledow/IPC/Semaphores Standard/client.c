#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define SHM_SIZE 1024
#define SHM_KEY 1234

int main()
{
        int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
        int semid = semget(SHM_KEY, 1, 0666);
        char *shmaddr = shmat(shmid, (void*)0, 0);

        fgets(shmaddr, SHM_SIZE, stdin);

        struct sembuf sem_st = {0, 1, 0};

        semop(semid, &sem_st, 1);

        sem_st.sem_op = -1;

        semop(semid, &sem_st, 1);

        char result[100];

        sprintf(result, "Klient - %s", shmaddr);

        sprintf(shmaddr, "%s", result);
        printf("Wysylam: %s\n", shmaddr);

        sem_st.sem_op = 1;
        semop(semid, &sem_st, 1);

        shmdt(shmaddr);

        return 0;
}
