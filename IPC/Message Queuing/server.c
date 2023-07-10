#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
 
typedef struct msg1
{
    long type;
    pid_t pid;
} p1;
typedef struct msg2
{
    long type;
    double el;
} p2;

char *path;
p1 *msg;
int f;
int czy_skasowac = 1;
int plik = 0;

void signalss(int i)
{
    free(msg);

    msgctl(f, IPC_RMID, NULL);
    if (czy_skasowac)
    {
        unlink(path);
    }
    free(path);
    exit(0);
}

int main(int argc, char *argv[])
{
    pid_t pid;
    key_t keyq;

    signal(SIGTERM, signalss);
    signal(SIGINT, signalss);
    sigignore(SIGCHLD);
 
    path = strndup("/tmp/roboczy.kolejka_serwer", strlen("/tmp/roboczy.kolejka_serwer"));

    if (access(path, F_OK) == 0)
        czy_skasowac = 0;

    plik = open(path, O_CREAT | O_EXCL, 0600);
    close(plik);

    keyq = ftok(path, 0);

    f = msgget(keyq, 0600 | IPC_CREAT | IPC_EXCL);
    
    for (;;)
    {
        msg = (p1 *)malloc(sizeof(p1));
        
        if ((msgrcv(f, msg, sizeof(p1) - sizeof(long), 1, 0)) == -1)
        {
            if (czy_skasowac)
                unlink(path);
            free(path);
            return -1;
        }
        
        if((pid = fork()) == -1)
            return -1;

        if (pid == 0)
        {
            int f1;
 
            keyq = ftok(path, msg->pid);
            f1 = msgget(keyq, 0600);

            p2 *praca = (p2 *)malloc(sizeof(p2));
            
            msgrcv(f1, praca, sizeof(p2) - sizeof(long), 2, 0);

            printf("%lf\n", praca->el);

            praca->el *= praca->el;
            praca->type = 3;

            msgsnd(f1, praca, sizeof(p2) - sizeof(long), 0);

            free(praca);
            return 0;
        }
        else
        {
            sigignore(SIGCHLD);
        }
    }
}