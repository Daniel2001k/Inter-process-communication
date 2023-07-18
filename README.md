# Inter-process-communication

## For build and run programs you can use:
- Build: gcc program.c -o program
- Run: ./program

## Funkcje

### FIFO:
- mknod(): Tworzy węzeł w systemie plików. - make node
- write(): Zapisuje dane do deskryptora pliku.
- read(): Odczytuje dane z deskryptora pliku.
- wait(): Oczekuje na zakończenie procesu potomnego.
  
### Kolejka komunikatów:
- msgget(): Uzyskuje identyfikator kolejki komunikatów IPC. - message get
- msgsnd(): Wysyła komunikat do kolejki komunikatów IPC. - message send
- msgrcv(): Odbiera komunikat z kolejki komunikatów IPC. - message receive
- msgctl(): Kontroluje kolejkę komunikatów IPC. - message control
  
### Semafory:
- semget(): Uzyskuje identyfikator zbioru semaforów. - semaphore get
- semctl(): Kontroluje zbiór semaforów. - semaphore control
- semop(): Wykonuje operacje na semaforach. - semaphore operation
- shmget() - shared memory get
- shmat(): Dołącza segment pamięci współdzielonej do przestrzeni adresowej procesu. - shared memory attach
- shmdt(): Odłącza segment pamięci współdzielonej od przestrzeni adresowej procesu. - shared memory detach
- shmctl(): Kontroluje segment pamięci współdzielonej. - shared memory control
- strdup() - string duplicate
- atoi() - ascii to integer

### Inne:
- exit(): Kończy program i powraca do systemu operacyjnego.
- signal(): Instaluje obsługę sygnału.
- getpid(): Zwraca identyfikator procesu. - get pid
- scanf(): Wczytuje dane z wejścia.
- close(): Zamyka otwarty deskryptor pliku.
- open(): Otwiera plik.
- unlink(): Usuwa plik z systemu plików.
- strcpy(): Kopiuje łańcuch znaków. - string copy
- sprintf(): Formatowany zapis do łańcucha znaków. - string printf
- fork(): Tworzy nowy proces poprzez duplikowanie bieżącego procesu. - process fork
- ftok(): Przekształca ścieżkę pliku w klucz IPC. - file to key
- malloc(): Alokacja pamięci dynamicznej. - memory allocation
- free(): Zwalnia zaalokowaną pamięć.
- strndup(): Tworzy kopię łańcucha znaków z ograniczeniem długości. - string duplicate with length
- access(): Sprawdza dostępność pliku.
- printf(): Wypisuje sformatowane dane na standardowym wyjściu.
- strlen(): Zwraca długość łańcucha znaków. - string length
- sprintf(): Formatowany zapis do łańcucha znaków. - string printf
- sigignore(): Ignoruje sygnał. - signal ignore
- strerror() - string error

### Przykłady:
- close
```c
close(fd);
```
- unlink
```c
unlink(path);
```
- exit
```c
exit(0);
```
- signal
```c
signal(SIGINT, handler);
```
- sigignore 
```c
sigignore(SIGCHLD);
```
- mknod 
```c
mknod(path, mode, dev);
```
- open
```c
int fd = open(path, O_RDONLY);
```
- read
```c
char buffer[100];
ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
```
- fork 
```c
pid_t childPid = fork();
if (childPid == 0) {
    // Kod dla procesu potomnego
} else if (childPid > 0) {
    // Kod dla procesu macierzystego
} else {
    // Błąd w funkcji fork
}
```
- strcpy 
```c
char dest[100];
char src[] = "Hello";
strcpy(dest, src);
```
- strlen 
```c
size_t length = strlen(str);
```
- sprintf
```c
char buffer[100];
int value = 42;
sprintf(buffer, "Value: %d", value);
```
- printf
```c
int age = 25;
printf("My age is %d\n", age);
```
- write
```c
char message[] = "Hello";
write(fd, message, strlen(message));
```
- wait
```c
int status;
pid_t terminatedPid = wait(&status);
```

- getpid
```c
pid_t pid = getpid();
```
- scanf 
```c
int value;
scanf("%d", &value);
```
- ftok
```c
key_t key = ftok(path, projId);
```
- msgget
```c
int msqid = msgget(key, 0666 | IPC_CREAT);
```
- malloc
```c
int* numbers = (int*)malloc(5 * sizeof(int));
```
- msgsnd
```c
struct msgbuf message;
message.mtype = 1;
strcpy(message.mtext, "Hello");
msgsnd(msqid, &message, sizeof(message.mtext), 0);
```
- msgrcv
```c
struct msgbuf message;
msgrcv(msqid, &message, sizeof(message.mtext), 1, 0);
printf("Received message: %s\n", message.mtext);
```
- msgctl
```c
msgctl(msqid, IPC_RMID, NULL);
```
- free
```c
free(ptr);
```
- strndup
```c
const char* source = "Hello";
char* copy = strndup(source, 5);
```
- access
```c
int result = access(path, F_OK);
```

- semget
```c
int semid = semget(key, 1, IPC_CREAT | 0666);
```
- semop
```c
struct sembuf sembuf;
sembuf.sem_num = 0;
sembuf.sem_op = -1;
sembuf.sem_flg = 0;
semop(semid, &sembuf, 1);
```
- shmat
```c
void* sharedMemory = shmat(shmid, NULL, 0);
```
- shmdt
```c
shmdt(sharedMemory);
```
- shmctl
```c
shmctl(shmid, IPC_RMID, NULL);
```
- semctl
```c
semctl(semid, 0, IPC_RMID, 0);
```
- fprintf
```c
FILE* file = fopen("file.txt", "w");
fprintf(file, "Hello, world!");
fclose(file);
```
- strerror
```c
int errorNumber = errno;
const char* errorMessage = strerror(errorNumber);
printf("Error: %s\n", errorMessage);
```
- strdup
```c
const char* source = "Hello";
char* copy = strdup(source);
```
- atoi
```c
const char* str = "42";
int value = atoi(str);
```

## Biblioteki
### FIFO:
```c
#include <sys/stat.h>
#include <sys/uio.h>
```
### Kolejka komunikatów:
```c
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/uio.h>
```
### Semafory:
```c
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>  
```
### Inne:
```c
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
```
