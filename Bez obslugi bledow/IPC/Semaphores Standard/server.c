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
        for(;;)
        {
                int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
                char *shmaddr = shmat(shmid, (void*)0, 0);
                int semid = semget(SHM_KEY, 1, IPC_CREAT | 0666);

                struct sembuf sem_st = {0, 0, 0};
                semop(semid, &sem_st, 1);

                sem_st.sem_op = -1;
                semop(semid, &sem_st, 1);

                printf("Wiadomosc: %s\n", shmaddr);

                char ws[100];
                double wd1;
                double wd2;

                sscanf(shmaddr, "%s %lf %lf", ws, &wd1, &wd2);
                sprintf(shmaddr, "Serwer: %s - %lf", ws, wd1 * wd2);

                sem_st.sem_op = 1;
                semop(semid, &sem_st, 1);

                sem_st.sem_op = -1;
                semop(semid, &sem_st, 1);
                printf("%s\n", shmaddr);

                shmdt(shmaddr);
                shmctl(shmid, IPC_RMID, NULL);
                semctl(semid, 0, IPC_RMID);
        }

        return 0;
}
