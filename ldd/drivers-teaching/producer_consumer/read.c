#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "demo.h"

int main(int argc,char **argv)
{
	int fd;
	int num=0;
	int i=0;
	fork();
	fork();
	fork();
	fd=open("/dev/buffer_device",O_RDONLY);
	for(i=0;i<10;i++)
	{
		if(ioctl(fd,CMD3,&num)==0)
		{
			printf("%d:读到:%d\n",getpid(),num);
		}
		else
		{
			printf("%d:读取失败\n",getpid());
		}
	}

	close(fd);
	return 0;
	
}
