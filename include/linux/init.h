//
// Created by ljy on 10/24/21.
//
#ifndef _LINUX_INIT_H
#define _LINUX_INIT_H
#define __init __attribute__ ((__section__ (".text.init")))
#define __exit		__attribute__ ((unused, __section__(".text.exit")))
#define __initdata	__attribute__ ((__section__ (".data.init")))
#define __exitdata	__attribute__ ((unused, __section__ (".data.exit")))
#define __initsetup	__attribute__ ((unused,__section__ (".setup.init")))
#define __init_call	__attribute__ ((unused,__section__ (".initcall.init")))
#define __exit_call	__attribute__ ((unused,__section__ (".exitcall.exit")))
/*
 * Used for initialization calls..
 */
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);
extern initcall_t __initcall_start, __initcall_end;

#define __initcall(fn)								\
	static initcall_t __initcall_##fn __init_call = fn
#define __exitcall(fn)								\
	static exitcall_t __exitcall_##fn __exit_call = fn

#define __init_call	__attribute__ ((unused,__section__ (".initcall.init")))

void init_all(void);
#endif
