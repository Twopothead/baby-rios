#ifndef _FORMAT_H
#define _FORMAT_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/bitmap.h>
#include <rios/fs.h>

extern union Super_Block_Sect rios_superblock;
void format_disk();
void format_superblock(union Super_Block_Sect rios_superblock);
void format_zone_bitmap_blks(union Super_Block_Sect  rios_superblock,int total_used_ctrl_blks);
void format_inode_bitmap_blks(union Super_Block_Sect  rios_superblock);
void format_inode_blks(union Super_Block_Sect  rios_superblock);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif