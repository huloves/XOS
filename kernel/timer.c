#include <linux/time.h>
#include <linux/sched.h>
#include <asm-i386/param.h>
#include <linux/interrupt.h>
#include <asm-i386/stdio.h>

/*
 * Timekeeping variables
 */

long tick = (1000000 + HZ/2) / HZ;	/* timer interrupt period */

/* The current time */
volatile struct timeval xtime __attribute__ ((aligned(16)));

unsigned long volatile jiffies;

void update_process_times()
{
	printk("update_process_times happen\n\n");
}

void timer_bh(void)
{
	printk("timer_bh happen\n");
}

void do_timer(struct pt_regs *regs)
{
	(*(unsigned long *)&jiffies)++;
	update_process_times();
	mark_bh(TIMER_BH);
}