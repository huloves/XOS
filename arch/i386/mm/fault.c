#include <linux/linkage.h>
#include <asm-i386/ptrace.h>
#include <asm-i386/stdio.h>

asmlinkage void do_page_fault(struct pt_regs *regs, unsigned long error_code)
{
	printk("do_page_fault!\n");
	while(1);
	return;
}
