#include <asm-i386/stdio.h>
#include <linux/string.h>




//uint32_t printf(const char* format, ...) {
//   va_list args;
//   va_start(args, format);
//   char buf[1024] = {0};
//   vsprintf(buf, format, args);
//   va_end(args);
//   return write(buf);
//}

void printk(const char* format, ...){
    va_list args;
    va_start(args, format);
    char buf[1024] = {0};
    vsprintf(buf, format, args);
    print_str(buf);
}