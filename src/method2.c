#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

int main(int argc,char **argv)
{
    struct event_base*base;
    enum event_method_feature f;
    base= event_base_new();
    if(!base)
    {
	puts("Couldn't	get an event_base!");
    }
    else
    {
	printf("Using Libevent with backend method:%s.\r\n",event_base_get_method(base));
	f=event_base_get_features(base);
	if((f&EV_FEATURE_ET))
	    printf("Edge-triggered events are supported.\r\n");
	if((f&EV_FEATURE_O1))
	    printf("O(1) event notification is supported.\r\n");
	if((f&EV_FEATURE_FDS))
	    printf("All FD types are supported.\r\n");
    }
    return 0;
}


