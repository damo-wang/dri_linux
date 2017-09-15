#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define DEV_NAME "/dev/bzr"

int binstr_to_int(char *binstr)
{
    int ret = 0, i = 0;
    char bnum[5];
    memset(bnum, '0', 4);
    int len = strlen(binstr);
    if (len > 4) 
        strcpy(bnum, binstr+len-4); 
    else 
        strcpy(bnum+4-len, binstr);
    
    for (i=0; i<4; i++) {
        ret <<= 1;
        ret += (bnum[i]=='0'? 1: 0);
    }
    return ret;
}

int main(int argc, char *argv[])
{
    
    if (argc > 2) {
        printf("Usage: %s <binary code>\n"
               "   eg. %s 1001 -- will turn on led 0 and 3,"
               " and turn off led 1 and 2\n", argv[0], argv[0]);
        _exit(EXIT_FAILURE);
    }

    int fd, ioarg=1000;
    if (-1==(fd=open (DEV_NAME, O_RDWR))) {
        printf("open dev error\n");
        _exit(EXIT_FAILURE);
    }
   
    	
    ioarg=argv[1][0]-48;
    printf("ioarg=%d\n", ioarg);
    switch(ioarg)
	{
	case 0:
	    ioctl(fd, 0 , ioarg);
	    break;
	case 1:
	    ioctl(fd, 1 , ioarg);
	    break;
	default:
		break;
	}

    _exit(EXIT_SUCCESS);
}
