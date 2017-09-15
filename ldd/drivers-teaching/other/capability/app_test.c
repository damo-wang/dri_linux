#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char **argv)
{
	int fd;

	fd=open(argv[1],O_RDWR);
	
	ioctl(fd,*argv[2]-'0');

	close(fd);
	return 0;
	
}
