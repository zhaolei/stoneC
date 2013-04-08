ifneq ($(KERNELRELEASE),)
    obj-m :=t1.o #说明要生成的文件此处要与驱动文件名对应
else
#驱动程序调用linux源码中的文件进行编译，所以编译驱动模块的时候你的linux  环境中要有一个完整的linux源码哦，KERNELDIR 指明你源码的路径*/
    KERNELDIR ?= /stone/c/tar/linux-3.0.1/ 
    PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules  
#编译驱动模块
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c
#make clean的工作
endif
