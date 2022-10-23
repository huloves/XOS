#include <asm-i386/types.h>
#include <asm-i386/stdio.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm-i386/system.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/slab.h>
#include <linux/kernel.h>

extern void setup_arch(void);
extern void init_IRQ(void);
extern void time_init(void);

void start_kernel(void)
{
    unsigned long mempages;
    printk("Hello, OUROS.\n");
    printk("init_all.\n");
    setup_arch();
    trap_init();
    init_IRQ();
    sched_init();
    time_init();
    softirq_init();
    kmem_cache_init();
    sti();   //

    mem_init();
    char name[100];
    char *str = "world";
    snprintf(name, sizeof(name), "hello %d!", 100);
    printk("name = %s\n", name);
    // kmem_cache_sizes_init();

    struct page *page = alloc_page(__GFP_HIGH);
    printk("%s: %d: 0x%p\n", __func__, __LINE__, page);
    printk("%s: %d: 0x%p\n", __func__, __LINE__, mem_map);
    
    while(1);
}
