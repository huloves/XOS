#include <linux/sched.h>
#include <asm-i386/processor.h>
#include <asm-i386/stdio.h>
#include <asm-i386/page.h>
#include <asm-i386/resource.h>
#include <linux/slab.h>
#include <asm-i386/errno.h>

struct task_struct *pidhash[PIDHASH_SZ];

int max_threads;

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

	if (clone_flags & CLONE_PID) {
		/* This is only allowed from the boot up thread */
		if (current->pid)
			return -EPERM;
	}

	// current->vfork_sem = &sem;
	p = alloc_task_struct();
	if (!p)
		// goto fork_out;

	*p = *current;

	retval = -EAGAIN;
// fork_out:
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
