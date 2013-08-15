#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{

    int fd;
    int sw,led;

    if(argc < 2) {
        printf("Usage ledx command lednum \r\n"); 
        return;
    }

    sw = atoi(argv[1]);
    led = atoi(argv[2]);

    printf("sw: %d, led:%d \r\n", sw, led);
    fd=open("/dev/leds",O_RDWR);


    
    ioctl(fd,sw,led);
    close(fd);

    return 0;
}


