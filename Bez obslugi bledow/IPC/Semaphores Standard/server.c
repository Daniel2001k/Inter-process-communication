#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#include <fcntl.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define SHM_KEY 1234

int main()
{
    for (;;)
    {
        int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
        char *shmaddr = shmat(shmid, (void *)0, 0);
        int semid = semget(SHM_KEY, 1, IPC_CREAT | 0666);

        struct sembuf sem_st = {0, 0, 0};

        semop(semid, &sem_st, 1);

        struct sembuf sem_wait = {0, -1, 0};

        semop(semid, &sem_wait, 1);

        printf("wiadomosc %s", shmaddr);

        char *wiadomosc = strtok(shmaddr, ",");
        char wiadomosci[3][100];
        int i = 0;
        while (wiadomosc != NULL && i < 3)
        {
            strcpy(wiadomosci[i], wiadomosc);
            i++;
            wiadomosc = strtok(NULL, ",");
        }
        sprintf(shmaddr, "%s + %lf", wiadomosci[0], atof(wiadomosci[1]) * atof(wiadomosci[2]));

        struct sembuf sem_send = {0, 1, 0};

        semop(semid, &sem_send, 1);
        shmdt(shmaddr);
        semctl(semid, 0, IPC_RMID);
        shmctl(shmid, IPC_RMID, NULL);
    }
}