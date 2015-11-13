

int log_fd;
int err_fd;
int plog_fd;

#define LOG_SIZE 200

int init_log(int argc, char **argv);

void access_log(struct evhttp_request *req);

void err_log(char *str);

void log_format();

void format_time(char *str);

size_t push_log(void *ptr, size_t size, size_t nmemb, void *stream);
