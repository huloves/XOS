#include <asm-i386/types.h>
#include <asm-i386/stdio.h>
#include <linux/init.h>
#include <linux/sched.h>

extern void setup_arch(void);

void start_kernel(void)
{
    unsigned long mempages;
    printk("Hello, OUROS.\n");
    printk("init_all.\n");
    setup_arch();
    trap_init();
    while(1);
}
