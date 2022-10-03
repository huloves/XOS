#ifndef __TIME_H
#define __TIME_H

#include <asm-i386/types.h>

// 适合习惯的时间格式
struct timespec {
	time_t tv_sec;   // 秒
	long   tv_nsec;  // 纳秒，nanoseconds
};

struct timeval {
	time_t tv_sec;   // 秒，seconds
	suseconds_t tv_usec;   // 微秒，micrseconds
};

struct timezone {
	int tz_minuteswest;   // 格林尼治时间往西方的时差
	int tz_dsttime;       // 时间修正方式
};

#endif
