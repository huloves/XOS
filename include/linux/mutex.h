#ifndef _LINUX_MUTEX_H
#define _LINUX_MUTEX_H

#include <linux/spinlock.h>
#include <linux/list.h>

struct mutex {
	/* 1: unlocked, 0: locked, negative: locked, possible waiters */
	atomic_t		count;
	spinlock_t		wait_lock;
	struct list_head	wait_list;
};

#endif /* _LINUX_MUTEX_H */
