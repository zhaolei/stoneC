#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <linux/ioctl.h>  
#include "ioctl.h"  
GPIO_Data_S led_ctl;  
int main(int argc, char **argv)  
{  
    int testdev = 0;  
    int i = 0;  
    int erro = 0;  
    char buf[10];  
      
    //打开设备文件  
    testdev = open ("/dev/test",O_RDWR);  
    if(testdev == -1)  
    {  
        printf("Cann't open file...../n");  
        exit(0);  
    }  
    while(1)  
    {  
        //led_ctl结构赋值  
        led_ctl.port='G';  
        led_ctl.bit=5;  
        led_ctl.value=0;  
          
        //调用ioctl函数  
        ioctl(testdev,GPIO_IO_SET,&led_ctl);  
        ioctl(testdev,GPIO_IO_WRITE,&led_ctl);  
        ioctl(testdev,GPIO_IO_READ,&led_ctl);  
        sleep(1);  
          
        led_ctl.port='G';  
        led_ctl.bit=6;  
        led_ctl.value=0;  
        ioctl(testdev,GPIO_IO_SET,&led_ctl);  
        ioctl(testdev,GPIO_IO_WRITE,&led_ctl);  
        ioctl(testdev,GPIO_IO_READ,&led_ctl);  
        sleep(1);  
    }  
    close(testdev);  
}  
