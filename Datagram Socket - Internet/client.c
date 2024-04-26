#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
        int server_fd;
        struct sockaddr_in server_addr;

        server_fd = socket(AF_INET, SOCK_DGRAM, 0);
        memset(&server_addr, 0, sizeof(server_addr));

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(PORT);


        char buffer[100];
        double a,b,result;

        fgets(buffer, sizeof(buffer), stdin);

        sendto(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        socklen_t serlen = sizeof(server_addr);

        recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &serlen);

        char temp[100];

        sprintf(temp, "To jest z klienta - %s", buffer);

        memset(buffer, 0, sizeof(buffer));

        sprintf(buffer, "%s", temp);

        sendto(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        printf("Wyslano: %s\n", buffer);

        close(server_fd);

        return 0;
}
