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

int main()
{
        size_t msgs_size = sizeof(messages) - sizeof(long);
        size_t msgd_size = sizeof(messaged) - sizeof(long);
        msgs = malloc(sizeof(messages));
        msgd = malloc(sizeof(messaged));


        key_t key = ftok(path, 0);
        qid = msgget(key, 0666);


        msgs->mtype = 1;
        msgd->mtype = 1;

        sprintf(msgs->msg, "auto");
        msgd->msg = -7.23;


        msgsnd(qid, (void*)msgs, msgs_size, 0);
        msgsnd(qid, (void*)msgd, msgd_size, 0);


        msgrcv(qid, (void*)msgs, msgs_size, 2, 0);
        msgrcv(qid, (void*)msgd, msgd_size, 2, 0);


        msgs->mtype = 3;
        sprintf(msgs->msg, "%s - %lf - Klient", msgs->msg, msgd->msg);


        msgsnd(qid, (void*)msgs, msgs_size, 0);


        free(msgs);
        free(msgd);


        return 0;
}
