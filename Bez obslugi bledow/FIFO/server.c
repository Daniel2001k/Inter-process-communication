#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <math.h>
#include <unistd.h>
 
int file;
 
void signalHandler(int sig) 
{
        printf("\nserver stop\n");
        close(file);
        unlink("/tmp/fifo");
        exit(0);
}
 
int main() {
        signal(SIGINT, signalHandler);

        mkfifo("/tmp/fifo", 0600 | O_CREAT);
 
        double x;
        long d;
        double xPowed;
        double result;

        while(1) 
        {
                file = open("/tmp/fifo", O_RDWR);
 
                read(file, &x, sizeof(double));
                read(file, &d, sizeof(double));
 
                printf("Liczba x: %f\nLiczba d: %ld\n", x, d);
                xPowed = pow(x, d);
                printf("Liczba x do potegi d: %f\n", xPowed);
 
                write(file, &xPowed, sizeof(double));

                sleep(3);
 
                read(file, &result, sizeof(double));
                
                printf("Wynik: %f\n", result);
 
                close(file);
                sleep(1);
        }
}