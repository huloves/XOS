#ifndef _LINUX_SCHED_H
#define _LINUX_SCHED_H

#include <linux/list.h>
#include <linux/spinlock.h>
#include <asm-i386/types.h>
#include <asm-i386/uaccess.h>
#include <asm/param.h>
#include <linux/time.h>
#include <asm/ptrace.h>
#include <linux/resource.h>
#include <asm/resource.h>
#include <asm/current.h>
#include <linux/personality.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <asm/mmu.h>
#include <asm/processor.h>
#include <linux/linkage.h>

/*
 * cloning flags:
 */
#define CSIGNAL			0x000000ff	/* signal mask to be sent at exit */
#define CLONE_VM		0x00000100	/* set if VM shared between processes */
#define CLONE_FS		0x00000200	/* set if fs info shared between processes */
#define CLONE_FILES		0x00000400	/* set if open files shared between processes */
#define CLONE_SIGHAND	0x00000800	/* set if signal handlers and blocked signals shared */
#define CLONE_PID		0x00001000	/* set if pid shared */
#define CLONE_PTRACE	0x00002000	/* set if we want to let tracing continue on the child too */
#define CLONE_VFORK		0x00004000	/* set if the parent wants the child to wake it up on mm_release */
#define CLONE_PARENT	0x00008000	/* set if we want to have the same parent as the cloner */
#define CLONE_THREAD	0x00010000	/* Same thread group? */

#define CLONE_SIGNAL	(CLONE_SIGHAND | CLONE_THREAD)

extern int nr_running, nr_threads;
extern int last_pid;

#define TASK_RUNNING			0   // 可以被调度
#define TASK_INTERRUPTIBLE		1   // 可以因信号的到来而被唤醒
#define TASK_UNINTERRUPTIBLE	2   // 深度睡眠，不受信号的打扰
#define TASK_ZOMBIE				4
#define TASK_STOPPED			8   // 收到 SIGSTOP 信号后，刮起状态

#define _STK_LIM	(8*1024*1024)
#define DEF_COUNTER	(10*HZ/100)	/* 100 ms time slice */
#define MAX_COUNTER	(20*HZ/100)
#define DEF_NICE	(0)
/*
 * Scheduling policies
 */
#define SCHED_OTHER		0
#define SCHED_FIFO		1
#define SCHED_RR		2

/*
 * This is an additional bit set when we want to
 * yield the CPU for one re-schedule..
 */
#define SCHED_YIELD		0x10

void trap_init(void);

/*
 * This serializes "schedule()" and also protects
 * the run-queue from deletions/modifications (but
 * _adding_ to the beginning of the run-queue has
 * a separate lock).
 */
extern rwlock_t tasklist_lock;
extern spinlock_t runqueue_lock;
extern spinlock_t mmlist_lock;

extern void sched_init(void);
asmlinkage void schedule(void);

struct mm_struct {
	struct vm_area_struct * mmap;		/* list of VMAs */
	struct vm_area_struct * mmap_avl;	/* tree of VMAs */
	struct vm_area_struct * mmap_cache;	/* last find_vma result */
	pgd_t * pgd;
	atomic_t mm_users;			/* How many users with user space? */
	atomic_t mm_count;			/* How many references to "struct mm_struct" (users count as 1) */
	int map_count;				/* number of VMAs */
	// struct semaphore mmap_sem;
	spinlock_t page_table_lock;

	struct list_head mmlist;		/* List of all active mm's */

	unsigned long start_code, end_code, start_data, end_data;
	unsigned long start_brk, brk, start_stack;
	unsigned long arg_start, arg_end, env_start, env_end;
	unsigned long rss, total_vm, locked_vm;
	unsigned long def_flags;
	unsigned long cpu_vm_mask;
	unsigned long swap_cnt;	/* number of pages to swap on next pass */
	unsigned long swap_address;

	/* Architecture-specific MM context */
	mm_context_t context;
};

/*
 * Some day this will be a full-fledged user tracking system..
 */
struct user_struct {
	atomic_t __count;	/* reference count */
	atomic_t processes;	/* How many processes does this user have? */
	atomic_t files;		/* How many open files does this user have? */

	/* Hash table maintenance information */
	struct user_struct *next, **pprev;
	uid_t uid;
};

#define get_current_user() ({ 				\
	struct user_struct *__user = current->user;	\
	atomic_inc(&__user->__count);			\
	__user; })

extern struct user_struct root_user;

#define INIT_USER (&root_user)

struct task_struct {
	/*
	 * offsets of these are hardcoded elsewhere - touch with care
	 */
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	unsigned long flags;	/* per process flags, defined below */
	int sigpending;   // 表示进程收到信号但尚未处理
	/*mm_segment_t addr_limit;	/* thread address space:
					 	0-0xBFFFFFFF for user-thead
						0-0xFFFFFFFF for kernel-thread
					 */
	// struct exec_domain *exec_domain;   // 指向描述本进程所属执行域的数据结构
	volatile long need_resched;   // 表示CPU从系统空间返回用户空间需要一次调度
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
	struct mm_struct *mm;
	int has_cpu, processor;
	unsigned long cpus_allowed;
	/*
	 * (only the 'next' pointer fits into the cacheline, but
	 * that's just fine.)
	 */
	struct list_head run_list;   // 可执行队列节点，只有在可执行队列才有可能被调度运行
	unsigned long sleep_time;

