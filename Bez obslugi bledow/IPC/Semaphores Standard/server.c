#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define SIZE 1024
#define KEY 1234

int main()
{
        for(;;)
        {
                int shmid = shmget(KEY, SIZE, IPC_CREAT | 0666);
                char *shmaddr = shmat(shmid, (void*)0, 0);
                int semid = semget(KEY, 1, IPC_CREAT | 0666);

                struct sembuf semcfg = {0,0,0};
                semop(semid, &semcfg, 1);

                semcfg.sem_op = -1;
                semop(semid, &semcfg, 1);

                char ws[100];
                double wd1,wd2;

                sscanf(shmaddr, "%s %lf %lf", ws, &wd1, &wd2);
                sprintf(shmaddr, "Serwer: %s - %lf", ws, wd1 * wd2);

                semcfg.sem_op = 1;
                semop(semid, &semcfg, 1);

                semcfg.sem_op = -1;
                semop(semid, &semcfg, 1);
                printf("Wynik: %s\n", shmaddr);

                shmdt(shmaddr);
                shmctl(shmid, IPC_RMID, NULL);
                semctl(semid, 0, IPC_RMID);
        }

        return 0;
}
