#include "lib.h"
#include "crc.h"

#define eval(x) method?(x>threshhold?1:0):(x<=threshhold?1:0)

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
size_t method, length;
size_t (*meassure)(void (* addr)(void));
size_t frequency, interval;
uint16_t length_ = 0;
uint32_t checksum_ = 0;

void* receiver(void* _);

int main(int argc, char* argv[])
{
    if(argc == 5)
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

        threshhold = strtoll(argv[2], NULL, 10);
        if(threshhold==0) goto invalid;

        frequency = strtoll(argv[3], NULL, 10);
        if(frequency==0) goto invalid;
        // interval in nsecs
        interval = 1000000000/frequency;

        length = strtoll(argv[4], NULL, 10);
        if(length==0) goto invalid;
    }
    else
    {
        invalid:
        fprintf(stderr, "usage: ./meassure_recv [-ff/-fr] [THRESHHOLD] [FREQUENCY] [TESTPOINTS]\n");
        exit(1);
    }
    printf("Receiving process->process, with hardwareclock-sync and ethernet frames:\n"
           "- Frequency in Hz: %zu\n"
           "- Paket Length: %zu\n"
           "- Method: %s\n"
           "- Threshhold: %zu\n",
           frequency, length, method == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", threshhold);

    pthread_t r;
    pthread_create(&r, NULL, receiver, NULL);
    pthread_join(r, NULL);

    // retrieve length
    for(int i=0; i<16; i++)
    {
        length_ |= eval(frame.length[i])<<(15-i);
    }

    // check length:
    if(length!=length_)
    {
        fprintf(stderr, "length not matching: %lu(arg) - %hu(transmitted)\n", length, length_);
        for(int i=0; i<16; i++)
        {
            printf("%d\n", eval(frame.length[i]));
        }
    }

    // retieve checksum:
    for(int i=0; i<32; i++)
    {
        checksum_ |= eval(frame.payload[i + length])<< (31 - i);
    }

    // check checksum:
    if(checksum_!=crcFast((const unsigned char*) frame.mac_addr, sizeof(size_t) * ((6 + 6 + 2) * 8 + length)))
    {
        fprintf(stderr, "checksum not matching\n");
    }

    // prepare predictions
    {
        FILE* f_pred = fopen("pred.txt", "w");
        for(int i = 0; i < length; i++)
        {
            fprintf(f_pred, "%d\n", eval(frame.payload[i]));
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
        size_t d = meassure(function + 64);
        d = eval(d);

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
        size_t d = meassure(function + 64);

        frame.mac_addr[j] = d;
        WAIT_FOR_CLOCK;
    }

    while(1)
    {
        sched_yield();
        size_t d = meassure(function + 64);

        frame.payload[i++] = d;

        if(i == length)
            return NULL;

        WAIT_FOR_CLOCK
    }
}
