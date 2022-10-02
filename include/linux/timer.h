#ifndef _LINUX_TIMER_H
#define _LINUX_TIMER_H

#include <linux/list.h>

struct timer_list {
	struct list_head list;
	unsigned long expires;   // 到点时间
	unsigned long data;      // function函数的参数
	void (*function)(unsigned long);   // 预定在到点时执行的bh函数
};

static inline int timer_pending (const struct timer_list * timer)
{
	return timer->list.next != NULL;
}

#endif /* _LINUX_TIMER_H */
