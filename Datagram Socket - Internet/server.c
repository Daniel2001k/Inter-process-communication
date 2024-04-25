#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int sockfd;

int main() {
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUF_SIZE], response[BUF_SIZE];
    socklen_t len;
    double a;
    long b;
    char text[256];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }


    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n > 0) {
            sscanf(buffer, "%lf %ld %[^\n]", &a, &b, text);
            double result = pow(a, (double)b);
            int text_length = strlen(text);
            sprintf(response, "%f%s%d", result, text, text_length);

            sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&cliaddr, len);
        }

        memset(buffer, 0, BUF_SIZE);  // Clear the buffer for the next message

        n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        printf("%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
