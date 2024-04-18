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

    if ((gniazdo = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Serwer: Blad wywolania funkcji socket.\n");
        return -1;
    }
    bzero((char *)&adres, sizeof(SockAddr_un));
    adres.sun_family = AF_UNIX;
    strncpy(adres.sun_path, sciezka, strlen(sciezka));
    i = sizeof(SockAddr_un);

    if (bind(gniazdo, (SockAddr *)&adres, i) == -1)
    {
        perror("Serwer: Blad dowiazania gniazda.\n");
        free(sciezka);
        close(gniazdo);
        return -1;
    }
    if (getsockname(gniazdo, (SockAddr *)&adres, &i) == -1)
    {
        perror("Serwer: Blad funkcji getsockname.\n");
        free(sciezka);
        close(gniazdo);
        unlink(adres.sun_path);
        return -1;
    }
    printf("Sciezka : %s.\n", adres.sun_path);

    if (listen(gniazdo, 5) == -1)
    {
        perror("Serwer: Blad nasluchu gniazda.\n");
        free(sciezka);
        close(gniazdo);
        unlink(adres.sun_path);
        return -1;
    }
    for (;;)
    {
        if ((gniazdo1 = accept(gniazdo, (SockAddr *)&adres_k, &i)) == -1)
        {
            perror("Serwer: Blad funkcji accept.\n");
            continue;
        }
        ssize_t liczba_bajtow;
        char buf[1024];

        if ((liczba_bajtow = read(gniazdo1, buf, sizeof(buf))) == -1)
        {
            perror("Serwer: Blad funkcji read.\n");
            close(gniazdo1);
            continue;
        }

        buf[liczba_bajtow] = '\0';
        printf("Serwer: Przeczytalem %ld bajtow od klienta, wiadomosc od klienta : %s.\n", liczba_bajtow, buf);

        double temp = atof(buf) * atof(buf);
        sprintf(buf, "%f", temp);

        strcat(buf, " to jest z serwera");

        if (write(gniazdo1, buf, strlen(buf)) == -1)
        {
            perror("Serwer: Blad funkcji write.\n");
            close(gniazdo1);
            continue;
        }

        close(gniazdo1);
    }
}
