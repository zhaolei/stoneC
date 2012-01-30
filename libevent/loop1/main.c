void get_time(int fd,short event,struct event *arg)  /*获取系统时间并将其往回write*/
{
 3……
 4    localtime_r(&now,&t);
 5    asctime_r(&t,buf);
 6    write( fd,buf,strlen(buf) );
 7……
 8}
 9void con_accept(int fd,short event,void *arg) /*提供给事件的回调函数，接收一个连接*/
10{
11    struct sockaddr_in s_in;
12    socklen_t len = sizeof(s_in);
13    int ns = accept(fd,(struct sockaddr *) &s_in,&len);
14    struct event *ev = malloc(sizeof(struct event));
15    event_set(ev,ns,EV_WRITE,(void *)get_time,ev);
16    event_add(ev,NULL);
17}
18                
19int main()
20{
21    int sockfd = socket(PF_INET,SOCK_STREAM,0);
22    struct sockaddr_in s_in;
23……
24    bind(sockfd,(struct sockaddr*) &s_in,sizeof(s_in));
25    listen(sockfd,5);
26    event_init(); //libevent初始化
27
28    struct event ev;
29    event_set(&ev,sockfd,EV_READ|EV_PERSIST,con_accept,&ev);
30//设置事件属性为可读、持续,回调函数为con_accept()
31    event_add(&ev,NULL);//添加事件，未设置超时时间
32    event_dispatch();//进入libevent主循环
33    return 0;
34}
