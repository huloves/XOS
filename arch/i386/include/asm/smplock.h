/*
 * <asm/smplock.h>
 *
 * i386 SMP lock implementation
 */
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/debug.h>
#include <asm/hardirq.h>

extern spinlock_t kernel_flag;

#define kernel_locked() 1

#define lock_kernel() do { } while(0)

#define unlock_kernel()				do { } while(0)
#define release_kernel_lock(task, cpu)		do { } while(0)
#define reacquire_kernel_lock(task)		do { } while(0)
