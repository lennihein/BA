#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD 0
#define STREAM_LENGTH 2048*4
#define THRESHHOLD 190
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

size_t output_stream[STREAM_LENGTH];
struct field // hope this aligns properly :o
{
    size_t mac_addr[6 * 8];
    size_t mac_src[6 * 8];
    size_t tag[4 * 8];
    size_t length[2 * 8];
};
struct field field_stream;

void* receiver(void* _);

int main()
{
    printf("Receiving process->process, with hardwareclock-sync and ethernet frames:\n\n"
           "- STREAM_LENGTH: %d\n"
           "- METHOD: %s\n"
           "- THRESHHOLD: %d\n",
           STREAM_LENGTH, METHOD == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", THRESHHOLD);

    pthread_t r;
    pthread_create(&r, NULL, receiver, NULL);
    pthread_join(r, NULL);

    {
        FILE* f_pred = fopen("pred.txt", "w");
        for(int i = 0; i < STREAM_LENGTH; i++)
        {
            fprintf(f_pred, "%lu\n", output_stream[i] SATISFIES THRESHHOLD ? 1lu : 0lu);
        }
        fclose(f_pred);
    }

    return 0;
}

void* receiver(void* _)
{
    INIT_CLOCK
    int i = 0;
    int preamble_counter = 0;
    while(1)
    {
        sched_yield();
        size_t d = MEASSURE(function + 64);
        d = d SATISFIES THRESHHOLD ? 1lu : 0lu;

        if(preamble_counter < 7 * 8 + 7)
        {
            if(d == preamble_counter % 2)
            {
                // wrong bit
                //printf("counter=%i\n", preamble_counter);
                preamble_counter = 0;
            }
            else
            {
                // correct bit
                preamble_counter++;
            }
        }
        else
        {
            if(d == 1)
            {
                WAIT_FOR_CLOCK
                break;
            }
            else
            {
                preamble_counter -= 2;
            }
        }

        WAIT_FOR_CLOCK
    }
    while(1)
    {
        sched_yield();
        size_t d = MEASSURE(function + 64);

        output_stream[i++] = d;

        if(i == STREAM_LENGTH)
            return NULL;

        WAIT_FOR_CLOCK
    }
}