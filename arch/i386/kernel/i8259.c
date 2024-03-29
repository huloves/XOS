#include <linux/spinlock.h>
#include <asm/io.h>
#include <linux/irq.h>
#include <asm/hw_irq.h>
#include <asm/atomic.h>
#include <asm/delay.h>
#include <asm/segment.h>
#include <linux/timex.h>
#include <asm/processor.h>
#include <asm/stdio.h>
#include <asm/desc.h>

BUILD_COMMON_IRQ()

#define BI(x,y) \
	BUILD_IRQ(x##y)

#define BUILD_16_IRQS(x) \
	BI(x,0) BI(x,1) BI(x,2) BI(x,3) \
	BI(x,4) BI(x,5) BI(x,6) BI(x,7) \
	BI(x,8) BI(x,9) BI(x,a) BI(x,b) \
	BI(x,c) BI(x,d) BI(x,e) BI(x,f)

/*
 * ISA PIC or low IO-APIC triggered (INTA-cycle or APIC) interrupts:
 * (these are usually mapped to vectors 0x20-0x2f)
 */
BUILD_16_IRQS(0x0)

#ifdef CONFIG_X86_IO_APIC
/*
 * The IO-APIC gives us many more interrupt sources. Most of these 
 * are unused but an SMP system is supposed to have enough memory ...
 * sometimes (mostly wrt. hw bugs) we get corrupted vectors all
 * across the spectrum, so we really want to be prepared to get all
 * of these. Plus, more powerful systems might have more than 64
 * IO-APIC registers.
 *
 * (these are usually mapped into the 0x30-0xff vector range)
 */
		   BUILD_16_IRQS(0x1) BUILD_16_IRQS(0x2) BUILD_16_IRQS(0x3)
BUILD_16_IRQS(0x4) BUILD_16_IRQS(0x5) BUILD_16_IRQS(0x6) BUILD_16_IRQS(0x7)
BUILD_16_IRQS(0x8) BUILD_16_IRQS(0x9) BUILD_16_IRQS(0xa) BUILD_16_IRQS(0xb)
BUILD_16_IRQS(0xc) BUILD_16_IRQS(0xd)
#endif

// #undef BUILD_16_IRQS
// #undef BI

#define IRQ(x,y) \
	IRQ##x##y##_interrupt

#define IRQLIST_16(x) \
	IRQ(x,0), IRQ(x,1), IRQ(x,2), IRQ(x,3), \
	IRQ(x,4), IRQ(x,5), IRQ(x,6), IRQ(x,7), \
	IRQ(x,8), IRQ(x,9), IRQ(x,a), IRQ(x,b), \
	IRQ(x,c), IRQ(x,d), IRQ(x,e), IRQ(x,f)

void (*interrupt[NR_IRQS])(void) = {
	IRQLIST_16(0x0),   // 将会被替换为 IRQ0x00_interrupt, IRQ0x01_interrupt ... IRQ0x0f_interrupt
};

extern struct desc_struct idt_table[256];

/*
 * This is the 'legacy' 8259A Programmable Interrupt Controller,
 * present in the majority of PC/AT boxes.
 * plus some generic x86 specific things if generic specifics makes
 * any sense at all.
 * this file should become arch/i386/kernel/irq.c when the old irq.c
 * moves to arch independent land
 */

spinlock_t i8259A_lock = SPIN_LOCK_UNLOCKED;

static void end_8259A_irq (unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED|IRQ_INPROGRESS)))
		enable_8259A_irq(irq);
}

#define shutdown_8259A_irq	disable_8259A_irq

void mask_and_ack_8259A(unsigned int);

static unsigned int startup_8259A_irq(unsigned int irq)
{ 
	enable_8259A_irq(irq);
	return 0; /* never anything pending */
}

static struct hw_interrupt_type i8259A_irq_type = {
	"XT-PIC",
	startup_8259A_irq,
	shutdown_8259A_irq,
	enable_8259A_irq,
	disable_8259A_irq,
	mask_and_ack_8259A,
	end_8259A_irq,
	NULL
};

/*
 * 8259A PIC functions to handle ISA devices:
 */

/*
 * This contains the irq mask for both 8259A irq controllers,
 */
static unsigned int cached_irq_mask = 0xffff;

#define __byte(x,y) 	(((unsigned char *)&(y))[x])
#define cached_21		(__byte(0,cached_irq_mask))
#define cached_A1		(__byte(1,cached_irq_mask))

/*
 * Not all IRQs can be routed through the IO-APIC, eg. on certain (older)
 * boards the timer interrupt is not really connected to any IO-APIC pin,
 * it's fed to the master 8259A's IR0 line only.
 *
 * Any '1' bit in this mask means the IRQ is routed through the IO-APIC.
 * this 'mixed mode' IRQ handling costs nothing because it's only used
 * at IRQ setup time.
 */
