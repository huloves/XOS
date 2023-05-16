#include <asm/print.h>
// #include <asm-i386/interrupt.h>
#include <asm/stdio.h>

/* 打印文件名,行号,函数名,条件并使程序悬停 */
void panic_spin(char* filename,
	        int line,
		const char* func)
{
//   intr_disable();	// 因为有时候会单独调用panic_spin,所以在此处关中断。
    printk("\n\n\n!!!!! error !!!!!\n");
    printk("filename:");printk("%s", filename);printk("\n");
    printk("line:");printk("%d",line);printk("\n");
    printk("function:");printk("%s", (char*)func);printk("\n");
    while(1);
}
