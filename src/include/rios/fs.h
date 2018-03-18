#ifndef _FS_H
#define _FS_H
#ifdef __cplusplus

#include <rios/type.h>
#include <asm/x86.h>
#include <rios/sched.h>
#include <rios/bitmap.h>

extern "C" {
#endif /* __cplusplus */

/*struct task_struct haven't been implemented yet :(*/

#define RIFS_MAGIC_NUMBER 0x88
#define MAX_NAME_LEN 14
#define _SECTOR_SIZE 512
#define _BLOCK_SIZE 1024
#define INODES_PER_BLOCK ((_BLOCK_SIZE)/sizeof(struct d_inode))	
#define DIR_ENTRIES_PER_BLOCK ((_BLOCK_SIZE)/(sizeof(struct dir_entry)))

#define RIFS_MAX_INFO_LEN 16
struct super_block{

	u16 s_ninodes;
	u16 s_capacity_blks;		/*capacity count in blocks*/
	u16 s_startsect;
	u16 s_zone_bitmap_blks;		/*according to Prof Jiang,we will not use this policy (data block bitmap) anymore.*/
	u16 s_inode_bitmap_blks;	/*num of blks that bitmap takes up*/
	u16 s_inode_blks;
	u16 s_firstdatazone;		/*first data zone locates at which sector*/
	u16 s_specific_blk_nr;	/*free space management:grouping, the specific block's block number,coounting from 1*/
/*成组链接专用块对应磁盘上的盘块号(从1计数)*/
	u16 s_magic;			/*ri_fs magic:0x88*/
/*These are only in memeory*/
	/*code here.addtional info in memory*/
};

struct d_super_block
{
	u16 s_ninodes;
	u16 s_capacity_blks;		/*capacity count in blocks*/
	u16 s_startsect;
	u16 s_zone_bitmap_blks;		/*according to Prof Jiang,we will not use this policy (data block bitmap) anymore.*/
	u16 s_inode_bitmap_blks;	/*num of blks that bitmap takes up*/
	u16 s_inode_blks;
	u16 s_firstdatazone;
	u16 s_specific_blk_nr_group;	/*成组链接专用块对应磁盘上的组号*/
	u16 s_magic;			/*ri_fs magic:0x88*/
};

#define NORMAL_FILE	0
#define DIR_FILE  	1

struct m_inode
{
	u8 i_mode;			/*file type(dir/normal) and attribute(rwx)*/
	u8 i_size;
	u8 i_uid;			/*user id*/
	u8 i_gid;			/*group id*/
	u8 i_nlinks;			/*num of files that link to it*/
	u8 padding0;
	u32 i_creat_time;	
	u16 i_zone[10];
	u16 i_ino;			/*inode id号　(bitmap)*/
	u32 padding1[8];		/*占位　8*32个字节*/
/* ok,let's make sizeof(d_inode) exactly equal to 64,that's 512bits,
 * a sector can put exactly 8 of d_inode.
 * if we attemp to extend the m_inode and d_inode,make sure that
 * they are in sync with each other,and adjust the fields and paddings
 * without changing the sizeof(d_inode)
 */

/*请控制好d_inode的大小以及与m_inode同步性．这里设置几个padding的意义在于占位，
 *我把d_inode 的大小控制在8*6+32+16*10+16+32*8=512 bits,这样一个扇区512*8=4096bits,
 *正好可以放８个d_inode,尽量避免跨扇区操作inode;
 */	

/*
 * zone[0~6]:	direct block 
 * zone[7]:	single indirect block
 * zone[8]:	double indirect block 
 * zone[9]:	trible indirect block
 */	

/*These are only in memeory*/
	u32 i_access_time;
	u8 i_mount;
	u8 i_dev;			/*hd0 or hd1*/
	u8 i_dirty;
	u8 i_updated;
	struct task_struct *i_wait;	/*not implemented yet*/
}__attribute__((packed));
/*一定要加，不然字节对不齐，会多用空间*/

struct d_inode{
	u8 i_mode;			/*file type(dir/normal) and attribute(rwx)*/
	u8 i_size;
	u8 i_uid;			/*user id*/
	u8 i_gid;			/*group id*/
	u8 i_nlinks;			/*num of files that link to it*/
	u8 padding0;
	u32 i_creat_time;	
	u16 i_zone[10];
	u16 i_ino;			/*inode id号*/
	u32 padding1[8];
}__attribute__((packed));
/*一定要加，不然字节对不齐，会多用空间*/

struct dir_entry{
	u32 inode;
	u8 name[MAX_NAME_LEN];
}__attribute__((packed));

struct file
{
	
};

void init_fs();
void get_file_attrib(m_inode *fd);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* RIfs maximum capacity is decided by zones, it's about 4G.
 * zone[0~6]:	direct block  		7*512 = 3584B = 3.6KB
 * zone[7]:	single indirect block 	128*512 = 64 KB
 * zone[8]:	double indirect block  	(128^2)*512 = 8MB
 * zone[9]:	trible indirect block 	(128^3)*512 = 4096MB
 */


#endif
/*
｜＿＿＿＿｜＿＿＿＿｜＿＿＿＿｜＿＿＿＿｜＿＿＿＿｜＿  ＿  ＿  ＿｜		
｜＿＿＿＿｜＿＿＿＿｜＿＿＿＿｜＿＿＿＿｜＿＿＿＿｜＿  ＿  ＿  ＿｜		
bootsector   |	　　bmap    imap    inodes   DATA
　     super_block
*/