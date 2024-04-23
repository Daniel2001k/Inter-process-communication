#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int g, g1;

void al(int i)
{
    printf("Koniec pracy serwera\n");
    close(g);
    exit(0);
}

int main()
{
    socklen_t i = sizeof(SockAddr_in);
    SockAddr_in adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);

    g = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&adres, sizeof(SockAddr_in));
    adres.sin_family = AF_INET;
    adres.sin_port = htons(strtol("8080", NULL, 10));
    adres.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(g, (SockAddr*)&adres, i);
    getsockname(g, (SockAddr*)&adres, &i);
    listen(g, 5);

    for (;;)
    {
        g1 = accept(g, (SockAddr*)&adres_k, &i);
        char message[1024];
        ssize_t liczba_bajtow = read(g1, message, sizeof(message));
        message[liczba_bajtow] = '\0';
        printf("%s\n", message);
        //memset(message, 0, sizeof(message));
        write(g1, message, strlen(message));

        close(g1);
    }
}
