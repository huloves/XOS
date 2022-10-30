#include <linux/sched.h>

struct user_struct root_user = {
	__count:	ATOMIC_INIT(1),
	processes:	ATOMIC_INIT(1),
	files:		ATOMIC_INIT(0)
};
