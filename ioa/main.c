#include <stdio.h>
#include <strings.h>

int main() 
{
    char buf[100];
    char buf1[100];
    char buf2[100];
    char buf3[100];
    char buf4[100];
    
    int ret;

    while(1) {
        bzero(buf, sizeof(buf));
        bzero(buf1, sizeof(buf1));
        bzero(buf2, sizeof(buf2));
        ret = scanf("%s%s%s%s%s", buf, buf1, buf2, buf3,buf4);
        printf("ret: %d\r\n", ret);
        if(ret == EOF) {
            break;
        }
        
        printf("buf : %s \r\nbuf1: %s \r\nbuf2: %s \r\nbuf3: %s \r\nbuf4: %s\r\n", buf, buf1, buf2,buf3,buf4);
        printf("---------------------------------------\r\n");
    }
    return 0;
}

void quto_str(char *src) 
{
    char *tmp = src;
    while(*tmp) 
    {
       if(*tmp != '"') {
            *src=*tmp
       }
        tmp++;
    }
}
