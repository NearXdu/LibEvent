#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <sys/socket.h>
#include <string.h>

#include <netinet/in.h>
#include <netdb.h>

void errorcb(struct bufferevent*bev,short events,void *ptr)
{
    printf("in errorcb\r\n");
    if(events&BEV_EVENT_CONNECTED)
    {
	printf("ok connected\r\n");
    }
    else if(events&(BEV_EVENT_ERROR|BEV_EVENT_EOF))
    {
	struct event_base *base=ptr;
	printf("closing... \r\n");
	bufferevent_free(bev);

	event_base_loopexit(base,NULL);
    }
}

void readcb(struct bufferevent*bev,void *arg )
{
    printf("in readcb\r\n");
    struct evbuffer *input;
    struct evbuffer *output;
    char *request_line;
    size_t len;
    input = bufferevent_get_input(bev);
    output = bufferevent_get_output(bev);


    size_t input_len = evbuffer_get_length(input);
    while(1)
    {
	request_line = evbuffer_readln(input,&len,EVBUFFER_EOL_CRLF);

	if(NULL==request_line)
	{
	    //printf("evbuffer_readln error\r\n");
	    free(request_line);
	    break;
	}
	else
	{
	    char request_buf[256];
	    snprintf(request_buf,len+5,"> %s\r\n",request_line);
	    write(1,request_buf,strlen(request_buf));
	}
    }
    char content[]="<html><head><title>This is title</title></head><body><h1>Hello</h1></body></html>";
    char tmp[]="HTTP/1.1 200 OK\r\nServer: ZhangXiao\r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n%s";
    int contentlen=strlen(content);
    char response[256];
    
    snprintf(response,256,tmp,contentlen,content);
    char *p = response;
    evbuffer_add(output,p,strlen(response));


    free(request_line);

}

void do_accept(evutil_socket_t listener,short event,void*arg)
{
    printf("in accept\r\n");
    struct event_base *base = (struct event_base *)arg;
    struct sockaddr_in ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener,(struct sockaddr *)&ss,&slen);
    assert(fd>0);


    struct bufferevent *bev;
    evutil_make_socket_nonblocking(fd);
    //使用buffer_socket_new创建一个struct bufferevent*，关联fd
    //托管给event_base，BEV_OPT_CLOSE_ON_FREE表示释放bufferevent的时候
    //关闭底层套接字等...
    bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);


    //设置对应的回调函数
    bufferevent_setcb(bev,readcb,NULL,errorcb,base);

    bufferevent_enable(bev,EV_READ|EV_WRITE);//event_add
}

int main(int argc,char **argv)
{
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base *base;
    struct event* listener_event;

    base = event_base_new();
    assert(NULL!=base);

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port=htons(1025);

    listener = socket(AF_INET,SOCK_STREAM,0);
    assert(listener>0);

    evutil_make_socket_nonblocking(listener);//fcntl的封装

    if(bind(listener,(struct sockaddr*)&sin,sizeof(sin))<0)
    {
	perror("bind");
	return -1;
    }

    if(listen(listener,16)<0)
    {
	perror("listen");
	return -1;
    }

    listener_event=event_new(base,listener,EV_READ|EV_PERSIST,do_accept,(void*)base);
    event_add(listener_event,NULL);//add to pending event lists
    event_base_dispatch(base);
    return 0;
}


