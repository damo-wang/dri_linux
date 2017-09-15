#include <linux/init.h>
#include <linux/module.h>
//#include <linux/moduleparam.h>

//module_lisence("BSD/GPL");
static int count=1;
static char *name="hello";
module_param(count,int,S_IRUGO);
module_param(name,charp,S_IRUGO);

static int __init demo_init(void)
{
	int i;
	printk("DEMO_INIT:hello world!\n");
	for(i=0;i<count;i++)
		printk(KERN_INFO "%s\n",name);
	return 0;
} 

static void __exit demo_exit(void)
{
	printk(KERN_INFO "DEMO_EXIT:goodbye world!\n");
}

module_init(demo_init);
module_exit(demo_exit);
