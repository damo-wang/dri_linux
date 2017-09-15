#ifndef __DEMO_H
#define __DEMO_H
#include <linux/ioctl.h>
#define DEVICE_NAME "demo_led_device"
int CMD1=_IOWR(1,1,int);
int CMD2=_IOWR(1,2,char *);
int CMD3=_IO(1,3);

#endif
