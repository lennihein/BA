#include "lib.h"

size_t meassure_ff(void (* addr)(void))
{
    uint64_t start, end, delta;
    uint64_t lo, hi;
    asm volatile ("LFENCE");
    asm volatile ("RDTSC": "=a" (lo), "=d" (hi));
    start = (hi<<32) | lo;
    asm volatile ("LFENCE");

    asm volatile ("CLFLUSH 0(%0)\n":: "c" (addr): "rax");

    asm volatile ("MFENCE");
    asm volatile ("RDTSC": "=a" (lo), "=d" (hi));
    end = (hi<<32) | lo;
    asm volatile ("LFENCE");
    delta = end - start;
    return delta;

}

size_t meassure_fr(void (* addr)(void))
{
    uint64_t start, end, delta;
    uint64_t lo, hi;
    asm volatile ("LFENCE");
    asm volatile ("RDTSC": "=a" (lo), "=d" (hi));
    start = (hi<<32) | lo;
    asm volatile ("LFENCE");

    asm volatile ("movq (%0), %%rax\n"
    :
    : "c" (addr)
    : "rax");

    asm volatile ("LFENCE");
    asm volatile ("RDTSC": "=a" (lo), "=d" (hi));
    end = (hi<<32) | lo;
    asm volatile ("LFENCE");
    delta = end - start;
    return delta;
}

// following code is taken from D. Gruss
// https://github.com/IAIK/flush_flush

void maccess(void* p)
{
    asm volatile ("movq (%0), %%rax\n"
    :
    : "c" (p)
    : "rax");
}

void flush(void* p)
{
    asm volatile ("clflush 0(%0)\n"
    :
    : "c" (p)
    : "rax");
}

// end of D. Gruss' code