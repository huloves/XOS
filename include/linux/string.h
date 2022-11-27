#ifndef _LINUX_STRING_H_
#define _LINUX_STRING_H_

#include <asm-i386/types.h>

/* 将dst_起始的size个字节置为value */
extern void* memset(void* dst_, uint8_t value, uint32_t size);
/* 将src_起始的size个字节复制到dst_ */
extern void memcpy(const void* dst, const void* src, unsigned int size);

/* 连续比较以地址a_和地址b_开头的size个字节,若相等则返回0,若a_大于b_返回+1,否则返回-1 */
extern int memcmp(const void * s1, const void *s2, int n);

/* 将字符串从src_复制到dst_ */
extern char* strcpy(char* dst_, const char* src_);

/* 返回字符串长度 */
extern uint32_t strlen(const char* str);

/* 比较两个字符串,若a_中的字符大于b_中的字符返回1,相等时返回0,否则返回-1. */
extern int8_t strcmp (const char* a, const char* b);

/* 从左到右查找字符串str中首次出现字符ch的地址(不是下标,是地址) */
extern char* strchr(const char* str, const uint8_t ch);

/* 从后往前查找字符串str中首次出现字符ch的地址(不是下标,是地址) */
extern char* strrchr(const char* str, const uint8_t ch);

/* 将字符串src_拼接到dst_后,将回拼接的串地址 */
extern char* strcat(char* dst_, const char* src_);

/* 在字符串str中查找指定字符ch出现的次数 */
extern uint32_t strchrs(const char* str, uint8_t ch);

// #define struct_cpy(x,y) 			\
// ({						\
// 	memcpy(x, y, sizeof(*(x)));		\
// })

extern char* itoa(int num,char* dst,int radix);

extern char* uitoa(unsigned int num,char* dst,int radix);

extern uint32_t strnlen(const char* str, uint32_t max);

extern char * strstr(const char * s1,const char * s2);

extern int strncmp(const char * cs,const char * ct,size_t count);
#endif
