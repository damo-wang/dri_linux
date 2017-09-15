#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	char s[]="1234567890";
	fd=open("/dev/demo1",O_WRONLY);
	write(fd,s,sizeof(s));
	close(fd);
	return 0;
}
