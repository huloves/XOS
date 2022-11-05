#ifndef _LINUX_EXT2_FS_I
#define _LINUX_EXT2_FS_I

#include <asm-i386/types.h>

/*
 * second extended file system inode data in memory
 */
struct ext2_inode_info {
	__u32	i_data[15];
	__u32	i_flags;
	__u32	i_faddr;
	__u8	i_frag_no;
	__u8	i_frag_size;
	__u16	i_osync;
	__u32	i_file_acl;
	__u32	i_dir_acl;
	__u32	i_dtime;
	__u32	not_used_1;	/* FIX: not used/ 2.2 placeholder */
	__u32	i_block_group;
	__u32	i_next_alloc_block;
	__u32	i_next_alloc_goal;
	__u32	i_prealloc_block;
	__u32	i_prealloc_count;
	__u32	i_high_size;
	int		i_new_inode:1;	/* Is a freshly allocated inode */
};

#endif	/* _LINUX_EXT2_FS_I */
