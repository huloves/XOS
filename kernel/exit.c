#include <linux/linkage.h>
#include <asm/current.h>
#include <asm-i386/stdio.h>
#include <asm/hardirq.h>
#include <asm-i386/stdio.h>

void do_exit(long code)
{
	printk("123.\n");
// 	struct task_struct *tsk = current;

// 	if (in_interrupt())
// 		printk("Aiee, killing interrupt handler!");
// 	if (!tsk->pid)
// 		printk("Attempted to kill the idle task!");
// 	if (tsk->pid == 1)
// 		printk("Attempted to kill init!");
// 	tsk->flags |= PF_EXITING;
// 	// del_timer_sync(&tsk->real_timer);

// fake_volatile:
// #ifdef CONFIG_BSD_PROCESS_ACCT
// 	acct_process(code);
// #endif
// 	__exit_mm(tsk);

// 	lock_kernel();
// 	sem_exit();
// 	__exit_files(tsk);
// 	__exit_fs(tsk);
// 	exit_sighand(tsk);
// 	exit_thread();

// 	if (current->leader)
// 		disassociate_ctty(1);

// 	put_exec_domain(tsk->exec_domain);
// 	if (tsk->binfmt && tsk->binfmt->module)
// 		__MOD_DEC_USE_COUNT(tsk->binfmt->module);

// 	tsk->exit_code = code;
// 	exit_notify();
// 	schedule();
// 	BUG();
// /*
//  * In order to get rid of the "volatile function does return" message
//  * I did this little loop that confuses gcc to think do_exit really
//  * is volatile. In fact it's schedule() that is volatile in some
//  * circumstances: when current->state = ZOMBIE, schedule() never
//  * returns.
//  *
//  * In fact the natural way to do all this is to have the label and the
//  * goto right after each other, but I put the fake_volatile label at
//  * the start of the function just in case something /really/ bad
//  * happens, and the schedule returns. This way we can try again. I'm
//  * not paranoid: it's just that everybody is out to get me.
//  */
// 	goto fake_volatile;
}

asmlinkage long sys_exit(int error_code)
{
	do_exit((error_code & 0xff) << 8);
}
