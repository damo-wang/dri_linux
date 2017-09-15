/*capability权能和受限
头文件linux/capability.h
检查函数int capable(int capability);
功能介绍，检查调用进程是否有指定的权能，返回值为真或假。
参数说明：capability可取如下宏：
CAP_DAC_OVERRIDE	越过文件或目录的访问权限
CAP_NET_ADMIN		执行网络管理任务的能力，包括那些能影响网络接口的人物
CAP_SYS_MODULE		载入或卸除内核模块的能力
CAP_SYS_RAWIO		执行“裸”I/O操作的能力
CAP_SYS_ADMIN		解惑的能力，他提供了访问许多系统管理操作的途径
CAP_SYS_TTY_CONFIG	执行tty配置任务的能力

*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/capability.h>
#include "demo.h"
static int ok6410_leds_ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg)
{
	return 0;
}
static long ok6410_leds_unlocked_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	//printk("cmd:%d\narg:%s",cmd,arg);
	if(!capable(CAP_SYS_RAWIO))
		return -EPERM;
	printk("cmd:%d\nnoarg",cmd);
	put_user();
	
	return 0;
}

static struct file_operations dev_fops={
	.owner	= THIS_MODULE,
//	.ioctl	= ok6410_leds_ioctl,
	.unlocked_ioctl	= ok6410_leds_unlocked_ioctl,
};

static struct miscdevice demoled={
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &dev_fops,
};

static int __init dev_init(void)
{
	printk(KERN_INFO "%s initialized\n",DEVICE_NAME);
	misc_register(&demoled);
	return 0;	
}

static void __exit dev_exit(void)
{
	printk(KERN_INFO "%s exited\n",DEVICE_NAME);
	misc_deregister(&demoled);
}

module_init(dev_init);
module_exit(dev_exit);
