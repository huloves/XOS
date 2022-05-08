#ifndef __LIB_STDIO_H
#define __LIB_STDIO_H

#include <asm-i386/types.h>

typedef char* va_list;

extern int sprintf(char * buf, const char *fmt, ...);
extern int vsprintf(char *buf, const char *fmt, va_list args);
extern char* itoa(int num,char* dst,int radix);
extern char* uitoa(unsigned int num,char* dst,int radix);
extern void print_int(int num);
extern void print_hex(unsigned int num);
extern void put_char(char ch);
extern void printk(const char* format, ...);
extern void print_str(char *str);

#define va_start(ap, v) ap = (va_list)&v
#define va_arg(ap, t) *((t*)(ap += 4))
#define va_end(ap) ap = NULL

#endif
