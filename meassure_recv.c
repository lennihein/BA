#include "lib.h"

size_t* payload;
size_t method, length;
size_t (*meassure)(void (* addr)(void));
size_t frequency, interval;

void* receiver(void* _);

int main(int argc, char* argv[])
{
    if(argc == 4)
    {
        if(strcmp(argv[1], "-ff")==0)
        {
            method = FLUSH_FLUSH;
            meassure = meassure_ff;
        }
        else
        {
            if(strcmp(argv[1], "-fr")==0)
            {
                method = FLUSH_RELOAD;
                meassure = meassure_fr;
            }
            else
            {
                goto invalid;
            }
        }

        frequency = strtoll(argv[2], NULL, 10);
        if(frequency==0) goto invalid;
        // interval in nsecs
        interval = 1000000000/frequency;

        length = strtoll(argv[3], NULL, 10);
        if(length==0) goto invalid;
    }
    else
    {
        invalid:
        fprintf(stderr, "usage: ./meassure_recv [-ff/-fr] [FREQUENCY] [TESTPOINTS]\n");
        exit(1);
    }

    payload = malloc(sizeof(size_t)*length);
    if(!payload){fprintf(stderr, "could not allocate memory\n"); exit(2);}


    printf("Receiving alternating \'1\' and \'0\'\n- Frequency in Hz: %zu\n"
           "- Testdata Length: %lu\n"
           "- Method: %s\n",
           frequency, length, method == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload");

    pthread_t r;
    pthread_create(&r, NULL, receiver, NULL);
    pthread_join(r, NULL);

    {
        FILE* group_A = fopen("groupA.txt", "w");
        for(int i = 0; i < length; i+=2)
        {
            fprintf(group_A, "%lu\n", payload[i]);
        }

        fclose(group_A);

        FILE* group_B = fopen("groupB.txt", "w");
        for(int i = 1; i < length; i+=2)
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
        size_t d = meassure(function + 64);

        payload[i++] = d;

        if(i == length)
            return NULL;

        WAIT_FOR_CLOCK
    }
}