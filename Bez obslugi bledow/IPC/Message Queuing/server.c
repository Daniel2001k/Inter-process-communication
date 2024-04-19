#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

char path[]="/tmp/aa";

int queue_id;

typedef struct mystring {
    long mtype;       
    char text[1024];   
} message_s;

typedef struct mydouble {
    long mtype;         
    double liczba;
} message_d;

message_d* msg_d;
message_s* msg_s;

void usun_wszystko(){
    msgctl(queue_id, IPC_RMID, NULL);
    free(msg_d);
    free(msg_s);
    unlink(path);
    printf("server zakonczyl prace\n");
    exit(0);
}


int main(){
    
    signal(SIGTERM, usun_wszystko);
    signal(SIGINT, usun_wszystko);


    size_t msg_size_s = sizeof(message_s) - sizeof(long);
    size_t msg_size_d = sizeof(message_d) - sizeof(long);

    
    int file = open(path, O_EXCL | O_CREAT | 0666);
    close(file);
    
    if(access(path, F_OK) == -1){
        unlink(path);
        perror("no access");
        return -1;
    }

    //key_t key;
    key_t key = ftok(path, 0);

    msg_s = malloc(sizeof(message_s));
    msg_d = malloc(sizeof(message_d));

    queue_id = msgget(key, O_EXCL | IPC_CREAT | 0600);

    for(;;){
        msgrcv(queue_id, (void*)msg_s, msg_size_s, 1, 0);
        printf("odebralem wiad %s\n",msg_s->text);

        msgrcv(queue_id, (void*)msg_d, msg_size_d, 2, 0);
        printf("odebralem wiad %lf\n",msg_d->liczba);

        msg_s->mtype = 3;
        sprintf(msg_s->text, "%s %lf", msg_s->text, msg_d->liczba);

        msg_d->mtype = 3;
        msg_d->liczba = msg_d->liczba * msg_d->liczba;

        
        msgsnd(queue_id, (void*)msg_s, msg_size_s, 0);
        printf("wyslalem wiad %lf\n",msg_d->liczba);
        
        msgsnd(queue_id, (void*)msg_d, msg_size_d, 0);
        printf("wyslalem wiad %s\n",msg_s->text);

        msgrcv(queue_id, (void*)msg_s, msg_size_s, 4, 0);
        printf("odebralem wiad %s\n",msg_s->text);
        printf("%s\n",msg_s->text);
    }

    return 0;
}
// ssize_t msgrcv(int msqid,       void *msgp, size_t msgsz, long msgtyp, int msgflg);
//     int msgsnd(int msqid, const void *msgp, size_t msgsz,              int msgflg);