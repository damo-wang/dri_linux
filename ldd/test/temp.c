#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
dev_t dev_id;
struct mydev
{
	struct cdev *cdev;
	char *data;
};
struct mydev *mydev;

int temp_open (struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "temp_open\n");
	return 0;
}
int temp_release (struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "temp_release\n");
	return 0;
}

ssize_t temp_read (struct file *filp, char __user *buff, size_t count, loff_t *lof)
{
	printk(KERN_INFO "temp_read\n");
	return 0;
}
ssize_t temp_write (struct file *filp, const char __user *buff, size_t count, loff_t *lof)
{
	printk(KERN_INFO "temp_write\n");
	return 0;
}
struct file_operations fop=
{
	.owner	= THIS_MODULE,
	.open	= temp_open,
	.release= temp_release,
	.write	= temp_write,
	.read	= temp_read
};
static int __init temp_init(void)
{
	alloc_chrdev_region(&dev_id,0,4,"temp_dev");
	mydev=kmalloc(sizeof(struct mydev),GFP_KERNEL);
	memset(mydev,0,sizeof(struct mydev));
	mydev->cdev=cdev_alloc();
	cdev_init(mydev->cdev,&fop);
	mydev->cdev->owner=THIS_MODULE;
	mydev->data=kmalloc(100,GFP_KERNEL);
	memset(mydev->data,0,100);
	cdev_add(mydev->cdev,dev_id,1);
	return 0;
}

static void __exit temp_exit(void)
{
	cdev_del(mydev->cdev);
	kfree(mydev->data);
	kfree(mydev);
	unregister_chrdev_region(dev_id,4);
	return;
}

module_init(temp_init);
module_exit(temp_exit);
MODULE_LICENSE("GPL");
