#ifndef _ASM_HW_IRQ_H
#define _ASM_HW_IRQ_H

#include <asm-i386/atomic.h>
#include <linux/linkage.h>

/*
 * IDT vectors usable for external interrupt sources start
 * at 0x20:
 */
#define FIRST_EXTERNAL_VECTOR	0x20

#define SYSCALL_VECTOR			0x80

/*
 * Various low-level irq details needed by irq.c, process.c,
 * time.c, io_apic.c and smp.c
 *
 * Interrupt entry/exit code at both C and assembly level
 */
extern void disable_8259A_irq(unsigned int irq);
extern void enable_8259A_irq(unsigned int irq);

extern atomic_t irq_err_count;

#define IRQ_NAME2(nr) nr##_interrupt(void)
#define IRQ_NAME(nr) IRQ_NAME2(IRQ##nr)
#define __STR(x) #x
#define STR(x) __STR(x)

#define SAVE_ALL \
	"cld\n\t" \
	"pushl %es\n\t" \
	"pushl %ds\n\t" \
	"pushl %eax\n\t" \
	"pushl %ebp\n\t" \
	"pushl %edi\n\t" \
	"pushl %esi\n\t" \
	"pushl %edx\n\t" \
	"pushl %ecx\n\t" \
	"pushl %ebx\n\t" \
	"movl $" STR(__KERNEL_DS) ",%edx\n\t" \
	"movl %edx,%ds\n\t" \
	"movl %edx,%es\n\t"

// #define BUILD_COMMON_IRQ() \
// asmlinkage void call_do_IRQ(void); \
// __asm__( \
// 	"\n" __ALIGN_STR"\n" \
// 	"common_interrupt:\n\t" \
// 	SAVE_ALL \
// 	SYMBOL_NAME_STR(call_do_IRQ)":\n\t" \
// 	"call " SYMBOL_NAME_STR(do_IRQ) "\n\t" \
// 	"jmp ret_from_intr\n");

#define BUILD_COMMON_IRQ() \
asmlinkage void call_do_IRQ(void); \
__asm__( \
	"\n" __ALIGN_STR"\n" \
	"common_interrupt:\n\t" \
	SAVE_ALL \
	"pushl $ret_from_intr\n\t" \
	SYMBOL_NAME_STR(call_do_IRQ)":\n\t" \
	"jmp "SYMBOL_NAME_STR(do_IRQ));

/* 
 * subtle. orig_eax is used by the signal code to distinct between
 * system calls and interrupted 'random user-space'. Thus we have
 * to put a negative value into orig_eax here. (the problem is that
 * both system calls and IRQs want to have small integer numbers in
 * orig_eax, and the syscall code has won the optimization conflict ;)
 *
 * Subtle as a pigs ear.  VY
 */

#define BUILD_IRQ(nr) \
void IRQ_NAME(nr); \
__asm__( \
"\n"__ALIGN_STR"\n" \
SYMBOL_NAME_STR(IRQ) #nr "_interrupt:\n\t" \
	"pushl $"#nr"-256\n\t" \
	"jmp common_interrupt");

#define GET_CURRENT \
	"movl %esp, %ebx\n\t" \
	"andl %-8192, %ebx\n\t"

#endif
