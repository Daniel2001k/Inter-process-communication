#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;

int g, g1;
char path[] = "/tmp/unix";

void al(int i)
{
    printf("Koniec pracy serwera\n");
    close(g);
    unlink(path);
    exit(0);
}

int main()
{
    socklen_t i = sizeof(SockAddr_un);
    SockAddr_un adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);

    g = socket(AF_UNIX, SOCK_STREAM, 0);
    bzero((char *)&adres, sizeof(SockAddr_un));
    adres.sun_family = AF_UNIX;
    strncpy(adres.sun_path, path, strlen(path));

    bind(g, (SockAddr *)&adres, i);
    getsockname(g, (SockAddr *)&adres, &i);
    listen(g, 5);

    for (;;)
    {
        g1 = accept(g, (SockAddr *)&adres_k, &i);
        char message[1024];
        char message1[1024];
        ssize_t liczba_bajtow = read(g1, message, sizeof(message));

        double temp;
        sscanf(message, "%lf", &temp);
        printf("Otrzymano: %lf\n", temp);

        memset(message, 0, sizeof(message));
        sprintf(message, "To jest z serwera - %lf", temp * temp);
        printf("Wyslano: %s\n", message);

        write(g1, message, sizeof(message));

        liczba_bajtow = read(g1, message, sizeof(message));

        printf("Wynik: %s\n", message);

        close(g1);
    }
}
