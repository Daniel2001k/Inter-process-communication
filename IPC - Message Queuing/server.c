#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct
{
        long mtype;
        double msg;
} messaged;

typedef struct
{
        long mtype;
        char msg[100];
} messages;

char path[] = "/tmp/kolejki";
int qid;

messaged* msgd;
messages* msgs;

void al(int a)
{
        printf("Koniec pracy serwera\n");

        msgctl(qid, IPC_RMID, NULL);

        free(msgd);
        free(msgs);

        unlink(path);

        exit(0);
}

int main()
{
        signal(SIGTERM, al);
        signal(SIGINT, al);


        size_t msgs_size = sizeof(messages) - sizeof(long);
        size_t msgd_size = sizeof(messaged) - sizeof(long);
        msgs = malloc(sizeof(messages));
        msgd = malloc(sizeof(messaged));


        int file = open(path, O_EXCL | O_CREAT | 0666);
        if (access(path, F_OK) == -1)
        {
                perror("Brak dostepu do pliku");
                unlink(path);
                return -1;
        }
        close(file);


        key_t key = ftok(path, 0);
        qid = msgget(key, O_EXCL | IPC_CREAT | 0666);

        for (;;)
        {
                msgrcv(qid, (void*)msgs, msgs_size, 1, 0);
                msgrcv(qid, (void*)msgd, msgd_size, 1, 0);


                msgs->mtype = 2;
                msgd->mtype = 2;

                sprintf(msgs->msg, "%s %lf", msgs->msg, msgd->msg);
                msgd->msg = pow(msgd->msg, 2);


                msgsnd(qid, (void*)msgs, msgs_size, 0);
                msgsnd(qid, (void*)msgd, msgd_size, 0);


                msgrcv(qid, (void*)msgs, msgs_size, 3, 0);
                printf("Wynik: %s\n", msgs->msg);
        }

        return 0;
}
