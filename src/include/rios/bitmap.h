#ifndef _BITMAP_H
#define _BITMAP_H

#include <rios/type.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <asm/x86.h>
#include <rios/dpt.h>
#include <rios/fs.h>
#include <rios/hd.h>
#include <rios/ramdisk.h>

#define NR_BOOT_BLK(_superblock) 		((_superblock).s_startsect)
#define NR_SUPER_BLK(_superblock)		NR_BOOT_BLK(_superblock)  + 1	
#define NR_ZONE_MAP_BLK(_superblock) 		NR_SUPER_BLK(_superblock) + 1
#define NR_INODE_MAP_BLK(_superblock)		NR_ZONE_MAP_BLK(_superblock) + (_superblock).s_zone_bitmap_blks
#define NR_INODE_BLK(_superblock)		NR_INODE_MAP_BLK(_superblock) + (_superblock).s_inode_bitmap_blks
#define NR_DATA_BLK(_superblock)		NR_INODE_BLK(_superblock) + INODE_BLKS 

#define HDB_SUPER_BLOCK_SEC 2
/*sector1:boot sector
 *settor2:superblock;
 */
extern int hdb_sect_total;
union Super_Block_Sect{
/*利用联合体去占坑位*/
		u8 bytes[512]={0};
		struct {
			u16 s_ninodes;
			u16 s_capacity_blks;		/*capacity count in blocks*/
			u16 s_startsect;
			u16 s_zone_bitmap_blks;		/*in rios.a zone is a sector.*/
/*according to Prof Jiang,we will not use this policy (data block bitmap) anymore.*/			
			u16 s_inode_bitmap_blks;	/*num of blks that bitmap takes up*/
			u16 s_inode_blks;
			u16 s_firstdatazone;
			u16 s_specific_blk_nr_group;	/*成组链接专用块对应磁盘上的组号*/
			u16 s_magic;			/*ri_fs magic:0x8888*/
		};
}; 

void check_rifs();
void format_disk();
void format_superblock(union Super_Block_Sect rios_superblock);
void format_zone_bitmap_blks(union Super_Block_Sect  rios_superblock,int total_used_ctrl_blks);
void format_inode_bitmap_blks(union Super_Block_Sect  rios_superblock);
void format_inode_blks(union Super_Block_Sect  rios_superblock);
void init_root_dir(union Super_Block_Sect  rios_superblock);
void set_specific_blk_nr(int i);
void init_free_space_grouping();

#define INODE_BITMAP_BLK 1 		/* 512<<3 = 4096 inodes*/
#define INODES_PER_BLK	(512/sizeof(d_inode))
#define INODE_BLKS 	((INODE_BITMAP_BLK*(512<<3))+ (INODES_PER_BLK-1))/INODES_PER_BLK


void free_space_grouping();


void _debug_visit_free_group_ctr();


#ifdef __cplusplus
}
#endif

#endif