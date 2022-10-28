#include <linux/slab.h>
#include <asm-i386/types.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <asm-i386/cache.h>
#include <asm-i386/page.h>
#include <linux/debug.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <asm-i386/stdio.h>
#include <linux/compiler.h>
#include <asm-i386/hardirq.h>
#include <linux/string.h>

/*
 * DEBUG	- 1 for kmem_cache_create() to honour; SLAB_DEBUG_INITIAL,
 *		  SLAB_RED_ZONE & SLAB_POISON.
 *		  0 for faster, smaller code (especially in the critical paths).
 *
 * STATS	- 1 to collect stats for /proc/slabinfo.
 *		  0 for faster, smaller code (especially in the critical paths).
 *
 * FORCED_DEBUG	- 1 enables SLAB_RED_ZONE and SLAB_POISON (if possible)
 */

#ifdef CONFIG_DEBUG_SLAB
#define	DEBUG		1
#define	STATS		1
#define	FORCED_DEBUG	1
#else
#define	DEBUG		0
#define	STATS		0
#define	FORCED_DEBUG	0
#endif

/*
 * Parameters for kmem_cache_reap
 */
#define REAP_SCANLEN	10
#define REAP_PERFECT	10

/* Shouldn't this be in a header file somewhere? */
#define	BYTES_PER_WORD		sizeof(void *)

/* Legal flag mask for kmem_cache_create(). */
#if DEBUG
# define CREATE_MASK	(SLAB_DEBUG_INITIAL | SLAB_RED_ZONE | \
			 SLAB_POISON | SLAB_HWCACHE_ALIGN | \
			 SLAB_NO_REAP | SLAB_CACHE_DMA | \
			 SLAB_MUST_HWCACHE_ALIGN)
#else
# define CREATE_MASK	(SLAB_HWCACHE_ALIGN | SLAB_NO_REAP | \
			 SLAB_CACHE_DMA | SLAB_MUST_HWCACHE_ALIGN)
#endif

/*
 * kmem_bufctl_t:
 *
 * Bufctl's are used for linking objs within a slab
 * linked offsets.
 *
 * This implementation relies on "struct page" for locating the cache &
 * slab an object belongs to.
 * This allows the bufctl structure to be small (one int), but limits
 * the number of objects a slab (not a cache) can contain when off-slab
 * bufctls are used. The limit is the size of the largest general cache
 * that does not use off-slab slabs.
 * For 32bit archs with 4 kB pages, is this 56.
 * This is not serious, as it is only for large objects, when it is unwise
 * to have too many per slab.
 * Note: This limit can be raised by introducing a general cache whose size
 * is less than 512 (PAGE_SIZE<<3), but greater than 256.
 */

#define BUFCTL_END 0xffffFFFF
#define	SLAB_LIMIT 0xffffFFFE
typedef unsigned int kmem_bufctl_t;

/* Max number of objs-per-slab for caches which use off-slab slabs.
 * Needed to avoid a possible looping condition in kmem_cache_grow().
 */
static unsigned long offslab_limit;

/*
 * slab_t
 *
 * Manages the objs in a slab. Placed either at the beginning of mem allocated
 * for a slab, or allocated from an general cache.
 * Slabs are chained into three list: fully used, partial, fully free slabs.
 */
typedef struct slab_s {
	struct list_head	list;   // 该slab所属的链表，slab_full, slab_partial or slab_free中的一个
	unsigned long		colouroff;   // 这是从slab中第一个对象的基地址偏移的颜色。第一个对象的地址是s_mem + colouroff
	void				*s_mem;		/* including colour offset */ // 这给出了slab中第一个对象的起始地址
	unsigned int		inuse;		/* num of objs active in slab */ // slab中活动对象的数量
	kmem_bufctl_t		free;   // 这是一个 bufctl 数组，用于存储空闲对象的位置
} slab_t;

#define slab_bufctl(slabp) \
	((kmem_bufctl_t *)(((slab_t*)slabp)+1))

#define CACHE_NAMELEN	20	/* max name length for a slab cache */

struct kmem_cache_s {
/* 1) each alloc & free */
	/* full, partial first, then free */
	struct list_head	slabs_full;
	struct list_head	slabs_partial;
	struct list_head	slabs_free;
	unsigned int		objsize;   // slab中每个对象的大小
	unsigned int	 	flags;	/* constant flags */ // 决定分配器应该如何处理高速缓存
	unsigned int		num;	/* # of objs per slab */   // 每一个slab中包含的对象个数
	spinlock_t			spinlock;
#ifdef CONFIG_SMP
	unsigned int		batchcount;   //为per-cpu高速缓存批量分配对象的数目
#endif

/* 2) slab additions /removals */
	/* order of pgs per slab (2^n) */
	unsigned int		gfporder;   // 表示slab的大小（以页为单位）

	/* force GFP flags, e.g. GFP_DMA */
	unsigned int		gfpflags;   // 调用伙伴分配器分配页面时使用的 GFP 标志存储在这里

