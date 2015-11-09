/*
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <evhttp.h>

#include "log.h"


int init_log(int argc, char **argv) {
    log_fd = open("log/access.log", O_RDWR | O_CREAT | O_APPEND , 0644);
    lseek(log_fd, 0, SEEK_END);
    err_fd = open("log/err.log", O_RDWR | O_CREAT | O_APPEND , 0644);
    lseek(err_fd, 0, SEEK_END);
}

void access_log(struct evhttp_request *req) {
    char buf[LOG_SIZE];
    char ftime[30];
    int len;
    const char *uri;
    char *decoded_uri = NULL;
    struct evkeyvalq params;
    struct evkeyvalq *header;

    format_time(ftime);
    //GET
    uri = evhttp_request_uri(req);

    decoded_uri = evhttp_decode_uri(uri);
    evhttp_parse_query(decoded_uri, &params);
    header = evhttp_request_get_input_headers(req);

    //printf("GET: s=%s\n", evhttp_find_header(&params, "s"));
    //printf("Content-Length: s=%s\n", evhttp_find_header(header, "Content-Length"));

    memset(buf, 0, LOG_SIZE);
    len = sprintf(buf, "[%s]  \n", ftime);
    write(log_fd, buf, len);

    free(decoded_uri);
}

void error_log(char *str) {
    char buf[LOG_SIZE];
    char ftime[40];
    int len;

    format_time(ftime);
    memset(buf, 0, LOG_SIZE);
    len = sprintf(buf, "[%s] log %s \n", ftime, str);

    write(err_fd, buf, len);
}

void format_time(char *str) {
    char ftime[40];
    struct tm *local; 
    time_t it;  
    int len;

    time(&it);
    local=localtime(&it);  
    strftime(str,40,"%F %T",local);  
}

void log_format(req) {
    

}
