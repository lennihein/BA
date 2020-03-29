#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD FLUSH_FLUSH
#define STREAM_LENGTH 1024*8*8
#define THRESHHOLD 155
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

size_t array[5 * 1024];

size_t input_stream[STREAM_LENGTH];
size_t output_stream[STREAM_LENGTH];

void* sender(void* _);
void* receiver(void* _);

int main()
{
    printf("Transmitting thread-thread, with hardwareclock-sync:\n\n"
           "- STREAM_LENGTH: %d\n"
           "- METHOD: %s\n"
           "- THRESHHOLD: %d\n",
           STREAM_LENGTH, METHOD == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", THRESHHOLD);

    //setup
    memset(array, -1, 5 * 1024 * sizeof(size_t));
    srand(time(0));
    for(int i = 0; i < STREAM_LENGTH; i++)
    {
        input_stream[i] = rand() % 2;
    }

    pthread_t s, r;
    void* ret_val;
    pthread_create(&s, NULL, sender, NULL);
    pthread_create(&r, NULL, receiver, NULL);

    pthread_join(s, NULL);
    pthread_join(r, &ret_val);

    {   // evaluation and print
        size_t TP_counter = 0;
        size_t TN_counter = 0;
        size_t FP_counter = 0;
        size_t FN_counter = 0;

        FILE* f_pred = fopen("pred.txt", "w");
        FILE* f_act = fopen("act.txt", "w");
        for(int i = 0; i < STREAM_LENGTH; i++)
        {
            fprintf(f_pred, "%lu\n", output_stream[i] SATISFIES THRESHHOLD ? 1lu : 0lu);
            fprintf(f_act, "%lu\n", input_stream[i]);
            if(input_stream[i] == output_stream[i] SATISFIES THRESHHOLD ? 1lu : 0lu)
            {
                if(input_stream[i] == 1)
                    TP_counter++;
                else
                    TN_counter++;
            }
            else
            {
                if(input_stream[i] == 1)
                    FN_counter++;
                else
                    FP_counter++;
            }
        }
        fclose(f_pred);
        fclose(f_act);
        fprintf(stdout, "Accuracy: %lf%%\n", ((double) (TP_counter + TN_counter) * 100) / (STREAM_LENGTH));
        fprintf(stdout, "Accuracy (1s) aka Sensitivity: %lu/%zu = %lf%%\n", TP_counter, (FN_counter + TP_counter),
                ((double) (TP_counter) * 100) / (double) (FN_counter + TP_counter));
        fprintf(stdout, "Accuracy (0s) aka Specificity: %lu/%zu = %lf%%\n", TN_counter, (FP_counter + TN_counter),
                ((double) (TN_counter) * 100) / (double) (FP_counter + TN_counter));
    }

    return 0;
}

void* sender(void* _)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    uint64_t target = t.tv_sec * 1000000000;
    target += t.tv_nsec;
    uint64_t current;

    for(int i = 0; i < STREAM_LENGTH; i++)
    {
        target += INTERVAL;
        while(1)
        {
            /**************************/
            if(input_stream[i])
            {
                maccess(array + 2 * 1024);
            }
            else
            {
                flush(array + 2 * 1024);
            }
            /**************************/
            clock_gettime(CLOCK_MONOTONIC, &t);
            current = (t.tv_sec * 1000000000) + t.tv_nsec;
            if(current>=target)
                break;
        }

    }
    return NULL;
}

void* receiver(void* _)
{
    struct timespec t0;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    uint32_t nanosecs;

    for(int i = 0; i < STREAM_LENGTH; i++)
    {
        sched_yield();
        size_t d = MEASSURE(array + 2 * 1024);

        output_stream[i] = d;

        // wait to be in sync with next phase
        nanosecs = INTERVAL;
        nanosecs += t0.tv_nsec;
        // overflow into seconds
        if(unlikely(nanosecs > 999999999))
        {
            nanosecs -= 1000000000;
            t0.tv_sec++;
        }
        t0.tv_nsec = nanosecs;
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t0, NULL);
    }

    return NULL;
}
