#include <linux/mm.h>
#include <linux/mmzone.h>
#include <asm-i386/page.h>
#include <asm-i386/stdio.h>
#include <linux/debug.h>
#include <linux/bootmem.h>
#include <linux/list.h>
#include <linux/string.h>
#include <asm-i386/bitops.h>

pg_data_t *pgdat_list;

/*
 *
 * The zone_table array is used to look up the address of the
 * struct zone corresponding to a given zone number (ZONE_DMA,
 * ZONE_NORMAL, or ZONE_HIGHMEM).
 */
zone_t *zone_table[MAX_NR_ZONES*MAX_NR_NODES];

static char *zone_names[MAX_NR_ZONES] = { "DMA", "Normal", "HighMem" };
static int zone_balance_ratio[MAX_NR_ZONES] = { 128, 128, 128, };
static int zone_balance_min[MAX_NR_ZONES] = { 20 , 20, 20, };
static int zone_balance_max[MAX_NR_ZONES]  = { 255 , 255, 255, };

/*
 * Temporary debugging check.
 */
#define BAD_RANGE(zone, page)										\
(																	\
	(((page) - mem_map) >= ((zone)->zone_start_mapnr+(zone)->size))	\
	|| (((page) - mem_map) < (zone)->zone_start_mapnr)				\
	|| ((zone) != page_zone(page))									\
)

#define MARK_USED(index, order, area) \
	__change_bit((index) >> (1+(order)), (area)->map)

static inline struct page * expand (zone_t *zone, struct page *page,
                                    unsigned long index, int low, int high, free_area_t * area)
{
    unsigned long size = 1 << high;

    while (high > low) {
        if (BAD_RANGE(zone,page))
            BUG();
        area--;
        high--;
        size >>= 1;
        list_add(&(page)->list, &(area)->free_list);
        MARK_USED(index, high, area);
        index += size;
        page += size;
    }
    if (BAD_RANGE(zone,page))
        BUG();
    return page;
}

static struct page * rmqueue(zone_t *zone, unsigned int order)
{
    free_area_t * area = zone->free_area + order;
    unsigned int curr_order = order;
    struct list_head *head, *curr;
    unsigned long flags;
    struct page *page;

    spin_lock_irqsave(&zone->lock, flags);
    do {
        head = &area->free_list;
        curr = head->next;

        if (curr != head) {
            unsigned int index;

            page = list_entry(curr, struct page, list);
            if (BAD_RANGE(zone,page))
                BUG();
            list_del(curr);
            index = page - zone->zone_mem_map;
            if (curr_order != MAX_ORDER-1)
                MARK_USED(index, curr_order, area);
            zone->free_pages -= 1UL << order;

            page = expand(zone, page, index, order, curr_order, area);
            spin_unlock_irqrestore(&zone->lock, flags);

            set_page_count(page, 1);
            if (BAD_RANGE(zone,page))
                BUG();
            if (PageLRU(page))
            BUG();
            if (PageActive(page))
                BUG();
            return page;
        }
        curr_order++;
        area++;
    } while (curr_order < MAX_ORDER);
    spin_unlock_irqrestore(&zone->lock, flags);

    return NULL;
}

/* This is the 'heart' of the zoned buddy allocator: */
struct page * __alloc_pages(unsigned int gfp_mask, unsigned int order, zonelist_t *zonelist)
{
	unsigned long min;
	zone_t **zone, * classzone;
	struct page * page;
	int freed;