	struct task_struct *next_task, *prev_task;
	struct mm_struct *active_mm;

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
	struct task_struct *p_opptr, *p_pptr, *p_cptr, *p_ysptr, *p_osptr;   // p_opptr,p_pptr指向父进程的task_struct，p_cptr指向最年轻的子进程，p_ysptr和p_osptr指向弟弟和哥哥
	struct list_head thread_group;

	/* PID hash table linkage. */
	struct task_struct *pidhash_next;
	struct task_struct **pidhash_pprev;

	wait_queue_head_t wait_chldexit;	/* for wait4() */
	// struct semaphore *vfork_sem;		/* for vfork() */
	unsigned long rt_priority;
	unsigned long it_real_value, it_prof_value, it_virt_value;
	unsigned long it_real_incr, it_prof_incr, it_virt_incr;
	struct timer_list real_timer;
	// struct tms times;   // 对per_cpu_utime[] and per_cpu_stime[] 的汇总
	unsigned long start_time;
	// long per_cpu_utime[NR_CPUS], per_cpu_stime[NR_CPUS];   // 表示进程在各个处理器上运行于用户空间和系统空间的累计时间
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
	struct user_struct *user;
/* limits */
	struct rlimit rlim[RLIM_NLIMITS];
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
	struct thread_struct thread;
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

/*
 * Per process flags
 */
#define PF_ALIGNWARN	0x00000001	/* Print alignment warning msgs */
									/* Not implemented yet, only for 486*/
#define PF_STARTING		0x00000002	/* being created */
#define PF_EXITING		0x00000004	/* getting shut down */
#define PF_FORKNOEXEC	0x00000040	/* forked but didn't exec */
#define PF_SUPERPRIV	0x00000100	/* used super-user privileges */
#define PF_DUMPCORE		0x00000200	/* dumped core */
#define PF_SIGNALED		0x00000400	/* killed by a signal */
#define PF_MEMALLOC		0x00000800	/* Allocating memory */
#define PF_VFORK		0x00001000	/* Wake up parent in mm_release */

#define PF_USEDFPU		0x00100000	/* task used FPU this quantum (SMP) */

/*
 * Ptrace flags
 */

#define PT_PTRACED		0x00000001
#define PT_TRACESYS		0x00000002
#define PT_DTRACE		0x00000004		/* delayed trace (used on m68k, i386) */
#define PT_TRACESYSGOOD	0x00000008

#define INIT_TASK(tsk)	\
{																				\
    state:				0,														\
    flags:				0,														\
    sigpending:			0,														\
    lock_depth:			-1,														\
    counter:			DEF_COUNTER,											\
    nice:				DEF_NICE,												\
    policy:				SCHED_OTHER,											\
    cpus_allowed:		-1,														\
    run_list:			LIST_HEAD_INIT(tsk.run_list),							\
    next_task:			&tsk,													\
    prev_task:			&tsk,													\
    p_opptr:			&tsk,													\
    p_pptr:				&tsk,													\
    thread_group:		(struct list_head)LIST_HEAD_INIT(tsk.thread_group),		\
    keep_capabilities:	0,														\
	user:				INIT_USER,												\
    comm:				"swapper",												\
    alloc_lock:			SPIN_LOCK_UNLOCKED										\
}

#ifndef INIT_TASK_SIZE
# define INIT_TASK_SIZE	2048*sizeof(long)
#endif

union task_union {
    struct task_struct task;
    unsigned long stack[INIT_TASK_SIZE/sizeof(long)];
};

extern union task_union init_task_union;

/* PID hashing. (shouldnt this be dynamic?) */
#define PIDHASH_SZ (4096 >> 2)
extern struct task_struct *pidhash[PIDHASH_SZ];

#define pid_hashfn(x)	((((x) >> 8) ^ (x)) & (PIDHASH_SZ - 1))

static inline void hash_pid(struct task_struct *p)
{
	struct task_struct **htable = &pidhash[pid_hashfn(p->pid)];

	if((p->pidhash_next = *htable) != NULL)
		(*htable)->pidhash_pprev = &p->pidhash_next;
	*htable = p;
	p->pidhash_pprev = htable;
}

/* per-UID process charging. */
extern struct user_struct * alloc_uid(uid_t);
extern void free_uid(struct user_struct *);

extern unsigned long volatile jiffies;
volatile struct timeval xtime __attribute__ ((aligned(16)));

void do_timer(struct pt_regs *regs);

extern void proc_caches_init(void);

extern void wake_up_process(struct task_struct * p);
extern int do_fork(unsigned long, unsigned long, struct pt_regs *, unsigned long);

#define SET_LINKS(p) do { \
	(p)->next_task = &init_task; \
	(p)->prev_task = init_task.prev_task; \
	init_task.prev_task->next_task = (p); \
	init_task.prev_task = (p); \
	(p)->p_ysptr = NULL; \
	if (((p)->p_osptr = (p)->p_pptr->p_cptr) != NULL) \
		(p)->p_osptr->p_ysptr = p; \
	(p)->p_pptr->p_cptr = p; \
	} while (0)

#define for_each_task(p) \
	for (p = &init_task ; (p = p->next_task) != &init_task ; )

extern int  copy_thread(int, unsigned long, unsigned long, unsigned long, struct task_struct *, struct pt_regs *);

static inline void del_from_runqueue(struct task_struct * p)
{
	nr_running--;
	p->sleep_time = jiffies;
	list_del(&p->run_list);
	p->run_list.next = NULL;
}

static inline int task_on_runqueue(struct task_struct *p)
{
	return (p->run_list.next != NULL);
}

#endif /* _LINUX_SCHED_H */
