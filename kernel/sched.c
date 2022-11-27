#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/interrupt.h>
#include <asm-i386/stdio.h>

static LIST_HEAD(runqueue_head);

extern void timer_bh(void);

struct kernel_stat kstat;

extern void init_timervecs(void);

static inline int goodness(struct task_struct * p, int this_cpu, struct mm_struct *this_mm)
{
    int weight;

    /*
     * select the current process after every other
     * runnable process, but before the idle thread.
     * Also, dont trigger a counter recalculation.
     */
    weight = -1;
    if (p->policy & SCHED_YIELD)
        goto out;

    /*
     * Non-RT process - normal case first.
     */
    if (p->policy == SCHED_OTHER) {
        /*
         * Give the process a first-approximation goodness value
         * according to the number of clock-ticks it has left.
         *
         * Don't do any other calculations if the time slice is
         * over..
         */
        weight = p->counter;
        if (!weight)
            goto out;
        /* .. and a slight advantage to the current MM */
//        if (p->mm == this_mm || !p->mm)
//            weight += 1;
        weight += 20 - p->nice;
        goto out;
    }

    /*
     * Realtime process, select the first one on the
     * runqueue (taking priorities within processes
     * into account).
     */
    weight = 1000 + p->rt_priority;
out:
    return weight;
}

static inline int preemption_goodness(struct task_struct * prev, struct task_struct * p, int cpu)
{
    return goodness(p, cpu, prev->active_mm) - goodness(prev, cpu, prev->active_mm);
}

static void reschedule_idle(struct task_struct * p)
{
	int this_cpu = 0;
	struct task_struct *tsk;

	tsk = current;
	if (preemption_goodness(tsk, p, this_cpu) > 1)
		tsk->need_resched = 1;
}

/*
 * Careful!
 *
 * This has to add the process to the _beginning_ of the
 * run-queue, not the end. See the comment about "This is
 * subtle" in the scheduler proper..
 */
static inline void add_to_runqueue(struct task_struct * p)
{
	list_add(&p->run_list, &runqueue_head);
	nr_running++;
}

static inline void move_last_runqueue(struct task_struct * p)
{
	list_del(&p->run_list);
	list_add_tail(&p->run_list, &runqueue_head);
}

static inline void move_first_runqueue(struct task_struct * p)
{
	list_del(&p->run_list);
	list_add(&p->run_list, &runqueue_head);
}

/*
 * Wake up a process. Put it on the run-queue if it's not
 * already there.  The "current" process is always on the
 * run-queue (except when the actual re-schedule is in
 * progress), and as such you're allowed to do the simpler
 * "current->state = TASK_RUNNING" to mark yourself runnable
 * without the overhead of this.
 */
void wake_up_process(struct task_struct * p)
{
	unsigned long flags;

	/*
	 * We want the common case fall through straight, thus the goto.
	 */
	spin_lock_irqsave(&runqueue_lock, flags);
	p->state = TASK_RUNNING;
	if (task_on_runqueue(p))
		goto out;
	add_to_runqueue(p);   // 介入可执行队列
	reschedule_idle(p);   // 将唤醒进程与当前进程比较,如果唤醒进程比当前进程权值高,那就把当前进程的need_resched设置为1
out:
	spin_unlock_irqrestore(&runqueue_lock, flags);
}

void sched_init(void)
{
	int cpu = 0;
	int nr;

	init_task.processor = cpu;

	for(nr = 0; nr < PIDHASH_SZ; nr++) {
		pidhash[nr] = NULL;
	}

	init_timervecs();   // 初始化内核定时器

	init_bh(TIMER_BH, timer_bh);
}
