#include <curl/curl.h>

CURLcode res;
CURL *curl;

int post_init();
int post_url(char *filename, char *data);

