#ifndef _I386_TYPES_H
#define _I386_TYPES_H

#include <asm-i386/posix_types.h>

#define NULL ((void*)0)
#define BITS_PER_LONG 32

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool;

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef __kernel_dev_t		dev_t;
typedef signed long long s64;
typedef unsigned long long u64;
typedef unsigned int __u32;
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef __kernel_pid_t		pid_t;
typedef __kernel_mode_t		mode_t;

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;
typedef __kernel_ssize_t	ssize_t;
#define true 1
#define false 0
#define DIV_ROUND_UP(X, STEP) ((X + STEP - 1) / (STEP))
#ifndef _SIZE_T
#define _SIZE_T
typedef __kernel_size_t		size_t;
#endif
typedef __kernel_clock_t	clock_t;
typedef uint32_t dma_addr_t;
typedef unsigned short umode_t;
typedef __kernel_nlink_t	nlink_t;
typedef __kernel_uid32_t	uid_t;
typedef __kernel_gid32_t	gid_t;
typedef __kernel_uid16_t        uid16_t;
typedef __kernel_gid16_t        gid16_t;
typedef __kernel_loff_t		loff_t;
typedef __kernel_time_t		time_t;
typedef unsigned long long __u64;

typedef __kernel_off_t		off_t;
typedef __kernel_ino_t		ino_t;
#endif /* _LINUX_TYPES_H */
