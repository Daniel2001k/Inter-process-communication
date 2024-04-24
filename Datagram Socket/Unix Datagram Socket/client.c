#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#define SER_PATH "./ser"
#define CLI_PATH "./cli"

int main()
{
    int server_fd;
    struct sockaddr_un server_addr;
    int num1, num2;
    socklen_t server_len = sizeof(server_addr);
    char buffer[256];
    if ((server_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, CLI_PATH);
    unlink(CLI_PATH);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SER_PATH);
    if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Podaj dwie liczby po spacji \n");
    scanf("%d %d", &num1, &num2);
    sprintf(buffer, "%d %d", num1, num2);
    send(server_fd, buffer, strlen(buffer) + 1, 0);
    printf("Wyslano \n");
    recv(server_fd, buffer, sizeof(buffer), 0);
    printf("Odczytano %s \n", buffer);
    close(server_fd);
    unlink(CLI_PATH);
}
