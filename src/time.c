#include "config.h"
#ifdef HANDLER_TIMES
#include <stdio.h>
#include <sys/time.h>

unsigned long avg[70], count[70];
struct timeval pre, post;

void print_times(void);
void log_time(int type, struct timeval *pre, struct timeval *post)
{
    static int init=0;
    unsigned long usecs;
    int i;

    if(!init) {
	for(i=0;i<70;i++)
	    avg[i] = count[i] = 0;
	init=1;
	atexit(print_times);
    }

    usecs = (post->tv_sec * 1000000 + post->tv_usec) - 
            ( pre->tv_sec * 1000000 +  pre->tv_usec); 
    avg[type] = ((avg[type]*count[type])+usecs)/(count[type]+1);
    count[type]++;
}
    
void print_times()
{
    int i, t_cnt=0, t_avg=0;
    
    printf("Packet handler average times:\n");
    printf("Type Count Average time (secs)\n");
    for(i=0;i<70;i++) {
	if(count[i]) {
	    printf("  %2d %5d %8.7f\n",i,count[i],(double)((double)avg[i]/1000000.0));
	    t_cnt+=count[i];
	    t_avg+=avg[i];
	}
    }
    t_avg /= t_cnt;
    printf("Totals:\n");
    printf("  -- %5d, %8.7f\n", t_cnt, (double)((double)t_avg/1000000.0));
}

start_log()
{
    gettimeofday(&pre, 0);
}

stop_log(int type)
{
    gettimeofday(&post, 0);
    log_time(type, &pre, &post);
}
#endif