	size_t				colour;		/* cache colouring range */ // 如果可能，每个slab 将对象存储在不同的缓存行中
	unsigned int		colour_off;	/* colour offset */ // 保持slab字节对齐
	unsigned int		colour_next;	/* cache colouring */ // 下一个要使用的缓存行（被染色的）
	kmem_cache_t		*slabp_cache;
	unsigned int		growing;   // // 此标志设置为指示缓存是否正在增长
	unsigned int		dflags;		/* dynamic flags */ // 这些是在缓存生命周期内更改的动态标志

	/* constructor func */
	void (*ctor)(void *, kmem_cache_t *, unsigned long);   // 一个复杂的对象可以选择提供一个构造函数来初始化每个新对象

	/* de-constructor func */
	void (*dtor)(void *, kmem_cache_t *, unsigned long);   // 对象析构函数，可能为 NULL

	unsigned long		failures;   // 除了初始化为 0 之外，该字段在代码中的任何地方都没有使用

/* 3) cache creation/removal */
	char				name[CACHE_NAMELEN];
	struct list_head	next;
#ifdef CONFIG_SMP
/* 4) per-cpu data */
	cpucache_t			*cpudata[NR_CPUS];
#endif
#if STATS
	unsigned long		num_active;
	unsigned long		num_allocations;
	unsigned long		high_mark;
	unsigned long		grown;
	unsigned long		reaped;
	unsigned long 		errors;
#ifdef CONFIG_SMP
	atomic_t		allochit;
	atomic_t		allocmiss;
	atomic_t		freehit;
	atomic_t		freemiss;
#endif
#endif
};

/* internal c_flags */
#define	CFLGS_OFF_SLAB	0x010000UL	/* slab management in own cache */
#define	CFLGS_OPTIMIZE	0x020000UL	/* optimized slab lookup */

/* c_dflags (dynamic flags). Need to hold the spinlock to access this member */
#define	DFLGS_GROWN	0x000001UL		/* don't reap a recently grown */

#define	OFF_SLAB(x)	((x)->flags & CFLGS_OFF_SLAB)
#define	OPTIMIZE(x)	((x)->flags & CFLGS_OPTIMIZE)
#define	GROWN(x)	((x)->dlags & DFLGS_GROWN)

#if STATS
#define	STATS_INC_ACTIVE(x)	((x)->num_active++)
#define	STATS_DEC_ACTIVE(x)	((x)->num_active--)
#define	STATS_INC_ALLOCED(x)	((x)->num_allocations++)
#define	STATS_INC_GROWN(x)	((x)->grown++)
#define	STATS_INC_REAPED(x)	((x)->reaped++)
#define	STATS_SET_HIGH(x)	do { if ((x)->num_active > (x)->high_mark) \
					(x)->high_mark = (x)->num_active; \
				} while (0)
#define	STATS_INC_ERR(x)	((x)->errors++)
#else
#define	STATS_INC_ACTIVE(x)	do { } while (0)
#define	STATS_DEC_ACTIVE(x)	do { } while (0)
#define	STATS_INC_ALLOCED(x)	do { } while (0)
#define	STATS_INC_GROWN(x)	do { } while (0)
#define	STATS_INC_REAPED(x)	do { } while (0)
#define	STATS_SET_HIGH(x)	do { } while (0)
#define	STATS_INC_ERR(x)	do { } while (0)
#endif

#if STATS && defined(CONFIG_SMP)
#define STATS_INC_ALLOCHIT(x)	atomic_inc(&(x)->allochit)
#define STATS_INC_ALLOCMISS(x)	atomic_inc(&(x)->allocmiss)
#define STATS_INC_FREEHIT(x)	atomic_inc(&(x)->freehit)
#define STATS_INC_FREEMISS(x)	atomic_inc(&(x)->freemiss)
#else
#define STATS_INC_ALLOCHIT(x)	do { } while (0)
#define STATS_INC_ALLOCMISS(x)	do { } while (0)
#define STATS_INC_FREEHIT(x)	do { } while (0)
#define STATS_INC_FREEMISS(x)	do { } while (0)
#endif

#if DEBUG
/* Magic nums for obj red zoning.
 * Placed in the first word before and the first word after an obj.
 */
#define	RED_MAGIC1	0x5A2CF071UL	/* when obj is active */
#define	RED_MAGIC2	0x170FC2A5UL	/* when obj is inactive */

/* ...and for poisoning */
#define	POISON_BYTE	0x5a		/* byte value for poisoning */
#define	POISON_END	0xa5		/* end-byte of poisoning */

#endif

/* maximum size of an obj (in 2^order pages) */
#define	MAX_OBJ_ORDER	5	/* 32 pages */

/*
 * Do not go above this order unless 0 objects fit into the slab.
 */
#define	BREAK_GFP_ORDER_HI	2
#define	BREAK_GFP_ORDER_LO	1
static int slab_break_gfp_order = BREAK_GFP_ORDER_LO;

/*
 * Absolute limit for the gfp order
 */
#define	MAX_GFP_ORDER	5	/* 32 pages */

/* Macros for storing/retrieving the cachep and or slab from the
 * global 'mem_map'. These are used to find the slab an obj belongs to.
 * With kfree(), these are used to find the cache which an obj belongs to.
 */
