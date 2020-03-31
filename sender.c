#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD 0
#define STREAM_LENGTH 2048*4
#define THRESHHOLD IRRELEVANT
#define INTERVAL 100000 //0.1ms -> 10KHz
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

size_t input_stream[STREAM_LENGTH];
struct field // hope this aligns properly :o
{
    size_t mac_addr[6 * 8];
    size_t mac_src[6 * 8];
    size_t tag[4 * 8];
    size_t length[2 * 8];
};
struct field field_stream;

void* sender(void* _);

int main()
{
    printf("Sending thread-thread, with hardwareclock-sync and ethernet frames:\n\n"
           "- STREAM_LENGTH: %d\n"
           "- METHOD: %s\n",
           STREAM_LENGTH, METHOD == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload");

    //setup
    srand(time(0));
    for(int i = 0; i < STREAM_LENGTH; i++)
    {
        input_stream[i] = rand() % 2;
    }

    {   // print
        FILE* f_act = fopen("act.txt", "w");
        for(int i = 0; i < STREAM_LENGTH; i++)
        {
            fprintf(f_act, "%lu\n", input_stream[i]);
        }
        fclose(f_act);
    }

    pthread_t s;
    pthread_create(&s, NULL, sender, NULL);
    pthread_join(s, NULL);
    return 0;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void* sender(void* _)
{
    struct timespec t;
    uint64_t current;
    uint64_t target;

    loop:
    printf("send...\n");
    clock_gettime(CLOCK_MONOTONIC, &t);
    target = t.tv_sec * 1000000000;
    target += t.tv_nsec;

    for(int i = 0; i < (7 * 8 + 7); i++)
    {
        target += INTERVAL;
        while(1)
        {
            /**************************/
            if(!(i % 2))
            {
                maccess(function + 64);
            }
            else
            {
                flush(function + 64);
            }
            /**************************/
            clock_gettime(CLOCK_MONOTONIC, &t);
            current = (t.tv_sec * 1000000000) + t.tv_nsec;
            if(current >= target)
                break;
        }
    }

    target += INTERVAL;
    while(1)
    {
        /**************************/
        maccess(function + 64);
        /**************************/
        clock_gettime(CLOCK_MONOTONIC, &t);
        current = (t.tv_sec * 1000000000) + t.tv_nsec;
        if(current >= target)
            break;
    }

    for(int i = 0; i < STREAM_LENGTH; i++)
    {
        target += INTERVAL;
        while(1)
        {
            /**************************/
            if(input_stream[i])
            {
                maccess(function + 64);
            }
            else
            {
                flush(function + 64);
            }
            /**************************/
            clock_gettime(CLOCK_MONOTONIC, &t);
            current = (t.tv_sec * 1000000000) + t.tv_nsec;
            if(current >= target)
                break;
        }

    }
    goto loop;
    return NULL;
}
#pragma clang diagnostic pop