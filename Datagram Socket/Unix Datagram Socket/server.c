#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#define SER_PATH "./ser"
#define CLI_PATH "./cli"

int main()
{
    int server_fd;
    struct sockaddr_un server_addr, client_addr;
    int num1, num2, result;
    socklen_t client_len = sizeof(client_addr);
    if ((server_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SER_PATH);
    unlink(SER_PATH);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    char buffer[256];
    while (1)
    {
        if (recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len) == -1)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        printf("Wiadmosc od klienta %s \n", buffer);
        sscanf(buffer, "%d %d", &num1, &num2);
        result = num1 + num2;
        sprintf(buffer, "%d", result);
        if (sendto(server_fd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client_addr, client_len) == -1)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        printf("Wyslano \n");
    }
    close(server_fd);
}