#define	SET_PAGE_CACHE(pg,x)  ((pg)->list.next = (struct list_head *)(x))
#define	GET_PAGE_CACHE(pg)    ((kmem_cache_t *)(pg)->list.next)
#define	SET_PAGE_SLAB(pg,x)   ((pg)->list.prev = (struct list_head *)(x))
#define	GET_PAGE_SLAB(pg)     ((slab_t *)(pg)->list.prev)

/* Size description struct for general caches. */
typedef struct cache_sizes {
	size_t		 	cs_size;   // 内存块大小
	kmem_cache_t	*cs_cachep;   // normal内存使用的块缓存
	kmem_cache_t	*cs_dmacachep;   // 用于DMA的块缓存
} cache_sizes_t;

static cache_sizes_t cache_sizes[] = {
#if PAGE_SIZE == 4096
	{    32,	NULL, NULL},
#endif
	{    64,	NULL, NULL},
	{   128,	NULL, NULL},
	{   256,	NULL, NULL},
	{   512,	NULL, NULL},
	{  1024,	NULL, NULL},
	{  2048,	NULL, NULL},
	{  4096,	NULL, NULL},
	{  8192,	NULL, NULL},
	{ 16384,	NULL, NULL},
	{ 32768,	NULL, NULL},
	{ 65536,	NULL, NULL},
	{131072,	NULL, NULL},
	{     0,	NULL, NULL}
};

/* internal cache of cache description objs */
static kmem_cache_t cache_cache = {
	slabs_full:		LIST_HEAD_INIT(cache_cache.slabs_full),
	slabs_partial:	LIST_HEAD_INIT(cache_cache.slabs_partial),
	slabs_free:		LIST_HEAD_INIT(cache_cache.slabs_free),
	objsize:		sizeof(kmem_cache_t),
	flags:			SLAB_NO_REAP,
	spinlock:		SPIN_LOCK_UNLOCKED,
	colour_off:		L1_CACHE_BYTES,
	name:			"kmem_cache",
};

#define cache_chain (cache_cache.next)

/* Cal the num objs, wastage, and bytes left over for a given slab size. */
static void kmem_cache_estimate (unsigned long gfporder, size_t size,
		 int flags, size_t *left_over, unsigned int *num)
{
	int i;
	size_t wastage = PAGE_SIZE<<gfporder;
	size_t extra = 0;
	size_t base = 0;

	if (!(flags & CFLGS_OFF_SLAB)) {
		base = sizeof(slab_t);
		extra = sizeof(kmem_bufctl_t);
	}
	i = 0;
	while (i*size + L1_CACHE_ALIGN(base+i*extra) <= wastage)
		i++;
	if (i > 0)
		i--;

	if (i > SLAB_LIMIT)
		i = SLAB_LIMIT;

	*num = i;
	wastage -= i*size;
	wastage -= L1_CACHE_ALIGN(base+i*extra);
	*left_over = wastage;
}

/* Initialisation - setup the `cache' cache. */
void kmem_cache_init(void)
{	
	printk("kmem_cache_init start.\n");
	size_t left_over;

	INIT_LIST_HEAD(&cache_chain);   // 初始化高速缓存链表
	kmem_cache_estimate(0, cache_cache.objsize, 0,
			&left_over, &cache_cache.num);   // 计算对象的数量和消耗的字节数
	if (!cache_cache.num)
		BUG();
	
	cache_cache.colour = left_over/cache_cache.colour_off;
	cache_cache.colour_next = 0;   // 指示接下来使用哪一行，从0开始
	printk("kmem_cache_init down.\n");
}

/* Initialisation - setup remaining internal and general caches.
 * Called after the gfp() functions have been enabled, and before smp_init().
 */
void kmem_cache_sizes_init(void)
{
	printk("kmem_cache_sizes_init start.\n");
	cache_sizes_t *sizes = cache_sizes;
	char name[20];
	/*
	 * Fragmentation resistance on low memory - only use bigger
	 * page orders on machines with more than 32MB of memory.
	 */
	if (num_physpages > (32 << 20) >> PAGE_SHIFT)
		slab_break_gfp_order = BREAK_GFP_ORDER_HI;
	do {
		/* For performance, all the general caches are L1 aligned.
		 * This should be particularly beneficial on SMP boxes, as it
		 * eliminates "false sharing".
		 * Note for systems short on memory removing the alignment will
		 * allow tighter packing of the smaller caches. */
		snprintf(name, sizeof(name), "size-%Zd",sizes->cs_size);
		if (!(sizes->cs_cachep =
			kmem_cache_create(name, sizes->cs_size,
					0, SLAB_HWCACHE_ALIGN, NULL, NULL))) {
			BUG();
		}

		/* Inc off-slab bufctl limit until the ceiling is hit. */
		if (!(OFF_SLAB(sizes->cs_cachep))) {
			offslab_limit = sizes->cs_size-sizeof(slab_t);
			offslab_limit /= 2;
		}
		snprintf(name, sizeof(name), "size-%Zd(DMA)",sizes->cs_size);
		sizes->cs_dmacachep = kmem_cache_create(name, sizes->cs_size, 0,
			      SLAB_CACHE_DMA|SLAB_HWCACHE_ALIGN, NULL, NULL);
		if (!sizes->cs_dmacachep)
			BUG();
		sizes++;
	} while (sizes->cs_size);
	printk("kmem_cache_size_init down.\n");
}

