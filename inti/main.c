#include <asm-i386/types.h>
#include <asm-i386/stdio.h>

extern void setup_arch(void);

void start_kernel(void)
{
    unsigned long mempages;
    printk("Hello, OUROS.\n");
    printk("init_all.\n");
    setup_arch();
    while(1);
}
