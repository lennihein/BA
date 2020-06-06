#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD 1
#define STREAM_LENGTH 1024*8
#define THRESHHOLD IRRELEVANT
#define FREQUENCY 100
#define INTERVAL 1000000/FREQUENCY //0.1ms -> 10KHz
/***************************/
#define FLUSH_FLUSH_COMP >
#define FLUSH_RELOAD_COMP <=
#define FLUSH_RELOAD_MEASSURE meassure_fr
#define FLUSH_FLUSH_MEASSURE meassure_ff
#if METHOD == FLUSH_FLUSH
#define SATISFIES FLUSH_FLUSH_COMP
#define MEASSURE FLUSH_FLUSH_MEASSURE
#endif
#if METHOD == FLUSH_RELOAD
#define SATISFIES FLUSH_RELOAD_COMP
#define MEASSURE FLUSH_RELOAD_MEASSURE
#endif
#define unlikely(expr) __builtin_expect(!!(expr), 0)
/***************************/

static size_t counter = 0;


void signal_handler(int _);

int main()
{
    printf("Sending thread-thread, with hardwareclock-sync:\nInterval in usec: %zu\n",
           INTERVAL);


    signal(SIGALRM, signal_handler);

    ualarm(INTERVAL, INTERVAL);

    loop:

    while(1)
    {
        if(counter % 2)
        {
            maccess(function + 64);
        }
        else
        {
            flush(function + 64);
        }
    }

    goto loop;

}

void signal_handler(int _)
{
    counter++;
}