#ifndef __irq_cpustat_h
#define __irq_cpustat_h

#include <asm-i386/hardirq.h>

// extern irq_cpustat_t irq_stat[];

#define __IRQ_STAT(member) (irq_stat[0].member)

/* arch independent irq_stat fields */
#define softirq_active()	__IRQ_STAT(__softirq_active)
#define softirq_mask()	__IRQ_STAT(__softirq_mask)
#define local_irq_count()	__IRQ_STAT(__local_irq_count)
#define local_bh_count()	__IRQ_STAT(__local_bh_count)
#define syscall_count()	__IRQ_STAT(__syscall_count)
/* arch dependent irq_stat fields */
#define nmi_count()		__IRQ_STAT(__nmi_count)		/* i386, ia64 */

#endif /* __irq_cpustat.h  */
