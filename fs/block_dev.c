#include <linux/fs.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <asm-i386/stdio.h>
#include <linux/string.h>

#define HASH_BITS 6
#define HASH_SIZE (1UL << HASH_BITS)
#define HASH_MASK (HASH_SIZE - 1)
static struct list_head bdev_hashtable[HASH_SIZE];
static spinlock_t bdev_lock = SPIN_LOCK_UNLOCKED;
static kmem_cache_t * bdev_cachep;

#define alloc_bdev() \
	 ((struct block_device *) kmem_cache_alloc(bdev_cachep, SLAB_KERNEL))
#define destroy_bdev(bdev) kmem_cache_free(bdev_cachep, (bdev))

static void init_once(void * foo, kmem_cache_t * cachep, unsigned long flags)
{
	struct block_device * bdev = (struct block_device *) foo;

	if ((flags & (SLAB_CTOR_VERIFY|SLAB_CTOR_CONSTRUCTOR)) ==
	    SLAB_CTOR_CONSTRUCTOR)
	{
		memset(bdev, 0, sizeof(*bdev));
		sema_init(&bdev->bd_sem, 1);
	}
}

void bdev_init(void)
{
	int i;
	struct list_head *head = bdev_hashtable;

	i = HASH_SIZE;
	do {
		INIT_LIST_HEAD(head);
		head++;
		i--;
	} while (i);

	bdev_cachep = kmem_cache_create("bdev_cache",
					 sizeof(struct block_device),
					 0, SLAB_HWCACHE_ALIGN, init_once,
					 NULL);
	if (!bdev_cachep) {
		printk("Cannot create bdev_cache SLAB cache");
		while(1);
	}
}
