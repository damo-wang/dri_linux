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
#include <plat/gpio-bank-f.h>      
#include <mach/map.h>  
//#include <mach/regs-clock.h>  
//#include <mach/regs-gpio.h>  
      
#include <plat/gpio-cfg.h>  
#include <plat/regs-gpio.h>
//#include <mach/gpio-bank-e.h>  
//#include <mach/gpio-bank-f.h>  
//#include <mach/gpio-bank-k.h>  

#define DEVICE_NAME     "pwm"  
//#define S3C64XX_GPFCON 0x7F0080A0      
#define PWM_IOCTL_SET_FREQ      1  
#define PWM_IOCTL_STOP          0  
      
static struct semaphore lock; //信号量  
      
/* freq:  pclk/50/16/65536 ~ pclk/50/16   
      * if pclk = 50MHz, freq is 1Hz to 62500Hz  
      * human ear : 20Hz~ 20000Hz  
      */  
static void PWM_Set_Freq( unsigned long freq )  
{  
	unsigned long tcon;  
        unsigned long tcnt;  
        unsigned long tcfg1;  
        unsigned long tcfg0;  
      
        struct clk *clk_p;  
        unsigned long pclk;  
      
        unsigned tmp;  
      
        tmp = readl(S3C64XX_GPFCON);    
        //tmp &= ~(0x3U << 28);  
        //tmp |=  (0x2U << 28);  
      
             tmp &=~(0x3U << 30);  //GPF15  [31:30]    10 = PWM TOUT[1]   
             tmp |=  (0x2U << 30);  
      
        writel(tmp, S3C64XX_GPFCON);  
      
        tcon = __raw_readl(S3C_TCON);  //定时器控制寄存器  
        tcfg1 = __raw_readl(S3C_TCFG1); //时钟多路复用器和 DMA模式的选择  
        tcfg0 = __raw_readl(S3C_TCFG0); //时钟预定标器和死区结构  
      
        //prescaler = 50   Prescaler 0 [7:0] R/W Prescaler 0 value for timer 0 & 1   
        tcfg0 &= ~S3C_TCFG_PRESCALER0_MASK;  //{prescaler value} = 1~255    
        tcfg0 |= (50 - 1);  //prescaler value=50  
      
        //mux = 1/16  TCFG1  Divider MUX0 [3:0] R/W Select Mux input for PWM Timer 0   
        tcfg1 &= ~S3C_TCFG1_MUX0_MASK;  
        tcfg1 |= S3C_TCFG1_MUX0_DIV16; // 0100: 1/16    
      
        __raw_writel(tcfg1, S3C_TCFG1);  
        __raw_writel(tcfg0, S3C_TCFG0);  
      
      
    /* clk_get获取一个名为id的时针  
     * 输入参数dev:   可以为NULL  
     * 输入参数id:    时针名称,如fclk、hclk、pclk等  
     * 返回值:        返回该时钟的clk结构体  
     *  
     *再将clk_get返回的clk结构体传递给clk_get_rate，获取该时钟的频率  
     */  
    /*PCLK is used for APB bus, which is used by the peripherals   
     *such as WDT, IIS, I2C, PWM timer, MMC */  
       
        clk_p = clk_get(NULL, "pclk");  //获取一个名为id的时针  
        pclk  = clk_get_rate(clk_p);  //获取该时钟的频率  
        //定时器输入时钟频率   
        //Timer input clock Frequency = PCLK / ( {prescaler value + 1} ) / {divider value}   
        tcnt  = (pclk/50/16)/freq;    
      
        __raw_writel(tcnt, S3C_TCNTB(0)); //TCNTB0:定时器0计数缓冲器。   
        __raw_writel(tcnt/2, S3C_TCMPB(0));//TCMPB0:定时器0比较缓冲寄存器。   
                      
        tcon &= ~0x1f;  
        tcon |= 0xb;        //disable deadzone, auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 0  
        __raw_writel(tcon, S3C_TCON);  
          
    /*Note: Manual update bit must be 1’b0 before Start/Stop bit is 1’b1.   
       If Manual update bit is 1’b1 and Start/Stop bit is 1’b1,   
       timer counter is not update by new value.   
       Timer counter value is last value. */  
          
        tcon &= ~2;         //clear manual update bit  
        __raw_writel(tcon, S3C_TCON);  
}  
      
void PWM_Stop( void )  
{  
        unsigned tmp;  
        tmp = readl(S3C64XX_GPFCON);  
        //tmp &= ~(0x3U << 28);  
            tmp &= ~(0x3U << 30);  
      
        writel(tmp, S3C64XX_GPFCON);  
}  
      
/*　该函数尝试获得信号量sem，如果能够立刻获得，  
    它就获得该信号量并返回0，否则，返回非0值。  
    它不会导致调用者睡眠，可以在中断上下文使用。  
      
    */  
static int s3c64xx_pwm_open(struct inode *inode, struct file *file)  
{  
        if (!down_trylock(&lock))  
            return 0;  
        else  
            return -EBUSY;  
}  
      
      
static int s3c64xx_pwm_close(struct inode *inode, struct file *file)  
{  
        up(&lock);  //该函数释放信号量sem，唤醒等待者  
        return 0;  
}  
      
      
static long s3c64xx_pwm_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)  
{  
        switch (cmd) {  
            case PWM_IOCTL_SET_FREQ:  
                if (arg == 0)  
                    return -EINVAL;  
                PWM_Set_Freq(arg);  
                break;  
      
            case PWM_IOCTL_STOP:  
            default:  
                PWM_Stop();  
                break;  
}  
      
	return 0;  
}  
      
      
static struct file_operations dev_fops = {  
        .owner          = THIS_MODULE,  
        .open           = s3c64xx_pwm_open,  
        .release        = s3c64xx_pwm_close,   
        .unlocked_ioctl = s3c64xx_pwm_ioctl,  
};  
      
static struct miscdevice misc = {  
        .minor = MISC_DYNAMIC_MINOR,  
        .name = DEVICE_NAME,  
        .fops = &dev_fops,  
};  
      
static int __init dev_init(void)  
{  
        int ret;  
      
    /*　该函数用于初始化一个互斥锁，即它把信号量sem的值设置为1，  
    等同于sema_init (struct semaphore *sem, 1)*/  
        init_MUTEX(&lock);  
        ret = misc_register(&misc);  
      
        printk (DEVICE_NAME"\tinitialized\n");  
            return ret;  
}  
      
static void __exit dev_exit(void)  
{  
        misc_deregister(&misc);  
}  
      
module_init(dev_init);  
module_exit(dev_exit);  
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("FORLINX Inc.");  
MODULE_DESCRIPTION("S3C6410 PWM Driver");  
