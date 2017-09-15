#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/irq.h>
#include <mach/gpio.h>
#include <plat/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/hardware.h>
#include <linux/io.h>

#define BUZZER_MAJOR 240

int buzzer_open(struct inode *inode, struct file *filp)
{
    unsigned int tmp;
    tmp = readl(S3C64XX_GPFCON);
    tmp = (tmp & ~(0xc0000000) | (0x40000000)); //set the GPIO output mode
    writel(tmp, S3C64XX_GPFCON);
    printk("$$$$$$$$$$$buzzer_open$$$$$$$$$\n");
return 0;
}

ssize_t buzzer_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk("$$$$$$$$$$buzzer_read$$$$$$$$$\n");
    return count;
}

ssize_t buzzer_write(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    char mbuf[10];
    unsigned int tmp;
    copy_from_user(mbuf,buf,count);
    switch(mbuf[0])
    {
        case 0:
            tmp = readl(S3C64XX_GPFDAT);
            tmp |= (0x8000);
            writel(tmp, S3C64XX_GPFDAT);
            break;
        case 1:
            tmp = readl(S3C64XX_GPFDAT);
            tmp &= ~(0x8000); 
            writel(tmp, S3C64XX_GPFDAT);
            break;
        default:
            break;
    }
    printk("$$$$$$$$$$buzzer_write$$$$$$$$$\n");
return count;
}

int buzzer_release(struct inode *inode, struct file *filp)
{
    printk("$$$$$$$$$$buzzer_release$$$$$$$$$\n");
return 0;
}

struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = buzzer_open,
    .read = buzzer_read,
    .write = buzzer_write,
    .release = buzzer_release,
};

static int buzzer_init(void)
{
    int rc;
    printk("Test buzzer dev\n");
    rc = register_chrdev(BUZZER_MAJOR, "buzzer", &my_fops);
    if(rc < 0)
    {
        printk("register %s  dev error\n", "buzzer");
        return -1;
    }
    printk("$$$$$$$$$ register buzzer dev OK\n");
return 0;
}

static void buzzer_exit(void)
{
    unregister_chrdev(BUZZER_MAJOR, "buzzer");
    printk("Good Bye!\n");
}

MODULE_LICENSE("GPL");
module_init(buzzer_init);
module_exit(buzzer_exit);
