/************* 
linux headers 
**************/  
#include <linux/types.h>  
#include <linux/fs.h>  
#include <linux/mm.h>  
#include <linux/errno.h>  
#include <linux/module.h>  
#include <linux/moduleparam.h>  
#include <linux/kernel.h>  
#include <asm/uaccess.h>  
#include <linux/cdev.h>  
#include <linux/ioctl.h>  
#include <linux/slab.h>  
#include <linux/fcntl.h>  
#include <asm/segment.h>  
#include <asm/io.h>  
//#include <asm/arch/regs-gpio.h>  
//#include <mach/gpio.h> 
#include <mach/map.h> 
#include <mach/regs-gpio.h> 
#include <mach/gpio-bank-g.h> 
//#include <mach/gpio-bank-k.h> 
#include "ioctl.h"  
//设备主次设备号  
unsigned int test_major=253;  
unsigned int test_minor=0;  
struct cdev cdevc;  
MODULE_LICENSE("Dual BSD/GPL");  
GPIO_Data_S led;  

static void led_ctl()  
{  
    //G50 时写寄存器，具体参见s3c2440 datasheet  
    //这里主要是控制开发板的led等闪烁，每秒闪以下  
    if(led.port=='G'&& led.bit==5 && led.value==0)  
    {  
        printk("G50/n");  
        __raw_writel(0x400,S3C64XX_GPGCON);  
                __raw_writel(0xffdf,S3C64XX_GPGDAT);  
    }  
    //G60 时写寄存器  
    else if(led.port=='G' && led.bit==6 && led.value==0)  
    {  
        printk("G60/n");      
        __raw_writel(0x1000,S3C64XX_GPGCON);  
                __raw_writel(0xffbf,S3C64XX_GPGDAT);  
    }  
}  
static int read_test(struct file *file,char *buf,int count ,loff_t *f_pos)  
{  
    printk("/n read_test");  
    printk("%d",count);  
    return count;  
}  
static int write_test(struct file *file,const char *buf,int count, loff_t *f_pos)  
{     
    printk("/n write_test");  
    return count;  
}  
static int ioctl_test(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)  
{  
    if(copy_from_user(&led,arg,sizeof(GPIO_Data_S)))  
    {  
        return - EFAULT;  
    }  
    printk("%c %d %d/n",led.port,led.bit,led.value);  
    printk("/n ioctl_test");  
    switch(cmd)  
    {  
        //case GPIO_IO_SET_GPG :led_ctl();break;  
        //case GPIO_IO_GET_GPG :led_ctl();break;  
    //  case GPIO_IO_WRITE :printk("cmd3") ;break;  
    //  case GPIO_IO_READ :printk("cmd3") ;break;  
        default:break;  
    }  
    return cmd;  
}  
static int open_test(struct inode *inode,struct file *file)  
{  
    printk("/n open_test");  
    return 0;  
}  
static void release_test(struct inode *inode,struct file *file)  
{  
    printk("/n release_test");  
}  
struct file_operations test_fops = {  
    .owner=THIS_MODULE,  
    .read=read_test,  
    .write=write_test,  
    .open=open_test,  
    //.ioctl=ioctl_test,  
    .release=release_test,  
};  
int simple_c_init_module(void)  
{  
    int result;  
    dev_t dev = 0;  
      
    dev=MKDEV(test_major,test_minor);  
    result = register_chrdev_region(dev,1,"test");  
      
    printk("major= %d,minor=%d /n",test_major,test_minor);  
      
    if(result < 0)  
    {  
        printk(KERN_INFO "test : can't get major number/n");  
        return result;  
    }  
      
    cdev_init(&cdevc,&test_fops);  
    cdevc.owner= THIS_MODULE;  
    cdevc.ops=&test_fops;  
    result=cdev_add(&cdevc,dev,1);  
    if(result)  
        printk("Error %d adding test",result);  
    return 0;  
}  
void simple_c_cleanup_module(void)  
{  
    dev_t dev=0;  
    dev = MKDEV(test_major,test_minor);  
    cdev_del(&cdevc);  
    unregister_chrdev_region(dev,1);  
}     
module_init(simple_c_init_module);  
module_exit(simple_c_cleanup_module);  