unsigned long io_apic_irqs;

void disable_8259A_irq(unsigned int irq)
{
	unsigned int mask = 1 << irq;
	unsigned long flags;

	spin_lock_irqsave(&i8259A_lock, flags);
	cached_irq_mask |= mask;
	if (irq & 8)
		outb(cached_A1,0xA1);
	else
		outb(cached_21,0x21);
	spin_unlock_irqrestore(&i8259A_lock, flags);
}

void enable_8259A_irq(unsigned int irq)
{
	unsigned int mask = ~(1 << irq);
	unsigned long flags;

	spin_lock_irqsave(&i8259A_lock, flags);
	cached_irq_mask &= mask;
	if (irq & 8) {
		outb(cached_A1, 0xA1);
	} else {
		outb(cached_21, 0x21);
	}
	spin_unlock_irqrestore(&i8259A_lock, flags);
}

int i8259A_irq_pending(unsigned int irq)
{
	unsigned int mask = 1<<irq;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&i8259A_lock, flags);
	if (irq < 8)
		ret = inb(0x20) & mask;
	else
		ret = inb(0xA0) & (mask >> 8);
	spin_unlock_irqrestore(&i8259A_lock, flags);

	return ret;
}

// void make_8259A_irq(unsigned int irq)
// {
// 	disable_irq_nosync(irq);
// 	io_apic_irqs &= ~(1<<irq);
// 	irq_desc[irq].handler = &i8259A_irq_type;
// 	enable_irq(irq);
// }

/*
 * This function assumes to be called rarely. Switching between
 * 8259A registers is slow.
 * This has to be protected by the irq controller spinlock
 * before being called.
 */
static inline int i8259A_irq_real(unsigned int irq)
{
	int value;
	int irqmask = 1<<irq;

	if (irq < 8) {
		outb(0x0B,0x20);		/* ISR register */
		value = inb(0x20) & irqmask;
		outb(0x0A,0x20);		/* back to the IRR register */
		return value;
	}
	outb(0x0B,0xA0);		/* ISR register */
	value = inb(0xA0) & (irqmask >> 8);
	outb(0x0A,0xA0);		/* back to the IRR register */
	return value;
}

/*
 * Careful! The 8259A is a fragile beast, it pretty
 * much _has_ to be done exactly like this (mask it
 * first, _then_ send the EOI, and the order of EOI
 * to the two 8259s is important!
 */
void mask_and_ack_8259A(unsigned int irq)
{
	unsigned int irqmask = 1 << irq;
	unsigned long flags;

	spin_lock_irqsave(&i8259A_lock, flags);
	/*
	 * Lightweight spurious IRQ detection. We do not want
	 * to overdo spurious IRQ handling - it's usually a sign
	 * of hardware problems, so we only do the checks we can
	 * do without slowing down good hardware unnecesserily.
	 *
	 * Note that IRQ7 and IRQ15 (the two spurious IRQs
	 * usually resulting from the 8259A-1|2 PICs) occur
	 * even if the IRQ is masked in the 8259A. Thus we
	 * can check spurious 8259A IRQs without doing the
	 * quite slow i8259A_irq_real() call for every IRQ.
	 * This does not cover 100% of spurious interrupts,
	 * but should be enough to warn the user that there
	 * is something bad going on ...
	 */
	if (cached_irq_mask & irqmask)
		goto spurious_8259A_irq;
	cached_irq_mask |= irqmask;

handle_real_irq:
	if (irq & 8) {
		inb(0xA1);		/* DUMMY - (do we need this?) */
		outb(cached_A1,0xA1);
		outb(0x60+(irq&7),0xA0);/* 'Specific EOI' to slave */
		outb(0x62,0x20);	/* 'Specific EOI' to master-IRQ2 */
	} else {
		inb(0x21);		/* DUMMY - (do we need this?) */
		outb(cached_21,0x21);
		outb(0x60+irq,0x20);	/* 'Specific EOI' to master */
	}
	spin_unlock_irqrestore(&i8259A_lock, flags);
	return;

spurious_8259A_irq:
	/*
	 * this is the slow path - should happen rarely.
	 */
	if (i8259A_irq_real(irq))
		/*
		 * oops, the IRQ _is_ in service according to the
		 * 8259A - not spurious, go handle it.
		 */
		goto handle_real_irq;

	{
		static int spurious_irq_mask;
		/*
		 * At this point we can be sure the IRQ is spurious,
		 * lets ACK and report it. [once per IRQ]
		 */
		if (!(spurious_irq_mask & irqmask)) {
			printk("spurious 8259A interrupt: IRQ%d.\n", irq);
			spurious_irq_mask |= irqmask;
		}
		atomic_inc(&irq_err_count);
		/*
		 * Theoretically we do not have to handle this IRQ,
		 * but in Linux this does not cause problems and is
		 * simpler for us.
		 */
		goto handle_real_irq;
	}
}

