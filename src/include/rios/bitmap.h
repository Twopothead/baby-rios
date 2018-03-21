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
#include <rios/grouping.h>
#include <rios/syscall.h>
#include <rios/fs.h>
#include <rios/console.h>
#include <rios/ramdisk.h>
#include <asm/x86.h>
#include <rios/format.h>
#include <rios/inode.h>

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
			u16 s_specific_blk_nr;		/*free space management:grouping, the specific block's block number,coounting from 1*/
/*成组链接专用块对应磁盘上的盘块号(从1计数)*/
			u16 s_magic;			/*ri_fs magic:0x88*/
		};
}; 

#define SECTORS_EACH_DATA_BLK 	2			/*in data block,each block contains 2 sectors*/
#define DATA_BLK_NR_TO_SECTOR_NR(nr_blk) NR_DATA_BLK(rios_superblock)+SECTORS_EACH_DATA_BLK*(nr_blk-1)

union free_space_grouping_head;

void check_rifs();
void init_root_dir(union Super_Block_Sect  rios_superblock);
void set_specific_blk_nr(int i);
void dir_root();
void ls();

#define INODE_BITMAP_BLK 1 		/* 512<<3 = 4096 inodes*/
#define INODES_PER_BLK	(512/sizeof(d_inode))
#define INODE_BLKS 	((INODE_BITMAP_BLK*(512<<3))+ (INODES_PER_BLK-1))/INODES_PER_BLK


int new_block();
void free_block(int block);
#ifdef __cplusplus
}
#endif

#endif