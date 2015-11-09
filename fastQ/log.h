

int log_fd;
int err_fd;

#define LOG_SIZE 200

int init_log(int argc, char **argv);

void access_log(struct evhttp_request *req);

void err_log(char *str);

void log_format();

void format_time(char *str);
