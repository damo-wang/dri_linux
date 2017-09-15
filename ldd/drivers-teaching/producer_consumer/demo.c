/*ioctl
2.6代码支持ioctl和unlocked_ioctl，两个都存在时，用户ioctl优先调用unlocked_ioctl
3.0代码去掉了对ioctl的支持。
用户空间原型 int ioctl(int fd,unsigned long cmd, ...);
驱动程序的ioctl和unlocked_ioctl原型如下：
int (*ioctl)(struct inode *inode,struct file *filp,unsigned int cmd, unsigned long arg);
long (*unlocked_ioctl)(struct file *filp,unsigned int cmd,unsigned long arg);
inode和filp对应用户空间应用程序传递的文件描述符fd，
cmd由用户空间不经修改的传递给驱动程序，
可选的参数arg则无论用户程序使用的是指针还是整数值，都以unsigned long的形式传递给驱动程序。如果调用程序没有传递第三个参数，那么驱动程序所接受到的arg就处在未定义状态。
为了防止对错误的设备使用正确的命令，命令号应该在系统范围内唯一。
为方便程序员创建唯一的ioctl命令号，定义号码的方法使用了4个位字段，含义如下：
type	幻数。避免了和别的设备的冲突
number	序数。命令顺序编号。
direction	若命令涉及到数据的传输，则该位字段定义数据传输方向。
size	用户数据大小。
在linux/ioctl.h中包含的头文件asm/ioctl.h中定义了一些构造命令编号的宏:
_IO（type,nr）;构造无参数的命令编号
_IOR(type,nr,datatype);构造读数据的命令编号
_IOW(type,nr,datatype);构造写数据的命令编号
_IOWR(type,nr,datatype);构造双向传输
type和nr:bumber通过参数传入，size参数通过对datatype参数取sizeof获得。
通过下面宏可以从cmd中解出4位.

*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include "demo.h"
#define SIZE 100
struct mydev
{
	struct miscdevice *mymisc;
	struct semaphore sem;
	int *data;
	int flag;
	int size;
	int count;
	int i_r;
	int i_w;
};
struct mydev *mydev;

void init_mydev(struct mydev *p)
{
	memset(p->data,0,p->size*(sizeof(int)));
	p->count=0;
	p->i_r=0;
	p->i_w=0;
}

int write_mydev(struct mydev *p,int data)
{
	int rt=0;
	if(p->count<p->size)
	{
		p->data[p->i_w]=data;
		p->count++;
		p->i_w++;
		/*if(p->i_w==p->size)
		{
			p->i_w=0;
		}*/
		p->i_w %= p->size;
		rt=0;
	}
	else
	{
		rt=-1;
	}
	
	return rt;
}

int read_mydev(struct mydev *p,int *data)
{
	if(p->count>0)
	{
		*data=p->data[p->i_r];
		p->count--;
		p->i_r++;
		p->i_r %= p->size;
		return 0;
	}
	return -1;
}

static long buffer_unlocked_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	int temp;
	int rt=0;
	if(cmd==CMD1)
	{
		//清空栈操作
		init_mydev(mydev);
	}
	if(cmd==CMD2)
	{
		//生产数据操作
		rt=write_mydev(mydev,(int)arg);
	}
	if(cmd==CMD3)
	{
		//消费数据操作
		rt=read_mydev(mydev,&temp);
		copy_to_user((int *)arg,&temp,sizeof(int));
	}
	return rt;
}

int misc_open (struct inode *inode, struct file *file)
{
	printk(KERN_INFO "open:locked\n");
	if(down_interruptible(&(mydev->sem)))
	{
		return -ERESTARTSYS;
	}
	return 0;
}
int misc_release (struct inode *inode, struct file *file)
{
	printk(KERN_INFO "release:unlocked\n");
	up(&(mydev->sem));
	return 0;
}
static struct file_operations dev_fops={
	.owner	= THIS_MODULE,
//	.ioctl	= ok6410_leds_ioctl,
	.unlocked_ioctl	= buffer_unlocked_ioctl,
	.open	= misc_open,
	.release= misc_release,
};

static struct miscdevice demoled={
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &dev_fops,
};

static int __init dev_init(void)
{
	printk(KERN_INFO "%s initialized\n",DEVICE_NAME);
	mydev=kzalloc(sizeof(struct mydev),GFP_KERNEL);
	mydev->size=SIZE;
	mydev->data=kzalloc(sizeof(int) * mydev->size,GFP_KERNEL);
	init_mydev(mydev);
	sema_init(&(mydev->sem),2);
	mydev->flag=1;
	mydev->mymisc=&demoled;
	misc_register(&demoled);
	return 0;	
}

static void __exit dev_exit(void)
{
	printk(KERN_INFO "%s exited\n",DEVICE_NAME);
	misc_deregister(&demoled);
	kfree(mydev->data);
	kfree(mydev);
}

module_init(dev_init);
module_exit(dev_exit);
