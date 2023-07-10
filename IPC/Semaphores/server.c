#include <stdio.h>  
#include <string.h>  
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>  
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>  
 
typedef struct sembuf bufor_semaforow;  
int sem;
int mem;   
char *memS;  
char *sciezka;  
int czy_skasowac = 1;  
 

void al(int i)  
{
    if (i == SIGTERM || i == SIGINT)  
    {
        printf("Serwer: Koniec pracy serwera.\n");
        shmdt(memS);  
        shmctl(mem, IPC_RMID, NULL);
        semctl(sem, 0, IPC_RMID, NULL);  
 
        if (czy_skasowac)  
        {
            unlink(sciezka); 
        }
 
        free(sciezka);  
        exit(0); 
    }
}
 
int main(int argc, char *argv[])  
{
    key_t klucz_semafory, klucz_memS; 
    bufor_semaforow setSem;
    int plik = 0;
 
    signal(SIGTERM, al);
    signal(SIGINT, al);
 
    if (argc > 1)
        sciezka = strndup(argv[1], strlen(argv[1]));
    else
        sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
 
 
    if (access(sciezka, F_OK) == 0)
        czy_skasowac = 0;
 
 
    if (czy_skasowac && ((plik = open(sciezka, O_CREAT | O_EXCL, 0600)) == -1))
    {
        fprintf(stderr, "Serwer: Blad utworzenia pliku %s: %s.\n", sciezka, strerror(errno));
        free(sciezka);
        return -1;
    }
    else
    {
        close(plik);
        klucz_semafory = ftok(sciezka, 0);
        klucz_memS = ftok(sciezka, 1);
        mem = shmget(klucz_memS, 50 * sizeof(char), 0600 | IPC_CREAT | IPC_EXCL);
        memS = shmat(mem, (char *)0, 0);
        sem = semget(klucz_semafory, 2, 0600 | IPC_CREAT | IPC_EXCL);
        u_short wartosci_poczatkowe_semaforow[] = {1, 0};
 
        semctl(sem, 0, SETALL, wartosci_poczatkowe_semaforow);
 
        for (;;)
        {
            setSem.sem_num = 1;
            setSem.sem_op = -1;
            setSem.sem_flg = 0;
 
            if (semop(sem, &setSem, 1) == -1)
            {
                shmdt(memS);
                shmctl(mem, IPC_RMID, NULL);
                semctl(sem, 0, IPC_RMID, NULL);
 
                if (czy_skasowac)
                {
                    unlink(sciezka);
                }
 
                free(sciezka);
                return -1;
            }
            else
            {
                char *msg = strdup(memS);
 
                setSem.sem_num = 0;
                setSem.sem_op = 1;
                setSem.sem_flg = 0;
 
                if (semop(sem, &setSem, 1) == -1)
                {
                    free(msg);
                    shmdt(memS);
                    shmctl(mem, IPC_RMID, NULL);
                    semctl(sem, 0, IPC_RMID, NULL);
 
                    if (czy_skasowac)
                    {
                        unlink(sciezka);
                    }
 
                    free(sciezka);
                    return -1;
                }
                else
                {
                    printf("%s\n", msg);

                    double wynik = atoi(memS) * atoi(memS);

                    sprintf(msg, "%lf", wynik);
                    strcpy(memS, msg);
                    printf("%s\n", msg);
                }
 
                free(msg);
            }
        }
    }
}