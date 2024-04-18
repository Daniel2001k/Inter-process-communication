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

    if ((gniazdo = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Klient: Blad wywolania funkcji socket.\n");
        return -1;
    }
    bzero((char *)&adres, sizeof(SockAddr_un));
    adres.sun_family = AF_UNIX;
    strncpy(adres.sun_path, sciezka, strlen(sciezka));
    free(sciezka);
    i = sizeof(SockAddr_un);

    if (connect(gniazdo, (SockAddr *)&adres, i) == -1)
    {
        perror("Klient: Blad funkcji connect.\n");
        close(gniazdo);
        return -1;
    }
    printf("Klient: Wpisz wiadomosc do wyslania:\n");
    fgets(msg, sizeof(msg), stdin);
    msg[strcspn(msg, "\n")] = '\0'; // Usunięcie znaku nowej linii z końca stringu

    if (write(gniazdo, msg, strlen(msg)) == -1)
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
