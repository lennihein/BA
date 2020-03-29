#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

size_t meassure_ff(size_t* addr);
size_t meassure_fr(size_t* addr);
void flush(void* p);
void maccess(void* p);

#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))

#define WAIT_FOR_CLOCK                                                  \
nanosecs = INTERVAL;                                                    \
nanosecs += t0.tv_nsec;                                                 \
if(unlikely(nanosecs > 999999999))                                      \
{nanosecs -= 1000000000;                                                \
t0.tv_sec++;}                                                           \
t0.tv_nsec = nanosecs;                                                  \
clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t0, NULL);

#define INIT_CLOCK                                                      \
struct timespec t0;                                                     \
clock_gettime(CLOCK_MONOTONIC, &t0);                                    \
uint32_t nanosecs;