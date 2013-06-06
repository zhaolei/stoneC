#include <stdio.h>
#include <stdlib.h>

typedef struct _item {
    int d;
    char k[1];
}item;

int main()
{
    item x;
    char *t = x.k;
    char buf[] = "Hello World";
    //*t = buf;
    x.k = &buf;
    return 0;
}
