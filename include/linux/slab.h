#ifndef _LINUX_SLAB_H
#define _LINUX_SLAB_H

#include <asm-i386/types.h>

typedef struct kmem_cache_s kmem_cache_t;

/* flags to pass to kmem_cache_create().
 * The first 3 are only valid when the allocator as been build
 * SLAB_DEBUG_SUPPORT.
 */
#define	SLAB_DEBUG_FREE			0x00000100UL	/* Peform (expensive) checks on free */
#define	SLAB_DEBUG_INITIAL		0x00000200UL	/* Call constructor (as verifier) */
#define	SLAB_RED_ZONE			0x00000400UL	/* Red zone objs in a cache */
#define	SLAB_POISON				0x00000800UL	/* Poison objects */
#define	SLAB_NO_REAP			0x00001000UL	/* never reap from the cache */
#define	SLAB_HWCACHE_ALIGN		0x00002000UL	/* align objs on a h/w cache lines */
#define SLAB_CACHE_DMA			0x00004000UL	/* use GFP_DMA memory */
#define SLAB_MUST_HWCACHE_ALIGN	0x00008000UL	/* force alignment */

/* flags passed to a constructor func */
#define	SLAB_CTOR_CONSTRUCTOR	0x001UL		/* if not set, then deconstructor */
#define SLAB_CTOR_ATOMIC		0x002UL		/* tell constructor it can't sleep */
#define	SLAB_CTOR_VERIFY		0x004UL		/* tell constructor it's a verify call */

/* prototypes */
extern void kmem_cache_init(void);
extern void kmem_cache_sizes_init(void);

extern kmem_cache_t *kmem_cache_create(const char *, size_t, size_t, unsigned long,
				       void (*)(void *, kmem_cache_t *, unsigned long),
				       void (*)(void *, kmem_cache_t *, unsigned long));
extern void *kmem_cache_alloc(kmem_cache_t *, int);

#endif /* _LINUX_SLAB_H */
