#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;

int main()
{
    int gniazdo;
    socklen_t i;
    SockAddr_un adres;
    char *sciezka;
    char msg[1024];
    char buf[1024];

    sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));

    gniazdo = socket(PF_UNIX, SOCK_STREAM, 0);
    bzero((char *)&adres, sizeof(SockAddr_un));
    adres.sun_family = AF_UNIX;
    strncpy(adres.sun_path, sciezka, strlen(sciezka));
    free(sciezka);
    i = sizeof(SockAddr_un);

    connect(gniazdo, (SockAddr *)&adres, i);
    printf("Klient: Wpisz wiadomosc do wyslania:\n");
    fgets(msg, sizeof(msg), stdin);

    write(gniazdo, msg, strlen(msg));

    ssize_t liczba_bajtow = read(gniazdo, buf, sizeof(buf));
    printf("Klient: Odebrano %ld bajtow od serwera, wiadomosc od serwera: %s\n", liczba_bajtow, buf);
    close(gniazdo);
    return 0;
}
