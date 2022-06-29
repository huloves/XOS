#ifndef _ASM_SEGMENT_H
#define _ASM_SEGMENT_H

#define	 RPL0  0
#define	 RPL1  1
#define	 RPL2  2
#define	 RPL3  3

#define TI_GDT 0
#define TI_LDT 1
#define SELECTOR_K_CODE	   ((1 << 3) + (TI_GDT << 2) + RPL0)
#define __KERNEL_CS	((1 << 3) + (TI_GDT << 2) + RPL0)
#define __KERNEL_DS	((2 << 3) + (TI_GDT << 2) + RPL0)

#define __USER_CS	0x23
#define __USER_DS	0x2B

#endif