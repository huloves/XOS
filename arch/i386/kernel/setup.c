#include <define.h>
#include <asm/page.h>
#include <linux/multiboot.h>
#include <linux/bootmem.h>
#include <asm/e820.h>
#include <linux/init.h>
#include <linux/string.h>
#include <asm/stdio.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <define.h>
#include <linux/screen_info.h>

/*
 * Machine setup..
 */

char ignore_irq13;
struct cpuinfo_x86 boot_cpu_data = { 0, 0, 0, 0, -1, 1, 0, 0, -1 };

unsigned long mmu_cr4_features;   // extern in include/asm/processor.h

struct screen_info screen_info;

/* For PCI or other memory-mapped resources */
unsigned long pci_mem_start = 0x10000000;

#define PFN_UP(x) 	(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
#define PFN_DOWN(x) ((x) >> PAGE_SHIFT)
#define PFN_PHYS(x) ((x) << PAGE_SHIFT)

/*
 * Reserved space for vmalloc and iomap - defined in asm/page.h
 */
#define MAXMEM_PFN	PFN_DOWN(MAXMEM)
#define MAX_NONPAE_PFN	(1 << 20)

extern char _text, _etext, _edata, _end, _start;

struct e820map biosmap __attribute__ ((__section__ (".data.init")));
struct e820map e820;

struct resource standard_io_resources[] = {
	{ "dma1", 0x00, 0x1f, IORESOURCE_BUSY },
	{ "pic1", 0x20, 0x3f, IORESOURCE_BUSY },
	{ "timer", 0x40, 0x5f, IORESOURCE_BUSY },
	{ "keyboard", 0x60, 0x6f, IORESOURCE_BUSY },
	{ "dma page reg", 0x80, 0x8f, IORESOURCE_BUSY },
	{ "pic2", 0xa0, 0xbf, IORESOURCE_BUSY },
	{ "dma2", 0xc0, 0xdf, IORESOURCE_BUSY },
	{ "fpu", 0xf0, 0xff, IORESOURCE_BUSY }
};

#define STANDARD_IO_RESOURCES (sizeof(standard_io_resources)/sizeof(struct resource))

static struct resource code_resource = { "Kernel code", 0x100000, 0 };
static struct resource data_resource = { "Kernel data", 0, 0 };
static struct resource vram_resource = { "Video RAM area", 0xa0000, 0xbffff, IORESOURCE_BUSY };

/* System ROM resources */
#define MAXROMS 6
static struct resource rom_resources[MAXROMS] = {
	{ "System ROM", 0xF0000, 0xFFFFF, IORESOURCE_BUSY },
	{ "Video ROM", 0xc0000, 0xc7fff, IORESOURCE_BUSY }
};

#define romsignature(x) (*(unsigned short *)(x) == 0xaa55)

static void probe_roms(void)
{
	int roms = 1;
	unsigned long base;
	unsigned char *romstart;

	request_resource(&iomem_resource, rom_resources+0);

	/* Video ROM is standard at C000:0000 - C7FF:0000, check signature */
	for (base = 0xC0000; base < 0xE0000; base += 2048) {
		romstart = bus_to_virt(base);   // 将已经映射的物理内存地址转换为虚拟地址
		if (!romsignature(romstart))
			continue;
		request_resource(&iomem_resource, rom_resources + roms);
		roms++;
		break;
	}

	/* Extension roms at C800:0000 - DFFF:0000 */
	for (base = 0xC8000; base < 0xE0000; base += 2048) {
		unsigned long length;

		romstart = bus_to_virt(base);
		if (!romsignature(romstart))
			continue;
		length = romstart[2] * 512;
		if (length) {
			unsigned int i;
			unsigned char chksum;

			chksum = 0;
			for (i = 0; i < length; i++)
				chksum += romstart[i];

			/* Good checksum? */
			if (!chksum) {
				rom_resources[roms].start = base;
				rom_resources[roms].end = base + length - 1;
				rom_resources[roms].name = "Extension ROM";
				rom_resources[roms].flags = IORESOURCE_BUSY;

				request_resource(&iomem_resource, rom_resources + roms);
				roms++;
				if (roms >= MAXROMS)
					return;
			}
		}
	}

	/* Final check for motherboard extension rom at E000:0000 */
	base = 0xE0000;
	romstart = bus_to_virt(base);

	if (romsignature(romstart)) {
		rom_resources[roms].start = base;
		rom_resources[roms].end = base + 65535;
		rom_resources[roms].name = "Extension ROM";
		rom_resources[roms].flags = IORESOURCE_BUSY;

		request_resource(&iomem_resource, rom_resources + roms);
	}
}

