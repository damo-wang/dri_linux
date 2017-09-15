#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
//MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("wang");
int count=1;
module_param(count,int,S_IRUGO);

static int __init init_test(void)
{
	int i;
	for(i=0;i<count;i++)
		printk(KERN_INFO "Hello linux Kernel!\n");
	return 0;
}

static void __exit exit_test(void)
{
	printk(KERN_INFO "Goodbye linux Kernle!\n");
}

module_init(init_test);
module_exit(exit_test);
