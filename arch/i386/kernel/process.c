#include <asm-i386/processor.h>
#include <asm-i386/unistd.h>
#include <asm-i386/string.h>
#include <linux/linkage.h>
#include <asm-i386/ptrace.h>
#include <linux/sched.h>
#include <asm-i386/signal.h>
#include <asm-i386/stdio.h>

asmlinkage void ret_from_fork(void) __asm__("ret_from_fork");

/*
 * Create a kernel thread
 */
int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags)
{
	long retval, d0;

	__asm__ __volatile__(
		"movl %%esp,%%esi\n\t"
		"int $0x80\n\t"		/* Linux/i386 system call */
		"cmpl %%esp,%%esi\n\t"	/* child or parent? */
		"je 1f\n\t"		/* parent - jump */
		/* Load the argument into eax, and push it.  That way, it does
		 * not matter whether the called function is compiled with
		 * -mregparm or not.  */
		// "movl %4,%%eax\n\t"
		"pushl %%eax\n\t"		
		"call *%5\n\t"		/* call fn */
		"movl %3,%0\n\t"	/* exit */
		"int $0x80\n"
		"1:\t"
		:"=&a" (retval), "=&S" (d0)
		:"0" (__NR_clone), "i" (__NR_exit),
		 "r" (arg), "r" (fn),
		 "b" (flags | CLONE_VM)
		: "memory");
	return retval;
}

/*
 * Save a segment.
 */
#define savesegment(seg,value) \
	asm volatile("mov %%" #seg ",%0":"=m" (*(int *)&(value)))

int copy_thread(int nr, unsigned long clone_flags, unsigned long esp,
		unsigned long unused,
		struct task_struct * p, struct pt_regs * regs)
{
	struct pt_regs * childregs;

	childregs = ((struct pt_regs *) (THREAD_SIZE + (unsigned long) p)) - 1;
	struct_cpy(childregs, regs);
	childregs->eax = 0;
	childregs->esp = esp;

	p->thread.esp = (unsigned long) childregs;
	p->thread.esp0 = (unsigned long) (childregs+1);   // 指向子进程的系统空间堆栈的顶端

	p->thread.eip = (unsigned long) ret_from_fork;

	savesegment(fs,p->thread.fs);
	savesegment(gs,p->thread.gs);

	// unlazy_fpu(current);
	// struct_cpy(&p->thread.i387, &current->thread.i387);

	return 0;
}

/*
 * This special macro can be used to load a debugging register
 */
#define loaddebug(thread,register) \
		__asm__("movl %0,%%db" #register  \
			: /* no output */ \
			:"r" (thread->debugreg[register]))

void __switch_to(struct task_struct *prev_p, struct task_struct *next_p)
{
	struct thread_struct *prev = &prev_p->thread,
				 *next = &next_p->thread;
	struct tss_struct *tss = init_tss + 0;

	// unlazy_fpu(prev_p);

	/*
	 * Reload esp0, LDT and the page table pointer:
	 */
	tss->esp0 = next->esp0;

	/*
	 * Save away %fs and %gs. No need to save %es and %ds, as
	 * those are always kernel segments while inside the kernel.
	 */
	asm volatile("mov %%fs,%0":"=m" (*(int *)&prev->fs));
	asm volatile("mov %%gs,%0":"=m" (*(int *)&prev->gs));

	/*
	 * Restore %fs and %gs.
	 */
	loadsegment(fs, next->fs);
	loadsegment(gs, next->gs);

	/*
	 * Now maybe reload the debug registers
	 */
	if (next->debugreg[7]){
		loaddebug(next, 0);
		loaddebug(next, 1);
		loaddebug(next, 2);
		loaddebug(next, 3);
		/* no 4 and 5 */
		loaddebug(next, 6);
		loaddebug(next, 7);
	}

	if (prev->ioperm || next->ioperm) {
		if (next->ioperm) {
			/*
			 * 4 cachelines copy ... not good, but not that
			 * bad either. Anyone got something better?
			 * This only affects processes which use ioperm().
			 * [Putting the TSSs into 4k-tlb mapped regions
			 * and playing VM tricks to switch the IO bitmap
			 * is not really acceptable.]
			 */
			memcpy(tss->io_bitmap, next->io_bitmap,
				 IO_BITMAP_SIZE*sizeof(unsigned long));
			tss->bitmap = IO_BITMAP_OFFSET;
		} else
			/*
			 * a bitmap offset pointing outside of the TSS limit
			 * causes a nicely controllable SIGSEGV if a process
			 * tries to use a port IO instruction. The first
			 * sys_ioperm() call sets up the bitmap properly.
			 */
			tss->bitmap = INVALID_IO_BITMAP_OFFSET;
	}
}

asmlinkage int sys_fork(struct pt_regs regs)
{
	return do_fork(SIGCHLD, regs.esp, &regs, 0);
}

asmlinkage int sys_clone(struct pt_regs regs)
{	
	printk("sys_clone start.\n");
	unsigned long clone_flags;
	unsigned long newsp;

	clone_flags = regs.ebx;
	newsp = regs.ecx;
	printk("newsp = %d\n", newsp);
	if (!newsp)
		newsp = regs.esp;
	return do_fork(clone_flags, newsp, &regs, 0);
}
