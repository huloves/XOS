#include <asm-i386/processor.h>
#include <asm-i386/unistd.h>
#include <asm-i386/string.h>
#include <linux/linkage.h>
#include <asm-i386/ptrace.h>
#include <linux/sched.h>

asmlinkage void ret_from_fork(void) __asm__("ret_from_fork");

/*
 * Create a kernel thread
 */
int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags)
{
	// long retval, d0;

	// __asm__ __volatile__(
	// 	"movl %%esp,%%esi\n\t"
	// 	"int $0x80\n\t"		/* Linux/i386 system call */
	// 	"cmpl %%esp,%%esi\n\t"	/* child or parent? */
	// 	"je 1f\n\t"		/* parent - jump */
	// 	/* Load the argument into eax, and push it.  That way, it does
	// 	 * not matter whether the called function is compiled with
	// 	 * -mregparm or not.  */
	// 	"movl %4,%%eax\n\t"
	// 	"pushl %%eax\n\t"		
	// 	"call *%5\n\t"		/* call fn */
	// 	"movl %3,%0\n\t"	/* exit */
	// 	"int $0x80\n"
	// 	"1:\t"
	// 	:"=&a" (retval), "=&S" (d0)
	// 	:"0" (__NR_clone), "i" (__NR_exit),
	// 	 "r" (arg), "r" (fn),
	// 	 "b" (flags | CLONE_VM)
	// 	: "memory");
	// return retval;
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
	p->thread.esp0 = (unsigned long) (childregs+1);

	p->thread.eip = (unsigned long) ret_from_fork;

	savesegment(fs,p->thread.fs);
	savesegment(gs,p->thread.gs);

	// unlazy_fpu(current);
	// struct_cpy(&p->thread.i387, &current->thread.i387);

	return 0;
}
