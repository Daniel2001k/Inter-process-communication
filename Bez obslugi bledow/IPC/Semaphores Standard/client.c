#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_SIZE 1024
#define SEM_KEY 1234

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    int shmid = shmget(SEM_KEY, SHM_SIZE, 0666);
    char *shmaddr = shmat(shmid, (void *)0, 0);
    int semid = semget(SEM_KEY, 1, 0666);

    printf("Wpisz wiadomosc: (np: test1,2,5)");
    fgets(shmaddr, SHM_SIZE, stdin);

    struct sembuf sem_op = {0 , 1, 0};

    semop(semid, &sem_op, 1);

    sem_op.sem_op = -1;
    
    semop(semid, &sem_op, 1);

    printf("Odpowiedz serwera: %s\n", shmaddr);

    shmdt(shmaddr);

    return 0;
}