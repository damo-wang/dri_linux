/*混杂设备
因为很多字符设备都有相同的特性，所以为简化操作，将这些字符设备抽象出特性，简化操作方法，形成一类新的设备，混杂设备。
混杂设备主设备号默认为10，每个混杂设备可设定自己的次设备号，因此一个混杂设备只能有对应一个设备。
头文件include/linux/miscdevice.h
实现代码:drivers/char/misc.c

混杂设备的表示
struct miscdevice{
	int minor;	//次设备号，MISC_DYNANIC_MINOR表示自动分配
	const char *name;
	const struct file_operations *fops;
	struct list_head list;
	struct device *parent;
	struct device *this_device;
	const char *nodename;
	mode_t mode;
};
设备注册函数misc_register(struct miscdevice *);
设备注销函数misc_deregister(struct miscdevice *);
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include "demo.h"
static int ok6410_leds_ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg)
{
	return 0;
}
static long ok6410_leds_unlocked_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	return 0;
}

int misc_open (struct inode *inode, struct file *filp)
{
	printk("dev_t:%x\n",(inode->i_rdev));
	return 0;
}
int misc_release (struct inode *inode, struct file *filp)
{
	printk("dev_t:%x\n",(inode->i_rdev));
	return 0;
}
static struct file_operations dev_fops={
	.owner	= THIS_MODULE,
	.open	= misc_open,
	.release=misc_release,
//	.unlocked_ioctl	= ok6410_leds_ioctl,
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
