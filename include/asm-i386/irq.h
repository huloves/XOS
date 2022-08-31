#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

/*
 * 16 8259A IRQ's, 208 potential APIC interrupt sources.
 * Right now the APIC is mostly only used for SMP.
 * 256 vectors is an architectural limit. (we can have
 * more than 256 devices theoretically, but they will
 * have to use shared interrupts)
 * Since vectors 0x00-0x1f are used/reserved for the CPU,
 * the usable vector space is 0x20-0xff (224 vectors)
 */
#define NR_IRQS 224

extern void disable_irq_nosync(unsigned int);

#endif /* _ASM_IRQ_H */
