# 4. Februar 2020

[Code von Gruss](https://github.com/IAIK/flush_flush/blob/master/histogram/ff/calibration.c):

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include "../cacheutils.h"

size_t array[5*1024];                       // warum nicht uint64_t?                           

size_t hit_histogram[600];
size_t miss_histogram[600];

size_t onlyreload(size_t* addr)
{
  size_t time = rdtsc();
  flush(addr);
  size_t delta = rdtsc() - time;
  maccess(addr);                            // warum 2x maccess?
  maccess(addr);                            // ~~was genau macht maccess?~~ 
  return delta;
}

size_t flushandreload(size_t* addr)
{
  size_t time = rdtsc();
  flush(addr);
  size_t delta = rdtsc() - time;
  flush(addr);
  return delta;
}

int main(int argc, char** argv)
{
  memset(array,-1,5*1024*sizeof(size_t));   // warum mit '-1' initialisiert?
  maccess(array + 2*1024);
  sched_yield();
  for (int i = 0; i < 1*1024*1024; ++i)
  {
    size_t d = onlyreload(array+2*1024);
    hit_histogram[MIN(599,d)]++;
    for (size_t i = 0; i < 30; ++i)         // warum so viele yields?
      sched_yield();                        
  }
  flush(array+1024);               // warum hier nicht array+2*1024??
  for (int i = 0; i < 1*1024*1024; ++i)
  {
    size_t d = flushandreload(array+2*1024);
    miss_histogram[MIN(599,d)]++;
    for (size_t i = 0; i < 30; ++i)
      sched_yield();
  }
  printf(".\n");
  size_t hit_max = 0;
  size_t hit_max_i = 0;
  size_t miss_min_i = 0;
  for (size_t i = 0; i < 600; ++i)
  {
    printf("%3zu: %10zu %10zu\n",i,hit_histogram[i],miss_histogram[i]);
  }
  return 0;
}
```

40960 Byte werden reserviert, auf byte 8*2*1024 wird zugegriffen, also 16KiB vorher.
Die Cacheline ist 64 Byte lang, also von 16384+64=16448 bis 40960.

Warum werden vorne und nachher so viel Platz gelassen? Würde es nicht reichen ein `uint64_t array[8]` zu reservieren?

folgender Code

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include "../cacheutils.h"

uint64_t array[8];

size_t hit_histogram[600];
size_t miss_histogram[600];

size_t onlyreload(size_t* addr)
{
  size_t time = rdtsc();
  flush(addr);
  size_t delta = rdtsc() - time;
  maccess(addr);
  return delta;
}

size_t flushandreload(size_t* addr)
{
  size_t time = rdtsc();
  flush(addr);
  size_t delta = rdtsc() - time;
  flush(addr);
  return delta;
}

int main(int argc, char** argv)
{
  memset(array,0,8*sizeof(size_t));
  maccess(array);
  sched_yield();
  for (int i = 0; i < 1*1024*1024*16; ++i)
  {
    size_t d = onlyreload(array);
    hit_histogram[MIN(599,d)]++;
  }
  flush(array);
  for (int i = 0; i < 1*1024*1024*16; ++i)
  {
    size_t d = flushandreload(array);
    miss_histogram[MIN(599,d)]++;
  }
  printf(".\n");
  size_t hit_max = 0;
  size_t hit_max_i = 0;
  size_t miss_min_i = 0;
  for (size_t i = 0; i < 600; ++i)
  {
    printf("%3zu: %10zu %10zu\n",i,hit_histogram[i],miss_histogram[i]);
  }
  return 0;
}

```

entstand folgende Messung:

![grafik](/assets/messung.png)

## Eigener Code mit Threading

```c
#include "main.h"

size_t array[5*1024];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

size_t hit_histogram[600];
size_t miss_histogram[600];


int main(int argc, char** argv)
{
    memset(array,-1,5*1024*sizeof(size_t));
    for (int i = 0; i < 1*1024; ++i)
    {
        size_t d = DO_HIT();
        hit_histogram[MIN(599,d)]++;
    }
    for (int i = 0; i < 1*1024; ++i)
    {
        size_t d = DO_MISS();
        miss_histogram[MIN(599,d)]++;
    }
    printf(".\n");
    for (size_t i = 0; i < 600; ++i)
    {
        printf("%3zu: %10zu %10zu\n",i,hit_histogram[i],miss_histogram[i]);
    }
    return 0;
}

size_t meassure_ff(size_t* addr)
{
    size_t time = rdtsc();
    flush(addr);
    size_t delta = rdtsc() - time;
    return delta;
}

size_t DO_HIT()
{
    pthread_t s, r;
    void* ret_val;

    pthread_create(&s, NULL, sender_hit, NULL);
    pthread_create(&r, NULL, receiver, NULL);

    pthread_join(s, NULL);
    pthread_join(r, &ret_val);

    return (size_t)ret_val;
}

size_t DO_MISS()
{
    pthread_t s, r;
    void* ret_val;

    pthread_create(&s, NULL, sender_miss, NULL);
    pthread_create(&r, NULL, receiver, NULL);

    pthread_join(s, NULL);
    pthread_join(r, &ret_val);

    return (size_t)ret_val;
}

void* sender_hit(void* data)
{
    pthread_mutex_lock(&mutex);

    maccess(array+2*1024);
    sched_yield();

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* sender_miss(void* data)
{
    pthread_mutex_lock(&mutex);

    flush(array+2*1024);
    sched_yield();

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* receiver(void* data)
{
    pthread_mutex_lock(&mutex);

    size_t d = meassure_ff(array+2*1024);

    pthread_mutex_unlock(&mutex);

    return (void*) d;
}
```

Unter anderem, leider viel zu oft mit zu viel noise:

![messung2](/assets/messung2.png)