#ifndef __LINUX_CACHE_H
#define __LINUX_CACHE_H

#include <asm/cache.h>


#define SMP_CACHE_BYTES L1_CACHE_BYTES

#define ____cacheline_aligned __attribute__((__aligned__(SMP_CACHE_BYTES)))

#define __cacheline_aligned ____cacheline_aligned

#endif