static void add_memory_region(unsigned long long start, unsigned long long size, int type)
{
	int x = e820.nr_map;
	if(x == E820MAX) {
		printk("Ooops! Too many entries in the memory map!\n");
		return;
	}
	e820.map[x].addr = start;
	e820.map[x].size = size;
	e820.map[x].type = type;
	e820.nr_map++;
}

static void print_memory_map(char *who)
{
	int i;

	for (i = 0; i < e820.nr_map; i++) {
		// printk(" %s: 0x%08X - 0x%08X ", who,
        //     (uint32_t)e820.map[i].addr,
        //     (uint32_t)e820.map[i].addr + (uint32_t)e820.map[i].size);
		printk("base_addr = 0x%08X, length = 0x%08X, type = 0x%X",
            (uint32_t)e820.map[i].addr,
            (uint32_t)e820.map[i].size,
            (uint32_t)e820.map[i].type);
		// printk(" %s: %016Lx - %016Lx ", who,
			// e820.map[i].addr,
			// e820.map[i].addr + e820.map[i].size);
		switch (e820.map[i].type) {
		case E820_RAM:	printk("(usable)\n");
				break;
		case E820_RESERVED:
				printk("(reserved)\n");
				break;
		case E820_ACPI:
				printk("(ACPI data)\n");
				break;
		case E820_NVS:
				printk("(ACPI NVS)\n");
				break;
		default:
				printk("type %lu\n", e820.map[i].type);
				break;
		}
	}
}

/**
 * sanitize_e820_map - 对整个表排序
 * Some e820 responses include overlapping entries.  The following
 * replaces the original e820 map with a new one, removing overlaps.
 * **/
