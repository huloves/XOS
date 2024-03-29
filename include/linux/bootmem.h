#ifndef _LINUX_BOOTMEM_H
#define _LINUX_BOOTMEM_H

#include <linux/init.h>
#include <asm/page.h>
#include <linux/cache.h>
#include <asm/dma.h>
#include <linux/mmzone.h>

/**
 * simple boot-time physical memory area allocator.
 * 简单的启动时物理内存区域分配器。
**/

extern unsigned long max_low_pfn;
extern unsigned long min_low_pfn;
extern unsigned long max_pfn;

/*
 * node_bootmem_map is a map pointer - the bits represent all physical
 * memory pages (including holes) on the node.
 * node_bootmem_map 是一个指向位图的指针 - 位图的位代表节点上的所有物理内存页（包括空洞）
 */
typedef struct bootmem_data {
	unsigned long node_boot_start;   // 表示起始物理地址块
	unsigned long node_low_pfn;      // 表示结束物理地址块，即 ZONE_NORMAL 的结束地址块
	void *node_bootmem_map;          // 用于表示节点内存块的位图
	unsigned long last_offset;       // 表示最后一次分配结束时页面内的偏移量，若为0，表示页面已经用完了
	unsigned long last_pos;          // 表示上次分配时使用的页面PFN
} bootmem_data_t;

// extern unsigned long __init init_bootmem (unsigned long addr, unsigned long memend);
unsigned long init_bootmem (unsigned long start, unsigned long pages);
void free_bootmem (unsigned long addr, unsigned long size);
void reserve_bootmem (unsigned long addr, unsigned long size);
void * __alloc_bootmem (unsigned long size, unsigned long align, unsigned long goal);
void * __alloc_bootmem_node (pg_data_t *pgdat, unsigned long size, unsigned long align, unsigned long goal);

#define alloc_bootmem(x) \
	__alloc_bootmem((x), SMP_CACHE_BYTES, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_low(x) \
	__alloc_bootmem((x), SMP_CACHE_BYTES, 0)
#define alloc_bootmem_pages(x) \
	__alloc_bootmem((x), PAGE_SIZE, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_low_pages(x) \
	__alloc_bootmem((x), PAGE_SIZE, 0)

#define alloc_bootmem_node(pgdat, x) \
	__alloc_bootmem_node((pgdat), (x), SMP_CACHE_BYTES, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_pages_node(pgdat, x) \
	__alloc_bootmem_node((pgdat), (x), PAGE_SIZE, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_low_pages_node(pgdat, x) \
	__alloc_bootmem_node((pgdat), (x), PAGE_SIZE, 0)

extern unsigned long free_all_bootmem(void);

#endif /* _LINUX_BOOTMEM_H */
