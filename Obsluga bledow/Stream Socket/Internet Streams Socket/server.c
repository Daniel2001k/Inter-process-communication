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

    if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Serwer: Blad wywolania funkcji socket.\n");
        return -1;
    }
    
    adres.sin_family = AF_INET;
    adres.sin_port = htons(strtol("8080", NULL, 10));
    adres.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(gniazdo, (SockAddr *)&adres, i) == -1)
    {
        perror("Serwer: Blad dowiazania gniazda.\n");
        close(gniazdo);
        return -1;
    }
    if (getsockname(gniazdo, (SockAddr *)&adres, &i) == -1)
    {
        perror("Serwer: Blad funkcji getsockname.\n");
        close(gniazdo);
        return -1;
    }
    printf("Serwer: Nr portu : %d\n", ntohs(adres.sin_port));
    if (listen(gniazdo, 5) == -1)
    {
        perror("Serwer: Blad nasluchu gniazda.\n");
        close(gniazdo);
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

        printf("Serwer: Polaczenie od %s z portu %d.\n", inet_ntoa(adres_k.sin_addr), ntohs(adres_k.sin_port));

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