/* Interface to system's page allocator. No need to hold the cache-lock.
 */
static inline void * kmem_getpages (kmem_cache_t *cachep, unsigned long flags)
{
	void	*addr;

	/*
	 * If we requested dmaable memory, we will get it. Even if we
	 * did not request dmaable memory, we might get it, but that
	 * would be relatively rare and ignorable.
	 */
	flags |= cachep->gfpflags;
	addr = (void*) __get_free_pages(flags, cachep->gfporder);
	/* Assume that now we have the pages no one else can legally
	 * messes with the 'struct page's.
	 * However vm_scan() might try to test the structure to see if
	 * it is a named-page or buffer-page.  The members it tests are
	 * of no interest here.....
	 */
	return addr;
}

/* Interface to system's page release. */
static inline void kmem_freepages (kmem_cache_t *cachep, void *addr)
{
	unsigned long i = (1<<cachep->gfporder);
	struct page *page = virt_to_page(addr);

	/* free_pages() does not clear the type bit - we do that.
	 * The pages have been unlinked from their cache-slab,
	 * but their 'struct page's might be accessed in
	 * vm_scan(). Shouldn't be a worry.
	 */
	while (i--) {
		PageClearSlab(page);
		page++;
	}
	free_pages((unsigned long)addr, cachep->gfporder);
}

/**
 * kmem_cache_create - Create a cache.
 * @name: A string which is used in /proc/slabinfo to identify this cache.
 * @size: The size of objects to be created in this cache.
 * @offset: The offset to use within the page.
 * @flags: SLAB flags
 * @ctor: A constructor for the objects.
 * @dtor: A destructor for the objects.
 *
 * Returns a ptr to the cache on success, NULL on failure.
 * Cannot be called within a int, but can be interrupted.
 * The @ctor is run when new pages are allocated by the cache
 * and the @dtor is run before the pages are handed back.
 * The flags are
 *
 * %SLAB_POISON - Poison the slab with a known test pattern (a5a5a5a5)
 * to catch references to uninitialised memory.
 *
 * %SLAB_RED_ZONE - Insert `Red' zones around the allocated memory to check
 * for buffer overruns.
 *
 * %SLAB_NO_REAP - Don't automatically reap this cache when we're under
 * memory pressure.
 *
 * %SLAB_HWCACHE_ALIGN - Align the objects in this cache to a hardware
 * cacheline.  This can be beneficial if you're counting cycles as closely
 * as davem.
 */
