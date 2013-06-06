#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct item {
    unsigned int x;
    char c;
    char buf[50];
};

int main(int argc, char *argv[])
{
    //struct item it;
    struct item ix;
    FILE *fp;
    bzero(&ix, sizeof(ix));
    //it.x = 0x88128812;
    //it.c = 'a';
    //strncpy(it.buf, argv[0], strlen(argv[0]));
    
    fp = fopen("fo", "rb");
    if(fp  == NULL) {
        printf("err\r\n");
    }

    //fwrite(&it, sizeof(it), 1, fp);
    //fseek(fp, 0L, SEEK_SET);
    fread(&ix, sizeof(struct item), 1, fp);
    printf("ix.x %u, ix.c %c ix.buf %s \r\n", ix.x, ix.c, ix.buf);
    fread(&ix, sizeof(struct item), 1, fp);
    printf("ix.x %ud, ix.c %c ix.buf %s \r\n", ix.x, ix.c, ix.buf);

    fclose(fp);

    

    return 0;
}
