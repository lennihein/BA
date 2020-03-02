#include "lib.h"

size_t meassure_ff(size_t* addr)
{
    uint64_t    start_high, start_low,
            end_high, end_low;

    asm volatile
    (
    "mfence\n\t"
    "CPUID\n\t"
    "RDTSC\n\t"
    "mov %%rdx, %0\n\t"
    "mov %%rax, %1\n\t"
    "mfence\n\t"
    :
    "=r" (start_high),
    "=r" (start_low)
    ::
    "%rax", "%rbx", "%rcx", "%rdx"
    );

    flush(addr);

    asm volatile
    (
    "mfence\n\t"
    "RDTSCP\n\t"
    "mov %%rdx, %0\n\t"
    "mov %%rax, %1\n\t"
    "CPUID\n\t"
    "mfence\n\t"
    :
    "=r" (end_high),
    "=r" (end_low)
    ::
    "%rax", "%rbx", "%rcx", "%rdx"
    );

    return ((end_high<<32) | end_low) - ((start_high<<32) | start_low);

}size_t meassure_fr(size_t* addr)
{
    uint64_t    start_high, start_low,
            end_high, end_low;

    asm volatile
    (
    "mfence\n\t"
    "CPUID\n\t"
    "RDTSC\n\t"
    "mov %%rdx, %0\n\t"
    "mov %%rax, %1\n\t"
    "mfence\n\t"
    :
    "=r" (start_high),
    "=r" (start_low)
    ::
    "%rax", "%rbx", "%rcx", "%rdx"
    );

    maccess(addr);

    asm volatile
    (
    "mfence\n\t"
    "RDTSCP\n\t"
    "mov %%rdx, %0\n\t"
    "mov %%rax, %1\n\t"
    "CPUID\n\t"
    "mfence\n\t"
    :
    "=r" (end_high),
    "=r" (end_low)
    ::
    "%rax", "%rbx", "%rcx", "%rdx"
    );

    return ((end_high<<32) | end_low) - ((start_high<<32) | start_low);
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

void flush(void* p) {
    asm volatile ("clflush 0(%0)\n"
    :
    : "c" (p)
    : "rax");
}

// end of D. Gruss' code