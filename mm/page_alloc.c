#include <linux/mm.h>
#include <linux/mmzone.h>
#include <asm-i386/page.h>
#include <asm-i386/stdio.h>
#include <linux/debug.h>
#include <linux/bootmem.h>
#include <linux/list.h>
#include <linux/string.h>

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

#define LONG_ALIGN(x) (((x)+(sizeof(long))-1)&~((sizeof(long))-1))

/*
 * Builds allocation fallback zone lists.
 */
static inline void build_zonelists(pg_data_t *pgdat)
{
	int i, j, k;

	for (i = 0; i <= GFP_ZONEMASK; i++) {
		zonelist_t *zonelist;
		zone_t *zone;

		zonelist = pgdat->node_zonelists + i;
		memset(zonelist, 0, sizeof(*zonelist));

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
	printk("123\n");
}


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
		realsize = size = zones_size[j];
		if (zholes_size)
			realsize -= zholes_size[j];

		printk("zone(%lu): %lu pages.\n", j, size);
		// while(1);
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

		zone->zone_mem_map = mem_map + offset;
		zone->zone_start_mapnr = offset;
		zone->zone_start_paddr = zone_start_paddr;

		if ((zone_start_paddr >> PAGE_SHIFT) & (zone_required_alignment-1)) {
			printk("BUG: wrong zone alignment, it will crash\n");
			BUG();
		}

		/*
		 * Initially all pages are reserved - free ones are freed
		 * up by free_all_bootmem() once the early boot process is
		 * done. Non-atomic initialization, single-pass.
		 */
		for (i = 0; i < size; i++) {
			struct page *page = mem_map + offset + i;   // 获得页面偏移
			set_page_zone(page, nid * MAX_NR_ZONES + j);   // 页面所在的管理区由页面标志编码
			set_page_count(page, 0);
			SetPageReserved(page);
			INIT_LIST_HEAD(&page->list);
			if (j != ZONE_HIGHMEM)
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
	printk("123123123123\n");
	printk("MMMMMMM\n");
	// while(1);
	build_zonelists(pgdat);
}

void free_area_init(unsigned long *zones_size)
{
	free_area_init_core(0, &contig_page_data, &mem_map, zones_size, 0, 0, 0);
}
