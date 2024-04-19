#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/types.h>

int main()
{
        int file = open("/tmp/fifo", O_RDWR);

        double a;
        long b;

        double power;
        double result;

        scanf("%lf", &a);
        scanf("%ld", &b);

        write(file, &a, sizeof(double));
        write(file, &b, sizeof(long));

        sleep(1);

        read(file, &power, sizeof(double));
        result = power + 10;
        write(file, &result, sizeof(double));

        close(file);
        return 0;
}
