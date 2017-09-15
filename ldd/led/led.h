#ifndef __LED_H__
#define __LED_H__

#include <linux/ioctl.h>

#define DEVICE_NAME "ok6410-led"

/*幻数*/
#define LED_IOC_MAGIC  'l'
/*led命令*/
#define LED_IOCGETDAT _IOR(LED_IOC_MAGIC, 1, int)
#define LED_IOCSETDAT _IOW(LED_IOC_MAGIC, 2, int)

#define LED_IOC_MAXNR 2

#endif /* __LED_H__ */
