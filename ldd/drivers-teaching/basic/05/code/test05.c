#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
dev_t dev_id;

/*定义设备结构体*/
struct mydev 
{
	struct cdev cdev;
	int count;//用来统计设备被打开个数
	char *data;//设备私有数据
	loff_t r_i;
	loff_t w_i;
};
struct mydev mydev;

int demo_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Call demo_open.\n");
	return 0;
}

int demo_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Call demo_release.\n");
	return 0;
}

ssize_t demo_read(struct file *filp,char __user *buff,size_t count,loff_t *lofp)
{
	printk(KERN_INFO "Call demo_read.\n");
	copy_to_user(buff,mydev.data+mydev.r_i,count);
	mydev.r_i+=count;
	printk(KERN_INFO "Call demo_write.:%s\n",buff);
	
	return 0;
}

ssize_t demo_write(struct file *filp,const char __user *buff,size_t count,loff_t *lofp)
{
	printk(KERN_INFO "Call demo_write.:%s\n",buff);
	copy_from_user(mydev.data+mydev.w_i,buff,count);
	mydev.w_i+=count;
	return 0;
}

struct file_operations fops={
	.owner		= THIS_MODULE,
	.open		= demo_open,
	.release	=demo_release,
	.read		= demo_read,
	.write		= demo_write,
};


static int __init demo_init(void)
{
	int err;
	printk(KERN_INFO "demo init success!\n");
	alloc_chrdev_region(&dev_id,0,4,"demo_dev");
//	mydev.cdev=cdev_alloc();
	cdev_init(&(mydev.cdev),&fops);
	mydev.cdev.owner=THIS_MODULE;
	mydev.data=(char *)kmalloc(100,GFP_KERNEL);
	if(mydev.data==NULL)
	{
		printk(KERN_INFO "mem alloc fail\n");
	}
	mydev.r_i=0;
	mydev.w_i=0;
	err=cdev_add(&(mydev.cdev),dev_id,4);
	if(err)
	{
		printk(KERN_NOTICE "Error %d adding demo_dev.",err);
	}
	return 0;
}

static void __exit demo_exit(void)
{
	printk(KERN_INFO "demo exit success!\n");
	cdev_del(&(mydev.cdev));
	kfree(mydev.data);
	unregister_chrdev_region(dev_id,4);
}

module_init(demo_init);
module_exit(demo_exit);
