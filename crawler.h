#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define EV_TIMEOUT      0x01
#define EV_READ         0x02
#define EV_WRITE        0x04
#define EV_SIGNAL       0x08
#define EV_PERSIST      0x10
#define EV_ET           0x20

#include<event2/event.h>
#include<event2/bufferevent.h>
#include<event2/buffer.h>

//读回调函数
void read_callback(struct bufferevent * pBufEv, void * pArg);
void event_callback(struct bufferevent * pBufEv, short sEvent, void * pArg);