kmem_cache_t *
kmem_cache_create (const char *name, size_t size, size_t offset,
	unsigned long flags, void (*ctor)(void*, kmem_cache_t *, unsigned long),
	void (*dtor)(void*, kmem_cache_t *, unsigned long))
{
	const char *func_nm = "kmem_create: ";
	size_t left_over, align, slab_size;
	kmem_cache_t *cachep = NULL;

	/*
	 * Sanity checks... these are all serious usage bugs.
	 */
	if ((!name) ||
		((strlen(name) >= CACHE_NAMELEN - 1)) ||
		in_interrupt() ||
		(size < BYTES_PER_WORD) ||
		(size > (1<<MAX_OBJ_ORDER)*PAGE_SIZE) ||
		(dtor && !ctor) ||
		(offset < 0 || offset > size))
			BUG();

#if DEBUG
	if ((flags & SLAB_DEBUG_INITIAL) && !ctor) {
		/* No constructor, but inital state check requested */
		printk("%sNo con, but init state check requested - %s\n", func_nm, name);
		flags &= ~SLAB_DEBUG_INITIAL;
	}

	if ((flags & SLAB_POISON) && ctor) {
		/* request for poisoning, but we can't do that with a constructor */
		printk("%sPoisoning requested, but con given - %s\n", func_nm, name);
		flags &= ~SLAB_POISON;
	}
#if FORCED_DEBUG
	if ((size < (PAGE_SIZE>>3)) && !(flags & SLAB_MUST_HWCACHE_ALIGN))
		/*
		 * do not red zone large object, causes severe
		 * fragmentation.
		 */
		flags |= SLAB_RED_ZONE;
	if (!ctor)
		flags |= SLAB_POISON;
#endif
#endif

	/*
	 * Always checks flags, a caller might be expecting debug
	 * support which isn't available.
	 */
	// BUG_ON(flags & ~CREATE_MASK);

	/* Get cache's description obj. */
	cachep = (kmem_cache_t *) kmem_cache_alloc(&cache_cache, SLAB_KERNEL);
	if (!cachep)
		goto opps;
	memset(cachep, 0, sizeof(kmem_cache_t));

	/* Check that size is in terms of words.  This is needed to avoid
	 * unaligned accesses for some archs when redzoning is used, and makes
	 * sure any on-slab bufctl's are also correctly aligned.
	 */
	if (size & (BYTES_PER_WORD-1)) {
		size += (BYTES_PER_WORD-1);
		size &= ~(BYTES_PER_WORD-1);
		printk("%sForcing size word alignment - %s\n", func_nm, name);
	}
	
#if DEBUG
	if (flags & SLAB_RED_ZONE) {
		/*
		 * There is no point trying to honour cache alignment
		 * when redzoning.
		 */
		flags &= ~SLAB_HWCACHE_ALIGN;
		size += 2*BYTES_PER_WORD;	/* words for redzone */
	}
#endif
	align = BYTES_PER_WORD;
	if (flags & SLAB_HWCACHE_ALIGN)
		align = L1_CACHE_BYTES;

	/* Determine if the slab management is 'on' or 'off' slab. */
	if (size >= (PAGE_SIZE>>3))
		/*
		 * Size is large, assume best to place the slab management obj
		 * off-slab (should allow better packing of objs).
		 */
		flags |= CFLGS_OFF_SLAB;

	if (flags & SLAB_HWCACHE_ALIGN) {
		/* Need to adjust size so that objs are cache aligned. */
		/* Small obj size, can get at least two per cache line. */
		/* FIXME: only power of 2 supported, was better */
		while (size < align/2)
			align /= 2;
		size = (size+align-1)&(~(align-1));
	}

	/* Cal size (in pages) of slabs, and the num of objs per slab.
	 * This could be made much more intelligent.  For now, try to avoid
	 * using high page-orders for slabs.  When the gfp() funcs are more
	 * friendly towards high-order requests, this should be changed.
	 */
	do {
		unsigned int break_flag = 0;
cal_wastage:
		kmem_cache_estimate(cachep->gfporder, size, flags,
						&left_over, &cachep->num);
		if (break_flag)
			break;
		if (cachep->gfporder >= MAX_GFP_ORDER)
			break;
		if (!cachep->num)
			goto next;
		if (flags & CFLGS_OFF_SLAB && cachep->num > offslab_limit) {
			/* Oops, this num of objs will cause problems. */
			cachep->gfporder--;
			break_flag++;
			goto cal_wastage;
		}

		/*
		 * Large num of objs is good, but v. large slabs are currently
		 * bad for the gfp()s.
		 */
		if (cachep->gfporder >= slab_break_gfp_order)
			break;

		if ((left_over*8) <= (PAGE_SIZE<<cachep->gfporder))
			break;	/* Acceptable internal fragmentation. */
next:
		cachep->gfporder++;
	} while (1);

	if (!cachep->num) {
		printk("kmem_cache_create: couldn't create cache %s.\n", name);
		kmem_cache_free(&cache_cache, cachep);
		cachep = NULL;
		goto opps;
	}
	slab_size = L1_CACHE_ALIGN(cachep->num*sizeof(kmem_bufctl_t)+sizeof(slab_t));

	/*
	 * If the slab has been placed off-slab, and we have enough space then
	 * move it on-slab. This is at the expense of any extra colouring.
	 */
	if (flags & CFLGS_OFF_SLAB && left_over >= slab_size) {
		flags &= ~CFLGS_OFF_SLAB;
		left_over -= slab_size;
	}

	/* Offset must be a multiple of the alignment. */
	offset += (align-1);
	offset &= ~(align-1);
	if (!offset)
		offset = L1_CACHE_BYTES;
	cachep->colour_off = offset;
	cachep->colour = left_over/offset;

	/* init remaining fields */
	if (!cachep->gfporder && !(flags & CFLGS_OFF_SLAB))
		flags |= CFLGS_OPTIMIZE;

	cachep->flags = flags;
	cachep->gfpflags = 0;
	if (flags & SLAB_CACHE_DMA)
		cachep->gfpflags |= GFP_DMA;
	spin_lock_init(&cachep->spinlock);
	cachep->objsize = size;
	INIT_LIST_HEAD(&cachep->slabs_full);
	INIT_LIST_HEAD(&cachep->slabs_partial);
	INIT_LIST_HEAD(&cachep->slabs_free);

	if (flags & CFLGS_OFF_SLAB)
		cachep->slabp_cache = kmem_find_general_cachep(slab_size,0);
	cachep->ctor = ctor;
	cachep->dtor = dtor;
	/* Copy name over so we don't have problems with unloaded modules */
	strcpy(cachep->name, name);

#ifdef CONFIG_SMP
	if (g_cpucache_up)
		enable_cpucache(cachep);
#endif
	/* Need the semaphore to access the chain. */
	// down(&cache_chain_sem);
	{
		struct list_head *p;

		list_for_each(p, &cache_chain) {
			kmem_cache_t *pc = list_entry(p, kmem_cache_t, next);

			/* The name field is constant - no lock needed. */
			if (!strcmp(pc->name, name))
				BUG();
		}
	}

	/* There is no reason to lock our new cache before we
	 * link it in - no one knows about it yet...
	 */
	list_add(&cachep->next, &cache_chain);
	// up(&cache_chain_sem);
opps:
	return cachep;
}

