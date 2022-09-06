#ifndef __ARCH_DESC_H
#define __ARCH_DESC_H

struct desc_struct {
	unsigned long a,b;
};

extern void set_intr_gate(unsigned int irq, void * addr);
extern void set_ldt_desc(unsigned int n, void *addr, unsigned int size);
extern void set_tss_desc(unsigned int n, void *addr);

#endif
