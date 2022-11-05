#ifndef _LINUX_EXT2_FS_H
#define _LINUX_EXT2_FS_H

#include <asm-i386/types.h>

/*
 * Constants relative to the data blocks
 */
#define	EXT2_NDIR_BLOCKS		12
#define	EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)

/*
 * Inode flags
 */
#define	EXT2_SECRM_FL				0x00000001 /* Secure deletion */
#define	EXT2_UNRM_FL				0x00000002 /* Undelete */
#define	EXT2_COMPR_FL				0x00000004 /* Compress file */
#define EXT2_SYNC_FL				0x00000008 /* Synchronous updates */
#define EXT2_IMMUTABLE_FL			0x00000010 /* Immutable file */
#define EXT2_APPEND_FL				0x00000020 /* writes to file may only append */
#define EXT2_NODUMP_FL				0x00000040 /* do not dump file */
#define EXT2_NOATIME_FL				0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#define EXT2_DIRTY_FL				0x00000100
#define EXT2_COMPRBLK_FL			0x00000200 /* One or more compressed clusters */
#define EXT2_NOCOMP_FL				0x00000400 /* Don't compress */
#define EXT2_ECOMPR_FL				0x00000800 /* Compression error */
/* End compression flags --- maybe not all used */	
#define EXT2_BTREE_FL				0x00001000 /* btree format dir */
#define EXT2_RESERVED_FL			0x80000000 /* reserved for ext2 lib */

#define EXT2_FL_USER_VISIBLE		0x00001FFF /* User visible flags */
#define EXT2_FL_USER_MODIFIABLE		0x000000FF /* User modifiable flags */

/*
 * Structure of an inode on the disk
 */
struct ext2_inode {
	__u16	i_mode;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_size;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Creation time */
	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_count;	/* Links count */
	__u32	i_blocks;	/* Blocks count */
	__u32	i_flags;	/* File flags */
	union {
		struct {
			__u32  l_i_reserved1;
		} linux1;
		struct {
			__u32  h_i_translator;
		} hurd1;
		struct {
			__u32  m_i_reserved1;
		} masix1;
	} osd1;				/* OS dependent 1 */
	__u32	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_dir_acl;	/* Directory ACL */
	__u32	i_faddr;	/* Fragment address */
	union {
		struct {
			__u8	l_i_frag;	/* Fragment number */
			__u8	l_i_fsize;	/* Fragment size */
			__u16	i_pad1;
			__u16	l_i_uid_high;	/* these 2 fields    */
			__u16	l_i_gid_high;	/* were reserved2[0] */
			__u32	l_i_reserved2;
		} linux2;
		struct {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_mode_high;
			__u16	h_i_uid_high;
			__u16	h_i_gid_high;
			__u32	h_i_author;
		} hurd2;
		struct {
			__u8	m_i_frag;	/* Fragment number */
			__u8	m_i_fsize;	/* Fragment size */
			__u16	m_pad1;
			__u32	m_i_reserved2[2];
		} masix2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Default values for user and/or group using reserved blocks
 */
#define	EXT2_DEF_RESUID		0
#define	EXT2_DEF_RESGID		0

/*
 * Structure of a directory entry
 */
#define EXT2_NAME_LEN 255

struct ext2_dir_entry {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	char	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * The new version of the directory entry.  Since EXT2 structures are
 * stored in intel byte order, and the name_len field could never be
 * bigger than 255 chars, it's safe to reclaim the extra byte for the
 * file_type field.
 */
struct ext2_dir_entry_2 {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	char	name[EXT2_NAME_LEN];	/* File name */
};

#endif /* _LINUX_EXT2_FS_H */
