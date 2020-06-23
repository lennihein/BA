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

        length = strtoll(argv[2], NULL, 10);
        if(length==0) goto invalid;
    }
    else
    {
        invalid:
        fprintf(stderr, "usage: ./meassure_send [FREQUENCY] [PAKETLENGTH]\n");
        exit(1);
    }
    state.message_length = length;
    state.interval = interval;
    state.cond = 1;
    state.counter = 0;
    state.output = 0;
    state.frame_length = (7 + 1 + 6 + 6 + 2) * 8 + length + 4 * 8;

    printf("Sending thread-thread, with hardwareclock-sync and ethernet frames:\n"
           "- Frequency in Hz: %zu\n"
           "- Paket Length: %zu\n",
           frequency, length);

    // randomise message
    srand(time(0));
    for(int i = 0; i < length; i++)
    {
        frame.payload[i] = rand() % 2;
    }

    // write message to file
    FILE* f_act = fopen("act.txt", "w");
    for(int i = 0; i < length; i++)
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