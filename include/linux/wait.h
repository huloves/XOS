#ifndef _LINUX_WAIT_H
#define _LINUX_WAIT_H

#include <linux/sched.h>
#include <linux/list.h>

struct __wait_queue {
    unsigned int flags;
#define WQ_FLAG_EXCLUSIVE	0x01
    struct task_struct * task;
    struct list_head task_list;
};
typedef struct __wait_queue wait_queue_t;

/*
 * 'dual' spinlock architecture. Can be switched between spinlock_t and
 * rwlock_t locks via changing this define. Since waitqueues are quite
 * decoupled in the new architecture, lightweight 'simple' spinlocks give
 * us slightly better latencies and smaller waitqueue structure size.
 */
#define USE_RW_WAIT_QUEUE_SPINLOCK 0

#if USE_RW_WAIT_QUEUE_SPINLOCK
# define wq_lock_t rwlock_t
# define WAITQUEUE_RW_LOCK_UNLOCKED RW_LOCK_UNLOCKED

# define wq_read_lock read_lock
# define wq_read_lock_irqsave read_lock_irqsave
# define wq_read_unlock_irqrestore read_unlock_irqrestore
# define wq_read_unlock read_unlock
# define wq_write_lock_irq write_lock_irq
# define wq_write_lock_irqsave write_lock_irqsave
# define wq_write_unlock_irqrestore write_unlock_irqrestore
# define wq_write_unlock write_unlock
#else
# define wq_lock_t spinlock_t
# define WAITQUEUE_RW_LOCK_UNLOCKED SPIN_LOCK_UNLOCKED

# define wq_read_lock spin_lock
# define wq_read_lock_irqsave spin_lock_irqsave
# define wq_read_unlock spin_unlock
# define wq_read_unlock_irqrestore spin_unlock_irqrestore
# define wq_write_lock_irq spin_lock_irq
# define wq_write_lock_irqsave spin_lock_irqsave
# define wq_write_unlock_irqrestore spin_unlock_irqrestore
# define wq_write_unlock spin_unlock
#endif

struct __wait_queue_head {
    wq_lock_t lock;
    struct list_head task_list;
};
typedef struct __wait_queue_head wait_queue_head_t;

static inline void init_waitqueue_head(wait_queue_head_t *q)
{
	q->lock = WAITQUEUE_RW_LOCK_UNLOCKED;
	INIT_LIST_HEAD(&q->task_list);
}

static inline void init_waitqueue_entry(wait_queue_t *q, struct task_struct *p)
{
#if WAITQUEUE_DEBUG
	if (!q || !p)
		WQ_BUG();
#endif
	q->flags = 0;
	q->task = p;
#if WAITQUEUE_DEBUG
	q->__magic = (long)&q->__magic;
#endif
}

static inline int waitqueue_active(wait_queue_head_t *q)
{
#if WAITQUEUE_DEBUG
	if (!q)
		WQ_BUG();
	CHECK_MAGIC_WQHEAD(q);
#endif

	return !list_empty(&q->task_list);
}

static inline void __add_wait_queue(wait_queue_head_t *head, wait_queue_t *new)
{
#if WAITQUEUE_DEBUG
	if (!head || !new)
		WQ_BUG();
	CHECK_MAGIC_WQHEAD(head);
	CHECK_MAGIC(new->__magic);
	if (!head->task_list.next || !head->task_list.prev)
		WQ_BUG();
#endif
	list_add(&new->task_list, &head->task_list);
}

/*
 * Used for wake-one threads:
 */
static inline void __add_wait_queue_tail(wait_queue_head_t *head,
						wait_queue_t *new)
{
#if WAITQUEUE_DEBUG
	if (!head || !new)
		WQ_BUG();
	CHECK_MAGIC_WQHEAD(head);
	CHECK_MAGIC(new->__magic);
	if (!head->task_list.next || !head->task_list.prev)
		WQ_BUG();
#endif
	list_add_tail(&new->task_list, &head->task_list);
}

static inline void __remove_wait_queue(wait_queue_head_t *head,
							wait_queue_t *old)
{
#if WAITQUEUE_DEBUG
	if (!old)
		WQ_BUG();
	CHECK_MAGIC(old->__magic);
#endif
	list_del(&old->task_list);
}

#endif /* _LINUX_WAIT_H */
