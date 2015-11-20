#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

#include "log.h"
#include "post.h"

//CURLcode code;
//CURL *curl

int post_init() {
    curl = curl_easy_init();//对curl进行初始化
}

int post_url(char *filename, char *data) {

    curl_easy_setopt(curl, CURLOPT_URL, filename); //设置下载地址
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);//设置超时时间
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, push_log);//设置写数据的函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, plog_fd);//设置写数据的变量
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    res = curl_easy_perform(curl);//执行下载

    printf("ok over\n");
    if(CURLE_OK != res) {
        return 0;//判断是否下载成功
    }

    return 1;
}
int ppmain()
{
    char url[200];
    int r;
    char *tourl = "http://127.0.0.1/fastq.php";
    curl = curl_easy_init();//对curl进行初始化
    //strcpy(url, "http://www.baidu.com/");

    //int id = get_argv_id(argc, argv);
    //char *data;
    //data = readEntity(dat_fd, index_fd, id); 
    //printf("dat:%s", data);

    init_push_log();
    //plog_fd = open("log/post.log", O_RDWR | O_CREAT | O_APPEND , 0644);
    char *result;
    post_url(tourl, "fffee");

    curl_easy_cleanup(curl);//释放curl资源
    return 0;
}
