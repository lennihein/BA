#include "lib.h"
#include "crc.h"

#define eval(x) (method?(x>threshhold?1:0):(x<=threshhold?1:0))

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

        threshhold = strtoll(argv[2], NULL, 10);
        if(threshhold==0) goto invalid;

        frequency = strtoll(argv[3], NULL, 10);
        if(frequency==0) goto invalid;
        // interval in nsecs
        interval = 1000000000/frequency;
    }
    else
    {
        invalid:
        fprintf(stderr, "usage: ./receiver [-ff/-fr] [THRESHHOLD] [FREQUENCY]\n");
        exit(1);
    }
    printf("Receiving process->process, with hardwareclock-sync and ethernet frames:\n"
           "- Frequency in Hz: %zu\n"
           "- Method: %s\n"
           "- Threshhold: %zu\n",
           frequency, method == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", threshhold);

    pthread_t r;
    _dispatch:
    memset(&frame, '0', sizeof(struct field));
    pthread_create(&r, NULL, receiver, NULL);
    pthread_join(r, NULL);

    if(length == 0)
    {
	    goto _dispatch;
    }

    // // retieve checksum:
    // for(int i=0; i<32; i++)
    // {
    //     checksum_ |= eval(frame.payload[i + length])<< (31 - i);
    // }

    // // check checksum:
    // if(checksum_!=crcFast((const unsigned char*) frame.mac_addr, sizeof(size_t) * ((6 + 6 + 2) * 8 + length)))
    // {
    //     fprintf(stderr, "checksum not matching\n");
    //     goto _dispatch;
    // }

    for(int i = 0; i < length/8; i++)
    {
        char __tmp = 0;
        for(int j = 0; j < 8; j++)
        {
            __tmp |= eval(frame.payload[i*8+j])<<(7-j);
        }
        printf("%c", __tmp);
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

        if(preamble_counter < 0 * 8 + 7)
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

    // retrieve length
    for(int i=0; i<16; i++)
    {
        length |= eval(frame.length[i])<<(15-i);
    }

    if(length > 1500 || length < 46)
    {
        // fprintf(stderr, "Length faulty: %d announced\n", length);
        length = 0;
        return NULL;        
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
