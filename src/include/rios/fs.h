#ifndef _FS_H
#define _FS_H
#ifdef __cplusplus

#include <rios/type.h>
#include <asm/x86>
#include <sched.h>

extern "C" {
#endif /* __cplusplus */

extern struct task_struct;/*haven't implement yet :(*/

#define RIFS_MAGIC_NUMBER 0x8888
#include MAX_NAME_LEN 14
#define _SECTOR_SIZE 512
#define _BLOCK_SIZE 1024
#define INODES_PER_BLOCK ((_BLOCK_SIZE)/sizeof(struct d_inode))	
#define DIR_ENTRIES_PER_BLOCK ((_BLOCK_SIZE)/(sizeof(struct dir_entry)))

#define RIFS_MAX_INFO_LEN 16
struct super_block{

	u16 s_magic;			/*ri_fs magic:0x8888*/
	u16 s_startsect;
	u16 s_capacity_blks;		/*capacity count in blocks*/
	u16 s_bitmap_blks;		/*num of blks that bitmap takes up*/
	u16 s_info[RIFS_MAX_INFO_LEN+1];/*"RiFS by qiuri"*/

/*These are only in memeory*/

};

struct d_super_block
{
	
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
	u32 i_creat_time;	
	u32 i_modify_time;
	u32 i_delet_time; 
	u16 i_zone[10];
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
	struct task_struct *i_wait;	/*not implement yet*/
};

struct d_inode{

};
struct dir_entry{
	u16 inode;
	u8 name[MAX_NAME_LEN];
};

struct file
{
	
};

void init_fs();
	
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