#ifndef _LINUX_FS_H
#define _LINUX_FS_H

#include <linux/kdev_t.h>
#include <asm-i386/atomic.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <asm-i386/semaphore.h>
#include <linux/dcache.h>
#include <linux/wait.h>
#include <linux/ext2_fs_i.h>
#include <linux/ext2_fs_sb.h>

extern void buffer_init(unsigned long);
extern void inode_init(unsigned long);

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

struct block_device {
	struct list_head	bd_hash;
	atomic_t			bd_count;
/*	struct address_space	bd_data; */
	dev_t				bd_dev;  /* not a kdev_t - it's a search key */
	atomic_t			bd_openers;
	const struct block_device_operations *bd_op;
	struct semaphore	bd_sem;	/* open/close mutex */
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
		struct ext2_inode_info		ext2_i;
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

/* Inode state bits.. */
#define I_DIRTY_SYNC		1 /* Not dirty enough for O_DATASYNC */
#define I_DIRTY_DATASYNC	2 /* Data-related inode changes pending */
#define I_DIRTY_PAGES		4 /* Data-related inode changes pending */
#define I_LOCK				8
#define I_FREEING			16
#define I_CLEAR				32

#define I_DIRTY (I_DIRTY_SYNC | I_DIRTY_DATASYNC | I_DIRTY_PAGES)

struct fown_struct {
	int pid;		/* pid or -pgrp where SIGIO should be sent */
	uid_t uid, euid;	/* uid/euid of process setting the owner */
	int signum;		/* posix.1b rt signal to be delivered on IO */
};

struct file {
	struct list_head		f_list;
	struct dentry			*f_dentry;
	// struct vfsmount         *f_vfsmnt;
	struct file_operations	*f_op;
	atomic_t				f_count;
	unsigned int 			f_flags;
	mode_t					f_mode;
	loff_t					f_pos;
	unsigned long 			f_reada, f_ramax, f_raend, f_ralen, f_rawin;
	struct fown_struct		f_owner;
	unsigned int			f_uid, f_gid;
	int						f_error;

	unsigned long			f_version;

	/* needed for tty driver, and maybe others */
	void					*private_data;
};

struct super_block {
	struct list_head	s_list;		/* Keep this first */
	// kdev_t			s_dev;
	unsigned long		s_blocksize;
	unsigned char		s_blocksize_bits;
	unsigned char		s_lock;
	unsigned char		s_dirt;
	// struct file_system_type	*s_type;
	struct super_operations	*s_op;
	// struct dquot_operations	*dq_op;
	unsigned long		s_flags;
	unsigned long		s_magic;
	struct dentry		*s_root;
	wait_queue_head_t	s_wait;

	struct list_head	s_dirty;	/* dirty inodes */
	struct list_head	s_files;

	// struct block_device	*s_bdev;
	struct list_head	s_mounts;	/* vfsmount(s) of this one */
	// struct quota_mount_options s_dquot;	/* Diskquota specific options */

	union {
		// struct minix_sb_info	minix_sb;
		struct ext2_sb_info	ext2_sb;
		// struct hpfs_sb_info	hpfs_sb;
		// struct ntfs_sb_info	ntfs_sb;
		// struct msdos_sb_info	msdos_sb;
		// struct isofs_sb_info	isofs_sb;
		// struct nfs_sb_info	nfs_sb;
		// struct sysv_sb_info	sysv_sb;
		// struct affs_sb_info	affs_sb;
		// struct ufs_sb_info	ufs_sb;
		// struct efs_sb_info	efs_sb;
		// struct shmem_sb_info	shmem_sb;
		// struct romfs_sb_info	romfs_sb;
		// struct smb_sb_info	smbfs_sb;
		// struct hfs_sb_info	hfs_sb;
		// struct adfs_sb_info	adfs_sb;
		// struct qnx4_sb_info	qnx4_sb;
		// struct bfs_sb_info	bfs_sb;
		// struct udf_sb_info	udf_sb;
		// struct ncp_sb_info	ncpfs_sb;
		// struct usbdev_sb_info   usbdevfs_sb;
		void			*generic_sbp;
	} u;
	/*
	 * The next field is for VFS *only*. No filesystems have any business
	 * even looking at it. You had been warned.
	 */
	struct semaphore s_vfs_rename_sem;	/* Kludge */

	/* The next field is used by knfsd when converting a (inode number based)
	 * file handle into a dentry. As it builds a path in the dcache tree from
	 * the bottom up, there may for a time be a subpath of dentrys which is not
	 * connected to the main tree.  This semaphore ensure that there is only ever
	 * one such free path per filesystem.  Note that unconnected files (or other
	 * non-directories) are allowed, but not unconnected diretories.
	 */
	struct semaphore s_nfsd_free_path_sem;
};

struct block_device_operations {
	int (*open) (struct inode *, struct file *);
	int (*release) (struct inode *, struct file *);
	int (*ioctl) (struct inode *, struct file *, unsigned, unsigned long);
	int (*check_media_change) (kdev_t);
	int (*revalidate) (kdev_t);
};

/*
 * NOTE:
 * read, write, poll, fsync, readv, writev can be called
 *   without the big kernel lock held in all filesystems.
 */
struct file_operations {
	// struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char *, size_t, loff_t *);
	// int (*readdir) (struct file *, void *, filldir_t);
	// unsigned int (*poll) (struct file *, struct poll_table_struct *);
	int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
	// int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct inode *, struct file *);
	int (*flush) (struct file *);
	int (*release) (struct inode *, struct file *);
	int (*fsync) (struct file *, struct dentry *, int datasync);
	int (*fasync) (int, struct file *, int);
	// int (*lock) (struct file *, int, struct file_lock *);
	// ssize_t (*readv) (struct file *, const struct iovec *, unsigned long, loff_t *);
	// ssize_t (*writev) (struct file *, const struct iovec *, unsigned long, loff_t *);
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

/*
 * NOTE: write_inode, delete_inode, clear_inode, put_inode can be called
 * without the big kernel lock held in all filesystems.
 */
struct super_operations {
	void (*read_inode) (struct inode *);
	void (*write_inode) (struct inode *, int);
	void (*put_inode) (struct inode *);
	void (*delete_inode) (struct inode *);
	void (*put_super) (struct super_block *);
	void (*write_super) (struct super_block *);
	// int (*statfs) (struct super_block *, struct statfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	void (*clear_inode) (struct inode *);
	void (*umount_begin) (struct super_block *);
};

/* fs/dcache.c */
extern void vfs_caches_init(unsigned long);

#define NR_LIST		4

#endif /* _LINUX_FS_H */
