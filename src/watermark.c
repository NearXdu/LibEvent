#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

#define LISTEN_PORT 9999
#define LISTEN_BACKLOG 32

void do_accept(evutil_socket_t listener, short event,void *arg);
void read_cb(struct bufferevent *bev,void *arg);
void error_cb(struct bufferevent*bev,short event,void *arg);
void write_cb(struct bufferevent*bev,void *arg);

int main(int argc,char **argv)
{
    int ret;
    evutil_socket_t listener;//fd
    listener=socket(AF_INET,SOCK_STREAM,0);//创建描述符
    assert(listener>0);//assert

    evutil_make_listen_socket_reuseable(listener);

    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=0;
    sin.sin_port=htons(LISTEN_PORT);

    if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0) 
    {
	perror("bind");
	return 1;
    }

    if (listen(listener, LISTEN_BACKLOG) < 0) 
    {
	perror("listen");
	return 1;
    }

    printf ("Listening...\r\n");
    evutil_make_socket_nonblocking(listener);
    struct event_base *base = event_base_new();
    assert(base != NULL);

    struct event *listen_event;
    listen_event = event_new(base, //base
	    listener, //fd
	    EV_READ|EV_PERSIST,//event
	    do_accept, //call back
	    (void*)base);//args

    event_add(listen_event,NULL);
    event_base_dispatch(base);

    printf("The End.\r\n");
    return 0;

}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    evutil_socket_t fd;
    struct sockaddr_in sin;
    socklen_t slen = sizeof(sin);
    fd = accept(listener, (struct sockaddr *)&sin, &slen);
    if (fd < 0) {
	perror("accept");
	return;
    }
    if (fd > FD_SETSIZE) { //这个if是参考了那个ROT13的例子，貌似是官方的疏漏，从select-based例子里抄过来忘了改
	perror("fd > FD_SETSIZE\n");
	return;
    }

    printf("ACCEPT: fd = %u\n", fd);

    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, read_cb, NULL, error_cb, arg);
    bufferevent_setwatermark(bev,EV_READ,0,5);
    bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);
}

void read_cb(struct bufferevent *bev, void *arg)
{
    printf("in readcb\r\n");
    struct evbuffer * input;
    input = bufferevent_get_input(bev);
    evbuffer_drain(input,2);//
}

void write_cb(struct bufferevent *bev, void *arg) {}

void error_cb(struct bufferevent *bev, short event, void *arg)
{
    evutil_socket_t fd = bufferevent_getfd(bev);
    printf("fd = %u, ", fd);
    if (event & BEV_EVENT_TIMEOUT) {
	printf("Timed out\n"); //if bufferevent_set_timeouts() called
    }
    else if (event & BEV_EVENT_EOF) {
	printf("connection closed\n");
    }
    else if (event & BEV_EVENT_ERROR) {
	printf("some other error\n");
    }
    bufferevent_free(bev);
}

