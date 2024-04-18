#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include <errno.h>

char path[]="/tmp/aa";

typedef struct mystring {
    long mtype;
    char text[1024];
} message_s;

typedef struct mydouble {
    long mtype;
    double liczba;
} message_d;


int main(){

    key_t key = ftok(path, 0);
    if(key == -1){
        perror("ftok failed");
    }


    int queue_id;
    if((queue_id = msgget(key, 0600 )) ==  -1){
        perror("msgget failed");
        return -1;
    }

    message_d* msg_d;
    message_s* msg_s;
    msg_d = malloc(sizeof(message_d));
    msg_s = malloc(sizeof(message_s));

    msg_s->mtype = 1;
    sprintf(msg_s->text,"auto");


    msg_d->mtype = 2;
    msg_d->liczba = -7.23;

    int msg_size_s = sizeof(message_s) - sizeof(long);
    int msg_size_d = sizeof(message_d) - sizeof(long);

    if(msgsnd(queue_id, msg_s, msg_size_s, 0)==-1){
        perror("error msgsnd");
        return -1;
    }
    printf("wyslalem wiadomosc: %s\n",msg_s->text);

    if(msgsnd(queue_id, msg_d, msg_size_d, 0)==-1){
        perror("error msgsnd");
        return -1;
    }
    printf("wyslalem wiadomosc: %lf\n",msg_d->liczba);

    if(msgrcv(queue_id, msg_s, msg_size_s, 3 ,0)==-1){
        perror("error msgrcv");
        return -1;
    }
    printf("odebralem wiadomosc: %s\n",msg_s->text);

    if(msgrcv(queue_id, msg_d, msg_size_d, 3 ,0)==-1){
        perror("error msgrcv");
        return -1;
    }
    printf("odebralem wiadomosc: %lf\n",msg_d->liczba);

    sprintf(msg_s->text,"%sKLIENT",msg_s->text);
     
    msg_s->mtype = 4;
    msg_d->mtype = 4;

    if(msgsnd(queue_id, msg_s, msg_size_s, 0)==-1){
        perror("error msgsnd");
        return -1;
    }
    printf("wyslalem wiad %s\n",msg_s->text);

    free(msg_d);
    free(msg_s);
}