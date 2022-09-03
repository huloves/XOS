#include <asm-i386/hardirq.h>
#include <linux/irq_cpustat.h>
#include <asm-i386/softirq.h>
#include <linux/kernel.h>
#include <linux/threads.h>
#include <linux/linkage.h>
#include <asm-i386/types.h>
#include <asm-i386/system.h>
#include <linux/interrupt.h>

irq_cpustat_t irq_stat[NR_CPUS];

static struct softirq_action softirq_vec[32] __cacheline_aligned;

asmlinkage void do_softirq()
{
	int cpu = 0;
	__u32 active, mask;

	if(in_interrupt()) {
		return;
	}

	local_bh_disable();

	local_irq_disable();
	mask = softirq_mask();
	active = softirq_active() & mask;

	if (active) {
		struct softirq_action *h;

restart:
		/* Reset active bitmask before enabling irqs */
		softirq_active() &= ~active;

		local_irq_enable();

		h = softirq_vec;
		mask &= ~active;

		do {
			if (active & 1) {
				h->action(h);
			}
			h++;
			active >>= 1;
		} while (active);

		local_irq_disable();

		active = softirq_active();
		if ((active &= mask) != 0) {
			goto retry;
		}
	}

	local_bh_enable();
	/* Leave with locally disabled hard irqs. It is critical to close
	 * window for infinite recursion, while we help local bh count,
	 * it protected us. Now we are defenceless.
	 */
	return;

retry:
	goto restart;
}
