#include <linux/time.h>
#include <linux/sched.h>
#include <asm-i386/param.h>
#include <linux/interrupt.h>
#include <asm-i386/stdio.h>
#include <linux/list.h>

/*
 * Timekeeping variables
 */

long tick = (1000000 + HZ/2) / HZ;	/* timer interrupt period */

/* The current time */
volatile struct timeval xtime __attribute__ ((aligned(16)));

unsigned long volatile jiffies;

/* Event timer code */
#define TVN_BITS 6
#define TVR_BITS 8
#define TVN_SIZE (1 << TVN_BITS)   // TVN_SIZE = 64
#define TVR_SIZE (1 << TVR_BITS)   // TVR_SIZE = 256
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)

struct timer_vec {
	int index;
	struct list_head vec[TVN_SIZE];
};

struct timer_vec_root {
	int index;
	struct list_head vec[TVR_SIZE];
};

static struct timer_vec tv5;
static struct timer_vec tv4;
static struct timer_vec tv3;
static struct timer_vec tv2;
static struct timer_vec_root tv1;

static struct timer_vec * const tvecs[] = {
	(struct timer_vec *)&tv1, &tv2, &tv3, &tv4, &tv5
};

#define NOOF_TVECS (sizeof(tvecs) / sizeof(tvecs[0]))

void init_timervecs(void)
{
	int i;
	for (i = 0; i < TVN_SIZE; i++) {
		INIT_LIST_HEAD(tv5.vec + i);
		INIT_LIST_HEAD(tv4.vec + i);
		INIT_LIST_HEAD(tv3.vec + i);
		INIT_LIST_HEAD(tv2.vec + i);
	}
	for (i = 0; i < TVR_SIZE; i++) {
		INIT_LIST_HEAD(tv1.vec + i);
	}
}

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