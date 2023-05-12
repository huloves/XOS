#ifndef __ASM_SOFTIRQ_H
#define __ASM_SOFTIRQ_H

#include <asm/atomic.h>
#include <asm-i386/hardirq.h>
#include <linux/kernel.h>

#define cpu_bh_disable()	do { local_bh_count()++; barrier(); } while (0)
#define cpu_bh_enable()	do { barrier(); local_bh_count()--; } while (0)

#define local_bh_disable()	cpu_bh_disable()
#define local_bh_enable()	cpu_bh_enable()

#define in_softirq() (local_bh_count(smp_processor_id()) != 0)

#endif	/* __ASM_SOFTIRQ_H */
