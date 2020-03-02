#include "lib.h"

// EDIT THESE!
#define STREAM_LENGTH 100000
#define THRESHHOLD 200
#define FLUSH_FLUSH >=
#define FLUSH_RELOAD <=
#define SATISFIES FLUSH_RELOAD
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

        // FILE* f = fopen("poc_out.txt", "w");
        for(int i = 0; i < STREAM_LENGTH; i++)
        {
            // fprintf(f, "%lu - %lu - %lu \n", output_stream[i], input_stream[i],output_stream[i] >= THRESHHOLD ? 1lu : 0lu);
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
        fprintf(stdout, "Accuracy: %lf%%\n", ((double) (TP_counter + TN_counter) * 100) / STREAM_LENGTH);
        fprintf(stdout, "Accuracy (1s) aka Sensitivity: %lu/%zu = %lf%%\n", TP_counter, (FN_counter + TP_counter),
                ((double) (TP_counter) * 100) / (double) (FN_counter + TP_counter));
        fprintf(stdout, "Accuracy (0s) aka Specificity: %lu/%zu = %lf%%\n", TN_counter, (FP_counter + TN_counter),
                ((double) (TN_counter) * 100) / (double) (FP_counter + TN_counter));
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
        size_t d = meassure_fr(array + 2 * 1024);

        output_stream[i] = d;

    }

    return NULL;
}