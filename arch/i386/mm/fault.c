#include <linux/linkage.h>
#include <asm/ptrace.h>
#include <asm/stdio.h>

asmlinkage void do_page_fault(struct pt_regs *regs, unsigned long error_code)
{
	printk("do_page_fault!\n");
	while(1);
	return;
}
