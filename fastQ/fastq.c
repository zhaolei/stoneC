#include <stdio.h>
#include <fcntl.h>
#include<unistd.h>

#include "dat.h"
#include "http.h"
#include "log.h"

char *usages = "Usage: fastq -v\n"
              "       fastq -h\n"
              "       fastq [-p/--port] <port> \n"
	          "       fastq read <id>\n";


void usage(void) {
    fprintf (stderr, "%s\n", usages) ;
    exit(1);
}

void version(void) {
    printf("fastq Ver 1.0\n");
    printf("Power By Zhaolei \n");
    exit(1);
}

void daemonize(void) {
    int fd;

    if (fork() != 0) exit(0); /* parent exits */
    setsid(); /* create a new session */

    /* Every output goes to /dev/null. If Redis is daemonized but
     * the 'logfile' is set to 'stdout' in the configuration file
     * it will not log at all. */
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) close(fd);
    }
}

int index_fd, dat_fd;    /* Input and output file descriptors */
int main(int argc, char* argv[]) {
    /*fd data index init*/
    initFd(&dat_fd, &index_fd);
    init_log(argc, argv);

    error_log("ttt");

    if (argc == 1) {
        fprintf (stderr, "%s\n", usages) ;
        return 1 ;
    }

    if (argc >= 2) {


        /* Handle special options --help and --version */
        if (strcmp(argv[1], "-v") == 0 ||
            strcmp(argv[1], "--version") == 0) version();
        if (strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "-h") == 0) usage();

        if (strcmp(argv[1], "--port") == 0 ||
            strcmp(argv[1], "-p") == 0) {

            http_init(argc, argv);
            daemonize();

        } 

        if (strcmp(argv[1], "read") == 0) {
            int id = 35000;
            char *data;
            data = readEntity(dat_fd, index_fd, id); 
            printf("dat:%s", data);
            return 1;
        }
        
    }


    //http server init
    http_init();

    /*
    int id = 35000;
    char *data;
    data = readEntity(dat_fd, index_fd, id); 
    printf("dat:%s", data);
    */

    return 0;
}
