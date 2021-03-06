#include <asm-i386/page.h>
#include <linux/multiboot.h>
#include <linux/bootmem.h>
#include <asm-i386/e820.h>
#include <linux/init.h>
#include <linux/string.h>
#include <asm-i386/stdio.h>
#include <asm-i386/pgtable.h>
#include <asm-i386/processor.h>
#include <linux/ioport.h>
#include <asm-i386/io.h>

/*
 * Machine setup..
 */

char ignore_irq13;
struct cpuinfo_x86 boot_cpu_data = { 0, 0, 0, 0, -1, 1, 0, 0, -1 };

unsigned long mmu_cr4_features;   // extern in include/asm-i386/processor.h

/* For PCI or other memory-mapped resources */
unsigned long pci_mem_start = 0x10000000;

#define PFN_UP(x) (((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
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
		romstart = bus_to_virt(base);   // ?????????????????????????????????????????????????????????
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
 * sanitize_e820_map - ??????????????????
 * Some e820 responses include overlapping entries.  The following
 * replaces the original e820 map with a new one, removing overlaps.
 * **/
static int sanitize_e820_map(struct e820entry biosmap[], int *pnr_map)
{
	struct change_member {
		struct e820entry *pbios;   // ???????????? bios?????? ????????????pointer to original bios entry
		unsigned long long addr;   // ????????????????????????address for this change point
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
	/* ??????????????? bios map ?????????????????????????????????????????? */
	for(i = 0; i < old_nr; i++) {
		if(biosmap[i].addr + biosmap[i].size < biosmap[i].addr) {
			return -1;
		}
	}

	/* create pointers for initial change-point information (for sorting) */
	/* ?????????????????????????????????????????????????????? */
	for(i = 0; i < 2 * old_nr; i++) {
		change_point[i] = &change_point_list[i];
	}

	/* record all known change-points (starting and ending addresses) ,
	   omitting those that are for empty memory regions 
	   ?????????????????????????????????????????????????????????,
	   ???????????????????????????????????? */
	chgidx = 0;
	for(i = 0; i < old_nr; i++) {   // change_point ??????????????? biosmap ?????????????????????????????????????????????????????????????????????????????????
		if(biosmap[i].size != 0) {
			change_point[chgidx]->addr = biosmap[i].addr;   // ??????????????????
			change_point[chgidx++]->pbios = &biosmap[i];
			change_point[chgidx]->addr = biosmap[i].addr + biosmap[i].size;   // ??????????????????
			change_point[chgidx++]->pbios = &biosmap[i];
		}
	}
	chg_nr = chgidx;   // true number of change-points

	/* sort change-point list by memory addresses (low -> high) */
	/* ????????????????????? change-point list?????? -> ??????*/
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
	/* ?????????????????? bios ??????????????????????????? */
	overlap_entries = 0;   // ????????????????????????(number of entries in the overlap table)
	new_bios_entry = 0;    // ???????????? BIOS ?????????????????????(index for creating new bios map entries)
	last_type = 0;         // start with undefined memory type = ????????????????????????????????????
	last_addr = 0;         // start with 0 as last starting address = ??? 0 ????????????????????????????????????
	/* loop through change-points, determining affect on the new bios map */
	/* ????????????????????????????????? bios map ????????? */
	for(chgidx = 0; chgidx < chg_nr; chgidx++) {
		/* keep track of all overlapping bios entries */
		/* ????????????????????? bios ?????? */
		if(change_point[chgidx]->addr == change_point[chgidx]->pbios->addr) {
			/* add map entry to overlap list (> 1 entry implies an overlap) */
			/* ???????????????????????????????????????> 1 ???????????????????????????*/
			overlap_list[overlap_entries++] = change_point[chgidx]->pbios;
		} else {
			/* remove entry from list (order independent, so swap with last) */
			/* ????????????????????????????????????????????????????????????????????????*/
			for(i = 0; i < overlap_entries; i++) {
				if(overlap_list[i] == change_point[chgidx]->pbios) {
					overlap_list[i] = overlap_list[overlap_entries - 1];
				}
				overlap_entries--;
			}
		}
		/* if there are overlapping entries, decide which "type" to use */
		/* (larger value takes precedence -- 1=usable, 2,3,4,4+=unusable) */
		/* ????????????????????????????????????????????????????????? */
		/*???????????????????????????1=?????????2,3,4,4+=????????????*/
		current_type = 0;
		for(i = 0; i < overlap_entries; i++) {
			if(overlap_list[i]->type > current_type) {
				current_type = overlap_list[i]->type;
			}
		}
		/* continue building up new bios map based on this information */
		/* ?????????????????????????????? bios map */
		if(current_type != last_type) {
			if(last_type != 0) {
				new_bios[new_bios_entry].size = change_point[chgidx]->addr - last_addr;
				/* move forward only if the new size was non-zero */
				/* ?????????????????????????????????????????? */
				if(new_bios[new_bios_entry].size != 0) {
					if(++new_bios_entry >= E820MAX) { break; }   /* ?????????????????????????????? bios ??????, no more space left for new bios entries */
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
	new_nr = new_bios_entry;   /* ????????? bios ??????????????????, retain count for new bios entries */
	/* copy new bios mapping into original location */
	/* ????????? bios ??????????????????????????? */
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
	sanitize_e820_map(biosmap.map, &biosmap.nr_map);
	copy_e820_map(biosmap.map, biosmap.nr_map);
	printk("BIOS-provided physical RAM map:\n");
	print_memory_map(who);
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
 * ???????????????????????? RAM ?????????
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
		 * ???????????????????????????????????????????????????
		 */
		curr_pfn = PFN_UP(e820.map[i].addr);
		if(curr_pfn >= max_low_pfn) {
			continue;
		}
		/*
		 * ... and at the end of the usable range downwards:
		 * ?????????????????????????????????????????????
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
	start_pfn = PFN_UP(__pa(&_end));   //????????????????????????????????????????????????__end??????????????????????????????????????????start_pfn??????????????????????????????????????????????????????
	// printk("start_pfn = 0x%x\n", start_pfn);
	
	find_max_pfn();   //??????e820???????????????????????????PFN
	
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
					(PFN_PHYS(start_pfn) + bootmap_size + PAGE_SIZE - 1) - (HIGH_MEMORY));   // ???????????? bootmem?????? ???????????????????????????????????????HIGH_MEMORY???1MB???????????????????????????
	// reserve_bootmem(HIGH_MEMORY, (&_end - 0xc0000000 - &_start + PAGE_SIZE - 1) - (HIGH_MEMORY));   // ???????????? bootmem?????? ???????????????????????????????????????HIGH_MEMORY???1MB???????????????????????????

	/*
	 * reserve physical page 0 - it's a special BIOS page on many boxes,
	 * enabling clean reboots, SMP operation, laptop functions.
	 */
	reserve_bootmem(0, PAGE_SIZE);
	printk("setup_memory down.\n");
	return max_low_pfn;
}

/*
 * Request address space for all standard RAM and ROM resources?????????????????????RAM???ROM????????????????????????
 * and also for regions reported as reserved by the e820.???????????????e820 ?????????????????????
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
	printk("setup_arch down.\n");
}
