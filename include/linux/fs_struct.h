#ifndef _LINUX_FS_STRUCT_H
#define _LINUX_FS_STRUCT_H

#include <asm/atomic.h>
#include <linux/spinlock.h>
#include <linux/dcache.h>

struct fs_struct {
	atomic_t count;
	rwlock_t lock;
	int umask;
	struct dentry * root, * pwd, * altroot;   // altroot指向本进程的替换根目录
	// struct vfsmount * rootmnt, * pwdmnt, * altrootmnt;
};

#define INIT_FS { \
	ATOMIC_INIT(1), \
	RW_LOCK_UNLOCKED, \
	0022, \
	NULL, NULL, NULL, NULL, NULL, NULL \
}

#endif /* _LINUX_FS_STRUCT_H */