static int sanitize_e820_map(struct e820entry biosmap[], int *pnr_map)
{
	struct change_member {
		struct e820entry *pbios;   // 指向原始 bios条目 的指针，pointer to original bios entry
		unsigned long long addr;   // 此更改点的地址，address for this change point
	};
	struct change_member change_point_list[2 * E820MAX];   // E820MAX = 32
	struct change_member *change_point[2 * E820MAX];
	struct e820entry *overlap_list[E820MAX];
	struct e820entry new_bios[E820MAX];
	struct change_member *change_tmp;

	unsigned long current_type, last_type;
	unsigned long long last_addr;
	int chgidx, still_changing;
	int overlap_entries;
	int new_bios_entry;
	int old_nr, new_nr, chg_nr;
	int i;

	/* if there's only one memory region, don't bother */
	if(*pnr_map < 2)
		return -1;

	old_nr = *pnr_map;

	/* bail out if we find any unreasonable addresses in bios map */
	/* 如果我们在 bios map 中发现任何不合理的地址，退出 */
	for(i = 0; i < old_nr; i++) {
		if(biosmap[i].addr + biosmap[i].size < biosmap[i].addr) {
			return -1;
		}
	}

	/* create pointers for initial change-point information (for sorting) */
	/* 为初始变化点信息创建指针（用于排序） */
	for(i = 0; i < 2 * old_nr; i++) {
		change_point[i] = &change_point_list[i];
	}

	/* record all known change-points (starting and ending addresses) ,
	   omitting those that are for empty memory regions 
	   记录所有已知的变更点（开始和结束地址）,
	   省略那些用于空内存区域的 */
	chgidx = 0;
	for(i = 0; i < old_nr; i++) {   // change_point 每两项对应 biosmap 的一项。两项中的前一项记录起始地址，后一项记录结束地址
		if(biosmap[i].size != 0) {
			change_point[chgidx]->addr = biosmap[i].addr;   // 记录起始地址
			change_point[chgidx++]->pbios = &biosmap[i];
			change_point[chgidx]->addr = biosmap[i].addr + biosmap[i].size;   // 记录结束地址
			change_point[chgidx++]->pbios = &biosmap[i];
		}
	}
	chg_nr = chgidx;   // true number of change-points

	/* sort change-point list by memory addresses (low -> high) */
	/* 按内存地址排序 change-point list（低 -> 高）*/
	still_changing = 1;
	while (still_changing)	{
		still_changing = 0;
		for (i=1; i < chg_nr; i++)  {
			/* if <current_addr> > <last_addr>, swap */
			/* or, if current=<start_addr> & last=<end_addr>, swap */
			if ((change_point[i]->addr < change_point[i-1]->addr) ||
				((change_point[i]->addr == change_point[i-1]->addr) &&
				 (change_point[i]->addr == change_point[i]->pbios->addr) &&
				 (change_point[i-1]->addr != change_point[i-1]->pbios->addr))
			   )
			{
				change_tmp = change_point[i];
				change_point[i] = change_point[i-1];
				change_point[i-1] = change_tmp;
				still_changing=1;
			}
		}
	}

	/* create a new bios memory map, removing overlaps */
	/* 创建一个新的 bios 内存映射，去除重叠 */
	overlap_entries = 0;   // 重叠表中的条目数(number of entries in the overlap table)
	new_bios_entry = 0;    // 创建新的 BIOS 映射条目的索引(index for creating new bios map entries)
	last_type = 0;         // start with undefined memory type = 从未定义的内存类型起始处
	last_addr = 0;         // start with 0 as last starting address = 以 0 开头作为最后一个起始地址
	/* loop through change-points, determining affect on the new bios map */
	/* 遍历更改点，确定对新的 bios map 的影响 */
	for(chgidx = 0; chgidx < chg_nr; chgidx++) {
		/* keep track of all overlapping bios entries */
		/* 跟踪所有重叠的 bios 条目 */
		if(change_point[chgidx]->addr == change_point[chgidx]->pbios->addr) {
			/* add map entry to overlap list (> 1 entry implies an overlap) */
			/* 将映射条目添加到重叠列表（> 1 个条目意味着重叠）*/
			overlap_list[overlap_entries++] = change_point[chgidx]->pbios;
		} else {
			/* remove entry from list (order independent, so swap with last) */
			/* 从列表中删除条目（顺序无关，因此与最后一个交换）*/
			for(i = 0; i < overlap_entries; i++) {
				if(overlap_list[i] == change_point[chgidx]->pbios) {
					overlap_list[i] = overlap_list[overlap_entries - 1];
				}
				overlap_entries--;
			}
		}
		/* if there are overlapping entries, decide which "type" to use */
		/* (larger value takes precedence -- 1=usable, 2,3,4,4+=unusable) */
		/* 如果有重叠的条目，决定使用哪个“类型” */
		/*（较大的值优先——1=可用，2,3,4,4+=不可用）*/
		current_type = 0;
		for(i = 0; i < overlap_entries; i++) {
			if(overlap_list[i]->type > current_type) {
				current_type = overlap_list[i]->type;
			}
		}
		/* continue building up new bios map based on this information */
		/* 根据这些信息构建新的 bios map */
		if(current_type != last_type) {
			if(last_type != 0) {
				new_bios[new_bios_entry].size = change_point[chgidx]->addr - last_addr;
				/* move forward only if the new size was non-zero */
				/* 仅当新大小不为零时才向前移动 */
				if(new_bios[new_bios_entry].size != 0) {
					if(++new_bios_entry >= E820MAX) { break; }   /* 没有更多空间用于新的 bios 条目, no more space left for new bios entries */
				}
			}
			if (current_type != 0)	{
				new_bios[new_bios_entry].addr = change_point[chgidx]->addr;
				new_bios[new_bios_entry].type = current_type;
				last_addr=change_point[chgidx]->addr;
			}
			last_type = current_type;
		}
	}
	new_nr = new_bios_entry;   /* 为新的 bios 条目保留计数, retain count for new bios entries */
	/* copy new bios mapping into original location */
	/* 将新的 bios 映射复制到原始位置 */
	memcpy(biosmap, new_bios, new_nr * sizeof(struct e820entry));
	*pnr_map = new_nr;

	return 0;
}

