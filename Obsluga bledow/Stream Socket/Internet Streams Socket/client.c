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

    if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Klient: Blad wywolania funkcji socket.\n");
        return -1;
    }
    struct hostent *Adres;

    i = sizeof(SockAddr_in);
    adres.sin_family = AF_INET;
    adres.sin_port = htons(strtol("8080", NULL, 10));
    Adres = gethostbyname("127.0.0.1");
    adres.sin_addr.s_addr = *(long *)(Adres->h_addr);

    if (connect(gniazdo, (SockAddr *)&adres, i) == -1)
    {
        perror("Klient: Blad funkcji connect.\n");
        close(gniazdo);
        return -1;
    }
    printf("Klient: Wpisz wiadomosc do wyslania:\n");
    fgets(msg, sizeof(msg), stdin);

    if (write(gniazdo, msg, sizeof(msg)) == -1)
    {
        perror("Klient: Blad funkcji write.\n");
        close(gniazdo);
        return -1;
    }

    ssize_t liczba_bajtow;
    if ((liczba_bajtow = read(gniazdo, buf, sizeof(buf))) == -1)
    {
        perror("Klient: Blad funkcji read.\n");
        close(gniazdo);
        return -1;
    }
    printf("Klient: Odebrano %ld bajtow od serwera, wiadomosc od serwera: %s\n", liczba_bajtow, buf);
    close(gniazdo);
    return 0;
}