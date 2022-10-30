#ifndef _LINUX_FS_H
#define _LINUX_FS_H

#include <linux/kdev_t.h>
#include <asm-i386/atomic.h>
#include <linux/wait.h>

/*
 * Try to keep the most commonly used fields in single cache lines (16
 * bytes) to improve performance.  This ordering should be
 * particularly beneficial on 32-bit processors.
 * 
 * We use the first 16 bytes for the data which is used in searches
 * over the block hash lists (ie. getblk() and friends).
 * 
 * The second 16 bytes we use for lru buffer scans, as used by
 * sync_buffers() and refill_freelist().  -- sct
 */
struct buffer_head {
	/* First cache line: */
	struct buffer_head *b_next;	/* Hash queue list */
	unsigned long b_blocknr;	/* block number */
	unsigned short b_size;		/* block size */
	unsigned short b_list;		/* List that this buffer appears */
	kdev_t b_dev;				/* device (B_FREE = free) */

	atomic_t b_count;			/* users using this block */
	kdev_t b_rdev;				/* Real device */
	unsigned long b_state;		/* buffer state bitmap (see above) */
	unsigned long b_flushtime;	/* Time when (dirty) buffer should be written */

	struct buffer_head *b_next_free;/* lru/free list linkage */
	struct buffer_head *b_prev_free;/* doubly linked list of buffers */
	struct buffer_head *b_this_page;/* circular list of buffers in one page */
	struct buffer_head *b_reqnext;	/* request queue */

	struct buffer_head **b_pprev;	/* doubly linked list of hash-queue */
	char * b_data;					/* pointer to data block */
	struct page *b_page;			/* the page this bh is mapped to */
	void (*b_end_io)(struct buffer_head *bh, int uptodate); /* I/O completion */
 	void *b_private;				/* reserved for b_end_io */

	unsigned long b_rsector;		/* Real buffer location on disk */
	wait_queue_head_t b_wait;

	struct list_head     b_inode_buffers;	/* doubly linked list of inode dirty buffers */
};

/* fs/dcache.c */
extern void vfs_caches_init(unsigned long);

#endif /* _LINUX_FS_H */
