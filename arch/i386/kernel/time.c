#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm-i386/signal.h>
#include <asm-i386/ptrace.h>
#include <linux/irq.h>
#include <asm-i386/stdio.h>

extern rwlock_t xtime_lock;

unsigned long get_cmos_time(void)
{
	unsigned int year, mon, day, hour, min, sec;
	int i;
}

/*
 * timer_interrupt() needs to keep up the real-time clock,
 * as well as call the "do_timer()" routine every clocktick
 */
static inline void do_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	do_timer(regs);
}

/*
 * This is the same as the above, except we _alse_ save the current
 * Time Stamp Counter value at the time of the timer interrupt, so that
 * we later on can estimate the time of day more exactly.
*/
static void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	int count;

	/*
	 * Here we are in the timer irq handler. We just have irqs locally
	 * disabled but we don't know if the timer_bh is running on the other
	 * CPU. We need to avoid to SMP race with it. NOTE: we don' t need
	 * the irq version of write_lock because as just said we have irq
	 * locally disabled. -arca
	 */
	do_timer_interrupt(irq, dev_id, regs);
}

static struct irqaction irq0  = { timer_interrupt, SA_INTERRUPT, 0, "timer", NULL, NULL};

void time_init(void)
{
	printk("time_init start.\n");
	setup_irq(0, &irq0);
	printk("time init down.\n");
	return;
}
