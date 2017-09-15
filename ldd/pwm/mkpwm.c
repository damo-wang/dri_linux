#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <plat/regs-timer.h>
#include <mach/regs-irq.h>
#include <asm/mach/time.h>
#include <linux/clk.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#include <plat/gpio-cfg.h>
#include <plat/regs-clock.h>
#include <plat/regs-gpio.h>

#include <plat/gpio-bank-e.h>

#include <linux/timer.h>
#include <linux/irq.h>
#define DEVICE_NAME "mkpwm"
#define mkpwm_irq IRQ_TIMER1



struct cdev *mkpwm_cdev;
unsigned int counter=0;
unsigned int pwm[]={110,55};
unsigned long tmp;
static irqreturn_t mkpwm_interrupt(int irq , void *dev_id)
{
if(irq !=IRQ_TIMER1){
printk("bad irq % d in timer0 /n", irq);
return -1;
}

// printk("good irq % d in timer0 /n", irq);


counter++;
if(counter ==pwm[0])
{
tmp = readl(S3C64XX_GPEDAT);
tmp |= (0x4);
writel(tmp, S3C64XX_GPEDAT);

}
if(counter ==pwm[1])
{
tmp = readl(S3C64XX_GPEDAT);
tmp |= (0x10);
writel(tmp, S3C64XX_GPEDAT);

}
if(counter ==600)
{
tmp = readl(S3C64XX_GPEDAT);
tmp &= ~(0x14);
writel(tmp, S3C64XX_GPEDAT);
counter=0;

}

   
return IRQ_RETVAL(IRQ_HANDLED);
}


void mkpwm_timerinit(void)
{

unsigned long tcfg0,tcfg1,tcon,cstat;
struct clk *clk_p;
unsigned long pclk,tcnt;
unsigned long freq=20000;

{
unsigned temp;
temp = readl(S3C64XX_GPEPUD);
temp = (temp & ~(0x33U<<4))|(0x22U<<4); //设置GPE2 和GPE4口上拉
writel(temp, S3C64XX_GPEPUD);

temp = readl(S3C64XX_GPECON);
temp = (temp & ~(0x0f0fU<<8))|(0x0101U<<8); //设置GPE2 和GPE4口为输出
writel(temp, S3C64XX_GPECON);

temp = readl(S3C64XX_GPEDAT);
temp |= (0x14);
writel(temp, S3C64XX_GPEDAT);
}



tcon = __raw_readl(S3C_TCON);
tcfg1 = __raw_readl(S3C_TCFG1);
tcfg0 = __raw_readl(S3C_TCFG0);

//prescaler = 50
tcfg0 &= ~S3C_TCFG_PRESCALER0_MASK;
tcfg0 |= (50 - 1);

//mux = 1/16
tcfg1 &= ~S3C_TCFG1_MUX1_MASK;
tcfg1 |= S3C_TCFG1_MUX1_DIV4;

__raw_writel(tcfg1, S3C_TCFG1);
__raw_writel(tcfg0, S3C_TCFG0);


clk_p = clk_get(NULL, "pclk");
pclk = clk_get_rate(clk_p);
tcnt = (pclk/50/16)/freq;
printk("TCNT is %ld/n",tcnt);
__raw_writel(tcnt, S3C_TCNTB(1));
__raw_writel(0, S3C_TCMPB(1));

 //定时器1中断使能
cstat = __raw_readl(S3C_TINT_CSTAT);
//cstat &=~S3C_TINT_CSTAT_T0INTEN;
cstat |=0x2;
__raw_writel(cstat,S3C_TINT_CSTAT);

tcon &= ~0xf1<<4;
 tcon |= 0xb<<8; //disable deadzone, auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 1
__raw_writel(tcon, S3C_TCON);

tcon &= ~0x2<<8; //clear manual update bit
__raw_writel(tcon, S3C_TCON);
printk("init mkpwm success!/n");
}
static int mkpwm_open(struct inode *inode,struct file *filp)
{  
printk("Register mkpwm success!/n");
return 0;
}



static int mkpwm_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned int arg)
{

switch (cmd)
{
case 0:
pwm[0]=arg;
break;
case 1:
pwm[1]=arg;
break;

default:
return -EINVAL;
}
return 0;
}

static int mkpwm_close(struct inode *inode,struct file *filp)
{
  // free_irq(mkpwm_irq,NULL);
return 0;
}

static struct file_operations mkpwm_fops={
.owner=THIS_MODULE,
.open=mkpwm_open,
.ioctl=mkpwm_ioctl,
.release=mkpwm_close,
};

static struct miscdevice misc = {
.minor = MISC_DYNAMIC_MINOR,
.name = DEVICE_NAME,
.fops = &mkpwm_fops,
};

static int __init mkpwm_init(void)
{
int ret;
ret = misc_register(&misc);
printk (DEVICE_NAME"/tinitialized/n");

mkpwm_timerinit();
ret=request_irq(mkpwm_irq,mkpwm_interrupt,IRQ_TYPE_LEVEL_HIGH, DEVICE_NAME, NULL);
if(ret<0){
printk("Register IOPWM failed!/n");
return ret;
}
printk("Register mkpwm success!/n");
return ret;
}

static void __exit mkpwm_exit(void)
{
free_irq(mkpwm_irq,NULL);
misc_deregister(&misc);
}

module_init(mkpwm_init);
module_exit(mkpwm_exit);

MODULE_LICENSE("GPL");