/* Get the memory for a slab management obj. */
static inline slab_t * kmem_cache_slabmgmt (kmem_cache_t *cachep,
			void *objp, int colour_off, int local_flags)
{
	slab_t *slabp;
	
	if (OFF_SLAB(cachep)) {
		/* Slab management obj is off-slab. */
		slabp = kmem_cache_alloc(cachep->slabp_cache, local_flags);
		if (!slabp)
			return NULL;
	} else {
		/* FIXME: change to
			slabp = objp
		 * if you enable OPTIMIZE
		 */
		slabp = objp+colour_off;
		colour_off += L1_CACHE_ALIGN(cachep->num *
				sizeof(kmem_bufctl_t) + sizeof(slab_t));
	}
	slabp->inuse = 0;
	slabp->colouroff = colour_off;
	slabp->s_mem = objp+colour_off;

	return slabp;
}

static inline void kmem_cache_init_objs (kmem_cache_t * cachep,
			slab_t * slabp, unsigned long ctor_flags)
{
	int i;

	for (i = 0; i < cachep->num; i++) {
		void* objp = slabp->s_mem+cachep->objsize*i;
#if DEBUG
		if (cachep->flags & SLAB_RED_ZONE) {
			*((unsigned long*)(objp)) = RED_MAGIC1;
			*((unsigned long*)(objp + cachep->objsize -
					BYTES_PER_WORD)) = RED_MAGIC1;
			objp += BYTES_PER_WORD;
		}
#endif

		/*
		 * Constructors are not allowed to allocate memory from
		 * the same cache which they are a constructor for.
		 * Otherwise, deadlock. They must also be threaded.
		 */
		if (cachep->ctor)
			cachep->ctor(objp, cachep, ctor_flags);
#if DEBUG
		if (cachep->flags & SLAB_RED_ZONE)
			objp -= BYTES_PER_WORD;
		if (cachep->flags & SLAB_POISON)
			/* need to poison the objs */
			kmem_poison_obj(cachep, objp);
		if (cachep->flags & SLAB_RED_ZONE) {
			if (*((unsigned long*)(objp)) != RED_MAGIC1)
				BUG();
			if (*((unsigned long*)(objp + cachep->objsize -
					BYTES_PER_WORD)) != RED_MAGIC1)
				BUG();
		}
#endif
		slab_bufctl(slabp)[i] = i+1;
	}
	slab_bufctl(slabp)[i-1] = BUFCTL_END;
	slabp->free = 0;
}

/*
 * Grow (by 1) the number of slabs within a cache.  This is called by
 * kmem_cache_alloc() when there are no active objs left in a cache.
 */
static int kmem_cache_grow (kmem_cache_t * cachep, int flags)
{
	slab_t		*slabp;
	struct page	*page;
	void		*objp;
	size_t		 offset;
	unsigned int	 i, local_flags;
	unsigned long	 ctor_flags;
	unsigned long	 save_flags;

	/* Be lazy and only check for valid flags here,
 	 * keeping it out of the critical path in kmem_cache_alloc().
	 */
	if (flags & ~(SLAB_DMA|SLAB_LEVEL_MASK|SLAB_NO_GROW))
		BUG();
	if (flags & SLAB_NO_GROW)
		return 0;

	/*
	 * The test for missing atomic flag is performed here, rather than
	 * the more obvious place, simply to reduce the critical path length
	 * in kmem_cache_alloc(). If a caller is seriously mis-behaving they
	 * will eventually be caught here (where it matters).
	 */
	if (in_interrupt() && (flags & SLAB_LEVEL_MASK) != SLAB_ATOMIC)
		BUG();

	ctor_flags = SLAB_CTOR_CONSTRUCTOR;
	local_flags = (flags & SLAB_LEVEL_MASK);
	if (local_flags == SLAB_ATOMIC)
		/*
		 * Not allowed to sleep.  Need to tell a constructor about
		 * this - it might need to know...
		 */
		ctor_flags |= SLAB_CTOR_ATOMIC;

	/* About to mess with non-constant members - lock. */
	spin_lock_irqsave(&cachep->spinlock, save_flags);

	/* Get colour for the slab, and cal the next value. */
	offset = cachep->colour_next;
	cachep->colour_next++;
	if (cachep->colour_next >= cachep->colour)
		cachep->colour_next = 0;
	offset *= cachep->colour_off;
	cachep->dflags |= DFLGS_GROWN;

	cachep->growing++;
	spin_unlock_irqrestore(&cachep->spinlock, save_flags);

	/* A series of memory allocations for a new slab.
	 * Neither the cache-chain semaphore, or cache-lock, are
	 * held, but the incrementing c_growing prevents this
	 * cache from being reaped or shrunk.
	 * Note: The cache could be selected in for reaping in
	 * kmem_cache_reap(), but when the final test is made the
	 * growing value will be seen.
	 */

	/* Get mem for the objs. */
	if (!(objp = kmem_getpages(cachep, flags)))
		goto failed;

	/* Get slab management. */
	if (!(slabp = kmem_cache_slabmgmt(cachep, objp, offset, local_flags)))
		goto opps1;

	/* Nasty!!!!!! I hope this is OK. */
	i = 1 << cachep->gfporder;
	page = virt_to_page(objp);
	do {
		SET_PAGE_CACHE(page, cachep);
		SET_PAGE_SLAB(page, slabp);
		PageSetSlab(page);
		page++;
	} while (--i);

	kmem_cache_init_objs(cachep, slabp, ctor_flags);

	spin_lock_irqsave(&cachep->spinlock, save_flags);
	cachep->growing--;

	/* Make slab active. */
	list_add_tail(&slabp->list, &cachep->slabs_free);
	STATS_INC_GROWN(cachep);
	cachep->failures = 0;

	spin_unlock_irqrestore(&cachep->spinlock, save_flags);
	return 1;
opps1:
	kmem_freepages(cachep, objp);
failed:
	spin_lock_irqsave(&cachep->spinlock, save_flags);
	cachep->growing--;
	spin_unlock_irqrestore(&cachep->spinlock, save_flags);
	return 0;
}

