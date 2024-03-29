#ifndef __irq_h
#define __irq_h

#include <linux/spinlock.h>
#include <linux/cache.h>
#include <linux/interrupt.h>
#include <asm/irq.h>

/*
 * Please do not include this file in generic code.  There is currently
 * no requirement for any architecture to implement anything held
 * within this file.
 *
 * Thanks. --rmk
 */

/*
 * IRQ line status.
 */
#define IRQ_INPROGRESS	1	/* IRQ handler active - do not enter! */
#define IRQ_DISABLED	2	/* IRQ disabled - do not enter! */
#define IRQ_PENDING		4	/* IRQ pending - replay on enable */
#define IRQ_REPLAY		8	/* IRQ has been replayed but not acked yet */
#define IRQ_AUTODETECT	16	/* IRQ is being autodetected */
#define IRQ_WAITING		32	/* IRQ not yet seen - for autodetection */
#define IRQ_LEVEL		64	/* IRQ level triggered */
#define IRQ_MASKED		128	/* IRQ masked - shouldn't be seen again */
#define IRQ_PER_CPU		256	/* IRQ is per CPU */

/*
 * Interrupt controller descriptor. This is all we need
 * to describe about the low-level hardware. 
 */
struct hw_interrupt_type {
	const char * typename;
	unsigned int (*startup)(unsigned int irq);
	void (*shutdown)(unsigned int irq);
	void (*enable)(unsigned int irq);
	void (*disable)(unsigned int irq);
	void (*ack)(unsigned int irq);
	void (*end)(unsigned int irq);
	void (*set_affinity)(unsigned int irq, unsigned long mask);
};

typedef struct hw_interrupt_type  hw_irq_controller;

/*
 * This is the "IRQ descriptor", which contains various information
 * about the irq, including what kind of hardware handling it has,
 * whether it is disabled etc etc.
 *
 * Pad this out to 32 bytes for cache and indexing reasons.
 */
typedef struct {
	unsigned int status;		/* IRQ status */
	hw_irq_controller *handler;
	struct irqaction *action;	/* IRQ action list */
	unsigned int depth;		/* nested irq disables */
	spinlock_t lock;
} ____cacheline_aligned irq_desc_t;

extern irq_desc_t irq_desc [NR_IRQS];

extern hw_irq_controller no_irq_type;  /* needed in every arch ? */

int setup_irq(unsigned int irq, struct irqaction *new);

#endif /* _irq_h */
