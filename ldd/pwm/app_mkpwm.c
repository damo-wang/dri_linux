#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/ioctl.h>

int main(int argc, char **argv)

{

    int pwm;

    int pwm_number;

    int fd;

    if (argc != 3 || sscanf(argv[1], "%d", &pwm_number) != 1 || sscanf(argv[2],"%d", &pwm) != 1)

    {

        fprintf(stderr, "input error/n");

        exit(1);

    }

    fd = open("/dev/mkpwm", 0);

    if (fd < 0)

    {

        perror("open device /dev/mkpwm");

        exit(1);

     }

    ioctl(fd, led_number , pwm);

    close(fd);

    return 0;

}
