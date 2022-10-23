#include <asm-i386/semaphore.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <asm-i386/current.h>
#include <asm-i386/atomic.h>

static spinlock_t semaphore_lock = SPIN_LOCK_UNLOCKED;

void __down(struct semaphore * sem)
{
	struct task_struct *tsk = current;
	DECLARE_WAITQUEUE(wait, tsk);   // wait代表tsk
	tsk->state = TASK_UNINTERRUPTIBLE;   // 设置为睡眠状态
	// add_wait_queue_exclusive(&sem->wait, &wait);   // 把当前进程的等待队列元素wait加入到sem->wait等待队列队尾

	spin_lock_irq(&semaphore_lock);
	sem->sleepers++;   // 等待进程数目+1
	for (;;) {
		int sleepers = sem->sleepers;   // 禁止本地中断并获取指定的锁

		/*
		 * Add "everybody else" into it. They aren't
		 * playing, because we own the spinlock.
		 */
		/**返回非0,表示进程需要等待
		 * 假设有2个进程,进程资源已经被占用,当前进程执行down失败,跳转到这里,等待调度
		 * 结束前一个进程归还了资源,count变为0(之前down2次为-1),sleeper-1也为0,相加等于0,于是可以进入临界区
		 **/
		if (!atomic_add_negative(sleepers - 1, &sem->count)) {
			sem->sleepers = 0;
			break;
		}
		sem->sleepers = 1;	/* us - see -1 above */
		spin_unlock_irq(&semaphore_lock);

		// schedule();
		tsk->state = TASK_UNINTERRUPTIBLE;
		spin_lock_irq(&semaphore_lock);
	}
	spin_unlock_irq(&semaphore_lock);
	// remove_wait_queue(&sem->wait, &wait);
	tsk->state = TASK_RUNNING;
	// wake_up(&sem->wait);
}

asm(
".align 4\n"
".globl __down_failed\n"
"__down_failed:\n\t"
	"pushl %eax\n\t"
	"pushl %edx\n\t"
	"pushl %ecx\n\t"
	"call __down\n\t"
	"popl %ecx\n\t"
	"popl %edx\n\t"
	"popl %eax\n\t"
	"ret"
);