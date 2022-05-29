#ifndef _LINUX_BOOTMEM_H
#define _LINUX_BOOTMEM_H

#include <linux/init.h>

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
 * node_bootmem_map 是一个指向映射的指针 - 映射的位代表节点上的所有物理内存页（包括空洞）
 */
typedef struct bootmem_data {
	unsigned long node_boot_start;
	unsigned long node_low_pfn;
	void *node_bootmem_map;
	unsigned long last_offset;
	unsigned long last_pos;
} bootmem_data_t;

// extern unsigned long __init init_bootmem (unsigned long addr, unsigned long memend);
unsigned long __init init_bootmem (unsigned long start, unsigned long pages);
void __init free_bootmem (unsigned long addr, unsigned long size);

#endif /* _LINUX_BOOTMEM_H */
