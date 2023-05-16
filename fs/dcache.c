#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <asm/stdio.h>
#include <linux/limits.h>
#include <linux/debug.h>
#include <asm/page.h>

#define DCACHE_PARANOIA 1
/* #define DCACHE_DEBUG 1 */

spinlock_t dcache_lock = SPIN_LOCK_UNLOCKED;

/* Right now the dcache depends on the kernel lock */
#define check_lock()	if (!kernel_locked()) BUG()

static kmem_cache_t *dentry_cache;

/*
 * This is the single most critical data structure when it comes
 * to the dcache: the hashtable for lookups. Somebody should try
 * to make this good - I've just made it work.
 *
 * This hash-function tries to avoid losing too many bits of hash
 * information, yet avoid using a prime hash-size or similar.
 */
#define D_HASHBITS     d_hash_shift
#define D_HASHMASK     d_hash_mask

static unsigned int d_hash_mask;
static unsigned int d_hash_shift;
static struct list_head *dentry_hashtable;
static LIST_HEAD(dentry_unused);

struct {
	int nr_dentry;
	int nr_unused;
	int age_limit;		/* age in seconds */
	int want_pages;		/* pages requested by system */
	int dummy[2];
} dentry_stat = {0, 0, 45, 0,};

static void init_buffer_head(void * foo, kmem_cache_t * cachep, unsigned long flags)
{
	if ((flags & (SLAB_CTOR_VERIFY|SLAB_CTOR_CONSTRUCTOR)) ==
	    SLAB_CTOR_CONSTRUCTOR)
	{
		struct buffer_head * bh = (struct buffer_head *) foo;

		memset(bh, 0, sizeof(*bh));
		init_waitqueue_head(&bh->b_wait);
	}
}

static void dcache_init(unsigned long mempages)
{
	struct list_head *d;
	unsigned long order;
	unsigned int nr_hash;
	int i;

	/* 
	 * A constructor could be added for stable state like the lists,
	 * but it is probably not worth it because of the cache nature
	 * of the dcache. 
	 * If fragmentation is too bad then the SLAB_HWCACHE_ALIGN
	 * flag could be removed here, to hint to the allocator that
	 * it should not try to get multiple page regions.  
	 */
	dentry_cache = kmem_cache_create("dentry_cache",
					 sizeof(struct dentry),
					 0,
					 SLAB_HWCACHE_ALIGN,
					 NULL, NULL);
	if (!dentry_cache)
		printk("Cannot create dentry cache");

#if PAGE_SHIFT < 13
	mempages >>= (13 - PAGE_SHIFT);
#endif
	mempages *= sizeof(struct list_head);
	for (order = 0; ((1UL << order) << PAGE_SHIFT) < mempages; order++)
		;

	do {
		unsigned long tmp;

		nr_hash = (1UL << order) * PAGE_SIZE /
			sizeof(struct list_head);
		d_hash_mask = (nr_hash - 1);

		tmp = nr_hash;
		d_hash_shift = 0;
		while ((tmp >>= 1UL) != 0UL)
			d_hash_shift++;

		dentry_hashtable = (struct list_head *)
			__get_free_pages(GFP_ATOMIC, order);
	} while (dentry_hashtable == NULL && --order >= 0);

	printk("Dentry-cache hash table entries: %d (order: %ld, %ld bytes)\n",
			nr_hash, order, (PAGE_SIZE << order));

	if (!dentry_hashtable)
		printk("Failed to allocate dcache hash table\n");

	d = dentry_hashtable;
	i = nr_hash;
	do {
		INIT_LIST_HEAD(d);
		d++;
		i--;
	} while (i);
}

/* SLAB cache for __getname() consumers */
kmem_cache_t *names_cachep;

/* SLAB cache for file structures */
kmem_cache_t *filp_cachep;

/* SLAB cache for dquot structures */
kmem_cache_t *dquot_cachep;

/* SLAB cache for buffer_head structures */
kmem_cache_t *bh_cachep;

void vfs_caches_init(unsigned long mempages)
{
	bh_cachep = kmem_cache_create("buffer_head",
			sizeof(struct buffer_head), 0,
			SLAB_HWCACHE_ALIGN, init_buffer_head, NULL);
	if(!bh_cachep)
		printk("Cannot create buffer head SLAB cache");
	
	names_cachep = kmem_cache_create("names_cache", 
			PATH_MAX, 0, 
			SLAB_HWCACHE_ALIGN, NULL, NULL);
	if (!names_cachep)
		printk("Cannot create names SLAB cache");
	
	// filp_cachep = kmem_cache_create("filp", 
	// 		sizeof(struct file), 0,
	// 		SLAB_HWCACHE_ALIGN, NULL, NULL);
	// if(!filp_cachep)
	// 	panic("Cannot create filp SLAB cache");
	dcache_init(mempages);
}
