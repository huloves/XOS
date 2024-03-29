#include <linux/init.h>
#include <asm/stdio.h>
#include <asm/page.h>
#include <linux/bootmem.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <linux/mmzone.h>
#include <linux/debug.h>
#include <asm/cpufeature.h>
#include <asm/io.h>
#include <asm/dma.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <asm/e820.h>

unsigned long highstart_pfn, highend_pfn;
static unsigned long totalram_pages;
static unsigned long totalhigh_pages;

pgd_t swapper_pg_dir[1024] __attribute__((__aligned__(PAGE_SIZE)));
// int a[1024];
// pgd_t b[1024]__attribute__((__aligned__(PAGE_SIZE)));
// pgd_t swapper_pg_dir_[1024] __attribute__((__aligned__(PAGE_SIZE)));

static void pagetable_init(void)
{
	printk("pagetable_init start.\n");
	// printk("swapper_pg_dir = %x\n", swapper_pg_dir);
	unsigned long vaddr, end;
	pgd_t *pgd, *pgd_base;
	int i, j, k;
	pmd_t *pmd;
	pte_t *pte, *pte_base;

	/*
	 * This can be zero as well - no problem, in that case we exit
	 * the loops anyway due to thse PTRS_PER_* conditions.
	 */
	end = (unsigned long)__va(max_low_pfn*PAGE_SIZE);
	pgd_base = swapper_pg_dir;
	i = __pgd_offset(PAGE_OFFSET);   // PAGE_OFFSET = 0xC0000000，获得地址为 0xC0000000 在 PGD 中的便宜，i=0xc00
	pgd = pgd_base + i;

	// printk("swapper_pg_dir = %x\n", swapper_pg_dir);
	for (; i < PTRS_PER_PGD; pgd++, i++) {
        vaddr = i * PGDIR_SIZE;   // 获得虚拟地址 PDG中 索引为 i 的虚拟地址, vaddr = 0xC0000000
        if (end && (vaddr >= end)) {
			break;
		}
		pmd = (pmd_t *)pgd;   // 没有中间页目录，就把中间页目录直接映射到总目录
		if (pmd != pmd_offset(pgd, 0))
			BUG();
		
        for (j = 0; j < PTRS_PER_PMD; pmd++, j++) {
            vaddr = i*PGDIR_SIZE + j*PMD_SIZE;   // vaddr = 0xC0000000
            if (end && (vaddr >= end))
                break;
            pte_base = pte = (pte_t *) alloc_bootmem_low_pages(PAGE_SIZE);   // 设置中间页目录项，为一个页表（4K）分配空间
			// printk("pte_base = %x\n", pte_base);

            for (k = 0; k < PTRS_PER_PTE; pte++, k++) {
                vaddr = i*PGDIR_SIZE + j*PMD_SIZE + k*PAGE_SIZE;   // vaddr = 0xC0000000
                if (end && (vaddr >= end))
                    break;
                *pte = mk_pte_phys(__pa(vaddr), PAGE_KERNEL);   // 宏mk_pte_phys()创建一个页表项，这个页表项的物理地址为__pa(vaddr)。属性PAGE_KERNEL表示只有在内核态才能访问这一页表项
            }
            set_pmd(pmd, __pmd(_KERNPG_TABLE + __pa(pte_base)));   // 通过调用set_pmd()把该页表追加到中间页目录中。这个过程一直继续，直到把所有的物理内存都映射到从PAGE_OFFSET开始的虚拟地址空间			// printk("%x\n", pte_offset(pmd, 0));
			// printk("pte_offset = %x\n", pte_offset(pmd, 0));
            if (pte_base != pte_offset(pmd, 0)) {
                BUG();
            }
        }
    }
	printk("pagetable_init down.\n");
}

