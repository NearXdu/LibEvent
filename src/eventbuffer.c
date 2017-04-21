#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <sys/socket.h>
#include <string.h>

#include <netinet/in.h>
#include <netdb.h>

void eventcb(struct bufferevent*bev,short events,void *ptr)
{
    if(events&BEV_EVENT_CONNECTED)
    {
	printf("ok connected\r\n");
    }
    else if(events&(BEV_EVENT_ERROR|BEV_EVENT_EOF))
    {
	struct event_base *base=ptr;
	printf("error\r\n");
	printf("closing \r\n");
	bufferevent_free(bev);
	event_base_loopexit(base,NULL);
    }
}

void readcb(struct bufferevent*bev,void *arg )
{
    char buf[1024];
    printf("readcb\r\n");
    int n;
    struct evbuffer *input;
    input = bufferevent_get_input(bev);
    while((n=evbuffer_remove(input,buf,sizeof(buf)))>0)
    {
//	fwrite(buf,1,n,stdout);
	write(1,buf,n);
    }

}

int main(int argc,char **argv)
{

    struct event_base* base;
    struct bufferevent* bev;
    struct sockaddr_in sin;
    const char hostname[]="www.baidu.com";
    const char query[]=
	"GET / HTTP/1.0\r\n"
	"Host: www.baidu.com\r\n"
	"\r\n";


    struct hostent * h;
    h=gethostbyname(hostname);
    if(!h)
    {
	fprintf(stderr,"Couldn't lookup %s: %s",hostname,hstrerror(h_errno));
	return 1;
    }
    if(h->h_addrtype!=AF_INET)
    {
	fprintf(stderr,"No ipv6 support sorry.");
	return 1;
    }  
    //#define h_addr h_addr_list[0]

    memset(&sin,0,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_port=htons(80);
    sin.sin_addr=*(struct in_addr*)h->h_addr;

    base = event_base_new();
    assert(base != NULL);

    bev=bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev,readcb,NULL,eventcb,base);
    bufferevent_enable(bev,EV_READ|EV_WRITE);
    evbuffer_add(bufferevent_get_output(bev),query,strlen(query));

    if(bufferevent_socket_connect(bev,
		(struct sockaddr*)&sin,sizeof(sin))<0)
    {
	bufferevent_free(bev);
	return -1;
    }
    event_base_dispatch(base);
    return 0;
}