static inline void kmem_cache_alloc_head(kmem_cache_t *cachep, int flags)
{
	if (flags & SLAB_DMA) {
		if (!(cachep->gfpflags & GFP_DMA))
			BUG();
	} else {
		if (cachep->gfpflags & GFP_DMA)
			BUG();
	}
}

static inline void * kmem_cache_alloc_one_tail (kmem_cache_t *cachep,
						slab_t *slabp)
{
	void *objp;

	STATS_INC_ALLOCED(cachep);
	STATS_INC_ACTIVE(cachep);
	STATS_SET_HIGH(cachep);

	/* get obj pointer */
	slabp->inuse++;
	objp = slabp->s_mem + slabp->free*cachep->objsize;
	slabp->free=slab_bufctl(slabp)[slabp->free];

	if (unlikely(slabp->free == BUFCTL_END)) {
		list_del(&slabp->list);
		list_add(&slabp->list, &cachep->slabs_full);
	}
#if DEBUG
	if (cachep->flags & SLAB_POISON)
		if (kmem_check_poison_obj(cachep, objp))
			BUG();
	if (cachep->flags & SLAB_RED_ZONE) {
		/* Set alloc red-zone, and check old one. */
		if (xchg((unsigned long *)objp, RED_MAGIC2) !=
							 RED_MAGIC1)
			BUG();
		if (xchg((unsigned long *)(objp+cachep->objsize -
			  BYTES_PER_WORD), RED_MAGIC2) != RED_MAGIC1)
			BUG();
		objp += BYTES_PER_WORD;
	}
#endif
	return objp;
}

/*
 * Returns a ptr to an obj in the given cache.
 * caller must guarantee synchronization
 * #define for the goto optimization 8-)
 */
#define kmem_cache_alloc_one(cachep)				\
({								\
	struct list_head * slabs_partial, * entry;		\
	slab_t *slabp;						\
								\
	slabs_partial = &(cachep)->slabs_partial;		\
	entry = slabs_partial->next;				\
	if (unlikely(entry == slabs_partial)) {			\
		struct list_head * slabs_free;			\
		slabs_free = &(cachep)->slabs_free;		\
		entry = slabs_free->next;			\
		if (unlikely(entry == slabs_free))		\
			goto alloc_new_slab;			\
		list_del(entry);				\
		list_add(entry, slabs_partial);			\
	}							\
								\
	slabp = list_entry(entry, slab_t, list);		\
	kmem_cache_alloc_one_tail(cachep, slabp);		\
})

static inline void * __kmem_cache_alloc (kmem_cache_t *cachep, int flags)
{
	unsigned long save_flags;
	void* objp;

	kmem_cache_alloc_head(cachep, flags);
try_again:
	local_irq_save(save_flags);
#ifdef CONFIG_SMP
	{
		cpucache_t *cc = cc_data(cachep);

		if (cc) {
			if (cc->avail) {
				STATS_INC_ALLOCHIT(cachep);
				objp = cc_entry(cc)[--cc->avail];
			} else {
				STATS_INC_ALLOCMISS(cachep);
				objp = kmem_cache_alloc_batch(cachep,cc,flags);
				if (!objp)
					goto alloc_new_slab_nolock;
			}
		} else {
			spin_lock(&cachep->spinlock);
			objp = kmem_cache_alloc_one(cachep);
			spin_unlock(&cachep->spinlock);
		}
	}
#else
	objp = kmem_cache_alloc_one(cachep);
#endif
	local_irq_restore(save_flags);
	return objp;
alloc_new_slab:
#ifdef CONFIG_SMP
	spin_unlock(&cachep->spinlock);
alloc_new_slab_nolock:
#endif
	local_irq_restore(save_flags);
	if (kmem_cache_grow(cachep, flags))
		/* Someone may have stolen our objs.  Doesn't matter, we'll
		 * just come back here again.
		 */
		goto try_again;
	return NULL;
}

/*
 * Release an obj back to its cache. If the obj has a constructed
 * state, it should be in this state _before_ it is released.
 * - caller is responsible for the synchronization
 */

#if DEBUG
# define CHECK_NR(pg)						\
	do {							\
		if (!VALID_PAGE(pg)) {				\
			printk(KERN_ERR "kfree: out of range ptr %lxh.\n", \
				(unsigned long)objp);		\
			BUG();					\
		} \
	} while (0)
