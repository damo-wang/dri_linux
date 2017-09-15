#ifndef __DEMO_H
#define __DEMO_H
#include <linux/ioctl.h>
#define DEVICE_NAME "buffer_device"
int CMD1=_IO(1,1);
int CMD2=_IOW(1,2,int);
int CMD3=_IOR(1,3,char *);

#endif
