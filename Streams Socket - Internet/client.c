#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int g;

int main()
{
    socklen_t i = sizeof(SockAddr_in);
    SockAddr_in adres;
    char buf[1024];

    g = socket(AF_INET, SOCK_STREAM, 0);
    memset(&adres, 0, sizeof(adres));

    adres.sin_family = AF_INET;
    adres.sin_port = htons(8080);
    adres.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(g, (SockAddr *)&adres, i);
    fgets(buf, sizeof(buf), stdin);

    write(g, buf, strlen(buf));

    ssize_t liczba_bajtow = read(g, buf, sizeof(buf));
    buf[liczba_bajtow] = '\0';

    char temp[1024];
    sprintf(temp, "To jest z klienta - %s", buf);

    memset(buf, 0, sizeof(buf));

    sprintf(buf, "%s", temp);
    printf("%s\n", buf);

    write(g, buf, sizeof(buf));

    close(g);

    return 0;
}
