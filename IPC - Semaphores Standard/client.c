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
        int shmid = shmget(KEY, SIZE, 0666);
        char *shmaddr = shmat(shmid, (void*)0, 0);
        int semid = semget(KEY, 1, 0666);


        fgets(shmaddr, SIZE, stdin);
        struct sembuf semcfg = {0,1,0};
        semop(semid, &semcfg, 1);


        semcfg.sem_op = -1;
        semop(semid, &semcfg, 1);


        char ws[100];
        sprintf(ws, "Klient: %s", shmaddr);
        sprintf(shmaddr, "%s", ws);


        semcfg.sem_op = 1;
        semop(semid, &semcfg, 1);


        shmdt(shmaddr);

        return 0;
}
