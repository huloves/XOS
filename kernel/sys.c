#include <asm/errno.h>
#include <linux/linkage.h>

asmlinkage long sys_ni_syscall(void)
{
	return -ENOSYS;
}
