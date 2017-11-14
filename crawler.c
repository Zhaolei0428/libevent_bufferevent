#include"crawler.h"

int port = 80;
FILE *fp;
int i=1;

int main(int argc,char **argv)
{
    struct event_base * base = NULL;
    struct bufferevent * pBufEv = NULL;

    if((fp=fopen("page.txt","w"))==NULL)
    {
         printf("file open failed!");
         return 0;
    }

    //创建事件驱动句柄
    base = event_base_new();
    //创建socket类型的bufferevent
    pBufEv = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

    //设置回调函数, 及回调函数的参数
    bufferevent_setcb(pBufEv, read_callback, NULL, event_callback, NULL);

    struct sockaddr_in tSockAddr;
    memset(&tSockAddr, 0, sizeof(tSockAddr));
    tSockAddr.sin_family = AF_INET;
    tSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    tSockAddr.sin_port = htons(port);

    //连接服务器
    if( bufferevent_socket_connect(pBufEv, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
    {
        printf("connect error\n");
        return 0;
    }

    //开始事件循环
    event_base_dispatch(base);
    //事件循环结束 资源清理

    const char *x =  event_base_get_method(base); //查看用了哪个IO多路复用模型，linux一下用epoll
    printf("METHOD:%s\n", x);

    bufferevent_free(pBufEv);
    event_base_free(base);
    fclose(fp);
    return 0;
}


//读回调处理
void read_callback(struct bufferevent * pBufEv, void * pArg)
{
    //获取输入缓存
    struct evbuffer * pInput = bufferevent_get_input(pBufEv);
    //获取输入缓存数据的长度
    int nLen = evbuffer_get_length(pInput);
    char pBody[nLen];
    bufferevent_read(pBufEv,pBody,nLen);

    //进行数据处理
    fprintf(fp,"%s",pBody);
    printf("%d %d\n",i++,nLen);
    return ;
}


//事件回调处理
void event_callback(struct bufferevent * pBufEv, short events, void * pArg)
{
    //成功连接通知事件
    if (events & BEV_EVENT_CONNECTED){
        printf("***BEV_EVENT_CONNECTED\n");
        bufferevent_enable(pBufEv, EV_READ);
        char message[512];
        sprintf(message, "GET / HTTP/1.1\r\n");
        strcat(message, "Host:");
        strcat(message, "news.sohu.com");
        strcat(message, "\r\n");
        strcat(message, "Accept: */     /* *\r\n");
        strcat(message, "User-Agent: Mozilla/4.0(compatible)\r\n");
        strcat(message, "connection:Keep-Alive\r\n");
        strcat(message, "\r\n\r\n");
        printf("%s",message);

        bufferevent_write(pBufEv,message,sizeof(message));
    }
    else if(events & BEV_EVENT_ERROR) {
        printf("***BEV_EVENT_ERROR\n");
        printf("%s\n",(char *)pArg);
    }

    return ;
}
