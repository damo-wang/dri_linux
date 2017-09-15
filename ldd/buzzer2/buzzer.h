#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <linux/ioctl.h>

#define DEVICE_NAME "ok6410-buzzer"

/*幻数*/
#define BZR_IOC_MAGIC  'b'
/*led命令*/
#define BZR_IOCGETDAT _IOR(BZR_IOC_MAGIC, 1, int)
#define BZR_IOCSETDAT _IOW(BZR_IOC_MAGIC, 2, int)

#define BZR_IOC_MAXNR 2

#endif /* __BUZZER_H__ */
