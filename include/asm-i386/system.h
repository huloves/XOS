#ifndef __ASM_SYSTEM_H
#define __ASM_SYSTEM_H

#define __cli() 		__asm__ __volatile__("cli": : :"memory")
#define __sti()			__asm__ __volatile__("sti": : :"memory")
#define cli() __cli()
#define sti() __sti()

/* interrupt control.. */
#define __save_flags(x)		__asm__ __volatile__("pushfl ; popl %0":"=g" (x): /* no input */)
#define __restore_flags(x) 	__asm__ __volatile__("pushl %0 ; popfl": /* no output */ :"g" (x):"memory", "cc")
#define __cli() 		__asm__ __volatile__("cli": : :"memory")
#define __sti()			__asm__ __volatile__("sti": : :"memory")
/* used in the idle loop; sti takes one instruction cycle to complete */
#define safe_halt()		__asm__ __volatile__("sti; hlt": : :"memory")

#define __save_and_cli(x)	do { __save_flags(x); __cli(); } while(0);
#define __save_and_sti(x)	do { __save_flags(x); __sti(); } while(0);
#define local_irq_save(x)	__save_and_cli(x)
#define local_irq_restore(x)	__restore_flags(x)
#define save_flags(x) __save_flags(x)
#define restore_flags(x) __restore_flags(x)

#define prepare_to_switch()	do { } while(0)
#define switch_to(prev,next,last) do {					\
	asm volatile("pushl %%esi\n\t"					\
		     "pushl %%edi\n\t"					\
		     "pushl %%ebp\n\t"					\
		     "movl %%esp,%0\n\t"	/* save ESP */		\
		     "movl %3,%%esp\n\t"	/* restore ESP */	\
		     "movl $1f,%1\n\t"		/* save EIP */		\
		     "pushl %4\n\t"		/* restore EIP */	\
		     "jmp __switch_to\n"				\
		     "1:\t"						\
		     "popl %%ebp\n\t"					\
		     "popl %%edi\n\t"					\
		     "popl %%esi\n\t"					\
		     :"=m" (prev->thread.esp),"=m" (prev->thread.eip),	\
		      "=b" (last)					\
		     :"m" (next->thread.esp),"m" (next->thread.eip),	\
		      "a" (prev), "d" (next));					\
} while (0)

/*
 * Load a segment. Fall back on loading the zero
 * segment if something goes wrong..
 */
#define loadsegment(seg,value)			\
	asm volatile("\n"			\
		"movl %0,%%" #seg "\n"		\
		: :"m" (*(unsigned int *)&(value)))

#endif //CODE_SYSTEM_H
