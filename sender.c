#include "lib.h"

#define FLUSH_FLUSH 1
#define FLUSH_RELOAD 0

// EDIT THIS!
#define METHOD 0
#define STREAM_LENGTH 1024*8
#define THRESHHOLD IRRELEVANT
#define INTERVAL 100 //0.1ms -> 10KHz
/***************************/
#define FLUSH_FLUSH_COMP >
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
#define unlikely(expr) __builtin_expect(!!(expr), 0)
/***************************/

struct field
{
    size_t sync[7 * 8];
    size_t delimiter[8];
    size_t mac_addr[6 * 8];
    size_t mac_src[6 * 8];
    size_t tag[4 * 8];
    size_t length[2 * 8];
    size_t payload[(1500 + 5) * 8];
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
    state.frame_length = (7 + 1 + 6 + 6 + 4 + 2) * 8 + STREAM_LENGTH + 4 * 8;

//    printf("%p\n%p\n", frame.sync+(7 + 1 + 6 + 6 + 4 + 2) * 8, frame.payload);
//    return  0;

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
        frame.payload[i] = i % 2;
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
    // set addresses
    // todo: just fill with NULL
    // set tag
    // todo: what this?
    // set length
    // todo: parse length into 2 byte
    // set CRC
    // put CRC starting from frame.payload[STEAM_LENGTH]
    // calculate length of frame


    struct sigaction sig;
    sig.sa_handler = signal_handler;
    sigaction(SIGALRM, &sig, NULL);
    ualarm(INTERVAL, INTERVAL);

    loop:

    while(state.cond)
    {
        if(state.counter % 2)
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
//    state.output = frame.sync[state.counter];
    state.counter++;
//    if(state.counter == state.frame_length)
//    {
//        state.cond = 0;
//    }
}