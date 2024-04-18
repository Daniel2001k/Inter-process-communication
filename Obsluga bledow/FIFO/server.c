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

        if (mkfifo("/tmp/fifo", 0600 | O_CREAT) == -1) 
        {
                perror("Serwer: blad stworzenia kolejki fifo");
        }
 
        double x;
        long d;
        double xPowed;
        double result;

        while(1) 
        {
                file = open("/tmp/fifo", O_RDWR);
                if (file == -1)
                {
                        perror("Serwer: blad otwarcia kolejki");
                        return -1;
                }
 
                if (read(file, &x, sizeof(double)) == -1) 
                {
                        perror("Serwer: blad odczytu wiadomosci");
                        return -1;
                }
 
                if (read(file, &d, sizeof(double)) == -1) 
                {
                        perror("Serwer: blad odczytu wiadomosci");
                        return -1;
                }
 
                printf("Liczba x: %f\nLiczba d: %ld\n", x, d);
                xPowed = pow(x, d);
                printf("Liczba x do potegi d: %f\n", xPowed);
 
                if (write(file, &xPowed, sizeof(double)) == -1) 
                {
                        perror("Serwer: blad wyslania wiadomosci");
                        return -1;
                }

                sleep(3);
 
                if (read(file, &result, sizeof(double)) == -1) 
                {
                        perror("Serwer: blad odczytu wiadomosci");
                        return -1;
                }
                
                printf("Wynik: %f\n", result);
 
                close(file);
                sleep(1);
        }
}