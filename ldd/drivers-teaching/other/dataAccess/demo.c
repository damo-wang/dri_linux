/*dataAccess
头文件asm/uaccess.h
int access_ok(int type,const void *addr,unsigned long size);	//函数功能：验证用户数据空间是否可访问，参数type可选VERIFY_WRITE（可写）和VERIFY_READ（可读）,返回值1表示验证成功，0表示验证失败。
操作数据为1、2、4、8字节，使用下面优化过的函数。
put_user(datum,ptr); __put_user(datum,ptr);//把datum写到用户空间，前者内存检查，后者不检查。
get_user(local,ptr); __get_user(local,ptr);//从用户空间读数据到local,前者内存检查，后者不检查。
//拷贝大量数据使用下面函数
unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);


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
	//printk("cmd:%d\narg:%s",cmd,arg);
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
