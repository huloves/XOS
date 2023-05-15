#ifndef __LINUX_TIMEX_H
#define __LINUX_TIMEX_H

#include <asm-i386/timex.h>
#include <asm/param.h>

/* LATCH is used in the interval timer and ftape setup. */
#define LATCH  ((CLOCK_TICK_RATE + HZ/2) / HZ)	/* For divider */

#endif
