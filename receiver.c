#include "lib.h"
#include "crc.h"

// EDIT THIS!
#define METHOD 1
#define STREAM_LENGTH 1024*8
#define FREQUENCY 100
/***************************/
#define INTERVAL 1000000000/FREQUENCY
#if METHOD == FLUSH_FLUSH
#define SATISFIES >
#define MEASSURE meassure_ff
#endif
#if METHOD == FLUSH_RELOAD
#define SATISFIES <=
#define MEASSURE meassure_fr
#endif
#define unlikely(expr) __builtin_expect(!!(expr), 0)
/***************************/

struct field
{
    size_t sync[7 * 8];
    size_t delimiter[8];
    size_t mac_addr[6 * 8];
    size_t mac_src[6 * 8];
    size_t length[2 * 8];
    size_t payload[(1500 + 4) * 8];
};

struct field frame;

size_t threshhold;
uint16_t length = 0;

void* receiver(void* _);

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        threshhold = strtoll(argv[1], NULL, 10);
    }
    else
    {
        fprintf(stderr, "usage: ./receiver [TRESHHOLD]\n");
        exit(1);
    }
    printf("Receiving process->process, with hardwareclock-sync and ethernet frames:\n\n"
           "- STREAM_LENGTH: %d\n"
           "- METHOD: %s\n"
           "- THRESHHOLD: %zu\n",
           STREAM_LENGTH, METHOD == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", threshhold);

    pthread_t r;
    pthread_create(&r, NULL, receiver, NULL);
    pthread_join(r, NULL);

    // retrieve length
    for(int i=0; i<16; i++)
    {
        length |= frame.length[i]<<15-i;
    }

    // check length:
    if(STREAM_LENGTH!=length)
    {
        fprintf(stderr, "length not matching\n");
    }

    // retieve checksum:
    for(int i=0; i<32; i++)
    {
        checksum |= frame.payload[i+STREAM_LENGTH]<<31-i;
    }

    // check checksum:
    if(checksum!=crcFast(frame.mac_addr, sizeof(size_t) * ((6+6+2)*8 + STREAM_LENGTH))
    {
        fprintf(stderr, "checksum not matching\n");
    }

    // prepare predictions
    {
        FILE* f_pred = fopen("pred.txt", "w");
        for(int i = 0; i < STREAM_LENGTH; i++)
        {
            fprintf(f_pred, "%lu\n", frame.payload[i] SATISFIES threshhold ? 1lu : 0lu);
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
        d = d SATISFIES threshhold ? 1lu : 0lu;

        if(preamble_counter < 7 * 8 + 7)
        {
            if(d == preamble_counter % 2)
            {
                // wrong sync bit
                preamble_counter = 0;
            }
            else
            {
                // correct sync bit
                preamble_counter++;
            }
        }
        else
        {
            if(d == 1)
            {
                // preamble done
                WAIT_FOR_CLOCK
                break;
            }
            else
            {
                // still in Sync
                preamble_counter -= 2;
            }
        }

        WAIT_FOR_CLOCK
    }

    for(int j = 0; j < (6 + 6 + 2) * 8; j++)
    {
        sched_yield();
        size_t d = MEASSURE(function + 64);

        frame.mac_addr[j] = d;
        WAIT_FOR_CLOCK;
    }

    while(1)
    {
        sched_yield();
        size_t d = MEASSURE(function + 64);

        frame.payload[i++] = d;

        if(i == STREAM_LENGTH)
            return NULL;

        WAIT_FOR_CLOCK
    }
}
