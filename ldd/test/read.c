#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd;
	char s[100]="";
	fd=open("/dev/mytemp",O_RDONLY);
	read(fd,s,3);
	close(fd);
	printf("%s\n",s);
	return 0;
}
