#ifndef _I386_PGTABLE_H
#define _I386_PGTABLE_H

#include <asm-i386/page.h>

#define pages_to_mb(x) ((x) >> (20-PAGE_SHIFT))

extern pgd_t swapper_pg_dir[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".data.init")));
extern pmd_t pmd0[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".data.init")));   // 0 - 4M
extern pmd_t pmd1[1024] __attribute__((__aligned__(PAGE_SIZE))) __attribute__ ((__section__ (".data.init")));   // 4 - 8M

#endif /* _I386_PGTABLE_H */
