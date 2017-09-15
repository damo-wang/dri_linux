#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "demo.h"

int main(int argc,char **argv)
{
	int fd;

	fd=open("/dev/demo_led_device",O_RDWR);
	
	ioctl(fd,CMD2,"abcde");

	close(fd);
	return 0;
	
}
