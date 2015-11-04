#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "dat.h" 

int writeData(int fd, void *data, ssize_t len) {
    int cur; 

    cur = lseek(fd, 0, SEEK_END);
    write (fd, data, len);
    return cur;
} 

int readData(int fd, void *data, int pos, int len) {
    int cur; 
    cur = lseek(fd, pos, SEEK_CUR);
    read(fd, data, len);
    return cur;
} 

int writeIndex(int fd, void *data) {
    lseek(fd, 0, SEEK_END);
    return write (fd, data, INDEX_SIZE);
} 

int readIndex(int fd, void *data, int id) {
    //lseek(fd, id * INDEX_SIZE, SEEK_CUR);
    //printf("dbug: %d %d\n", id * INDEX_SIZE, INDEX_SIZE);
    return read(fd, data, INDEX_SIZE);
} 


/*
 * dfd data file ,ifd index file   
 */
int initFd(int *dfd, int *ifd) {
    *dfd = open("dat", O_RDWR | O_CREAT | O_APPEND , 0644);
    //data file open
    if(dfd == -1){
        perror("open");
        return 3;
    }

    //index file open 
    *ifd = open("ind", O_RDWR | O_CREAT | O_APPEND , 0644);
    if(ifd == -1){
        perror("open");
        return 3;
    }

    return 2;
} 


int main(int argc, char* argv[]) {
 
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
    for(i=0; i < 10; i++) {
        ret_in = sprintf(buffer, "%d data is %d ok", i, i);
        //currpos = lseek(dat_fd, 0, SEEK_CUR);
        //ret_out = write (dat_fd, &buffer, (ssize_t) ret_in);
        currpos = writeData(dat_fd, (void *)&buffer, (ssize_t)ret_in);

        fstart[i] = (int)currpos;
        flen[i] = (int)ret_in;
        //printf("file start: %d, end:%d \n", (int)currpos, (int)ret_out);
        // index to file

        memset(&iq,0, sizeof(iq));
        iq.start = fstart[i];
        iq.len = flen[i];
        //write (index_fd, &iq, sizeof(iq));
        //printf("debug : write %d, size %d %d\n", ret_in, sizeof(iq), sizeof(struct indexq));

        writeIndex(index_fd, (void *)&iq);
    }

    lseek(dat_fd, 0, SEEK_SET);
    lseek(index_fd, 0, SEEK_SET);
    //currpos = lseek(dat_fd, 0, SEEK_CUR);
    //printf("cur: %d \n", (int)currpos);
    memset(buffer,0, BUF_SIZE);
    for(i=0; i < 10; i++) {
        memset(&iq,0, sizeof(iq));
        //read (index_fd, &iq, sizeof(iq));
        readIndex (index_fd, (void *)&iq, i);
        printf("index: %d  %d\n", iq.start, iq.len); 
        //printf("iq: %d \n", sizeof(iq)); 
        //lseek(dat_fd, iq.start, SEEK_SET);
        read (dat_fd, &buffer, iq.len);
        //readData (dat_fd, (void *)&buffer, iq.start, iq.len);
        printf("d: %s\n", buffer); 
    }

    /* Close file descriptors */
    close (dat_fd);
    close (index_fd);
 
    return (EXIT_SUCCESS);
}
