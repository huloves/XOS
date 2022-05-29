/*
 *  linux/mm/bootmem.c
 *
 *  Copyright (C) 1999 Ingo Molnar
 *  Discontiguous memory support, Kanoj Sarcar, SGI, Nov 1999
 *
 *  simple boot-time physical memory area allocator and
 *  free memory collector. It's used to deal with reserved
 *  system memory and memory holes as well.
 */

#include <linux/bootmem.h>
#include <linux/init.h>
#include <linux/mmzone.h>
#include <asm-i386/page.h>
#include <linux/string.h>
#include <asm-i386/stdio.h>
#include <asm-i386/io.h>
#include <linux/debug.h>
#include <asm-i386/bitops.h>

unsigned long max_low_pfn;
unsigned long min_low_pfn;
unsigned long max_pfn;

/*
 * init_bootmem_core - Called once to set up the allocator itself.
 * 					   调用一次以设置分配器本身。
 * 返回值：位图大小（以字节为单位）
 */
static unsigned long __init init_bootmem_core (pg_data_t *pgdat,
	unsigned long mapstart, unsigned long start, unsigned long end)   // init_bootmem_core(&contig_page_data, start, 0, pages)
{
	printk("bootmem_data_t init start.\n");
	bootmem_data_t *bdata = pgdat->bdata;   // pg_data_t contig_page_data = { bdata: &contig_bootmem_data };
	unsigned long mapsize = ((end - start)+7)/8;   // 计算内存页位图的大小（以字节为单位），end = max_low_pfn，start = 0
	printk("old mapsize = %d\n", mapsize);

	pgdat->node_next = pgdat_list;   // 把 pgdat_data_t 插入到 pgdat_list 链表中
	pgdat_list = pgdat;

	mapsize = (mapsize + (sizeof(long) - 1UL)) & ~(sizeof(long) - 1UL);   // 保证是 4 的倍数
	printk("now mapsize = %d\n", mapsize);
	bdata->node_bootmem_map = phys_to_virt(mapstart << PAGE_SHIFT);   // 初始化位图的起始地址，该位图用于表示页面的使用情况
	bdata->node_boot_start = (start << PAGE_SHIFT);   // 记录该节点的起始地址
	bdata->node_low_pfn = end;   // 记录该节点的结束地址

	/*
	 * Initially all pages are reserved - setup_arch() has to
	 * register free RAM areas explicitly.
	 */
	memset(bdata->node_bootmem_map, 0xff, mapsize);   // 初始化位图

	printk("bootmem_data_t init down.\n");
	return mapsize;
}

static void __init free_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
	unsigned long i;
	unsigned long start;
	/*
	 * round down end of usable mem, partially free pages are
	 * considered reserved.
	 * 向下取整可用内存的末端，部分空闲页保留
	 */
	unsigned long sidx;
	unsigned long eidx = (addr + size - bdata->node_boot_start) / PAGE_SIZE;
	unsigned long end = (addr + size) / PAGE_SIZE;

	if (!size) BUG();
	if (end > bdata->node_low_pfn) {
		BUG();
	}

	/*
	 * Round up the beginning of the address.
	 */
	start = (addr + PAGE_SIZE - 1) / PAGE_SIZE;
	sidx = start - (bdata->node_boot_start / PAGE_SIZE);

	for (i = sidx; i < eidx; i++) {
		if (!test_and_clear_bit(i, bdata->node_bootmem_map))
			BUG();
	}
}

static void __init reserve_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
	unsigned long i;
	/*
	 * round up, partially reserved pages are considered
	 * fully reserved.
	 */
	unsigned long sidx = (addr - bdata->node_boot_start)/PAGE_SIZE;
	unsigned long eidx = (addr + size - bdata->node_boot_start + 
							PAGE_SIZE-1)/PAGE_SIZE;
	unsigned long end = (addr + size + PAGE_SIZE-1)/PAGE_SIZE;

	if (!size) BUG();

	if (sidx < 0) { BUG(); }
	if (eidx < 0) { BUG(); }
	if (sidx >= eidx) { BUG(); }
	if ((addr >> PAGE_SHIFT) >= bdata->node_low_pfn) { BUG(); }
	if (end > bdata->node_low_pfn) { BUG(); }
	for (i = sidx; i < eidx; i++) {
		if (test_and_set_bit(i, bdata->node_bootmem_map)) {
			printk("hm, page %08lx reserved twice.\n", i*PAGE_SIZE);
		}
	}
}

unsigned long __init init_bootmem (unsigned long start, unsigned long pages)
{
	max_low_pfn = pages;
	min_low_pfn = start;
	return(init_bootmem_core(&contig_page_data, start, 0, pages));
}

void __init free_bootmem(unsigned long addr, unsigned long size)
{
	return(free_bootmem_core(contig_page_data.bdata, addr, size));
}

void __init reserve_bootmem (unsigned long addr, unsigned long size)
{
	return(reserve_bootmem_core(contig_page_data.bdata, addr, size));
}
