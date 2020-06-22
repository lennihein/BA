#include "lib.h"
#include "crc.h"

#include "CONFIG.H"
#define STREAM_LENGTH PACKET_LENGTH

/***************************/
#define INTERVAL 1000000/FREQUENCY
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

void signal_handler(int _);

int main()
{
    state.message_length = STREAM_LENGTH;
    state.interval = INTERVAL;
    state.cond = 1;
    state.counter = 0;
    state.output = 0;
    state.frame_length = (7 + 1 + 6 + 6 + 2) * 8 + STREAM_LENGTH + 4 * 8;

    printf("Sending thread-thread, with hardwareclock-sync and ethernet frames:\n\n"
           "- Length of Message: %zu\n"
           "- Method: %s\n"
           "Interval in usec: %zu\n",
           state.message_length, METHOD == FLUSH_FLUSH ? "Flush+Flush" : "Flush+Reload", state.interval);

    // randomise message
    srand(time(0));
    for(int i = 0; i < STREAM_LENGTH; i++)
    {
        frame.payload[i] = rand() % 2;
    }

    // write message to file
    FILE* f_act = fopen("act.txt", "w");
    for(int i = 0; i < STREAM_LENGTH; i++)
    {
        fprintf(f_act, "%lu\n", frame.payload[i]);
    }
    fclose(f_act);


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
    crc checksum = crcFast(frame.mac_addr, sizeof(size_t) * ((6+6+2)*8 + state.message_length));
    // sending the checksum little-endian
    for(int i = 0; i < 32; i++)
    {
        frame.payload[state.message_length+i] = (checksum>>(31-i)) & 0x1;
    }

    // start signal handler
    signal(SIGALRM, signal_handler);
    // alarm every communication cycle
    ualarm(INTERVAL, INTERVAL);

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

    printf("Sent over frame\n");
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