/*
 * include/asm-i386/processor.h
 *
 * Copyright (C) 1994 Linus Torvalds
 */

#ifndef _ASM_I386_PROCESSOR_H
#define _ASM_I386_PROCESSOR_H

#include <asm-i386/cpufeature.h>
#include <asm-i386/types.h>
#include <asm-i386/cache.h>
#include <asm-i386/page.h>
#include <linux/sched.h>
#include <asm-i386/desc.h>

extern unsigned long mmu_cr4_features;

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#define current_text_addr() ({ void *pc; __asm__("movl $1f,%0\n1:":"=g" (pc)); pc; })

/*
 *  CPU type and hardware bug flags. Kept separately for each CPU.
 *  Members of this structure are referenced in head.S, so think twice
 *  before touching them. [mj]
 */

struct cpuinfo_x86 {
	__u8	x86;			/* CPU family */
	__u8	x86_vendor;		/* CPU vendor */
	__u8	x86_model;
	__u8	x86_mask;
	char	wp_works_ok;	/* It doesn't on 386's */
	char	hlt_works_ok;	/* Problems on some 486Dx4's and old 386's */
	char	hard_math;
	char	rfu;
    int		cpuid_level;	/* Maximum supported CPUID level, -1=no CPUID */
	__u32	x86_capability[NCAPINTS];
	char	x86_vendor_id[16];
	char	x86_model_id[64];
	int 	x86_cache_size;  /* in KB - valid for CPUS which support this call  */
	int		fdiv_bug;
	int		f00f_bug;
	int		coma_bug;
	unsigned long loops_per_jiffy;
	unsigned long *pgd_quick;
	unsigned long *pmd_quick;
	unsigned long *pte_quick;
	unsigned long pgtable_cache_sz;
} __attribute__((__aligned__(SMP_CACHE_BYTES)));

#define X86_VENDOR_INTEL 		0
#define X86_VENDOR_CYRIX 		1
#define X86_VENDOR_AMD 			2
#define X86_VENDOR_UMC 			3
#define X86_VENDOR_NEXGEN 		4
#define X86_VENDOR_CENTAUR 		5
#define X86_VENDOR_RISE 		6
#define X86_VENDOR_TRANSMETA 	7
#define X86_VENDOR_NSC 			8
#define X86_VENDOR_SIS 			9
#define X86_VENDOR_UNKNOWN 		0xff

/*
 * capabilities of CPUs
 */
extern struct cpuinfo_x86 boot_cpu_data;

#define cpu_data (&boot_cpu_data)
#define current_cpu_data boot_cpu_data

extern char ignore_irq13;

/*
 * Intel CPU features in CR4
 */
#define X86_CR4_VME			0x0001	/* enable vm86 extensions */
#define X86_CR4_PVI			0x0002	/* virtual interrupts flag enable */
#define X86_CR4_TSD			0x0004	/* disable time stamp at ipl 3 */
#define X86_CR4_DE			0x0008	/* enable debugging extensions */
#define X86_CR4_PSE			0x0010	/* enable page size extensions */
#define X86_CR4_PAE			0x0020	/* enable physical address extensions */
#define X86_CR4_MCE			0x0040	/* Machine check enable */
#define X86_CR4_PGE			0x0080	/* enable global pages */
#define X86_CR4_PCE			0x0100	/* enable performance counters at ipl 3 */
#define X86_CR4_OSFXSR		0x0200	/* enable fast FPU save and restore */
#define X86_CR4_OSXMMEXCPT	0x0400	/* enable unmasked SSE exceptions */

#define load_cr3(pgdir) \
	asm volatile("movl %0,%%cr3": :"r" (__pa(pgdir)));

/*
 * Save the cr4 feature set we're using (ie
 * Pentium 4MB enable and PPro Global page
 * enable), so that any CPU's that boot up
 * after us can get the correct flags.
 */
extern unsigned long mmu_cr4_features;

static inline void set_in_cr4 (unsigned long mask)
{
	mmu_cr4_features |= mask;
	__asm__("movl %%cr4,%%eax\n\t"
		"orl %0,%%eax\n\t"
		"movl %%eax,%%cr4\n"
		: : "irg" (mask)
		:"ax");
}

static inline void clear_in_cr4 (unsigned long mask)
{
	mmu_cr4_features &= ~mask;
	__asm__("movl %%cr4,%%eax\n\t"
		"andl %0,%%eax\n\t"
		"movl %%eax,%%cr4\n"
		: : "irg" (~mask)
		:"ax");
}

/*
 * Size of io_bitmap in longwords: 32 is ports 0-0x3ff.
 */
#define IO_BITMAP_SIZE	32
#define IO_BITMAP_OFFSET offsetof(struct tss_struct,io_bitmap)
#define INVALID_IO_BITMAP_OFFSET 0x8000

struct tss_struct {
	unsigned short	back_link,__blh;
	unsigned long	esp0;
	unsigned short	ss0,__ss0h;
	unsigned long	esp1;
	unsigned short	ss1,__ss1h;
	unsigned long	esp2;
	unsigned short	ss2,__ss2h;
	unsigned long	__cr3;
	unsigned long	eip;
	unsigned long	eflags;
	unsigned long	eax,ecx,edx,ebx;
	unsigned long	esp;
	unsigned long	ebp;
	unsigned long	esi;
	unsigned long	edi;
	unsigned short	es, __esh;
	unsigned short	cs, __csh;
	unsigned short	ss, __ssh;
	unsigned short	ds, __dsh;
	unsigned short	fs, __fsh;
	unsigned short	gs, __gsh;
	unsigned short	ldt, __ldth;
	unsigned short	trace, bitmap;
	unsigned long	io_bitmap[IO_BITMAP_SIZE+1];
	/*
	 * pads the TSS to be cacheline-aligned (size is 0x100)
	 */
	unsigned long __cacheline_filler[5];
};

#define INIT_TSS  {						\
	0,0, /* back_link, __blh */				\
	sizeof(init_stack) + (long) &init_stack, /* esp0 */	\
	__KERNEL_DS, 0, /* ss0 */				\
	0,0,0,0,0,0, /* stack1, stack2 */			\
	0, /* cr3 */						\
	0,0, /* eip,eflags */					\
	0,0,0,0, /* eax,ecx,edx,ebx */				\
	0,0,0,0, /* esp,ebp,esi,edi */				\
	0,0,0,0,0,0, /* es,cs,ss */				\
	0,0,0,0,0,0, /* ds,fs,gs */				\
	__LDT(0),0, /* ldt */					\
	0, INVALID_IO_BITMAP_OFFSET, /* tace, bitmap */		\
	{~0, } /* ioperm */					\
}

#define init_task  (init_task_union.task)
#define init_stack (init_task_union.stack)

/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
static inline void rep_nop(void)
{
	__asm__ __volatile__("rep;nop" ::: "memory");
}

#endif
