#include <stdio.h>
#include "dat.h"
#include "http.h"
#include "log.h"
#include "post.h"
#include "conf.h"

char *usages = "Usage: fastp -v\n"
              "       fastp -h\n"
	          "       fastp read <id>\n";


void usage(void) {
    fprintf (stderr, "%s\n", usages) ;
    exit(1);
}

void version(void) {
    printf("fastp Ver 1.0\n");
    printf("Power By Zhaolei \n");
    exit(1);
}

int get_argv_id(int argc, char **argv) {
    int id = 0;

    if(argv > 2) {
        return atoi(argv[2]);
    }

    return id;
}

int index_fd, dat_fd;    /* Input and output file descriptors */
int main(int argc, char* argv[]) {
    /*fd data index init*/
    initFd(&dat_fd, &index_fd);
    init_log(argc, argv);
    init_push_log();

    //error_log("test");
    post_init();
    //char *tourl = "http://127.0.0.1/fastq.php";
    char *tourl = readConfUrl(); 

    if (argc >= 2) {
        /* Handle special options --help and --version */
        if (strcmp(argv[1], "-v") == 0 ||
            strcmp(argv[1], "--version") == 0) version();
        if (strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "-h") == 0) usage();

        if (strcmp(argv[1], "read") == 0) {
            int id = get_argv_id(argc, argv);
            char *data;
            data = readEntity(dat_fd, index_fd, id); 
            printf("dat:%s", data);
            post_url(tourl, data);
            free(data);
            return 1;
        }
    }

    free(tourl);
    curl_easy_cleanup(curl);//释放curl资源

    return 0;
}
