#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
 
#define BUF_SIZE 8192

struct indexq {
    int start;
    int end;
};
 
int main(int argc, char* argv[]) {
 
    int index_fd, dat_fd;    /* Input and output file descriptors */
    ssize_t ret_in, ret_out;    /* Number of bytes returned by read() and write() */
    char buffer[BUF_SIZE];      /* Character buffer */
    int i=0;
    int fstart[100];
    int flen[100];
    off_t currpos;
    struct indexq iq;

 
    /* Are src and dest file name arguments missing */
    /*
    if(argc != 3){
        printf ("Usage: ffile file1 file2\n");
        return 1;
    }
    */

 
    /*
    input_fd = open (argv [1], O_RDONLY);
    if (input_fd == -1) {
            perror ("open");
            return 2;
    }
    */
 
    /* Create output file descriptor */
    //dat_fd = open("dat", O_WRONLY | O_CREAT | O_RDONLY, 0644);
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
 
    for(i=0; i < 100; i++) {
        ret_in = sprintf(buffer, "%d data is %d ok", i, i);
        currpos = lseek(dat_fd, 0, SEEK_CUR);
        ret_out = write (dat_fd, &buffer, (ssize_t) ret_in);

        fstart[i] = (int)currpos;
        flen[i] = (int)ret_out;
        //printf("file start: %d, end:%d \n", (int)currpos, (int)ret_out);
        // index to file

        memset(&iq,0, sizeof(iq));
        iq.start = fstart[i];
        iq.end = flen[i];
        write (index_fd, &iq, sizeof(iq));
    }

    lseek(dat_fd, 0, SEEK_SET);
    lseek(index_fd, 0, SEEK_SET);
    currpos = lseek(dat_fd, 0, SEEK_CUR);
    printf("cur: %d \n", (int)currpos);
    memset(buffer,0, BUF_SIZE);
    for(i=0; i < 100; i++) {
        memset(&iq,0, sizeof(iq));
        read (index_fd, &iq, sizeof(iq));
        printf("index: %d  %d\n", iq.start, iq.end); 
        lseek(dat_fd, iq.start, SEEK_SET);

        read (dat_fd, &buffer, iq.end);
        printf("d: %s\n", buffer); 
    }

    //read (dat_fd, &buffer, flen[i]);
    //printf("d: %s\n", buffer); 
    /* Copy process */
    //while((ret_in = read (input_fd, &buffer, BUF_SIZE)) > 0){
        //printf("file: %s", buffer);
        /*
            ret_out = write (output_fd, &buffer, (ssize_t) ret_in);
            if(ret_out != ret_in){
                perror("write");
                return 4;
            }
        */
    //}
 
    /* Close file descriptors */
    close (dat_fd);
    //close (output_fd);
 
    return (EXIT_SUCCESS);
}
