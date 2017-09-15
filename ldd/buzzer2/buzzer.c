#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>


#include "buzzer.h"

unsigned long GPIOF_VA_BASE;

#define GPIOF_CON_VA 	GPIOF_VA_BASE		
#define GPIOF_DAT_VA 	GPIOF_VA_BASE+0x4		
#define GPIOF_PUD_VA 	GPIOF_VA_BASE+0x8					

#define GPIOF_PA_BASE 	0x7f0080A0

struct resource ok6410_bzr_resource = {
	.name  = "bzr io-mem",
	.start = GPIOF_PA_BASE,
	.end   = GPIOF_PA_BASE + 0xc,
	.flags = IORESOURCE_MEM,
};


static void ok6410_bzr_pin_setup(void)
{
	unsigned long start = ok6410_bzr_resource.start; 
	unsigned long size  = ok6410_bzr_resource.end - start; 
	unsigned long tmp; 
	

	request_mem_region(start, size, ok6410_bzr_resource.name);
	
				   
	GPIOF_VA_BASE = (unsigned long)ioremap(start, size);
	
	printk ("<1> [GPIOF_VA_BASE = 0x%lx]\n", GPIOF_VA_BASE);

	tmp = readl(GPIOF_CON_VA);
	tmp = (tmp & ~(0xc0000000U))|(0x40000000U);
	writel(tmp, GPIOF_CON_VA);
	

	tmp = readl(GPIOF_DAT_VA);
	tmp &= ~0x80;
	writel(tmp, GPIOF_DAT_VA);	
}


static void ok6410_bzr_pin_release(void)
{
	iounmap((void*)GPIOF_VA_BASE);
	release_mem_region(ok6410_bzr_resource.start,
					   ok6410_bzr_resource.end - ok6410_bzr_resource.start);
}


static unsigned long ok6410_bzr_getdat(void)
{
	return ((readl(GPIOF_DAT_VA)& 0x80)>>15);
}


static void ok6410_bzr_setdat(int dat)
{
	unsigned long tmp; 
	
	tmp = readl(GPIOF_DAT_VA);
	tmp = (tmp & ~0x80) | (dat<<15);
	writel(tmp, GPIOF_DAT_VA);
}


static long bzr_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ioarg, ret;
	
 
    if (_IOC_TYPE(cmd) != BZR_IOC_MAGIC) 
        return -EINVAL;
    if (_IOC_NR(cmd) > BZR_IOC_MAXNR) 
        return -EINVAL;

    switch(cmd) {
        case BZR_IOCGETDAT:  
            ioarg = ok6410_bzr_getdat();
            ret = put_user(ioarg, (int *)arg);
            break;
        case BZR_IOCSETDAT:  
            ret = get_user(ioarg, (int *)arg);
			ok6410_bzr_setdat(ioarg);
            break;
        default:  
            return -EINVAL;
    }
	return ret;
}

static struct file_operations dev_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= bzr_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	ok6410_bzr_pin_setup();//寄存器设置
	ret = misc_register(&misc);
	
	printk (DEVICE_NAME" initialized minor=%d\n", misc.minor);

	return ret;
}

static void __exit dev_exit(void)
{
	ok6410_bzr_pin_release();
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("www.3gosc.com");
