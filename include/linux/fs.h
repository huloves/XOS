#ifndef _LINUX_FS_H
#define _LINUX_FS_H

#include <linux/kdev_t.h>
#include <asm-i386/atomic.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <asm-i386/semaphore.h>
#include <linux/dcache.h>
#include <linux/wait.h>

extern void buffer_init(unsigned long);

/*
 * Try to keep the most commonly used fields in single cache lines (16
 * bytes) to improve performance.  This ordering should be
 * particularly beneficial on 32-bit processors.
 * 
 * We use the first 16 bytes for the data which is used in searches
 * over the block hash lists (ie. getblk() and friends).
 * 
 * The second 16 bytes we use for lru buffer scans, as used by
 * sync_buffers() and refill_freelist().  -- sct
 */
struct buffer_head {
	/* First cache line: */
	struct buffer_head *b_next;	/* Hash queue list */
	unsigned long b_blocknr;	/* block number */
	unsigned short b_size;		/* block size */
	unsigned short b_list;		/* List that this buffer appears */
	kdev_t b_dev;				/* device (B_FREE = free) */

	atomic_t b_count;			/* users using this block */
	kdev_t b_rdev;				/* Real device */
	unsigned long b_state;		/* buffer state bitmap (see above) */
	unsigned long b_flushtime;	/* Time when (dirty) buffer should be written */

	struct buffer_head *b_next_free;/* lru/free list linkage */
	struct buffer_head *b_prev_free;/* doubly linked list of buffers */
	struct buffer_head *b_this_page;/* circular list of buffers in one page */
	struct buffer_head *b_reqnext;	/* request queue */

	struct buffer_head **b_pprev;	/* doubly linked list of hash-queue */
	char * b_data;					/* pointer to data block */
	struct page *b_page;			/* the page this bh is mapped to */
	void (*b_end_io)(struct buffer_head *bh, int uptodate); /* I/O completion */
 	void *b_private;				/* reserved for b_end_io */

	unsigned long b_rsector;		/* Real buffer location on disk */
	wait_queue_head_t b_wait;

	struct list_head     b_inode_buffers;	/* doubly linked list of inode dirty buffers */
};

struct inode {
	struct list_head	i_hash;
	struct list_head	i_list;
	struct list_head	i_dentry;   // 链表记录与该文件相联系的所有dentry
	
	struct list_head	i_dirty_buffers;

	unsigned long		i_ino;   // 在同一文件系统中每个i节点号都是唯一的
	atomic_t			i_count;   // 共享计数
	kdev_t				i_dev;
	umode_t				i_mode;
	nlink_t				i_nlink;   // 计数器，用来记住这个文件有多少个链接
	uid_t				i_uid;
	gid_t				i_gid;
	kdev_t				i_rdev;
	loff_t				i_size;
	time_t				i_atime;   // 最后一次访问该文件的时间
	time_t				i_mtime;   // 最后一次修改该文件的时间
	time_t				i_ctime;   // 最初创建该文件的时间
	unsigned long		i_blksize;
	unsigned long		i_blocks;
	unsigned long		i_version;
	struct semaphore	i_sem;
	struct semaphore	i_zombie;
	struct inode_operations	*i_op;
	// struct file_operations	*i_fop;	/* former ->i_op->default_file_ops */
	// struct super_block		*i_sb;
	wait_queue_head_t		i_wait;
	// struct file_lock		*i_flock;
	// struct address_space	*i_mapping;
	// struct address_space	i_data;	
	// struct dquot			*i_dquot[MAXQUOTAS];
	// struct pipe_inode_info	*i_pipe;
	// struct block_device		*i_bdev;

	unsigned long		i_dnotify_mask; /* Directory notify events */
	struct dnotify_struct	*i_dnotify; /* for directory notifications */

	unsigned long		i_state;

	unsigned int		i_flags;
	unsigned char		i_sock;

	atomic_t		i_writecount;
	unsigned int		i_attr_flags;
	__u32			i_generation;
	union {
		// struct minix_inode_info		minix_i;
		// struct ext2_inode_info		ext2_i;
		// struct hpfs_inode_info		hpfs_i;
		// struct ntfs_inode_info		ntfs_i;
		// struct msdos_inode_info		msdos_i;
		// struct umsdos_inode_info	umsdos_i;
		// struct iso_inode_info		isofs_i;
		// struct nfs_inode_info		nfs_i;
		// struct sysv_inode_info		sysv_i;
		// struct affs_inode_info		affs_i;
		// struct ufs_inode_info		ufs_i;
		// struct efs_inode_info		efs_i;
		// struct romfs_inode_info		romfs_i;
		// struct shmem_inode_info		shmem_i;
		// struct coda_inode_info		coda_i;
		// struct smb_inode_info		smbfs_i;
		// struct hfs_inode_info		hfs_i;
		// struct adfs_inode_info		adfs_i;
		// struct qnx4_inode_info		qnx4_i;
		// struct bfs_inode_info		bfs_i;
		// struct udf_inode_info		udf_i;
		// struct ncp_inode_info		ncpfs_i;
		// struct proc_inode_info		proc_i;
		// struct socket			socket_i;
		// struct usbdev_inode_info        usbdev_i;
		void				*generic_ip;
	} u;
};

struct inode_operations {
	int (*create) (struct inode *,struct dentry *,int);
	struct dentry * (*lookup) (struct inode *,struct dentry *);
	int (*link) (struct dentry *,struct inode *,struct dentry *);
	int (*unlink) (struct inode *,struct dentry *);
	int (*symlink) (struct inode *,struct dentry *,const char *);
	int (*mkdir) (struct inode *,struct dentry *,int);
	int (*rmdir) (struct inode *,struct dentry *);
	int (*mknod) (struct inode *,struct dentry *,int,int);
	int (*rename) (struct inode *, struct dentry *,
			struct inode *, struct dentry *);
	int (*readlink) (struct dentry *, char *,int);
	// int (*follow_link) (struct dentry *, struct nameidata *);
	void (*truncate) (struct inode *);
	int (*permission) (struct inode *, int);
	int (*revalidate) (struct dentry *);
	// int (*setattr) (struct dentry *, struct iattr *);
	// int (*getattr) (struct dentry *, struct iattr *);
};

/* fs/dcache.c */
extern void vfs_caches_init(unsigned long);

#define NR_LIST		4

#endif /* _LINUX_FS_H */
