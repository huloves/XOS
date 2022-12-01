#ifndef __i386_UACCESS_H
#define __i386_UACCESS_H
// #include <asm-i386/processor.h>

#define MAKE_MM_SEG(s) { seg: s }

#define KERNEL_DS	MAKE_MM_SEG(0xFFFFFFFF)

#endif
