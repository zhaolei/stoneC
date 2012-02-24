#include <stdio.h>

int main() 
{
    char buf[20];
    char *f;
    int x;
    x =getc(stdin);
    putc(x, stdout);

    x = fgetc(stdin);
    printf("\r\nx: %d  ", x);
    fputc(x, stdout);
    
    f = fgets(buf, 20, stdin);
    fputs(buf,stdout);

    return 0;

}
