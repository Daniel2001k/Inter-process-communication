#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

int file;

void al(int a)
{
        printf("koniec pracy serwera");
        close(file);
        unlink("/tmp/fifo");
        exit(0);
}

int main()
{
        signal(SIGTERM, al);
        signal(SIGINT, al);

        mkfifo("/tmp/fifo", 0600 | O_CREAT);

        double a;
        long b;

        double power;
        double result;

        for(;;)
        {
                file = open("/tmp/fifo", O_RDWR);

                read(file, &a, sizeof(double));
                read(file, &b, sizeof(long));

                printf("a: %lf, b: %ld\n", a, b);
                power = pow(a, b);
                printf("%lf^%f=%lf\n", a, b, power);

                write(file, &power, sizeof(double));

                sleep(1);

                read(file, &result, sizeof(double));

                printf("wynik=%lf\n", result);

                close(file);
        }

        return 0;
}
