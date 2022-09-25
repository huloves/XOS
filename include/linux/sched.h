#ifndef _LINUX_SCHED_H
#define _LINUX_SCHED_H

#include <linux/list.h>
#include <linux/spinlock.h>
#include <asm-i386/types.h>
#include <asm-i386/uaccess.h>
#include <asm-i386/param.h>
#include <linux/time.h>
#include <asm-i386/ptrace.h>

#define _STK_LIM	(8*1024*1024)
#define DEF_COUNTER	(10*HZ/100)	/* 100 ms time slice */
#define DEF_NICE	(0)
/*
 * Scheduling policies
 */
#define SCHED_OTHER		0
#define SCHED_FIFO		1
#define SCHED_RR		2

void trap_init(void);

struct task_struct {
	/*
	 * offsets of these are hardcoded elsewhere - touch with care
	 */
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	unsigned long flags;	/* per process flags, defined below */
	int sigpending;
	/*mm_segment_t addr_limit;	/* thread address space:
					 	0-0xBFFFFFFF for user-thead
						0-0xFFFFFFFF for kernel-thread
					 */
	// struct exec_domain *exec_domain;
	volatile long need_resched;
	unsigned long ptrace;

	int lock_depth;		/* Lock depth */

/*
 * offset 32 begins here on 32-bit platforms. We keep
 * all fields in a single cacheline that are needed for
 * the goodness() loop in schedule().
 */
	long counter;
	long nice;
	unsigned long policy;
	// struct mm_struct *mm;
	int has_cpu, processor;
	unsigned long cpus_allowed;
	/*
	 * (only the 'next' pointer fits into the cacheline, but
	 * that's just fine.)
	 */
	struct list_head run_list;
	unsigned long sleep_time;

	struct task_struct *next_task, *prev_task;
	// struct mm_struct *active_mm;

/* task state */
	// struct linux_binfmt *binfmt;
	int exit_code, exit_signal;
	int pdeath_signal;  /*  The signal sent when the parent dies  */
	/* ??? */
	unsigned long personality;
	int dumpable:1;
	int did_exec:1;
	pid_t pid;
	pid_t pgrp;
	pid_t tty_old_pgrp;
	pid_t session;
	pid_t tgid;
	/* boolean value for session group leader */
	int leader;
	/* 
	 * pointers to (original) parent process, youngest child, younger sibling,
	 * older sibling, respectively.  (p->father can be replaced with 
	 * p->p_pptr->pid)
	 */
	struct task_struct *p_opptr, *p_pptr, *p_cptr, *p_ysptr, *p_osptr;
	struct list_head thread_group;

	/* PID hash table linkage. */
	struct task_struct *pidhash_next;
	struct task_struct **pidhash_pprev;

	// wait_queue_head_t wait_chldexit;	/* for wait4() */
	// struct semaphore *vfork_sem;		/* for vfork() */
	unsigned long rt_priority;
	unsigned long it_real_value, it_prof_value, it_virt_value;
	unsigned long it_real_incr, it_prof_incr, it_virt_incr;
	// struct timer_list real_timer;
	// struct tms times;
	unsigned long start_time;
	// long per_cpu_utime[NR_CPUS], per_cpu_stime[NR_CPUS];
/* mm fault and swap info: this can arguably be seen as either mm-specific or thread-specific */
	unsigned long min_flt, maj_flt, nswap, cmin_flt, cmaj_flt, cnswap;
	int swappable:1;
/* process credentials */
	uid_t uid,euid,suid,fsuid;
	gid_t gid,egid,sgid,fsgid;
	int ngroups;
	// gid_t	groups[NGROUPS];
	// kernel_cap_t   cap_effective, cap_inheritable, cap_permitted;
	int keep_capabilities:1;
	// struct user_struct *user;
/* limits */
	// struct rlimit rlim[RLIM_NLIMITS];
	unsigned short used_math;
	char comm[16];
/* file system info */
	int link_count;
	// struct tty_struct *tty; /* NULL if no tty */
	unsigned int locks; /* How many file locks are being held */
/* ipc stuff */
	// struct sem_undo *semundo;
	// struct sem_queue *semsleeping;
/* CPU-specific state of this task */
	// struct thread_struct thread;
/* filesystem information */
	// struct fs_struct *fs;
/* open file information */
	// struct files_struct *files;
/* signal handlers */
	// spinlock_t sigmask_lock;	/* Protects signal and blocked */
	// struct signal_struct *sig;

	// sigset_t blocked;
	// struct sigpending pending;

	unsigned long sas_ss_sp;
	size_t sas_ss_size;
	int (*notifier)(void *priv);
	void *notifier_data;
	// sigset_t *notifier_mask;
	
/* Thread group tracking */
   	u32 parent_exec_id;
   	u32 self_exec_id;
/* Protection of (de-)allocation: mm, files, fs, tty */
	spinlock_t alloc_lock;
};

#define INIT_TASK(tsk)	\
{									\
    state:		0,						\
    flags:		0,						\
    sigpending:		0,						\
    lock_depth:		-1,						\
    counter:		DEF_COUNTER,					\
    nice:		DEF_NICE,					\
    policy:		SCHED_OTHER,					\
    cpus_allowed:	-1,						\
    run_list:		LIST_HEAD_INIT(tsk.run_list),			\
    next_task:		&tsk,						\
    prev_task:		&tsk,						\
    p_opptr:		&tsk,						\
    p_pptr:		&tsk,						\
    thread_group:	(struct list_head)LIST_HEAD_INIT(tsk.thread_group),		\
    keep_capabilities:	0,						\
    comm:		"swapper",					\
    alloc_lock:		SPIN_LOCK_UNLOCKED				\
}

#ifndef INIT_TASK_SIZE
# define INIT_TASK_SIZE	2048*sizeof(long)
#endif

union task_union {
    struct task_struct task;
    unsigned long stack[INIT_TASK_SIZE/sizeof(long)];
};

extern union task_union init_task_union;

extern unsigned long volatile jiffies;
volatile struct timeval xtime __attribute__ ((aligned(16)));

void do_timer(struct pt_regs *regs);

#endif /* _LINUX_SCHED_H */
