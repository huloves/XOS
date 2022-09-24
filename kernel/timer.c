#include <linux/time.h>
#include <linux/sched.h>
#include <asm-i386/param.h>

/*
 * Timekeeping variables
 */

long tick = (1000000 + HZ/2) / HZ;	/* timer interrupt period */

/* The current time */
volatile struct timeval xtime __attribute__ ((aligned(16)));
