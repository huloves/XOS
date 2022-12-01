#ifndef __I386_MMU_CONTEXT_H
#define __I386_MMU_CONTEXT_H

#include <asm-i386/bitops.h>
#include <asm-i386/desc.h>

#ifdef CONFIG_SMP

static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk, unsigned cpu)
{
	if(cpu_tlbstate[cpu].state == TLBSTATE_OK)
		cpu_tlbstate[cpu].state = TLBSTATE_LAZY;	
}
#else
static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk, unsigned cpu)
{
}
#endif

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next, struct task_struct *tsk, unsigned cpu)
{
	if (prev != next) {
		/* stop flush ipis for the previous mm */
		clear_bit(cpu, &prev->cpu_vm_mask);
		/*
		 * Re-load LDT if necessary
		 */
		if (prev->context.segments != next->context.segments)
			load_LDT(next);
		set_bit(cpu, &next->cpu_vm_mask);
		/* Re-load page tables */
		asm volatile("movl %0,%%cr3": :"r" (__pa(next->pgd)));
	}
}

#endif
