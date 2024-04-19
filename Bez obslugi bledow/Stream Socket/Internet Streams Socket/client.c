#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int main()
{
    int gniazdo;
    socklen_t i;
    SockAddr_in adres;
    char msg[1024];
    char buf[1024];

    gniazdo = socket(PF_INET, SOCK_STREAM, 0);
    struct hostent *Adres;

    i = sizeof(SockAddr_in);
    adres.sin_family = AF_INET;
    adres.sin_port = htons(strtol("8080", NULL, 10));
    Adres = gethostbyname("127.0.0.1");
    adres.sin_addr.s_addr = *(long *)(Adres->h_addr);

    connect(gniazdo, (SockAddr *)&adres, i);
    printf("Klient: Wpisz wiadomosc do wyslania:\n");
    fgets(msg, sizeof(msg), stdin);

    write(gniazdo, msg, sizeof(msg));

    ssize_t liczba_bajtow = read(gniazdo, buf, sizeof(buf));
    printf("Klient: Odebrano %ld bajtow od serwera, wiadomosc od serwera: %s\n", liczba_bajtow, buf);
    close(gniazdo);
    return 0;
}