#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
 
static const char *dirServer = "/tmp/FIFO-TMP-SERWER";
 
int main() {
    pid_t processNo;
    int FIFOserver;
    int FIFOwrite;
    int FIFOread;
    char bufor[1000]; 
    char *pom = bufor;
 
 
    processNo = getpid();
    FIFOserver = open(dirServer, 1);

    strcpy(pom, "/tmp/FIFO-TMP-1-");
    pom += strlen(pom);
    sprintf(pom, "%d", (int) processNo);

    mknod(bufor, S_IFIFO | 0600, 0);
    *(pom - 2) = '2';
    mknod(bufor, S_IFIFO | 0600, 0);

    write(FIFOserver, &processNo, sizeof(pid_t));
    close(FIFOserver);
    
    FIFOwrite = open(bufor, O_WRONLY);
    *(pom - 2) = '1';
    FIFOread = open(bufor, O_RDONLY);


    double a, b;


    printf("Podaj liczbe: ");
    scanf("%lf", &a);
    write(FIFOwrite, &a, sizeof(double));
    read(FIFOread, &b, sizeof(double));
    printf("Wynik obliczen %lf^2 = %lf.\n", a, b);
    close(FIFOserver);
    close(FIFOwrite);
    close(FIFOread);
    unlink(bufor);
    *(pom - 2) = '2';
    unlink(bufor);
    return 0;
}