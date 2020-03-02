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