#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
  
#define DEVICE "/dev/myled"  

int main(void)  
{  
  
	int fd, i;  
  
	char buf[10] = {0, 1, 2, 3, 4, 5, 6, 7};  
  
	fd = open(DEVICE, O_RDWR);  
  
	if(fd < 0)  
	{  
		printf("Open /dev/myled file error\n");  
		return -1;   
	}  
  
	while(1)  
	{  
		for(i = 0; i < 8; i += 2)  
		{  
			write(fd, &buf[i], 1);  
			sleep(1);  
		}  
  
		for(i = 7; i > 0; i -= 2)  
		{  
			write(fd, &buf[i], 1);  
			sleep(1);  
		}  
  
	}  
  
	close(fd);  
	return 0;  
}  
