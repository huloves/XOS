#ifndef _DRM_DRMP_H
#define _DRM_DRMP_H

#include <drm/drm.h>
#include <linux/list.h>
#include <linux/mutex.h>

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
};

#endif /* _DRM_DRMP_H */
