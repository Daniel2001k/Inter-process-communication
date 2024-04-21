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

void al()
{
        msgctl(queue_id, IPC_RMID, NULL);

        free(msg_s);
        free(msg_d);

        unlink(path);

        printf("Koniec pracy serwera");

        exit(0);
}

int main()
{
        signal(SIGTERM, al);
        signal(SIGINT, al);

        size_t msg_s_size = sizeof(message_s) - sizeof(long);
        size_t msg_d_size = sizeof(message_d) - sizeof(long);

        int file = open(path, O_EXCL | O_CREAT | 0666);

        if (access(path, F_OK) == -1)
        {
                perror("Brak dostepu do pliku");
                unlink(path);
                return -1;
        }

        close(file);

        key_t key = ftok(path, 0);

        msg_s = malloc(sizeof(message_s));
        msg_d = malloc(sizeof(message_d));

        queue_id = msgget(key, O_EXCL | IPC_CREAT | 0666);

        for(;;)
        {
                msgrcv(queue_id, (void*)msg_s, msg_s_size, 1, 0);
                msgrcv(queue_id, (void*)msg_d, msg_d_size, 2, 0);

                msg_s->mtype = 3;
                sprintf(msg_s->text, "%s %lf", msg_s->text, msg_d->number);

                msg_d->mtype = 3;
                msg_d->number = pow(msg_d->number, 2);

                msgsnd(queue_id, (void*)msg_s, msg_s_size, 0);
                msgsnd(queue_id, (void*)msg_d, msg_d_size, 0);

                msgrcv(queue_id, (void*)msg_s, msg_s_size, 4, 0);
                printf("Odebralem wiadomosc: %s\n", msg_s->text);
        }

        return 0;
}
