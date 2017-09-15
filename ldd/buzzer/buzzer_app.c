#include <stdio.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

 

#define DEVICE "/dev/mybuzzer"

 

int main(void)

{

    int fd, i;

    char buf[10] = {0, 1};

    fd = open(DEVICE, O_RDWR);

    if(fd < 0)

    {

        printf("Open /dev/mybuzzer file error\n");

        return -1;

    }

    

    while(1)

    {

        write(fd, &buf[0], 1);

        usleep(10000);

        write(fd, &buf[1], 1);

        usleep(10000);

    }

    close(fd);

return 0;

}


