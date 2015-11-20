#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "dat.h" 

int writeData(int fd, void *data, int len) {
    int cur; 

    cur = lseek(fd, 0, SEEK_END);
    write (fd, data, len);
    return cur;
} 

int readData(int fd, void *data, int pos, int len) {
    int cur; 
    cur = lseek(fd, pos, SEEK_SET);
    read(fd, data, len);
    return cur;
} 

int writeIndex(int fd, void *data) {
    lseek(fd, 0, SEEK_END);
    write (fd, data, INDEX_SIZE);
    return lseek(fd, 0, SEEK_CUR);
} 

int readIndex(int fd, void *data) {
    return read(fd, data, INDEX_SIZE);
} 

int readIndexById(int fd, void *data, int id) {
    lseek(fd, id*INDEX_SIZE, SEEK_SET);
    return readIndex(fd, data);
}

int writeEntity(int dfd, int ifd, void *data, int len) {
    int cur;
    struct indexq iq;
    int id;
    cur = writeData(dfd, data, len); 

    iq.start = cur;
    iq.len = (int)len;
    id = writeIndex(ifd, (void *)&iq); 
    id = id/INDEX_SIZE;

    return id; 
}

char *readEntity(int dfd, int ifd, int id) {
    struct indexq iq;
    char *data;

    memset(&iq,0, sizeof(iq));
    readIndexById(ifd, (void *)&iq, id);
    data = malloc(iq.len+1);
    memset(data ,0, iq.len + 1);

    readData(dfd, data, iq.start, iq.len);

    return data;
}

void freeEntity(void *data) {
    free(data);
}

/*
 * dfd data file ,ifd index file   
 */
int initFd(int *dfd, int *ifd) {

    //data open
    *dfd = open("data/fastq.dat", O_RDWR | O_CREAT | O_APPEND , 0644);
    //index file open 
    *ifd = open("data/fastq.idx", O_RDWR | O_CREAT | O_APPEND , 0644);

    return 2;
} 


int datmain(int argc, char* argv[]) {
 
    int index_fd, dat_fd;    /* Input and output file descriptors */
    ssize_t ret_in, ret_out;    /* Number of bytes returned by read() and write() */
    char buffer[BUF_SIZE];      /* Character buffer */
    int i=0;
    int fstart[100];
    int flen[100];
    off_t currpos;
    struct indexq iq;

    initFd(&dat_fd, &index_fd);
 
    /* Create output file descriptor */
    //dat_fd = open("dat", O_WRONLY | O_CREAT | O_RDONLY, 0644);
    /*
    dat_fd = open("dat", O_RDWR | O_CREAT | O_APPEND , 0644);
    if(dat_fd == -1){
        perror("open");
        return 3;
    }

    index_fd = open("ind", O_RDWR | O_CREAT | O_APPEND , 0644);
    if(index_fd == -1){
        perror("open");
        return 3;
    }
 
    */
    int id;
    for(i=0; i < 10; i++) {
        ret_in = sprintf(buffer, "%d data is %d ok", i, i);
        //currpos = lseek(dat_fd, 0, SEEK_CUR);
        //ret_out = write (dat_fd, &buffer, (ssize_t) ret_in);
        //currpos = writeData(dat_fd, (void *)&buffer, (ssize_t)ret_in);
        id = writeEntity(dat_fd, index_fd, &buffer, ret_in);
        printf("data id: %d\n", id);

        //fstart[i] = (int)currpos;
        //flen[i] = (int)ret_in;
        //printf("file start: %d, end:%d \n", (int)currpos, (int)ret_out);
        // index to file

        //memset(&iq,0, sizeof(iq));
        //iq.start = fstart[i];
        //iq.len = flen[i];
        //write (index_fd, &iq, sizeof(iq));
        //printf("debug : write %d, size %d %d\n", ret_in, sizeof(iq), sizeof(struct indexq));

        //writeIndex(index_fd, (void *)&iq);
    }

    lseek(dat_fd, 0, SEEK_SET);
    lseek(index_fd, 0, SEEK_SET);
    //currpos = lseek(dat_fd, 0, SEEK_CUR);
    //printf("cur: %d \n", (int)currpos);
    memset(buffer,0, BUF_SIZE);
    for(i=0; i < 10; i++) {
        memset(&iq,0, sizeof(iq));
        memset(buffer,0, sizeof(buffer));
        readIndex (index_fd, (void *)&iq);
        readData(dat_fd, (void *)&buffer, iq.start, iq.len);
        printf("d: %s\n", buffer); 
    }

    memset(&iq,0, sizeof(iq));
    memset(buffer,0, sizeof(buffer));
    readIndexById(index_fd, (void *)&iq, 5);
    readData(dat_fd, (void *)&buffer, iq.start, iq.len);
    printf("d: %s\n", buffer); 

    ret_in = sprintf(buffer, "This data is debug data to{'data':'fff%d', \"ff\":\"ffe\"}", i);
    writeEntity(dat_fd, index_fd, &buffer, ret_in);

    id = 10;
    char *bf;
    bf = readEntity(dat_fd, index_fd, id);
    printf("d: %s\n", bf); 
    /* Close file descriptors */
    close (dat_fd);
    close (index_fd);
    free(bf);
 
    return (EXIT_SUCCESS);
}
