#include <asm/types.h>
#include <asm/stdio.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <asm/pgtable.h>
#include <linux/fs.h>

extern void setup_arch(void);
extern void init_IRQ(void);
extern void time_init(void);
extern void fork_init(unsigned long);
extern void bdev_init(void);

static int init(void * unused);

void start_kernel(void)
{
    unsigned long mempages;
    printk("Hello, XOS.\n");
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
    kmem_cache_sizes_init();
    pgtable_cache_init();

    /*
	 * For architectures that have highmem, num_mappedpages represents
	 * the amount of memory the kernel can use.  For other architectures
	 * it's the same as the total pages.  We need both numbers because
	 * some subsystems need to initialize based on how much memory the
	 * kernel can use.
	 */
	if (num_mappedpages == 0)
		num_mappedpages = num_physpages;

    mempages = num_physpages;

    // fork_init(num_mappedpages);
    // proc_caches_init();
    // vfs_caches_init(num_mappedpages);
    fork_init(mempages);
    proc_caches_init();
    vfs_caches_init(mempages);
	buffer_init(mempages);
    bdev_init();
    inode_init(mempages);

    kernel_thread(init, NULL, CLONE_FS | CLONE_FILES | CLONE_SIGNAL);
    printk("AAAAAAAA.\n");

    // struct page *page = alloc_page(__GFP_HIGH);
    // printk("%s: %d: 0x%p\n", __func__, __LINE__, page);
    // printk("%s: %d: 0x%p\n", __func__, __LINE__, mem_map);
    
    while(1);
}

static int init(void * unused)
{
    printk("init thread start.\n");
    while(1);
}
