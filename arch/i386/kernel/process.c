#include <asm-i386/processor.h>
#include <asm-i386/unistd.h>

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
