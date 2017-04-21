#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

#include <event2/event.h>
#include <event2/bufferevent.h>



void read_cb(evutil_socket_t fd,short event,void *arg)
{
    printf("in read callback\r\n");
    struct event_base*base = (struct event_base*)arg;
    event_base_loopbreak(base);
}

void sig_cb(evutil_socket_t fd,short event,void *arg)
{
    printf("In signal event callback\r\n");
    struct event  *ev = (struct event*)(arg);
    event_active(ev,event,0);
}

int main(int argc,char **argv)
{
    struct event_base* base=event_base_new();
    assert(base!=NULL);


    struct event*ev2=event_new(base,
	    1,
	    EV_READ,
	    read_cb,
	    base);
    struct event* ev=evsignal_new(base,
	    (evutil_socket_t)SIGALRM,
	    sig_cb,
	    ev2);
    event_add(ev2,NULL);
    evsignal_add(ev,NULL);//在event_add函数中，通过ev->base获取event_base对象
    alarm(5);
    event_base_dispatch(base);


    printf("exit...\r\n");
    return 0;
}


