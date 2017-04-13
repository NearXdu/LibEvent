//sockaddr_in
#include <netinet/in.h>
//socket
#include <sys/socket.h>
//get hostbyname
#include <netdb.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc,char **argv)
{
    const char query[]=
	"GET / HTTP/1.0\r\n"
	"Host: www.baidu.com\r\n"
	"\r\n";
    const char hostname[]="www.baidu.com";

    struct sockaddr_in sin;
    struct hostent *h;
    const char *cp;

    int fd;
    ssize_t n_written,remaining;
    char buf[1024];
    //
    h=gethostbyname(hostname);//ip
    if(!h)
    {
	fprintf(stderr,"Couldn't lookup %s: %s",hostname,hstrerror(h_errno));
	return 1;
    }
    if(h->h_addrtype!=AF_INET)
    {
	fprintf(stderr,"No ipv6 support sorry.");
	return 1;
    } fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
	perror("socket");
	return 1;
    }

    sin.sin_family=AF_INET;
    sin.sin_port=htons(80);
    sin.sin_addr=*(struct in_addr*)h->h_addr;
    if(connect (fd,(struct sockaddr *)&sin,sizeof(sin)))
    {
	perror("connect");
	close(fd);
	return 1;
    }
    cp = query;//
    remaining=strlen(query);
    while(remaining)
    {
	n_written=send(fd,cp,remaining,0);
	if(n_written <= 0)
	{
	    perror("send");
	    return 1;
	}
	remaining -= n_written;
	cp+=n_written;
    }
    //get an answer back
    while(1)
    {
	ssize_t result = recv(fd,buf,sizeof(buf),0);
	if(result==0)
	    break;
	else if(result < 0)
	{
	    perror("recv");
	    close(fd);
	    return 1;
	}
	fwrite(buf,1,result,stdout);
    }
    close(fd);
    return 0;
}