void init_8259A(int auto_eoi)
{
	unsigned long flags;

	spin_lock_irqsave(&i8259A_lock, flags);

	outb(0xff, 0x21);	/* mask all of 8259A-1 ，送数据到 OCW1，屏蔽所有外部中断*/
	outb(0xff, 0xA1);	/* mask all of 8259A-2 ，送数据到 OCW1，屏蔽所有外部中断*/

	/*
	 * outb_p - this has to work on a wide range of PC hardware.
	 */
	outb(0x11, 0x20);	/* ICW1: select 8259A-1 init ，0x11 = 0001_0001，外部中断请求信号为上升沿有效，系统中有多片 8259A 级联，还表示要向 ICW4 送数据 */
	outb(0x20 + 0, 0x21);	/* ICW2: 8259A-1 IR0-7 mapped to 0x20-0x27 ，0x20 = 0010_0000，设置中断向量 */
	outb(0x04, 0x21);	/* 8259A-1 (the master) has a slave on IR2，0000_0100，主片使用 IRQ2 连接从片 */
	if (auto_eoi)
		outb(0x03, 0x21);	/* master does Auto EOI */
	else
		outb(0x01, 0x21);	/* master expects normal EOI */

	outb(0x11, 0xA0);	/* ICW1: select 8259A-2 init，初始化从片 */
	outb(0x20 + 8, 0xA1);	/* ICW2: 8259A-2 IR0-7 mapped to 0x28-0x2f */
	outb(0x02, 0xA1);	/* 8259A-2 is a slave on master's IR2，0x20 = 0000_0010，指定主片的哪个接口连接从片 */
	outb(0x01, 0xA1);	/* (slave's support for AEOI in flat mode
				    is to be investigated)，表示 x86 处理器 */

	if (auto_eoi)
		/*
		 * in AEOI mode we just have to mask the interrupt
		 * when acking.
		 */
		i8259A_irq_type.ack = disable_8259A_irq;
	else
		i8259A_irq_type.ack = mask_and_ack_8259A;

	udelay(100);		/* wait for 8259A to initialize */

	outb(cached_21, 0x21);	/* restore master IRQ mask */
	outb(cached_A1, 0xA1);	/* restore slave IRQ mask */
	// outb(0xf8, 0x21);	/* restore master IRQ mask */
    // outb(0xbf, 0xA1);	/* restore slave IRQ mask */

	spin_unlock_irqrestore(&i8259A_lock, flags);
}

void init_ISA_irqs(void)
{
	int i;

	init_8259A(0);   // 参数 0：不开启自动发送 EOI

	for(i = 0; i < NR_IRQS; i++) {
		irq_desc[i].status = IRQ_DISABLED;
		irq_desc[i].action = 0;
		irq_desc[i].depth = 1;

		if(i < 16) {
			irq_desc[i].handler = &i8259A_irq_type;
		} else {
			irq_desc[i].handler = &no_irq_type;
		}
	}
}

void init_IRQ(void)
{
	printk("init_IRQ start.\n");
	int i;

	init_ISA_irqs();

	/*
	 * Cover the whole vector space, no vector can escape
	 * us. (some of these will be overridden and become
	 * 'special' SMP interrupts)
	 */
	for (i = 0; i < NR_IRQS; i++) {
		int vector = FIRST_EXTERNAL_VECTOR + i;
		if (vector != SYSCALL_VECTOR)   // 跳过用于系统调用的向量 0x80
			set_intr_gate(vector, interrupt[i]);
	}

	/* 加载idt */
    uint64_t idt_operand = ((sizeof(idt_table) - 1) | ((uint64_t)(uint32_t)idt_table << 16));
    asm volatile("lidt %0" : : "m" (idt_operand));

	/*
	 * Set the clock to HZ Hz, we already have a valid
	 * vector now:
	 */
	outb_p(0x34,0x43);				/* binary, mode 2, LSB/MSB, ch 0 ，写计数器0的控制字，工作方式2 */
	outb_p(LATCH & 0xff , 0x40);	/* LSB，写计数初值 LSB，计数初值低位字节 */
	outb(LATCH >> 8 , 0x40);		/* MSB，写计数初值 MSB 计数初值高位字节 */

	printk("init_IRQ down.\n");
}
