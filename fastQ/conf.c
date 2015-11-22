
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <libconfig.h>

char * readConfUrl() {

    char *conf_file = "conf/fastq.conf";
    config_t cfg;
    const char *str;
    char *url;

    config_init(&cfg);
    if(! config_read_file(&cfg, conf_file)) {
        printf("read err\n");
        config_destroy(&cfg);
    }

    config_lookup_string(&cfg, "url", &str);
    url = (char *)malloc(strlen(str));
    strcpy(url, str);

    config_destroy(&cfg);

    return url;
}

int ittmain() {
    char *url = readConfUrl();
    printf("url:%s\n", url);
    return 0;
}

