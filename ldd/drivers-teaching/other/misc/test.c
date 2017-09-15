#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd=0;

	fd=open("/dev/demo_led_device",O_RDWR);
	ioctl(fd,1);
	fclose(fd);
	return 0;
}
