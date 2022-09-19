#include <asm-i386/hardirq.h>
#include <linux/irq_cpustat.h>
#include <asm-i386/softirq.h>
#include <linux/kernel.h>
#include <linux/threads.h>
#include <linux/linkage.h>
#include <asm-i386/types.h>
#include <asm-i386/system.h>
#include <linux/interrupt.h>
#include <linux/cache.h>
#include <asm-i386/atomic.h>
#include <linux/spinlock.h>
#include <asm-i386/hardirq.h>
#include <asm-i386/types.h>

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

static spinlock_t softirq_mask_lock = SPIN_LOCK_UNLOCKED;

void open_softirq(int nr, void (*action)(struct softirq_action*), void *data)
{
	unsigned long flags;
	int i;

	spin_lock_irqsave(&softirq_mask_lock, flags);
	softirq_vec[nr].data = data;
	softirq_vec[nr].action = action;

	for (i=0; i<NR_CPUS; i++)
		softirq_mask() |= (1<<nr);
	spin_unlock_irqrestore(&softirq_mask_lock, flags);
}

/* Tasklets */

struct tasklet_head tasklet_vec[NR_CPUS] __cacheline_aligned;

static void tasklet_action(struct softirq_action *a)
{
	int cpu = 0;
	
	struct tasklet_struct *list;
	local_irq_disable();
	list = tasklet_vec[cpu].list;
	tasklet_vec[cpu].list = NULL;
	local_irq_enable();

	while(list != NULL) {
		struct tasklet_struct *t = list;
		
		list = list->next;

		if(tasklet_trylock(t)) {
			if (atomic_read(&t->count) == 0) {
				clear_bit(TASKLET_STATE_SCHED, &t->state);

				t->func(t->data);

				continue;
			}
			tasklet_unlock(t);
		}
		local_irq_disable();
		t->next = tasklet_vec[cpu].list;
		tasklet_vec[cpu].list = t;
		__cpu_raise_softirq(cpu, TASKLET_SOFTIRQ);
		local_irq_enable();
	}
}

struct tasklet_head tasklet_hi_vec[NR_CPUS] __cacheline_aligned;

void tasklet_init(struct tasklet_struct *t,
				  void (*func)(unsigned long), unsigned long data)
{
	t->func = func;
	t->data = data;
	t->state = 0;
	atomic_set(&t->count, 0);
}

/* Old style BHs */
static void (*bh_base[32])(void);
struct tasklet_struct bh_task_vec[32];

spinlock_t global_bh_lock = SPIN_LOCK_UNLOCKED;

static void bh_action(unsigned long nr)
{
	int cpu = 0;
	if (!spin_trylock(&global_bh_lock)) {
		goto resched;
	}
	if (!hardirq_trylock()) {
		goto resched_unlock;
	}
	if (bh_base[nr]) {
		bh_base[nr]();
	}

	hardirq_endlock();
	spin_unlock(&global_bh_lock);

resched_unlock:
	spin_unlock(&global_bh_lock);
resched:
	mark_bh(nr);
}

void softirq_init()
{
	int i;
	for (i = 0; i < 32; i++) {
		tasklet_init(bh_task_vec + i, bh_action, i);
	}
}
