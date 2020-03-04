#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD FLUSH_FLUSH
#define STREAM_LENGTH 1000000
#define THRESHHOLD 155
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
/***************************/

size_t array[5 * 1024];

sem_t sent;
sem_t ready_to_receive;

size_t input_stream[STREAM_LENGTH];
size_t output_stream[STREAM_LENGTH];

void* sender(void* data);

void* receiver(void* data);

int main(int argc, char** argv)
{
    printf("Transmitting thread-thread:\n\n"
           "- STREAM_LENGTH: %d\n"
           "- METHOD: %s\n"
           "- THRESHHOLD: %d\n",
           STREAM_LENGTH, METHOD == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", THRESHHOLD);
    {   //setup
        memset(array, -1, 5 * 1024 * sizeof(size_t));
        srand(time(0));
        for(int i = 0; i < STREAM_LENGTH; i++)
        {
            input_stream[i] = rand() % 2;
        }
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
        // fclose(f);
        fprintf(stdout, "\nAccuracy: %lf%%\n", ((double) (TP_counter + TN_counter) * 100) / STREAM_LENGTH);
//        fprintf(stdout, "Accuracy (1s) aka Sensitivity: %lu/%zu = %lf%%\n", TP_counter, (FN_counter + TP_counter),
//                ((double) (TP_counter) * 100) / (double) (FN_counter + TP_counter));
//        fprintf(stdout, "Accuracy (0s) aka Specificity: %lu/%zu = %lf%%\n", TN_counter, (FP_counter + TN_counter),
//                ((double) (TN_counter) * 100) / (double) (FP_counter + TN_counter));
    }

    return 0;
}

void* sender(void* data)
{
    for(int i = 0; i < STREAM_LENGTH; ++i)
    {

        sem_wait(&ready_to_receive);

        if(input_stream[i])
        {
            maccess(array + 2 * 1024);
        }
        else
        {
            flush(array + 2 * 1024);
        }

        sem_post(&sent);
    }

    return NULL;
}

void* receiver(void* data)
{
    for(int i = 0; i < STREAM_LENGTH; ++i)
    {
        sem_post(&ready_to_receive);
        sem_wait(&sent);

        sched_yield();
        size_t d = MEASSURE(array + 2 * 1024);

        output_stream[i] = d;

    }

    return NULL;
}
