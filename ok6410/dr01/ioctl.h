#ifndef __IOCTL_C_H__  
#define __IOCTL_C_H__  
typedef struct GPIO_Data_t  
{  
    unsigned char port;  
    unsigned int bit;  
    unsigned int value;  
}GPIO_Data_S;  
#define GPIO_IOC_MAGIC 12   //Documentation/ioctl-number.txt  
#define GPIO_IO_SET_GPG _IOW(GPIO_IOC_MAGIC,0,sizeof(GPIO_Data_S))  
#define GPIO_IO_GET_GPG _IOWR(GPIO_IOC_MAGIC,1,sizeof(GPIO_Data_S))  
#define GPIO_IO_WRITE _IOW(GPIO_IOC_MAGIC,2,sizeof(GPIO_Data_S))  
#define GPIO_IO_READ _IOWR(GPIO_IOC_MAGIC,3,sizeof(GPIO_Data_S))  
#endif  
