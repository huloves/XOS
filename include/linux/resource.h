#ifndef _LINUX_RESOURCE_H
#define _LINUX_RESOURCE_H

struct rlimit {
	unsigned long rlim_cur;
	unsigned long rlim_max;
};

#endif /* _LINUX_RESOURCE_H */
