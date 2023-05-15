#ifndef _I386_SEMAPHORE_H
#define _I386_SEMAPHORE_H

#include <asm/atomic.h>
#include <linux/wait.h>

struct semaphore {
	atomic_t count;
	int sleepers;
	wait_queue_head_t wait;
};

static inline void sema_init (struct semaphore *sem, int val)
{
/*
 *	*sem = (struct semaphore)__SEMAPHORE_INITIALIZER((*sem),val);
 *
 * i'd rather use the more flexible initialization above, but sadly
 * GCC 2.7.2.3 emits a bogus warning. EGCS doesnt. Oh well.
 */
	atomic_set(&sem->count, val);
	sem->sleepers = 0;
	init_waitqueue_head(&sem->wait);
#if WAITQUEUE_DEBUG
	sem->__magic = (int)&sem->__magic;
#endif
}

static inline void init_MUTEX (struct semaphore *sem)
{
	sema_init(sem, 1);
}

static inline void init_MUTEX_LOCKED (struct semaphore *sem)
{
	sema_init(sem, 0);
}

void __down_failed(void);

#endif /* _LINUX_SEMAPHORE_H */
