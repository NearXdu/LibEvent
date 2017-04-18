#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

void read_cb(evutil_socket_t fd,short event,void *arg)
{
    char buff[1024];
    memset(buff,0x00,1024);
    ssize_t n=read(fd,buff,1024);
    buff[n]='\0';
    printf("-----%s\r\n",buff);
}

int main(int argc,char **argv)
{
    struct event_base* base=event_base_new();
    assert(base!=NULL);
    struct event* ev=event_new(base,
	    (evutil_socket_t)1,
	    EV_READ|EV_PERSIST,
	    read_cb,
	    NULL);
    event_add(ev,NULL);//在event_add函数中，通过ev->base获取event_base对象
    event_base_dispatch(base);
    return 0;
}