# define CHECK_PAGE(page)					\
	do {							\
		CHECK_NR(page);					\
		if (!PageSlab(page)) {				\
			printk(KERN_ERR "kfree: bad ptr %lxh.\n", \
				(unsigned long)objp);		\
			BUG();					\
		}						\
	} while (0)

#else
# define CHECK_PAGE(pg)	do { } while (0)
#endif

static inline void kmem_cache_free_one(kmem_cache_t *cachep, void *objp)
{
	slab_t* slabp;

	CHECK_PAGE(virt_to_page(objp));
	/* reduces memory footprint
	 *
	if (OPTIMIZE(cachep))
		slabp = (void*)((unsigned long)objp&(~(PAGE_SIZE-1)));
	 else
	 */
	slabp = GET_PAGE_SLAB(virt_to_page(objp));

#if DEBUG
	if (cachep->flags & SLAB_DEBUG_INITIAL)
		/* Need to call the slab's constructor so the
		 * caller can perform a verify of its state (debugging).
		 * Called without the cache-lock held.
		 */
		cachep->ctor(objp, cachep, SLAB_CTOR_CONSTRUCTOR|SLAB_CTOR_VERIFY);

	if (cachep->flags & SLAB_RED_ZONE) {
		objp -= BYTES_PER_WORD;
		if (xchg((unsigned long *)objp, RED_MAGIC1) != RED_MAGIC2)
			/* Either write before start, or a double free. */
			BUG();
		if (xchg((unsigned long *)(objp+cachep->objsize -
				BYTES_PER_WORD), RED_MAGIC1) != RED_MAGIC2)
			/* Either write past end, or a double free. */
			BUG();
	}
	if (cachep->flags & SLAB_POISON)
		kmem_poison_obj(cachep, objp);
	if (kmem_extra_free_checks(cachep, slabp, objp))
		return;
#endif
	{
		unsigned int objnr = (objp-slabp->s_mem)/cachep->objsize;

		slab_bufctl(slabp)[objnr] = slabp->free;
		slabp->free = objnr;
	}
	STATS_DEC_ACTIVE(cachep);
	
	/* fixup slab chains */
	{
		int inuse = slabp->inuse;
		if (unlikely(!--slabp->inuse)) {
			/* Was partial or full, now empty. */
			list_del(&slabp->list);
			list_add(&slabp->list, &cachep->slabs_free);
		} else if (unlikely(inuse == cachep->num)) {
			/* Was full. */
			list_del(&slabp->list);
			list_add(&slabp->list, &cachep->slabs_partial);
		}
	}
}

/*
 * __kmem_cache_free
 * called with disabled ints
 */
static inline void __kmem_cache_free (kmem_cache_t *cachep, void* objp)
{
#ifdef CONFIG_SMP
	cpucache_t *cc = cc_data(cachep);

	CHECK_PAGE(virt_to_page(objp));
	if (cc) {
		int batchcount;
		if (cc->avail < cc->limit) {
			STATS_INC_FREEHIT(cachep);
			cc_entry(cc)[cc->avail++] = objp;
			return;
		}
		STATS_INC_FREEMISS(cachep);
		batchcount = cachep->batchcount;
		cc->avail -= batchcount;
		free_block(cachep,
					&cc_entry(cc)[cc->avail],batchcount);
		cc_entry(cc)[cc->avail++] = objp;
		return;
	} else {
		free_block(cachep, &objp, 1);
	}
#else
	kmem_cache_free_one(cachep, objp);
#endif
}

/**
 * kmem_cache_alloc - Allocate an object
 * @cachep: The cache to allocate from.
 * @flags: See kmalloc().
 *
 * Allocate an object from this cache.  The flags are only relevant
 * if the cache has no available objects.
 */
void * kmem_cache_alloc (kmem_cache_t *cachep, int flags)
{
	return __kmem_cache_alloc(cachep, flags);
}

/**
 * kmem_cache_free - Deallocate an object
 * @cachep: The cache the allocation was from.
 * @objp: The previously allocated object.
 *
 * Free an object which was previously allocated from this
 * cache.
 */
void kmem_cache_free (kmem_cache_t *cachep, void *objp)
{
	unsigned long flags;
#if DEBUG
	CHECK_PAGE(virt_to_page(objp));
	if (cachep != GET_PAGE_CACHE(virt_to_page(objp)))
		BUG();
#endif

	local_irq_save(flags);
	__kmem_cache_free(cachep, objp);
	local_irq_restore(flags);
}

kmem_cache_t * kmem_find_general_cachep (size_t size, int gfpflags)
{
	cache_sizes_t *csizep = cache_sizes;

	/* This function could be moved to the header file, and
	 * made inline so consumers can quickly determine what
	 * cache pointer they require.
	 */
	for ( ; csizep->cs_size; csizep++) {
		if (size > csizep->cs_size)
			continue;
		break;
	}
	return (gfpflags & GFP_DMA) ? csizep->cs_dmacachep : csizep->cs_cachep;
}
