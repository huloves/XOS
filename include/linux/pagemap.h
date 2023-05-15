#ifndef _LINUX_PAGEMAP_H
#define _LINUX_PAGEMAP_H

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <asm/page.h>

/*
 * The page cache can done in larger chunks than
 * one page, because it allows for more efficient
 * throughput (it can then be mapped into user
 * space in smaller chunks for same flexibility).
 *
 * Or rather, it _will_ be done in larger chunks.
 */
#define PAGE_CACHE_SHIFT	PAGE_SHIFT
#define PAGE_CACHE_SIZE		PAGE_SIZE
#define PAGE_CACHE_MASK		PAGE_MASK
#define PAGE_CACHE_ALIGN(addr)	(((addr)+PAGE_CACHE_SIZE-1)&PAGE_CACHE_MASK)

#define page_cache_get(x)	get_page(x)
#define page_cache_alloc()	alloc_pages(GFP_HIGHUSER, 0)
#define page_cache_free(x)	__free_page(x)
#define page_cache_release(x)	__free_page(x)

/*
 * From a kernel address, get the "struct page *"
 */
#define page_cache_entry(x)	virt_to_page(x)

// extern unsigned int page_hash_bits;
#define PAGE_HASH_BITS (page_hash_bits)
#define PAGE_HASH_SIZE (1 << PAGE_HASH_BITS)

#endif /* _LINUX_PAGEMAP_H */
