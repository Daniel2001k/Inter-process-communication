#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER "127.0.0.1"
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUF_SIZE], response[BUF_SIZE];
    double a = 5.0;
    long b = 3;
    char *text = "Hello Server";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER);

    sprintf(buffer, "%lf %ld %s", a, b, text);
    sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    int len = sizeof(servaddr);
    int n = recvfrom(sockfd, response, BUF_SIZE, 0, (struct sockaddr *) &servaddr, &len);
    response[n] = '\0';
    printf("Server response: %s\n", response);

    sprintf(buffer, "%s - To jest z klienta", response);
    sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    close(sockfd);
    return 0;
}
