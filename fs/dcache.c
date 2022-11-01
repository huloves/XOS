#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <asm-i386/stdio.h>
#include <linux/limits.h>

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
}
