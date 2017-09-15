#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	char s[]="abcdefg";
	fd=open("/dev/mydev",O_WRONLY);
	write(fd,s,sizeof(s));
	close(fd);
	return 0;
}
