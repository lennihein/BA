#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD 0

#define NUMBER_OF_POINTS 1024*100*1
/***************************/
#define FLUSH_FLUSH_COMP >=
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

void* sender_hit(void* data);
void* sender_miss(void* data);
void* receiver_hit(void* data);
void* receiver_miss(void* data);

size_t array[5 * 1024];

sem_t  sent;
sem_t  ready_to_receive;

size_t hit_histogram[600];
size_t miss_histogram[600];


int main(int argc, char** argv)
{
    //setup
    memset(array, -1, 5 * 1024 * sizeof(size_t));

    // figure hits
    {
        pthread_t s, r;
        void* ret_val;

        pthread_create(&s, NULL, sender_hit, NULL);
        pthread_create(&r, NULL, receiver_hit, NULL);

        pthread_join(s, NULL);
        pthread_join(r, &ret_val);
    }

    // figure misses
    {
        pthread_t s, r;
        void* ret_val;

        pthread_create(&s, NULL, sender_miss, NULL);
        pthread_create(&r, NULL, receiver_miss, NULL);

        pthread_join(s, NULL);
        pthread_join(r, &ret_val);
    }

    // print results
    FILE* f = fopen("threshholds_out.txt", "w");
    for(size_t i = 0; i < 600; ++i)
    {
        fprintf(f, "%3zu: %10zu %10zu\n", i, hit_histogram[i], miss_histogram[i]);
    }
    return 0;
}

void* sender_hit(void* data)
{
    for(int i = 0; i < NUMBER_OF_POINTS; ++i)
    {

        sem_wait(&ready_to_receive);

        maccess(array + 2 * 1024);

        sem_post(&sent);
    }

    return NULL;
}

void* sender_miss(void* data)
{
    for(int i = 0; i < NUMBER_OF_POINTS; ++i)
    {
        sem_wait(&ready_to_receive);

        flush(array + 2 * 1024);

        sem_post(&sent);
    }

    return NULL;
}

void* receiver_hit(void* data)
{
    for(int i = 0; i < NUMBER_OF_POINTS; ++i)
    {
        sem_post(&ready_to_receive);
        sem_wait(&sent);

        sched_yield();
        size_t d = MEASSURE(array + 2 * 1024);

        hit_histogram[MIN(599, d)]++;

    }

    return NULL;
}

void* receiver_miss(void* data)
{
    for(int i = 0; i < NUMBER_OF_POINTS; ++i)
    {
        sem_post(&ready_to_receive);
        sem_wait(&sent);

        sched_yield();
        size_t d = MEASSURE(array + 2 * 1024);

        miss_histogram[MIN(599, d)]++;
    }

    return NULL;
}