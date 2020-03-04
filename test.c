#include "lib.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define INTERVAL 1000000000
#define SLENGTH 8

int main()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    uint64_t target = t.tv_sec * 1000000000;
    target += t.tv_nsec;
    uint64_t current;

    for(int i = 0; i < SLENGTH; ++i)
    {
        /**************************/
        // STUFF HERE
        /**************************/
        target += INTERVAL;
        while(1)
        {
            clock_gettime(CLOCK_MONOTONIC, &t);
            current = (t.tv_sec * 1000000000) + t.tv_nsec;
            if(current>=target)
                break;
        }

    }
}