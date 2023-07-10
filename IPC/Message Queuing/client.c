#include <unistd.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
 

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


char *path = "/tmp/roboczy.kolejka_serwer";

p2 *praca;
p1 *msg;
int f, f1;
key_t keyq;

int main(int argc, char *argv[]) 
{
    pid_t pid = getpid();

    if ((keyq = ftok(path, 0)) != -1) 
    {
        if ((f = msgget(keyq, 0600)) != -1) 
        {
            msg = (p1*)malloc(sizeof(p1));
 
            msg->type = 1;
            msg->pid = pid;
            
            if (msgsnd(f, msg, sizeof(p1) - sizeof(long), 0) != -1) 
            {
                if ((keyq = ftok(path, pid)) != -1) 
                {

                    if ((f1 = msgget(keyq, 0600 | IPC_CREAT | IPC_EXCL)) != -1) 
                    {   
                        double a = 0;
 
                        scanf("%lf", &a);

                        praca = (p2 *) malloc(sizeof(p2));
                        praca->type = 2;
                        praca->el = a;
 
                        if (msgsnd(f1, praca, sizeof(p2) - sizeof(long), 0) != -1) 
                        {
                            msgrcv(f1, praca, sizeof(p2) - sizeof(long), 3, 0);
                            
                            printf("%lf\n", a, praca->el);

                            free(praca);
                            msgctl(f1, IPC_RMID, NULL);

                            return 0;
                        }
                        else
                        {
                            free(praca);
                            msgctl(f1, IPC_RMID, NULL);
                        }
                    }
                }
                else
                    free(path);
            }
            else
                free(msg);
        }
    }
    else
        free(path);
    
    return -1;
}