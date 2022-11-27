#include <linux/fs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <asm-i386/stdio.h>
#include <linux/string.h>
#include <asm-i386/page.h>

/*
 * Inode lookup is no longer as critical as it used to be:
 * most of the lookups are going to be through the dcache.
 */
#define I_HASHBITS	i_hash_shift
#define I_HASHMASK	i_hash_mask

static unsigned int i_hash_mask;
static unsigned int i_hash_shift;

/*
 * Each inode can be on two separate lists. One is
 * the hash list of the inode, used for lookups. The
 * other linked list is the "type" list:
 *  "in_use" - valid inode, i_count > 0, i_nlink > 0
 *  "dirty"  - as "in_use" but also dirty
 *  "unused" - valid inode, i_count = 0
 *
 * A "dirty" list is maintained for each super block,
 * allowing for low-overhead inode sync() operations.
 */

static LIST_HEAD(inode_in_use);
static LIST_HEAD(inode_unused);
static struct list_head *inode_hashtable;
static LIST_HEAD(anon_hash_chain); /* for inodes with NULL i_sb */

/*
 * A simple spinlock to protect the list manipulations.
 *
 * NOTE! You also have to own the lock if you change
 * the i_state of an inode while it is in use..
 */
spinlock_t inode_lock = SPIN_LOCK_UNLOCKED;

/*
 * Statistics gathering..
 */
struct {
	int nr_inodes;
	int nr_unused;
	int dummy[5];
} inodes_stat;

static kmem_cache_t * inode_cachep;

#define alloc_inode() \
	 ((struct inode *) kmem_cache_alloc(inode_cachep, SLAB_KERNEL))

/*
 * These are initializations that only need to be done
 * once, because the fields are idempotent across use
 * of the inode, so let the slab aware of that.
 */
static void init_once(void * foo, kmem_cache_t * cachep, unsigned long flags)
{
	struct inode * inode = (struct inode *) foo;

	if ((flags & (SLAB_CTOR_VERIFY|SLAB_CTOR_CONSTRUCTOR)) ==
	    SLAB_CTOR_CONSTRUCTOR)
	{
		memset(inode, 0, sizeof(*inode));
		init_waitqueue_head(&inode->i_wait);
		INIT_LIST_HEAD(&inode->i_hash);
		// INIT_LIST_HEAD(&inode->i_data.clean_pages);
		// INIT_LIST_HEAD(&inode->i_data.dirty_pages);
		// INIT_LIST_HEAD(&inode->i_data.locked_pages);
		INIT_LIST_HEAD(&inode->i_dentry);
		INIT_LIST_HEAD(&inode->i_dirty_buffers);
		sema_init(&inode->i_sem, 1);
		sema_init(&inode->i_zombie, 1);
		spin_lock_init(&inode->i_data.i_shared_lock);
	}
}

/*
 * Initialize the hash tables.
 */
void inode_init(unsigned long mempages)
{
	struct list_head *head;
	unsigned long order;
	unsigned int nr_hash;
	int i;

	mempages >>= (14 - PAGE_SHIFT);
	mempages *= sizeof(struct list_head);
	for (order = 0; ((1UL << order) << PAGE_SHIFT) < mempages; order++)
		;

	do {
		unsigned long tmp;

		nr_hash = (1UL << order) * PAGE_SIZE /
			sizeof(struct list_head);
		i_hash_mask = (nr_hash - 1);

		tmp = nr_hash;
		i_hash_shift = 0;
		while ((tmp >>= 1UL) != 0UL)
			i_hash_shift++;

		inode_hashtable = (struct list_head *)
			__get_free_pages(GFP_ATOMIC, order);
	} while (inode_hashtable == NULL && --order >= 0);

	printk("Inode-cache hash table entries: %d (order: %ld, %ld bytes)\n",
			nr_hash, order, (PAGE_SIZE << order));

	if (!inode_hashtable)
		printk("Failed to allocate inode hash table\n");

	head = inode_hashtable;
	i = nr_hash;
	do {
		INIT_LIST_HEAD(head);
		head++;
		i--;
	} while (i);

	/* inode slab cache */
	inode_cachep = kmem_cache_create("inode_cache", sizeof(struct inode),
					 0, SLAB_HWCACHE_ALIGN, init_once,
					 NULL);
	if (!inode_cachep)
		printk("cannot create inode slab cache");
}