	zone = zonelist->zones;
	classzone = *zone;
	if (classzone == NULL)
		return NULL;
	min = 1UL << order;
	for (;;) {
		zone_t *z = *(zone++);
		if (!z)
			break;

		min += z->pages_low;
		if (z->free_pages > min) {
			page = rmqueue(z, order);
			if (page)
				return page;
		}
	}
	return NULL;

// 	classzone->need_balance = 1;
// 	mb();
// 	if (waitqueue_active(&kswapd_wait))
// 		wake_up_interruptible(&kswapd_wait);

// 	zone = zonelist->zones;
// 	min = 1UL << order;
// 	for (;;) {
// 		unsigned long local_min;
// 		zone_t *z = *(zone++);
// 		if (!z)
// 			break;

// 		local_min = z->pages_min;
// 		if (!(gfp_mask & __GFP_WAIT))
// 			local_min >>= 2;
// 		min += local_min;
// 		if (z->free_pages > min) {
// 			page = rmqueue(z, order);
// 			if (page)
// 				return page;
// 		}
// 	}

// 	/* here we're in the low on memory slow path */

// rebalance:
// 	if (current->flags & (PF_MEMALLOC | PF_MEMDIE)) {
// 		zone = zonelist->zones;
// 		for (;;) {
// 			zone_t *z = *(zone++);
// 			if (!z)
// 				break;

// 			page = rmqueue(z, order);
// 			if (page)
// 				return page;
// 		}
// 		return NULL;
// 	}

// 	/* Atomic allocations - we can't balance anything */
// 	if (!(gfp_mask & __GFP_WAIT))
// 		return NULL;

// 	page = balance_classzone(classzone, gfp_mask, order, &freed);
// 	if (page)
// 		return page;

// 	zone = zonelist->zones;
// 	min = 1UL << order;
// 	for (;;) {
// 		zone_t *z = *(zone++);
// 		if (!z)
// 			break;

// 		min += z->pages_min;
// 		if (z->free_pages > min) {
// 			page = rmqueue(z, order);
// 			if (page)
// 				return page;
// 		}
// 	}

// 	/* Don't let big-order allocations loop */
// 	if (order > 3)
// 		return NULL;

// 	/* Yield for kswapd, and try again */
// 	yield();
// 	goto rebalance;
}

struct page *_alloc_pages(unsigned int gfp_mask, unsigned int order)
{
    return __alloc_pages(gfp_mask, order,
                         contig_page_data.node_zonelists+(gfp_mask & GFP_ZONEMASK));
}

/* common helper function */
unsigned long __get_free_pages(int gfp_mask, unsigned long order)
{
	struct page *page;

	page = alloc_pages(gfp_mask, order);
    if (!page)
        return 0;
    return (unsigned long) page_address(page);
}

/*
 * Builds allocation fallback zone lists.
 */
static inline void build_zonelists(pg_data_t *pgdat)
{
	int i, j, k;

	for (i = 0; i <= GFP_ZONEMASK; i++) {   // 遍历最大可能数量的管理区
		zonelist_t *zonelist;
		zone_t *zone;

		zonelist = pgdat->node_zonelists + i;
		memset(zonelist, 0, sizeof(*zonelist));   // 获得管理区的 zonelist，并且归零

		// 设置 k 为当前检查过的管理区类型
		j = 0;
		k = ZONE_NORMAL;
		if (i & __GFP_HIGHMEM)
			k = ZONE_HIGHMEM;
		if (i & __GFP_DMA)
			k = ZONE_DMA;

		switch (k) {
			default:
				BUG();
			/*
			 * fallthrough:
			 */
			case ZONE_HIGHMEM:
				zone = pgdat->node_zones + ZONE_HIGHMEM;
				if (zone->size) {
#ifndef CONFIG_HIGHMEM
					BUG();
#endif
					zonelist->zones[j++] = zone;
				}
			case ZONE_NORMAL:
				zone = pgdat->node_zones + ZONE_NORMAL;
				if (zone->size)
					zonelist->zones[j++] = zone;
			case ZONE_DMA:
				zone = pgdat->node_zones + ZONE_DMA;
				if (zone->size)
					zonelist->zones[j++] = zone;
		}
		zonelist->zones[j++] = NULL;
	}
}

#define LONG_ALIGN(x) (((x)+(sizeof(long))-1)&~((sizeof(long))-1))

/*
 * Set up the zone data structures:
 *   - mark all pages reserved
 *   - mark all memory queues empty
 *   - clear the memory bitmaps
 */
