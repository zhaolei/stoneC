#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct item {
    unsigned int x;
    char c;
    char buf[50];
    //char *buf;
};

int main(int argc, char *argv[])
{
    struct item it;
//    struct item ix;
    FILE *fp;
    //char *t = "Hello World";
    bzero(&it, sizeof(it));
    it.x = 0x00008181;
    it.c = 'a';
    strncpy(it.buf, argv[0], strlen(argv[0]));
    //it.buf = t;
    
    fp = fopen("fo", "wb");
    if(fp  == NULL) {
        printf("err\r\n");
    }

    fwrite(&it, sizeof(struct item), 1, fp);
    fwrite(&it, sizeof(struct item), 1, fp);
    fwrite(&it, sizeof(struct item), 1, fp);
    fwrite(&it, sizeof(struct item), 1, fp);
    //fseek(fp, 0L, SEEK_SET);
    //fread(&ix, sizeof(ix), 1, fp);

    //printf("ix.x %ud, ix.c %c \r\n", ix.x, ix.c);

    fclose(fp);

    

    return 0;
}
