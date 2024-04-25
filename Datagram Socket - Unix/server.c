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
    
    server_fd = socket(AF_UNIX, SOCK_DGRAM, 0));
    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SER_PATH);
    unlink(SER_PATH);
    
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    char buffer[256];
    
    while (1)
    {
        recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len)l
        printf("Wiadmosc od klienta %s \n", buffer);
        sscanf(buffer, "%d %d", &num1, &num2);
        result = num1 + num2;
        sprintf(buffer, "%d", result);
        sendto(server_fd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client_addr, client_len);
        printf("Wyslano \n");
    }
    close(server_fd);
}