void free_area_init_core(int nid, pg_data_t *pgdat, struct page **gmap,
								unsigned long *zones_size, unsigned long zone_start_paddr,
								unsigned long *zholes_size, struct page *lmem_map)
{
	unsigned long i, j;
	unsigned long map_size;
	unsigned long totalpages, offset, realtotalpages;
	const unsigned long zone_required_alignment = 1UL << (MAX_ORDER-1);   // 这个区域必须邻近由伙伴分配器分配的最大大小的块，从而进行位级操作

	if (zone_start_paddr & ~PAGE_MASK)   // 如果物理地址不是按页面排列的，就是一个 BUG
		BUG();

	totalpages = 0;   // 为这个节点初始化 totalpages 为 0
	for (i = 0; i < MAX_NR_ZONES; i++) {   // 通过遍历 zone_sizes 来计算节点的总大小
		unsigned long size = zones_size[i];
		totalpages += size;
	}
	realtotalpages = totalpages;
	if (zholes_size)   // 通过减去 zholes_size 的空洞大小来计算实际的内存量
		for (i = 0; i < MAX_NR_ZONES; i++)
			realtotalpages -= zholes_size[i];
	
	printk("On node %d totalpages: %lu\n", nid, realtotalpages);   // 打印提示信息告知用户这个节点可用的内存量

	/*
	 * Some architectures (with lots of mem and discontinous memory
	 * maps) have to search for a good mem_map area:
	 * For discontigmem, the conceptual mem map array starts from 
	 * PAGE_OFFSET, we need to align the actual array onto a mem map 
	 * boundary, so that MAP_NR works.
	 */
	map_size = (totalpages + 1)*sizeof(struct page);   // 计算数组所需的内存量。页面总数 * struct page 大小
	if (lmem_map == (struct page *)0) {   // 若映射图还没有分配，在这里分配
		printk("pgdat = %x\n", pgdat);
		// while(1);
		// lmem_map = (struct page *) alloc_bootmem_node(pgdat, map_size);
		lmem_map = (struct page *) alloc_bootmem_node(pgdat, map_size);
		lmem_map = (struct page *)(PAGE_OFFSET + 
			MAP_ALIGN((unsigned long)lmem_map - PAGE_OFFSET));
	}
	*gmap = pgdat->node_mem_map = lmem_map;
	pgdat->node_size = totalpages;   // 记录节点大小
	pgdat->node_start_paddr = zone_start_paddr;   // 记录起始物理地址
	pgdat->node_start_mapnr = (lmem_map - mem_map);   // 记录节点所占 mem_map 中的偏移
	pgdat->nr_zones = 0;   // 初始化管理区计数为 0，将在函数的后面设置

	offset = lmem_map - mem_map;   // offset 是 lmem_map 开始的局部部分相对 mem_map 的偏移
	for (j = 0; j < MAX_NR_ZONES; j++) {   // 循环处理节点中的每个 zone_t
		zone_t *zone = pgdat->node_zones + j;
		unsigned long mask;
		unsigned long size, realsize;

		zone_table[nid * MAX_NR_ZONES + j] = zone;   // 在 zone_table 中记录指向该管理区的指针
		realsize = size = zones_size[j];   // 计算管理区的实际大小
		if (zholes_size)
			realsize -= zholes_size[j];

		printk("zone(%lu): %lu pages.\n", j, size);   // 打印提示信息告知在这个管理区中的页面数

		zone->size = size;   // 初始化管理区的其他字段
		zone->name = zone_names[j];
		zone->lock = SPIN_LOCK_UNLOCKED;
		zone->zone_pgdat = pgdat;
		zone->free_pages = 0;
		zone->need_balance = 0;
		if (!size)
			continue;

		/*
		 * The per-page waitqueue mechanism uses hashed waitqueues
		 * per zone.
		 */
		// zone->wait_table_size = wait_table_size(size);
		// zone->wait_table_shift =
		// 	BITS_PER_LONG - wait_table_bits(zone->wait_table_size);
		// zone->wait_table = (wait_queue_head_t *)
		// 	alloc_bootmem_node(pgdat, zone->wait_table_size
		// 				* sizeof(wait_queue_head_t));

		// for(i = 0; i < zone->wait_table_size; ++i)
		// 	init_waitqueue_head(zone->wait_table + i);

		pgdat->nr_zones = j+1;   // 激活一个新的管理区，更新节点中的管理区数量

		// 计算管理区极值并记录管理区地址
		mask = (realsize / zone_balance_ratio[j]);   // 计算掩码
		if (mask < zone_balance_min[j])
			mask = zone_balance_min[j];
		else if (mask > zone_balance_max[j])
			mask = zone_balance_max[j];
		zone->pages_min = mask;
		zone->pages_low = mask*2;
		zone->pages_high = mask*3;

		zone->zone_mem_map = mem_map + offset;   // 记录管理区的第 1 个 struct page 在 mem_map 中的地址
		zone->zone_start_mapnr = offset;   // 记录 mem_map 中当前管理区起点的索引
		zone->zone_start_paddr = zone_start_paddr;   // 记录管理区的起始物理地址

		if ((zone_start_paddr >> PAGE_SHIFT) & (zone_required_alignment-1)) {   // 利用伙伴分配器保证管理区已经正确的排列可用（？）
			printk("BUG: wrong zone alignment, it will crash\n");
			BUG();
		}

		/*
		 * Initially all pages are reserved - free ones are freed
		 * up by free_all_bootmem() once the early boot process is
		 * done. Non-atomic initialization, single-pass.
		 */
		// 初识时，管理区中所有的页面都标记为保留，因为没有办法知道引导内存分配器使用的是哪些页面。
		// 当引导内存分配器在 free_all_bootmem() 中收回时，未使用页面中的 PG_reserved 会被清除。
		for (i = 0; i < size; i++) {
			struct page *page = mem_map + offset + i;   // 获得页面偏移
			set_page_zone(page, nid * MAX_NR_ZONES + j);   // 页面所在的管理区由页面标志编码
			set_page_count(page, 0);   // 设置计数为 0，因为管理区未被使用
			SetPageReserved(page);    // 设置保留标志，若页面不再被使用，引导内存分配器将清除该位
			INIT_LIST_HEAD(&page->list);   // 初始化页表链表头
			if (j != ZONE_HIGHMEM)    // 如果页面可用且页面在低端内存，设置 page->virtual 字段
				set_page_address(page, __va(zone_start_paddr));
			zone_start_paddr += PAGE_SIZE;   // 将 zone_start_paddr 增加一个页面大小，将用于记录下一个管理区的起点
		}

		// 初始化管理区的空闲链表，分配伙伴分配器在记录页面伙伴状态时的位图
		offset += size;
		for (i = 0; ; i++) {
			unsigned long bitmap_size;

			INIT_LIST_HEAD(&zone->free_area[i].free_list);
			if (i == MAX_ORDER-1) {
				zone->free_area[i].map = NULL;
				break;
			}

			/*
			 * Page buddy system uses "index >> (i+1)",
			 * where "index" is at most "size-1".
			 *
			 * The extra "+3" is to round down to byte
			 * size (8 bits per byte assumption). Thus
			 * we get "(size-1) >> (i+4)" as the last byte
			 * we can access.
			 *
			 * The "+1" is because we want to round the
			 * byte allocation up rather than down. So
			 * we should have had a "+7" before we shifted
			 * down by three. Also, we have to add one as
			 * we actually _use_ the last bit (it's [0,n]
			 * inclusive, not [0,n[).
			 *
			 * So we actually had +7+1 before we shift
			 * down by 3. But (n+8) >> 3 == (n >> 3) + 1
			 * (modulo overflows, which we do not have).
			 *
			 * Finally, we LONG_ALIGN because all bitmap
			 * operations are on longs.
			 */
			bitmap_size = (size-1) >> (i+4);
			bitmap_size = LONG_ALIGN(bitmap_size+1);
			zone->free_area[i].map = 
			  (unsigned long *) alloc_bootmem_node(pgdat, bitmap_size);
		}
	}
	build_zonelists(pgdat);
}

void free_area_init(unsigned long *zones_size)
{
	free_area_init_core(0, &contig_page_data, &mem_map, zones_size, 0, 0, 0);
}