static void init_biosmap()
{
	int count = 0;
    multiboot_t *mboot_ptr = glb_mboot_ptr;
    mmap_entry_t *mmap_start_addr = (mmap_entry_t *)mboot_ptr->mmap_addr;
    mmap_entry_t *mmap_end_addr = (mmap_entry_t *)(mboot_ptr->mmap_addr + mboot_ptr->mmap_length);
    mmap_entry_t *map_entry;

    max_pfn = 0;
    for (map_entry = mmap_start_addr; map_entry < mmap_end_addr; map_entry++) {
        unsigned long start, end, type;
        biosmap.map[count].addr = map_entry->base_addr_low;
        biosmap.map[count].size = map_entry->length_low;
        biosmap.map[count].type = map_entry->type;
        count++;
    }
    biosmap.nr_map = count;
}

/*
 * Copy the BIOS e820 map into a safe place.
 *
 * Sanity-check it while we're at it..
 *
 * If we're lucky and live on a modern system, the setup code
 * will have given us a memory map that we can use to properly
 * set up memory.  If we aren't, we'll fake a memory map.
 *
 * We check to see that the memory map contains at least 2 elements
 * before we'll use it, because the detection code in setup.S may
 * not be perfect and most every PC known to man has two memory
 * regions: one from 0 to 640k, and one from 1mb up.  (The IBM
 * thinkpad 560x, for example, does not cooperate with the memory
 * detection code.)
 */
static int copy_e820_map(struct e820entry * biosmap, int nr_map)
{
	/* Only one memory region (or negative)? Ignore it */
	if(nr_map < 2) {
		return -1;
	}
	do{
		unsigned long long start = biosmap->addr;
		unsigned long long size = biosmap->size;
		unsigned long long end = start + size;
		unsigned long type = biosmap->type;

		/* Overflow in 64 bits? Ignore the memory map. */
		if (start > end)
			return -1;

		/*
		 * Some BIOSes claim RAM in the 640k - 1M region.
		 * Not right. Fix it up.
		 */
		if (type == E820_RAM) {
			if (start < 0x100000ULL && end > 0xA0000ULL) {
				if (start < 0xA0000ULL)
					add_memory_region(start, 0xA0000ULL-start, type);
				if (end <= 0x100000ULL)
					continue;
				start = 0x100000ULL;
				size = end - start;
			}
		}
		add_memory_region(start, size, type);
	} while(biosmap++, --nr_map);
}

static void setup_memory_region(void)
{
	char *who = "BIOS-e820";
	
	init_biosmap();
	printk("biosmap.map = %p, biosmap.nr_map = %d\n", biosmap.map, biosmap.nr_map);
	sanitize_e820_map(biosmap.map, &biosmap.nr_map);   // 对整个表排序
	copy_e820_map(biosmap.map, biosmap.nr_map);   // 将biosmap中的内容复制到e820中
	printk("BIOS-provided physical RAM map:\n");
	print_memory_map(who);   // 打印e820内存信息
}

/*
 * Find the highest page frame number we have available
 */
static void find_max_pfn(void)
{
    int i;

	max_pfn = 0;
	for (i = 0; i < e820.nr_map; i++) {
		unsigned long start, end;
		/* RAM? */
		if (e820.map[i].type != E820_RAM)
			continue;
		start = PFN_UP(e820.map[i].addr);
		end = PFN_DOWN(e820.map[i].addr + e820.map[i].size);
		if (start >= end)
			continue;
		if (end > max_pfn)
			max_pfn = end;
	}
}

