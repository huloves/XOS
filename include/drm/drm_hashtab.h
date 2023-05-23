#ifndef _DRM_DRM_HASHTAB_H
#define _DRM_DRM_HASHTAB_H

#include <linux/list.h>

struct drm_hash_item {
	struct hlist_node head;
	unsigned long key;
};

struct drm_open_hash {
	unsigned int size;
	unsigned int order;
	unsigned int fill;
	struct hlist_head *table;
	int use_vmalloc;
};

#endif /* _DRM_DRM_HASHTAB_H */
