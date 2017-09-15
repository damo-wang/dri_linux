#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "demo.h"

int main(int argc,char **argv)
{
	int fd;
	int i=0;

	fd=open("/dev/buffer_device",O_WRONLY);
	ioctl(fd,CMD1);
	for(i=0;i<100;i++)
	{
		ioctl(fd,CMD2,i*10);
	}

	close(fd);
	return 0;
	
}
