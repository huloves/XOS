#ifndef __ASM_HARDIRQ_H
#define __ASM_HARDIRQ_H

#include <linux/irq_cpustat.h>
#include <linux/cache.h>

/* entry.S is sensitive to the offsets of these fields */
typedef struct {
	unsigned int __softirq_active;
	unsigned int __softirq_mask;
	unsigned int __local_irq_count;
	unsigned int __local_bh_count;
	unsigned int __syscall_count;
	unsigned int __nmi_count;	/* arch dependent */
} ____cacheline_aligned irq_cpustat_t;

extern irq_cpustat_t irq_stat[];

/*
 * Are we in an interrupt context? Either doing bottom half
 * or hardware interrupt processing?
 */
#define in_interrupt() ({ (local_irq_count() + local_bh_count() != 0); })

#define hardirq_trylock()	(local_irq_count() == 0)
#define hardirq_endlock()	do { } while (0)

#define irq_enter(irq)		(local_irq_count()++)
#define irq_exit(irq)		(local_irq_count()--)

#endif
