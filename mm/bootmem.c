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

unsigned long max_low_pfn;
unsigned long min_low_pfn;
unsigned long max_pfn;

/*
 * Called once to set up the allocator itself.
 * 调用一次以设置分配器本身。
 */
static unsigned long __init init_bootmem_core (pg_data_t *pgdat,
	unsigned long mapstart, unsigned long start, unsigned long end)
{
	
}

unsigned long __init init_bootmem (unsigned long start, unsigned long pages)
{
	max_low_pfn = pages;
	min_low_pfn = start;
	return(init_bootmem_core(&contig_page_data, start, 0, pages));
}
