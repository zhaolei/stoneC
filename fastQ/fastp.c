#include <stdio.h>
#include "dat.h"
#include "http.h"
#include "log.h"

char *usages = "Usage: fastq -v\n"
              "       fastq -h\n"
	          "       fastq read <id>\n";


void usage(void) {
    fprintf (stderr, "%s\n", usages) ;
    exit(1);
}

void version(void) {
    printf("fastp Ver 1.0\n");
    printf("Power By Zhaolei \n");
    exit(1);
}

int index_fd, dat_fd;    /* Input and output file descriptors */
int main(int argc, char* argv[]) {
    /*fd data index init*/
    initFd(&dat_fd, &index_fd);
    init_log(argc, argv);

    error_log("test");


    if (argc >= 2) {
        /* Handle special options --help and --version */
        if (strcmp(argv[1], "-v") == 0 ||
            strcmp(argv[1], "--version") == 0) version();
        if (strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "-h") == 0) usage();

        if (strcmp(argv[1], "read") == 0) {
            int id = 35000;
            char *data;
            data = readEntity(dat_fd, index_fd, id); 
            printf("dat:%s", data);
            return 1;
        }
        
    }

    //push server init
    //push_init();

    return 0;
}
