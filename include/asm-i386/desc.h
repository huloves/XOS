#ifndef __ARCH_DESC_H
#define __ARCH_DESC_H

#define __FIRST_TSS_ENTRY 12
#define __FIRST_LDT_ENTRY (__FIRST_TSS_ENTRY+1)

#define __TSS(n) (((n)<<2) + __FIRST_TSS_ENTRY)
#define __LDT(n) (((n)<<2) + __FIRST_LDT_ENTRY)


struct desc_struct {
	unsigned long a,b;
};

extern void set_intr_gate(unsigned int irq, void * addr);
extern void set_ldt_desc(unsigned int n, void *addr, unsigned int size);
extern void set_tss_desc(unsigned int n, void *addr);

#endif
