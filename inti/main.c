#include <asm-i386/types.h>
#include <asm-i386/stdio.h>

void start_kernel(void)
{
    printk("Hello, OUROS.\n");
    printk("printk complete.\n");
}
