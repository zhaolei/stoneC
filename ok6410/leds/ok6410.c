#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
int led_disp(int argc, char *argv[])
{
    int fd;
    char file_path[30];
    memset(file_path,30,0);
       int led=argv[1][0]-'0';
       int sw=argv[2][0]-'0';
     
     
    
    
    fd=open("/dev/leds",O_RDWR);
    
     ioctl(fd,sw,led);
    if(fd < 0)
    {
        printf("Open %s Device Faild!\n",file_path);
        exit(1);
    }
    else
    	{
    	printf("open %s ok\n",file_path)	;
    	}

    
    
      //  write(fd,argv[2],1);       
	printf("write %c to %s using fd=%d\n",argv[2][0],file_path,fd);
	sleep(3);


    close(fd);

    return 0;
}
int main(int argn,char * argv[])

{

  printf("Content-Type: text/html\n\n");

  
   printf("<html>\n");

    printf("<head><title>OK6410 Test </title></head>\n");


    printf("<body><pre>\n");


  

      while(argn-->0)  

      {

       printf("arg[%d]=%s",argn,argv[argn]);


        }//注意<br \>，这个是网页上的换行，因为C语言中\n是不能在网页上起到换行作用的
      led_disp(argn,argv);
	 printf("</body>\n");


	  printf("</html>\n");


	    return 0;

	    }


