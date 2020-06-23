#include "lib.h"

static size_t counter = 0;

void signal_handler(int _);

int main(int argc, char* argv[])
{
    size_t frequency, interval;
    if(argc == 2)
    {
        frequency = strtoll(argv[1], NULL, 10);
        // interval in usecs
        interval = 1000000/frequency;
    }
    else
    {
        fprintf(stderr, "usage: ./meassure_send [FREQUENCY]\n");
        exit(1);
    }
    printf("Sending alternating \'1\' and \'0\'\n- Frequency in Hz: %zu\n",
           frequency);


    signal(SIGALRM, signal_handler);

    ualarm(interval, interval);

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