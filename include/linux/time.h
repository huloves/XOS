#ifndef __TIME_H
#define __TIME_H

#include <asm-i386/types.h>

struct timeval {
	time_t tv_sec;
	suseconds_t tv_usec;
};

#endif
