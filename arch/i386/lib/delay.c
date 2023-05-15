#include <asm/delay.h>
#include <asm/processor.h>
#include <asm/msr.h>
#include <asm/param.h>

int x86_udelay_tsc = 0;		/* Delay via TSC */

/*
 *	Do a udelay using the TSC for any CPU that happens
 *	to have one that we trust.
 */

static void __rdtsc_delay(unsigned long loops)
{
	unsigned long bclock, now;
	rdtscl(bclock);
	do {
		rep_nop();
		rdtscl(now);
	} while ((now - bclock) < loops);
}

/*
 *	Non TSC based delay loop for 386, 486, MediaGX
 */

static void __loop_delay(unsigned long loops)
{
	int d0;
	__asm__ __volatile__(
		"\tjmp 1f\n"
		".align 16\n"
		"1:\tjmp 2f\n"
		".align 16\n"
		"2:\tdecl %0\n\tjns 2b"
		:"=&a" (d0)
		:"0" (loops)
	);
}

void __delay(unsigned long loops)
{
	if(x86_udelay_tsc) {
		__rdtsc_delay(loops);
	} else {
		__loop_delay(loops);
	}
}

inline void __const_udelay(unsigned long xloops)
{
	int d0;
	__asm__("mull %0"
			: "=d" (xloops), "=&a"(d0)
			: "1" (xloops), "0" (current_cpu_data.loops_per_jiffy));
	__delay(xloops * HZ);
}

void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * 0x000010c6);  /* 2**32 / 1000000 */
}

void __ndelay(unsigned long nsecs)
{
	__const_udelay(nsecs * 0x00005);  /* 2**32 / 1000000000 (rounded up) */
}
