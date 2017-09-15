#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>

dev_t dev_id;

static int __init test_init(void)
{
	printk(KERN_INFO "test init success!\n");
	alloc_chrdev_region(&dev_id,0,4,"demo_dev");
	return 0;
}

static void __exit test_exit(void)
{
	printk(KERN_INFO "test exit success!\n");
	unregister_chrdev_region(dev_id,4);
}

module_init(test_init);
module_exit(test_exit);
