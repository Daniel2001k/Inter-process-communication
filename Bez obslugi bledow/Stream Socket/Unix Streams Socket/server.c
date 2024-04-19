#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;
int gniazdo;
char *sciezka;

void al(int i)
{
    if (i == SIGTERM || i == SIGINT)
    {
        printf("Koniec pracy serwera ze sciezka %s.\n", sciezka);
        close(gniazdo);
        unlink(sciezka);
        free(sciezka);
        exit(0);
    }
}

int main()
{
    int gniazdo1;
    socklen_t i;
    SockAddr_un adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);

    sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));

    gniazdo = socket(PF_UNIX, SOCK_STREAM, 0);
    bzero((char *)&adres, sizeof(SockAddr_un));
    adres.sun_family = AF_UNIX;
    strncpy(adres.sun_path, sciezka, strlen(sciezka));
    i = sizeof(SockAddr_un);

    bind(gniazdo, (SockAddr *)&adres, i);
    getsockname(gniazdo, (SockAddr *)&adres, &i);
    printf("Sciezka : %s.\n", adres.sun_path);

    listen(gniazdo, 5);
    for (;;)
    {
        gniazdo1 = accept(gniazdo, (SockAddr *)&adres_k, &i);
        ssize_t liczba_bajtow;
        char buf[1024];

        liczba_bajtow = read(gniazdo1, buf, sizeof(buf));

        buf[liczba_bajtow] = '\0';
        printf("Serwer: Przeczytalem %ld bajtow od klienta, wiadomosc od klienta : %s.\n", liczba_bajtow, buf);

        double temp = atof(buf) * atof(buf);
        sprintf(buf, "%f", temp);

        strcat(buf, " to jest z serwera");

        write(gniazdo1, buf, strlen(buf));

        close(gniazdo1);
    }
}
