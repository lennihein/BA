#include "lib.h"
#include "crc.h"

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

struct state
{
    size_t output;
    size_t cond;
    size_t message_length;
    size_t frame_length;
    size_t interval;
    size_t counter;
};

struct state state;
size_t length, frequency, interval;

void signal_handler(int _);

int main(int argc, char* argv[])
{
    if(argc == 3)
    {
        frequency = strtoll(argv[1], NULL, 10);
        // interval in usecs
        interval = 1000000/frequency;

        // converting string to arr of size_t
        length = strlen(argv[2]);
        if(length > 1500)
        {
            fprintf(stdout, "Text must not be more than 1500 characters\n");
            exit(1);
        }
        if(length < 46) // pad with spaces
        {
            for(int i = length; i == 46; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    frame.payload[i*8+j] = (32>>(7-j)) & 0x1;
                }
            }
        }
        for(int i = 0; i < length; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                frame.payload[i*8+j] = (argv[2][i]>>(7-j)) & 0x1;
            }
        }
    }
    else
    {
        invalid:
        fprintf(stderr, "usage: ./sender [FREQUENCY] [ASCII]\n");
        exit(1);
    }
    state.message_length = length*8;
    state.interval = interval;
    state.cond = 1;
    state.counter = 0;
    state.output = 0;
    state.frame_length = (7 + 1 + 6 + 6 + 2) * 8 + length*8 + 4 * 8;

    printf("Sending thread-thread, with hardwareclock-sync and ethernet frames:\n"
           "- Frequency in Hz: %zu\n"
           "- Message: %s\n",
           frequency, argv[2]);


    // set sync (and part of delimit)
    memset(frame.sync, 0lu, 8 * 8 * sizeof(size_t));
    for(int i = 0; i < 8 * 8; i += 2)
    {
        frame.sync[i] = 1;
    }

    // set delimiter
    frame.delimiter[7] = 1;

    // setting addresses not required here
    
    // set length
    uint16_t length = state.message_length;
    // sending the length little-endian
    for(int i = 0; i < 16; i++)
    {
        frame.length[i] = (length>>(15-i)) & 0x1;
    }

    // set checksum
    crc checksum = crcFast((const unsigned char*)frame.mac_addr, sizeof(size_t) * ((6+6+2)*8 + state.message_length));
    // sending the checksum little-endian
    for(int i = 0; i < 32; i++)
    {
        frame.payload[state.message_length+i] = (checksum>>(31-i)) & 0x1;
    }

    // start signal handler
    signal(SIGALRM, signal_handler);
    // alarm every communication cycle
    ualarm(interval, interval);

    loop:

    while(state.cond)
    {
        if(state.output)
        {
            maccess(function + 64);
        }
        else
        {
            flush(function + 64);
        }
    }

    // printf("Sent over frame\n");
    state.cond = 1;
    state.counter = 0;
    goto loop;

}

void signal_handler(int _)
{
    // update symbol
    state.output = frame.sync[state.counter];
    state.counter++;
    // check if frame is fully sent
    if(state.counter == state.frame_length)
    {
        state.cond = 0;
    }
}
