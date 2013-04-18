#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX 20

int main()
{

    char buf[20];
    int status;
    pid_t pid;
    pid_t wpid;
    int len = 0;

    while(fgets(buf, MAX, stdin)>0) {
        len = strlen(buf);
        buf[len-1]='\0';
        
        pid = fork();
        if(pid == 0) {
            execlp(buf, buf, (char *)0);
            printf("err: %s \n", strerror(errno));
            exit(0);
        }


        wpid = wait(&status);
        printf("wpid:%d  %d \n", wpid,status);
    }

    return 0;
}
