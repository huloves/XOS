#include <asm/smplock.h>
#include <linux/spinlock.h>
#include <linux/debug.h>

/* The 'big kernel lock' */
spinlock_t kernel_flag = SPIN_LOCK_UNLOCKED;
