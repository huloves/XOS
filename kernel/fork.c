#include <linux/sched.h>
#include <asm-i386/processor.h>
#include <asm-i386/stdio.h>
#include <asm-i386/page.h>
#include <asm-i386/resource.h>
#include <linux/slab.h>
#include <asm-i386/errno.h>
#include <linux/threads.h>
#include <linux/spinlock.h>

/* The idle threads do not count.. */
int nr_threads;
int nr_running;

int max_threads;
unsigned long total_forks;	/* Handle normal Linux uptimes. */
int last_pid;

struct task_struct *pidhash[PIDHASH_SZ];

void fork_init(unsigned long mempages)
{
	printk("fork_init start.\n");
	/*
	 * The default maximum number of threads is set to a safe
	 * value: the thread structures can take up at most half
	 * of memory.
	 */
	max_threads = mempages / (THREAD_SIZE/PAGE_SIZE) / 8;

	init_task.rlim[RLIMIT_NPROC].rlim_cur = max_threads/2;
	init_task.rlim[RLIMIT_NPROC].rlim_max = max_threads/2;

	printk("fork_init down.\n");
}

/* Protects next_safe and last_pid. */
spinlock_t lastpid_lock = SPIN_LOCK_UNLOCKED;

static int get_pid(unsigned long flags)
{
	static int next_safe = PID_MAX;
	struct task_struct *p;

	if (flags & CLONE_PID)
		return current->pid;

	spin_lock(&lastpid_lock);
	if((++last_pid) & 0xffff8000) {
		last_pid = 300;		/* Skip daemons etc. */
		goto inside;
	}
	if(last_pid >= next_safe) {
inside:
		next_safe = PID_MAX;
		read_lock(&tasklist_lock);
	repeat:
		for_each_task(p) {
			if(p->pid == last_pid	||
			   p->pgrp == last_pid	||
			   p->session == last_pid) {
				if(++last_pid >= next_safe) {
					if(last_pid & 0xffff8000)
						last_pid = 300;
					next_safe = PID_MAX;
				}
				goto repeat;
			}
			if(p->pid > last_pid && next_safe > p->pid)
				next_safe = p->pid;
			if(p->pgrp > last_pid && next_safe > p->pgrp)
				next_safe = p->pgrp;
			if(p->session > last_pid && next_safe > p->session)
				next_safe = p->session;
		}
		read_unlock(&tasklist_lock);
	}
	spin_unlock(&lastpid_lock);

	return last_pid;
}

static inline void copy_flags(unsigned long clone_flags, struct task_struct *p)
{
	unsigned long new_flags = p->flags;

	new_flags &= ~(PF_SUPERPRIV | PF_USEDFPU | PF_VFORK);
	new_flags |= PF_FORKNOEXEC;
	if (!(clone_flags & CLONE_PTRACE))
		p->ptrace = 0;
	if (clone_flags & CLONE_VFORK)
		new_flags |= PF_VFORK;
	p->flags = new_flags;
}

/*
 *  Ok, this is the main fork-routine. It copies the system process
 * information (task[nr]) and sets up the necessary registers. It also
 * copies the data segment in its entirety.  The "stack_start" and
 * "stack_top" arguments are simply passed along to the platform
 * specific copy_thread() routine.  Most platforms ignore stack_top.
 * For an example that's using stack_top, see
 * arch/ia64/kernel/process.c.
 */
int do_fork(unsigned long clone_flags, unsigned long stack_start,
	    struct pt_regs *regs, unsigned long stack_size)
{
	int retval = -ENOMEM;
	struct task_struct *p;
	// DECLARE_MUTEX_LOCKED(sem);

	if (clone_flags & CLONE_PID) {
		/* This is only allowed from the boot up thread */
		if (current->pid)   // 只有0号进程允许使用CLONE_PID标志调用该函数
			return -EPERM;
	}

	// current->vfork_sem = &sem;
	p = alloc_task_struct();   // 为子进程分配两个连续的物理页面，低端用作子进程的task_struct结构，高端则用作系统空间堆栈
	if (!p)
		goto fork_out;

	*p = *current;   // 整个数据结构的赋值，父进程的整个task_struct复制到子进程的数据结构中

	retval = -EAGAIN;

	if (atomic_read(&p->user->processes) >= p->rlim[RLIMIT_NPROC].rlim_cur)
		goto bad_fork_free;
	atomic_inc(&p->user->__count);
	atomic_inc(&p->user->processes);

	/*
	 * Counter increases are protected by
	 * the kernel lock so nr_threads can't
	 * increase under us (but it may decrease).
	 */
	if (nr_threads >= max_threads)
		goto bad_fork_cleanup_count;
	
	// get_exec_domain(p->exec_domain);

	// if (p->binfmt && p->binfmt->module)
		// __MOD_INC_USE_COUNT(p->binfmt->module);

	p->did_exec = 0;
	p->swappable = 0;
	p->state = TASK_UNINTERRUPTIBLE;

	copy_flags(clone_flags, p);
fork_out:
	return retval;
bad_fork_cleanup_count:
	atomic_dec(&p->user->processes);
	free_uid(p->user);   // p->user->__count--，若减到0，则释放user
bad_fork_free:
	free_task_struct(p);
	goto fork_out;
}

/* SLAB cache for signal_struct structures (tsk->sig) */
kmem_cache_t *sigact_cachep;

/* SLAB cache for files_struct structures (tsk->files) */
kmem_cache_t *files_cachep;

/* SLAB cache for fs_struct structures (tsk->fs) */
kmem_cache_t *fs_cachep;

/* SLAB cache for vm_area_struct structures */
kmem_cache_t *vm_area_cachep;

/* SLAB cache for mm_struct structures (tsk->mm) */
kmem_cache_t *mm_cachep;

void proc_caches_init(void)
{
	// sigact_cachep = kmem_cache_create("signal_act",
	// 		sizeof(struct signal_struct), 0,
	// 		SLAB_HWCACHE_ALIGN, NULL, NULL);
	// if (!sigact_cachep)
	// 	panic("Cannot create signal action SLAB cache");

	// files_cachep = kmem_cache_create("files_cache", 
	// 		 sizeof(struct files_struct), 0, 
	// 		 SLAB_HWCACHE_ALIGN, NULL, NULL);
	// if (!files_cachep) 
	// 	panic("Cannot create files SLAB cache");

	// fs_cachep = kmem_cache_create("fs_cache", 
	// 		 sizeof(struct fs_struct), 0, 
	// 		 SLAB_HWCACHE_ALIGN, NULL, NULL);
	// if (!fs_cachep) 
	// 	panic("Cannot create fs_struct SLAB cache");
 
	// vm_area_cachep = kmem_cache_create("vm_area_struct",
	// 		sizeof(struct vm_area_struct), 0,
	// 		SLAB_HWCACHE_ALIGN, NULL, NULL);
	// if(!vm_area_cachep)
	// 	panic("vma_init: Cannot alloc vm_area_struct SLAB cache");

	// mm_cachep = kmem_cache_create("mm_struct",
	// 		sizeof(struct mm_struct), 0,
	// 		SLAB_HWCACHE_ALIGN, NULL, NULL);
	// if(!mm_cachep)
	// 	panic("vma_init: Cannot alloc mm_struct SLAB cache");
}
