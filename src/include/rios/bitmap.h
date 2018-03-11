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


#define BOOT_BLK(_superblock) 		((_superblock).s_startsect)
#define SUPER_BLK(_superblock)		((_superblock).s_startsect) + 1	
#define ZONE_MAP_BLK(_superblock) 	((_superblock).s_startsect) + 2
#define INODE_MAP_BLK(_superblock)	ZONE_MAP_BLK(_superblock) + (_superblock).s_zone_bitmap_blks
#define INODE_BLK(_superblock)		INODE_MAP_BLK(_superblock) + (_superblock).s_inode_bitmap_blks
#define DATA_BLK(_superblock)		INODE_BLK(_superblock) + (s_inode_blks)

void check_rifs();
void format_disk();

#define INODE_BITMAP_BLK 1 		/* 512<<3 = 4096 inodes*/
#define INODES_PER_BLK	(512/sizeof(d_inode))
#define INODE_BLKS 	((INODE_BITMAP_BLK*(512<<3))+ (INODES_PER_BLK-1))/INODES_PER_BLK


#ifdef __cplusplus
}
#endif

#endif