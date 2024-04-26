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
        struct sockaddr_in server_addr, client_addr;
        socklen_t client_len = sizeof(client_addr);

        server_fd = socket(AF_INET, SOCK_DGRAM, 0);
        memset(&server_addr, 0, sizeof(server_addr));

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        bind(server_fd, (const struct sockaddr*)&server_addr, sizeof(server_addr));

        char buffer[100];
        double a,b,result;

        for (;;)
        {
                recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);

                sscanf(buffer, "%lf %lf", &a, &b);

                result = pow(a, b);

                memset(buffer, 0, sizeof(buffer));

                sprintf(buffer, "To jest z serwera - %lf", result);

                sendto(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, client_len);

                memset(buffer, 0, sizeof(buffer));

                recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);

                printf("Wynik: %s\n", buffer);
        }

        close(server_fd);

        return 0;
}
