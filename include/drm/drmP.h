#ifndef _DRM_DRMP_H
#define _DRM_DRMP_H

#include <drm/drm.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <drm/drm_hashtab.h>

/**
 * DRM device structure. This structure represent a complete card that
 * may contain multiple heads.
 */
struct drm_device {
	struct list_head driver_item;   /* 每个驱动程序的设备列表 */
	char *devname;

	spinlock_t count_lock;
	struct mutex strut_mutex;

	/** \name Usage Counters */
	/*@{ */
	int open_count;			/**< Outstanding files open */
	atomic_t ioctl_count;		/**< Outstanding IOCTLs pending */
	atomic_t vma_count;		/**< Outstanding vma areas open */
	int buf_use;			/**< Buffers in use -- cannot alloc */
	atomic_t buf_alloc;		/**< Buffer allocation in progress */
	/*@} */

	/** \name Performance counters */
	/*@{ */
	unsigned long counters;
	enum drm_stat_type types[15];
	atomic_t counts[15];
	/*@} */

	struct list_head filelist;

	/** \name Memory management */
	/*@{ */
	struct list_head maplist;	/**< Linked list of regions */
	int map_count;			/**< Number of mappable regions */
	struct drm_open_hash map_hash;	/**< User token hash table for maps */

	/** \name Context handle management */
	/*@{ */
	struct list_head ctxlist;	/**< Linked list of context handles */
	int ctx_count;			/**< Number of context handles */
	struct mutex ctxlist_mutex;	/**< For ctxlist */

	// struct idr ctx_idr;

	struct list_head vmalist;	/**< List of vmas (for debugging) */

	/** \name DMA queues (contexts) */
	/*@{ */
	int queue_count;		/**< Number of active DMA queues */
	int queue_reserved;		  /**< Number of reserved DMA queues */
	int queue_slots;		/**< Actual length of queuelist */
	// struct drm_queue **queuelist;	/**< Vector of pointers to DMA queues */
	// struct drm_device_dma *dma;		/**< Optional pointer for DMA support */
};

#endif /* _DRM_DRMP_H */
