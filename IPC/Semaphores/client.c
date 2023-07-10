#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
typedef struct sembuf bufor_semaforow;
 
int main(int argc, char *argv[])
{
    key_t klucz_semafory, klucz_memS;
    bufor_semaforow setSem;
    int id_semafor;
    int id_pamieci;
    char *sciezka;
    pid_t pid = getpid();
 
 
    
    sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
 
    klucz_semafory = ftok(sciezka, 0);
    klucz_memS = ftok(sciezka, 1);

    free(sciezka);

    id_semafor = semget(klucz_semafory, 2, 0600);

    setSem.sem_num = 0;
    setSem.sem_op = -1;
    setSem.sem_flg = 0;

    semop(id_semafor, &setSem, 1);

    id_pamieci = shmget(klucz_memS, 50 * sizeof(char), 0600);
    char *msg = (char *)malloc(100 * sizeof(char));
    char *memS = shmat(id_pamieci, (char *)0, 0);
 
    double liczba;
    scanf("%lf", &liczba);
    sprintf(msg, "%lf", liczba);
    strcpy(memS, msg);


    printf("%s\n", msg);
    free(msg);
    shmdt(memS);
 
    setSem.sem_num = 1;
    setSem.sem_op = 1;
    setSem.sem_flg = 0;
    semop(id_semafor, &setSem, 1);
 
    return 0;
}