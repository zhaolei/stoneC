/*
 * libevent自带的例子，因为不考虑在windows平台下使用，删去了一点跨平台的内容
 * Compile with:
 * cc -I/usr/local/include -o signal-test signal-test.c -L/usr/local/lib -levent
 */

#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <event.h>

int called = 0;
int pos = 1;

static void signal_cb(int fd, short event, void *arg)
{
    struct event *signal = arg;
    struct timeval tv;

    time_t t = time(NULL);
    printf("%s[%s]: timer0 %u\n",__FILE__, __func__, (unsigned int)t);

    
    if(pos < called) {
        evutil_timerclear(&tv);
        tv.tv_sec = 2;
        event_add(signal, &tv);//加入事件队列
        pos++;
    }
}

int main (int argc, char **argv)
{
    struct event signal_int;
    struct timeval tv;
 
    if(argc > 1) 
    {
        called = atoi(argv[1]);
    }
    /* Initalize the event library */
    event_init();//初始化
    //可有 event_base main_base = event_init();
    //另外，有专用于http、dns、rpc等的初始化函数

    /* Initalize one event */
   // event_set(&signal_int, SIGINT, EV_SIGNAL|EV_PERSIST, signal_cb, &signal_int);

    evtimer_set(&signal_int, signal_cb, &signal_int);
    evutil_timerclear(&tv);
    tv.tv_sec = 2;
    //设置事件
    //事件类型包括EV_READ、EV_WRITE、EV_PERSIST等
    //另外有一个设置时间的函数evtimer_set(...);

    event_add(&signal_int, &tv);//加入事件队列
    //第二个参数为超时值，可以为空

    event_dispatch();//启动事件循环
    //另一种常用方式为event_base_loop(event_base类型变量/*event_base的返回值*/,0)

    return (0);
}
