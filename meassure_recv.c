#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD 1
#define STREAM_LENGTH 1024*8
#define FREQUENCY 1000
#define INTERVAL 1000000000/FREQUENCY //0.1ms -> 10KHz
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

size_t payload[STREAM_LENGTH];

void* receiver(void* _);

int main()
{
    printf("Receiving process->process, with hardwareclock-sync and ethernet frames:\n\n"
           "- STREAM_LENGTH: %d\n"
           "- METHOD: %s\n"
           "- Interval in usec: %d\n",
           STREAM_LENGTH, METHOD == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", INTERVAL/1000);

    pthread_t r;
    pthread_create(&r, NULL, receiver, NULL);
    pthread_join(r, NULL);

    {
        FILE* group_A = fopen("groupA.txt", "w");
        for(int i = 0; i < STREAM_LENGTH; i+=2)
        {
            fprintf(group_A, "%lu\n", payload[i]);
        }

        fclose(group_A);

        FILE* group_B = fopen("groupB.txt", "w");
        for(int i = 1; i < STREAM_LENGTH; i+=2)
        {
            fprintf(group_B, "%lu\n", payload[i]);
        }
        fclose(group_B);
    }

    return 0;
}

void* receiver(void* _)
{
    INIT_CLOCK
    int i = 0;

    while(1)
    {
        sched_yield();
        size_t d = MEASSURE(function + 64);

        payload[i++] = d;

        if(i == STREAM_LENGTH)
            return NULL;

        WAIT_FOR_CLOCK
    }
}