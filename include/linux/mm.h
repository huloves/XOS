#ifndef _LINUX_MM_H
#define _LINUX_MM_H

#include <linux/list.h>
#include <asm-i386/atomic.h>

typedef struct page {
    struct list_head list;			/* ->mapping has some page lists. 指向链表中的下一页*/
    // struct address_space *mapping;	/* The inode (or ...) we belong to. 用来指定我们正在映射的索引节点（inode）*/
    unsigned long index;			/* Our offset within mapping. 在映射表中的偏移*/
    struct page *next_hash;			/* Next page sharing our hash bucket in
					   					the pagecache hash table. 指向页高速缓存哈希表中下一个共享的页*/
    atomic_t count;					/* Usage count, see below. 引用这个页的个数*/
    unsigned long flags;			/* atomic flags, some possibly
					   					updated asynchronously 页面各种不同的属性*/
    struct list_head lru;			/* Pageout list, eg. active_list;
					   					protected by pagemap_lru_lock !! 用在active_list中*/
    struct page **pprev_hash;		/* Complement to *next_hash. 与next_hash相对应*/
    // struct buffer_head * buffers;	/* Buffer maps us to a disk block. 把缓冲区映射到一个磁盘块*/

} mem_map_t;

#endif
