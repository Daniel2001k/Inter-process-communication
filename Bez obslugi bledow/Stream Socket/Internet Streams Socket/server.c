#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;
int gniazdo;

void al(int i)
{
    if (i == SIGTERM || i == SIGINT)
    {
        printf("Serwer: Koniec pracy serwera.\n");
        close(gniazdo);
        exit(0);
    }
}

int main()
{
    int gniazdo1;
    socklen_t i = sizeof(SockAddr_in);
    SockAddr_in adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);

    gniazdo = socket(PF_INET, SOCK_STREAM, 0);

    adres.sin_family = AF_INET;
    adres.sin_port = htons(strtol("8080", NULL, 10));
    adres.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(gniazdo, (SockAddr *)&adres, i);
    getsockname(gniazdo, (SockAddr *)&adres, &i);
    printf("Serwer: Nr portu : %d\n", ntohs(adres.sin_port));
    listen(gniazdo, 5);

    for (;;)
    {
        gniazdo1 = accept(gniazdo, (SockAddr *)&adres_k, &i);
        ssize_t liczba_bajtow;
        char buf[1024];

        printf("Serwer: Polaczenie od %s z portu %d.\n", inet_ntoa(adres_k.sin_addr), ntohs(adres_k.sin_port));

        liczba_bajtow = read(gniazdo1, buf, sizeof(buf));

        buf[liczba_bajtow] = '\0';
        printf("Serwer: Przeczytalem %ld bajtow od klienta, wiadomosc od klienta : %s.\n", liczba_bajtow, buf);

        double temp = atof(buf) * atof(buf);
        sprintf(buf, "%f", temp);

        strcat(buf, " to jest z serwera");

        write(gniazdo1, buf, strlen(buf));

        close(gniazdo1);
    }
    return 0;
}
