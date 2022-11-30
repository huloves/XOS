#include <linux/sched.h>
#include <asm-i386/processor.h>
#include <asm-i386/stdio.h>
#include <asm-i386/page.h>
#include <asm-i386/resource.h>
#include <linux/slab.h>
#include <asm-i386/errno.h>
#include <linux/threads.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

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
	printk("do_fork start.\n");
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
	p->pid = get_pid(clone_flags);

	p->run_list.next = NULL;
	p->run_list.prev = NULL;

	if ((clone_flags & CLONE_VFORK) || !(clone_flags & CLONE_PARENT)) {
		p->p_opptr = current;
		if (!(p->ptrace & PT_PTRACED))
			p->p_pptr = current;
	}
	p->p_cptr = NULL;
	init_waitqueue_head(&p->wait_chldexit);
	// p->vfork_sem = NULL;
	spin_lock_init(&p->alloc_lock);

	p->sigpending = 0;
	// init_sigpending(&p->pending);   // 对子进程的待处理信号队列以及有关结构成分的初始化

	p->it_real_value = p->it_virt_value = p->it_prof_value = 0;
	p->it_real_incr = p->it_virt_incr = p->it_prof_incr = 0;
	init_timer(&p->real_timer);
	p->real_timer.data = (unsigned long) p;

	p->leader = 0;		/* session leadership doesn't inherit */
	p->tty_old_pgrp = 0;
	// p->times.tms_utime = p->times.tms_stime = 0;
	// p->times.tms_cutime = p->times.tms_cstime = 0;

	p->lock_depth = -1;		/* -1 = no lock */
	p->start_time = jiffies;

	retval = -ENOMEM;
	/* copy all the process information */
	// if (copy_files(clone_flags, p))
	// 	goto bad_fork_cleanup;
	// if (copy_fs(clone_flags, p))
	// 	goto bad_fork_cleanup_files;
	// if (copy_sighand(clone_flags, p))
	// 	goto bad_fork_cleanup_fs;
	// if (copy_mm(clone_flags, p))
	// 	goto bad_fork_cleanup_sighand;
	retval = copy_thread(0, clone_flags, stack_start, stack_size, p, regs);
	if (retval)
		goto bad_fork_cleanup_sighand;
	// p->semundo = NULL;

	/* Our parent execution domain becomes current domain
	   These must match for thread signalling to apply */
	
	p->parent_exec_id = p->self_exec_id;   // parent_exec_id表示父进程的执行域，self_exec_id表示本进程的执行域

	/* ok, now we should be set up.. */
	p->swappable = 1;   // swapperable表示本进程的存储页面可以被换出
	p->exit_signal = clone_flags & CSIGNAL;   // exit_signal为本进程执行exit()时向父进程发出的信号
	p->pdeath_signal = 0;   // pdeath_signal为父进程在执行exit()时向本进程发出的信号

	/*
	 * "share" dynamic priority between parent and child, thus the
	 * total amount of dynamic priorities in the system doesnt change,
	 * more scheduling fairness. This is only important in the first
	 * timeslice, on the long run the scheduling behaviour is unchanged.
	 */
	p->counter = (current->counter + 1) >> 1;   // counter字段的值就是进程的运行时间配额
	current->counter >>= 1;
	if (!current->counter)
		current->need_resched = 1;

	/*
	 * Ok, add it to the run-queues and make it
	 * visible to the rest of the system.
	 *
	 * Let it rip!
	 */
	retval = p->pid;
	p->tgid = retval;
	INIT_LIST_HEAD(&p->thread_group);
	write_lock_irq(&tasklist_lock);
	if (clone_flags & CLONE_THREAD) {   // 如果创建的是线程，则还要通过thread_group与父进程链接在一起
		p->tgid = current->tgid;
		list_add(&p->thread_group, &current->thread_group);
	}
	SET_LINKS(p);   // 将子进程的task_struct结构链入内核的进程队列
	hash_pid(p);    // 将子进程链入按其pid计算得的杂凑队列
	nr_threads++;
	write_unlock_irq(&tasklist_lock);

	// if (p->ptrace & PT_PTRACED)
	// 	send_sig(SIGSTOP, p, 1);

	wake_up_process(p);		/* do this last */
	++total_forks;

	printk("PPPPP\n");

fork_out:
	// if ((clone_flags & CLONE_VFORK) && (retval > 0)) 
		// down(&sem);
	return retval;

bad_fork_cleanup_sighand:
	// exit_sighand(p);
bad_fork_cleanup_fs:
	// exit_fs(p); /* blocking */
bad_fork_cleanup_files:
	// exit_files(p); /* blocking */
bad_fork_cleanup:
	// put_exec_domain(p->exec_domain);
	// if (p->binfmt && p->binfmt->module)
		// __MOD_DEC_USE_COUNT(p->binfmt->module);
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
