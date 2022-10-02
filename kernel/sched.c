#include <linux/sched.h>
#include <linux/kernel_stat.h>

extern void timer_bh(void);

struct kernel_stat kstat;

extern void init_timervecs(void);

void sched_init(void)
{
	int cpu = 0;
	int nr;

	init_task.processor = cpu;

	for(nr = 0; nr < PIDHASH_SZ; nr++) {
		pidhash[nr] = NULL;
	}

	init_timervecs();
}
