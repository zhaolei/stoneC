void get_time(int fd,short event,struct event *arg)  /*获取系统时间并将其往回write*/
{
     localtime_r(&now,&t);
     asctime_r(&t,buf);
     write( fd,buf,strlen(buf) );
 }
void con_accept(int fd,short event,void *arg) /*提供给事件的回调函数，接收一个连接*/
{
    struct sockaddr_in s_in;
    socklen_t len = sizeof(s_in);
    int ns = accept(fd,(struct sockaddr *) &s_in,&len);
    struct event *ev = malloc(sizeof(struct event));
    event_set(ev,ns,EV_WRITE,(void *)get_time,ev);
    event_add(ev,NULL);
}
                
int main()
{
    int sockfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in s_in;

    bind(sockfd,(struct sockaddr*) &s_in,sizeof(s_in));
    listen(sockfd,5);
    event_init(); //libevent初始化

    struct event ev;
    event_set(&ev,sockfd,EV_READ|EV_PERSIST,con_accept,&ev);
//设置事件属性为可读、持续,回调函数为con_accept()
    event_add(&ev,NULL);//添加事件，未设置超时时间
    event_dispatch();//进入libevent主循环
    return 0;
}
