#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;

char path[] = "/tmp/unix";
int g;

int main()
{
    socklen_t i = sizeof(SockAddr_un);
    SockAddr_un adres;
    char buf[1024];

    g = socket(AF_UNIX, SOCK_STREAM, 0);
    bzero((char *)&adres, sizeof(SockAddr_un));
    adres.sun_family = AF_UNIX;
    strncpy(adres.sun_path, path, strlen(path));

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
