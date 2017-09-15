#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h>

dev_t dev_no;

int demo_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "my open\n");
	return 0;
}

int demo_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "my release\n");
	return 0;
}

ssize_t demo_read(struct file *filp,char __user *msg,size_t s,loff_t *lof)
{
	printk(KERN_INFO "my write\n");
	return 0;
}

ssize_t demo_write(struct file *filp,const char __user *msg,size_t s,loff_t *lof)
{
	printk(KERN_INFO "my read\n");
	return 0;
}
struct file_operations demo_fops=
{
	.owner	= THIS_MODULE,
	.open	= demo_open,
	.release= demo_release,
	.write	= demo_write,
	.read	= demo_read,
};

struct my_dev{
	struct cdev cdev;
	char *pdata;
}my_dev;


static int __init demo_init(void)
{
	printk(KERN_INFO "Driver demo3 init.\n");
	alloc_chrdev_region(&dev_no,0,4,"demo_driver");
	my_dev.pdata=kmalloc(32,GFP_KERNEL);
	cdev_init(&(my_dev.cdev),&demo_fops);
	my_dev.cdev.owner=THIS_MODULE;
	my_dev.cdev.ops	= &demo_fops;
	cdev_add(&(my_dev.cdev),dev_no,1);	
	return 0;
}

static void __exit demo_exit(void)
{
	printk(KERN_INFO "Driver demo3 exit.\n");
	cdev_del(&(my_dev.cdev));
	
	unregister_chrdev_region(dev_no,4);
}

module_init(demo_init);
module_exit(demo_exit);