static void zone_sizes_init(void)
{
	printk("zone_sizes_init start.\n");
	unsigned long zones_size[MAX_NR_ZONES] = {0, 0, 0};
	unsigned int max_dma, high, low;

	max_dma = virt_to_phys((char*)MAX_DMA_ADDRESS) >> PAGE_SHIFT;   // 低于 16MB 的内存只能用于 DMA，因此，上面这条语句用于存放16MB的页面, MAX_DMA_ADDRESS = 0xC1000000, max_dma = 0x1000
	low = max_low_pfn;
	
	if(low < max_dma) {
		zones_size[ZONE_DMA] = low;
	} else {
		zones_size[ZONE_DMA] = max_dma;
		zones_size[ZONE_NORMAL] = low - max_dma;
	}
	free_area_init(zones_size);   // 初始化内存管理区并创建内存映射表
	printk("zone_sizes_init down.\n");
}

/*
 * paging_init() sets up the page tables - note that the first 8MB are
 * already mapped by head.S.
 *
 * This routines also unmaps the page at virtual kernel address 0, so
 * that we can trap those pesky NULL-reference errors in the kernel.
 */
void paging_init(void)
{
	printk("paging_init start.\n");
	pagetable_init();
	load_cr3(swapper_pg_dir);
	printk("load_cr3 complete.\n");
	__flush_tlb_all();
	printk("paging_init down.\n");
	zone_sizes_init();
	printk("paging_init down.\n");
}

static void set_max_mapnr_init(void)
{
	max_mapnr = num_mappedpages = num_physpages = max_low_pfn;
}

static int free_pages_init(void)
{
	int reservedpages, pfn;

	/* this will put all low memory onto the freelists */
}

static inline int page_is_ram (unsigned long pagenr)
{
	int i;

	for (i = 0; i < e820.nr_map; i++) {
		unsigned long addr, end;

		if (e820.map[i].type != E820_RAM)	/* not usable memory */
			continue;
		/*
		 *	!!!FIXME!!! Some BIOSen report areas as RAM that
		 *	are not. Notably the 640->1Mb area. We need a sanity
		 *	check here.
		 */
		addr = (e820.map[i].addr+PAGE_SIZE-1) >> PAGE_SHIFT;
		end = (e820.map[i].addr+e820.map[i].size) >> PAGE_SHIFT;
		if  ((pagenr >= addr) && (pagenr < end))
			return 1;
	}
	return 0;
}

extern char _text, _etext, _edata, _end, _start;

void mem_init()
{
	int codesize, reservedpages, datasize, initsize;
	int tmp;

	if(!mem_map) { 
		BUG();
	}

	set_max_mapnr_init();

	high_memory = (void*) __va(max_low_pfn * PAGE_SIZE);

	/* clear the zero-page */
	memset(empty_zero_page, 0, PAGE_SIZE);

	/* this will put all low memory onto the freelists */
	totalram_pages += free_all_bootmem();
	printk("%s: %d: totalram_pages = %d\n", __func__, __LINE__, totalram_pages);
	
	reservedpages = 0;
	for (tmp = 0; tmp < max_low_pfn; tmp++) {
		/*
		 * Only count reserved RAM pages
		 */
		if (page_is_ram(tmp) && PageReserved(mem_map+tmp))
			reservedpages++;
	}

	// printk("Memory: k/%luk available (%dk kernel code, %dk reserved, %dk data, %ldk highmem)\n",
	// 	// (unsigned long) nr_free_pages() << (PAGE_SHIFT-10),
	// 	max_mapnr << (PAGE_SHIFT-10),
	// 	codesize >> 10,
	// 	reservedpages << (PAGE_SHIFT-10),
	// 	datasize >> 10,
	// 	(unsigned long) (totalhigh_pages << (PAGE_SHIFT-10))
	//        );

	printk("kernel in memory start: 0x%08X\n", &_start);
    printk("kernel in memory end:   0x%08X\n", &_end - 0xc0000000);
    printk("kernel in memory used:   %d KB\n\n", (&_end - 0xc0000000 - &_start + 1023) / 1024);
}
