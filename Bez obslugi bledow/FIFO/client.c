#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

int main()
{
    int file = open("/tmp/fifo", O_RDWR);

    double x = 10;
    long d = 2;

    scanf("%lf", &x);
    scanf("%ld", &d);

    double xPowed;
    double result;

    write(file, &x, sizeof(double));
    write(file, &d, sizeof(long));

    sleep(1);

    read(file, &xPowed, sizeof(double));

    result = xPowed + 15;
    
    write(file, &result, sizeof(double));

    close(file);
    return 0;
}