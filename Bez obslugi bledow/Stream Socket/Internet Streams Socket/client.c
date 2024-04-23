#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int g;

int main()
{
    socklen_t i = sizeof(SockAddr_in);
    SockAddr_in adres;
    struct hostent *Adres = gethostbyname("127.0.0.1");
    char buf[1024];

    g = socket(AF_INET, SOCK_STREAM, 0);
    adres.sin_family = AF_INET;
    adres.sin_port = htons(strtol("8080", NULL, 10));
    adres.sin_addr.s_addr = *(long*)(Adres->h_addr);

    connect(g, (SockAddr*)&adres, i);
    fgets(buf, sizeof(buf), stdin);

    write(g, buf, strlen(buf));

    ssize_t liczba_bajtow = read(g, buf, sizeof(buf));
    printf("%s", buf);
    close(g);

    return 0;
}
