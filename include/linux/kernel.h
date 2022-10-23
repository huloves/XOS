#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

#include <asm-i386/types.h>

/* Optimization barrier */
/* The "volatile" is due to gcc bugs */
#define barrier() __asm__ __volatile__("": : :"memory")

extern int snprintf(char * buf, size_t size, const char * fmt, ...)
	__attribute__ ((format (printf, 3, 4)));

#endif
