#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

/* Optimization barrier */
/* The "volatile" is due to gcc bugs */
#define barrier() __asm__ __volatile__("": : :"memory")

#endif
