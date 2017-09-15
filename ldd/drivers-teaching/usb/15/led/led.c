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


#include "led.h"

unsigned long GPIOM_VA_BASE;

#define GPIOM_CON_VA 	GPIOM_VA_BASE		
#define GPIOM_DAT_VA 	GPIOM_VA_BASE+0x4		
#define GPIOM_PUD_VA 	GPIOM_VA_BASE+0x8					

#define GPIOM_PA_BASE 	0x7f008820

struct resource ok6410_led_resource = {
	.name  = "led io-mem",
	.start = GPIOM_PA_BASE,
	.end   = GPIOM_PA_BASE + 0xc,
	.flags = IORESOURCE_MEM,
};


static void ok6410_led_pin_setup(void)
{
	unsigned long start = ok6410_led_resource.start; 
	unsigned long size  = ok6410_led_resource.end - start; 
	unsigned long tmp; 
	

	request_mem_region(start, size, ok6410_led_resource.name);
	
				   
	GPIOM_VA_BASE = (unsigned long)ioremap(start, size);
	
	printk ("<1> [GPIOM_VA_BASE = 0x%lx]\n", GPIOM_VA_BASE);

	tmp = readl(GPIOM_CON_VA);
	tmp = (tmp & ~(0xffffU))|(0x1111U);
	writel(tmp, GPIOM_CON_VA);
	

	tmp = readl(GPIOM_DAT_VA);
	tmp |= 0xF;
	writel(tmp, GPIOM_DAT_VA);	
}


static void ok6410_led_pin_release(void)
{
	iounmap((void*)GPIOM_VA_BASE);
	release_mem_region(ok6410_led_resource.start,
					   ok6410_led_resource.end - ok6410_led_resource.start);
}


static unsigned long ok6410_led_getdat(void)
{
	return (readl(GPIOM_DAT_VA)& 0xF);
}


static void ok6410_led_setdat(int dat)
{
	unsigned long tmp; 
	
	tmp = readl(GPIOM_DAT_VA);
	tmp = (tmp & ~0xF) | (dat&0xF);
	writel(tmp, GPIOM_DAT_VA);
}


static long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ioarg, ret;
	
 
    if (_IOC_TYPE(cmd) != LED_IOC_MAGIC) 
        return -EINVAL;
    if (_IOC_NR(cmd) > LED_IOC_MAXNR) 
        return -EINVAL;

    switch(cmd) {
        case LED_IOCGETDAT:  
            ioarg = ok6410_led_getdat();
            ret = put_user(ioarg, (int *)arg);
            break;
        case LED_IOCSETDAT:  
            ret = get_user(ioarg, (int *)arg);
			ok6410_led_setdat(ioarg);
            break;
        default:  
            return -EINVAL;
    }
	return ret;
}

static struct file_operations dev_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= led_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	ok6410_led_pin_setup();//寄存器设置
	ret = misc_register(&misc);
	
	printk (DEVICE_NAME" initialized minor=%d\n", misc.minor);

	return ret;
}

static void __exit dev_exit(void)
{
	ok6410_led_pin_release();
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("www.3gosc.com");
