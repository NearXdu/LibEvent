#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

int main(int argc,char **argv)
{
    struct timeval ten_sec;
    struct event_base*base = event_base_new();

    ten_sec.tv_sec = 5;
    ten_sec.tv_usec = 0;

    /* Now we run the event_base for a series of 10-second intervals, printing
       "Tick" after each.  For a much better way to implement a 10-second
       timer, see the section below about persistent timer events. */
    while (1) {
	/* This schedules an exit ten seconds from now. */
	event_base_loopexit(base, &ten_sec);

	event_base_dispatch(base);
	puts("Tick");
    }
    return 0;
}