/*
 * Register fully available low RAM pages with the bootmem allocator.
 * 注册完全可用的低 RAM 页面。
 */
static void register_bootmem_low_pages(unsigned long max_low_pfn)
{
	printk("register_bootmem_low_pages start.\n");
	int i;
	for(i = 0; i < e820.nr_map; i++) {
		unsigned long curr_pfn, last_pfn, size;
		/*
		 * Reserve usable low memory
		 */
		if (e820.map[i].type != E820_RAM) {
			continue;
		}
		/*
		 * We are rounding up the start address of usable memory:
		 * 对可用内存的起始地址进行向上取整：
		 */
		curr_pfn = PFN_UP(e820.map[i].addr);
		if(curr_pfn >= max_low_pfn) {
			continue;
		}
		/*
		 * ... and at the end of the usable range downwards:
		 * 对可用的结束地址进行向下取整：
		 */
		last_pfn = PFN_DOWN(e820.map[i].addr + e820.map[i].size);

		if(last_pfn > max_low_pfn) {
			last_pfn = max_low_pfn;
		}

		/*
		 * .. finally, did all the rounding and playing
		 * around just make the area go away?
		 */
		if(last_pfn <= curr_pfn) {
			continue;
		}

		size = last_pfn - curr_pfn;
		free_bootmem(PFN_PHYS(curr_pfn), PFN_PHYS(size));
	}
	printk("register_bootmem_low_pages down.\n");
}

/*
 * Determine low and high memory ranges:
 */
static unsigned long find_max_low_pfn(void)
{
    unsigned long max_low_pfn;

    max_low_pfn = max_pfn;
    if (max_low_pfn > MAXMEM_PFN) {
        max_low_pfn = MAXMEM_PFN;
		printk("Warning only %ldMB will be used.\n", MAXMEM >> 20);
    }
    return max_low_pfn;
}

static unsigned long setup_memory(void)
{
	printk("setup_memory start.\n");
	unsigned long bootmap_size, start_pfn;
	
	/*
	 * partially used pages are not usable - thus
	 * we are rounding upwards:
	 */
	start_pfn = PFN_UP(__pa(&_end));   //将物理地址向上取整到下一个页面。__end是已载入内核的底端地址，所以start_pfn是第一块可以被用到的物理页面帧的偏移
	// printk("start_pfn = 0x%x\n", start_pfn);
	
	find_max_pfn();   //遍历e820图，查找最高的可用PFN
	
	max_low_pfn = find_max_low_pfn();

#ifdef CONFIG_HIGHMEM
	highstart_pfn = highend_pfn = max_pfn;
	if (max_pfn > max_low_pfn) {
		highstart_pfn = max_low_pfn;
	}
	printk(KERN_NOTICE "%ldMB HIGHMEM available.\n",
		pages_to_mb(highend_pfn - highstart_pfn));
#endif
	printk("%ldMB LOWMEM available.\n",
			pages_to_mb(max_low_pfn));
	/*
	 * Initialize the boot-time allocator (with low memory only):
	 */
	bootmap_size = init_bootmem(start_pfn, max_low_pfn);

	register_bootmem_low_pages(max_low_pfn);

	/*
	 * Reserve the bootmem bitmap itself as well. We do this in two
	 * steps (first step was init_bootmem()) because this catches
	 * the (very unlikely) case of us accidentally initializing the
	 * bootmem allocator with an invalid RAM area.
	 */
	reserve_bootmem(HIGH_MEMORY, 
					(PFN_PHYS(start_pfn) + bootmap_size + PAGE_SIZE - 1) - (HIGH_MEMORY));   // 把内核和 bootmem位图 所占的内存标记为“保留”，HIGH_MEMORY为1MB，即内核开始的地方
	// reserve_bootmem(HIGH_MEMORY, (&_end - 0xc0000000 - &_start + PAGE_SIZE - 1) - (HIGH_MEMORY));   // 把内核和 bootmem位图 所占的内存标记为“保留”，HIGH_MEMORY为1MB，即内核开始的地方

	/*
	 * reserve physical page 0 - it's a special BIOS page on many boxes,
	 * enabling clean reboots, SMP operation, laptop functions.
	 */
	reserve_bootmem(0, PAGE_SIZE);
	printk("setup_memory down.\n");
	return max_low_pfn;
}

