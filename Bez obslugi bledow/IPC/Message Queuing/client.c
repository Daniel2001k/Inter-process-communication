#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/msg.h>

char path[] = "/tmp/kolejki";
int queue_id;

typedef struct mystring
{
        long mtype;
        char text[1024];
} message_s;

typedef struct mydouble
{
        long mtype;
        double number;
} message_d;

message_s* msg_s;
message_d* msg_d;

int main()
{
        size_t msg_s_size = sizeof(message_s) - sizeof(long);
        size_t msg_d_size = sizeof(message_d) - sizeof(long);

        key_t key = ftok(path, 0);
        queue_id = msgget(key, 0666);

        msg_s = malloc(sizeof(message_s));
        msg_d = malloc(sizeof(message_d));

        msg_s->mtype= 1;
        sprintf(msg_s->text, "auto");

        msg_d->mtype = 2;
        msg_d->number = -7.23;

        msgsnd(queue_id, (void*)msg_s, msg_s_size, 0);
        msgsnd(queue_id, (void*)msg_d, msg_d_size, 0);

        msgrcv(queue_id, (void*)msg_s, msg_s_size, 3, 0);
        msgrcv(queue_id, (void*)msg_d, msg_d_size, 3, 0);

        printf("Odebralem: %s i %lf\n", msg_s->text, msg_d->number);

        sprintf(msg_s->text, "%s - %lf - Klient", msg_s->text, msg_d->number);

        msg_s->mtype = 4;
        msg_d->mtype = 4;

        msgsnd(queue_id, (void*)msg_s, msg_s_size, 0);

        printf("Wyslalem wiadomosc: %s\n", msg_s->text);

        free(msg_s);
        free(msg_d);

        return 0;
}
