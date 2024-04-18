#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
 
int main() 
{
        int file = open("/tmp/fifo", O_RDWR);
        if (file == -1) {
                perror("Klient: blad otwarcia kolejki");
                return -1;
        }
 
        double x = 10;
        long d = 2;
        
        scanf("%lf", &x);
        scanf("%ld", &d);

        double xPowed;
        double result;
 
        if (write(file, &x, sizeof(double)) == -1) {
                perror("Klient: blad wyslania wiadomosci");
                return -1;
        }
 
        if (write(file, &d, sizeof(long)) == -1) {
                perror("Klient: blad wyslania wiadomosci");
                return -1;
        }
 
        sleep(1);
 
        if (read(file, &xPowed, sizeof(double)) == -1) {
                perror("Klient: blad odczytu wiadomosci");
                return -1;
        }
 
        result = xPowed + 15;
        if (write(file, &result, sizeof(double)) == -1) {
                perror("Klient: blad wyslania wiadomosci");
                return -1;
        }
 
        close(file);
        return 0;
}