/*
 * Request address space for all standard RAM and ROM resources（请求所有标准RAM和ROM资源的地址空间）
 * and also for regions reported as reserved by the e820.（也适用于e820 报告的区域。）
 */
static void register_memory(unsigned long max_low_pfn)
{
	unsigned long low_mem_size;
	int i;
	probe_roms();
	for (i = 0; i < e820.nr_map; i++) {
		struct resource *res;
		if (e820.map[i].addr + e820.map[i].size > 0x100000000ULL)
			continue;
		res = alloc_bootmem_low(sizeof(struct resource));
		switch (e820.map[i].type) {
		case E820_RAM:	res->name = "System RAM"; break;
		case E820_ACPI:	res->name = "ACPI Tables"; break;
		case E820_NVS:	res->name = "ACPI Non-volatile Storage"; break;
		default:	res->name = "reserved";
		}
		res->start = e820.map[i].addr;
		res->end = res->start + e820.map[i].size - 1;
		res->flags = IORESOURCE_MEM | IORESOURCE_BUSY;
		request_resource(&iomem_resource, res);
		if (e820.map[i].type == E820_RAM) {
			/*
			 *  We dont't know which RAM region contains kernel data,
			 *  so we try it repeatedly and let the resource manager
			 *  test it.
			 */
			request_resource(res, &code_resource);
			request_resource(res, &data_resource);
		}
	}
	request_resource(&iomem_resource, &vram_resource);

	/* request I/O space for devices used on all i[345]86 PCs */
	for (i = 0; i < STANDARD_IO_RESOURCES; i++)
		request_resource(&ioport_resource, standard_io_resources+i);

	/* Tell the PCI layer not to allocate too close to the RAM area.. */
	low_mem_size = ((max_low_pfn << PAGE_SHIFT) + 0xfffff) & ~0xfffff;
	if (low_mem_size > pci_mem_start)
		pci_mem_start = low_mem_size;
}

void show_memory_map()
{
    uint32_t mmap_addr = ((multiboot_t*)glb_mboot_ptr)->mmap_addr;
    uint32_t mmap_length = ((multiboot_t*)glb_mboot_ptr)->mmap_length;

    printk("Memory map:\n");

    mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr;
    for (mmap = (mmap_entry_t *)mmap_addr; (uint32_t)mmap < mmap_addr + mmap_length; mmap++) {
        printk("base_addr = 0x%X%08X, length = 0x%X%08X, type = 0x%X\n",
            (uint32_t)mmap->base_addr_high, (uint32_t)mmap->base_addr_low,
            (uint32_t)mmap->length_high, (uint32_t)mmap->length_low,
            (uint32_t)mmap->type);
    }
	// setup_memory_region();
}

static void init_screen_info(void)
{
	screen_info.orig_video_mode = 0x6A;	/* 800*600/4 (VESA) */
	screen_info.orig_video_mode = 1;	/* 彩色模式 */
	screen_info.orig_video_points = 8;	/* 字体大小信息 */
	screen_info.orig_video_cols = 100;	/* 屏幕列信息 */
	screen_info.orig_video_lines = 75;	/* 屏幕行信息 */
}

void setup_arch(void)
{
	printk("setup_arch start.\n");
	unsigned long max_low_pfn;
	
	printk("kernel in memory start: 0x%08X\n", &_start);
	printk("kernel in memory end:   0x%08X\n", &_end - 0xc0000000);
	printk("kernel in memory used:   %d KB\n\n", (&_end - 0xc0000000 - &_start + 1023) / 1024);

	show_memory_map();
	setup_memory_region();
	max_low_pfn = setup_memory();
	// printk("max_low_pfn = %x\n", max_low_pfn);
	paging_init();
	register_memory(max_low_pfn);
	init_screen_info();
	printk("setup_arch down.\n");
}
