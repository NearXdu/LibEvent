#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

int main(int argc,char **argv)
{
    int	i;
    const char **methods=event_get_supported_methods();
    printf("Starting Libevent%s. \nAvailable methods are:\n",
	    event_get_version());
    for	(i=0;methods[i]	!=NULL;++i)	
    {
	printf("%s\n",	methods[i]);
    }
    return 0;
}


