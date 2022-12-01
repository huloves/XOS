#ifndef __ARCH_DESC_H
#define __ARCH_DESC_H

#include <asm-i386/ldt.h>
#include <linux/sched.h>

struct desc_struct {
	unsigned long a,b;
};

// #define load_TR(n) __asm__ __volatile__("ltr %%ax"::"a" (__TSS(n)<<3))

// #define __load_LDT(n) __asm__ __volatile__("lldt %%ax"::"a" (__LDT(n)<<3))

/*
 * This is the ldt that every process will get unless we need
 * something other than this.
 */
extern struct desc_struct default_ldt[];
extern void set_intr_gate(unsigned int irq, void * addr);
extern void set_ldt_desc(unsigned int n, void *addr, unsigned int size);
extern void set_tss_desc(unsigned int n, void *addr);

/*
 * load one particular LDT into the current CPU
 */
extern void load_LDT (struct mm_struct *mm);

#endif
