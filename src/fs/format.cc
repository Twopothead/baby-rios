#include <rios/format.h>

void format_superblock(union Super_Block_Sect rios_superblock)
{
	IDE_write_sector((void *)&rios_superblock,NR_SUPER_BLK(rios_superblock));
}

void format_zone_bitmap_blks(union Super_Block_Sect  rios_superblock,int total_used_ctrl_blks)
{
	u8 sector[512]={0};
	memset(sector,0xff,sizeof(sector)/sizeof(sector[0]));/*set bitmap with 11111...*/
	int nr_zonemap_p = NR_ZONE_MAP_BLK(rios_superblock);
	for(int i=total_used_ctrl_blks/(512<<3);i>0;--i){
		IDE_write_sector((void *)&sector,nr_zonemap_p++);
		/* in case that bitmap uses more than one sector*/
	}
	for(int i=total_used_ctrl_blks%(512<<3);i<(512<<3);++i){
		bitmap_clear_bit(i,sector);
/* skip those used ones,and clear the rest bit*/
	}
	IDE_write_sector((void *)&sector,nr_zonemap_p);
}

void format_inode_bitmap_blks(union Super_Block_Sect  rios_superblock)
{
	u8 sector[512]={0};memset(sector,0x00,sizeof(sector)/sizeof(sector[0]));/*aa*/
	for(int i=NR_INODE_MAP_BLK(rios_superblock);i<NR_INODE_BLK(rios_superblock);i++)
		IDE_write_sector((void *)&sector,i);
}

void format_inode_blks(union Super_Block_Sect  rios_superblock)
{
	u8 sector[512]={0};memset(sector,0x00,sizeof(sector)/sizeof(sector[0]));/*bb*/
	for(int i= NR_INODE_BLK(rios_superblock);i<NR_DATA_BLK(rios_superblock);i++)
		IDE_write_sector((void *)&sector,i);
}

void format_disk()
{
	
	rios_superblock.s_startsect = 1;			/*sect0 for MBR and sect1 for superblock*/
	rios_superblock.s_capacity_blks = hdb_sect_total/1;	/*call get_hd_size first*/	
	rios_superblock.s_magic = RIFS_MAGIC_NUMBER;
	rios_superblock.s_inode_bitmap_blks = INODE_BITMAP_BLK;	/*we use a blk for inode bitmap*/
	rios_superblock.s_inode_blks = INODE_BLKS;
	rios_superblock.s_zone_bitmap_blks = (hdb_sect_total+0xfff)>>12;
/** 
 * a zone bitmap can represent 512*8=4096sectors, 
 * 10MB= 20160sectors,need (20160+0xfff)>>12=5setors for zone_bitmap
 */
	rios_superblock.s_ninodes = INODE_BITMAP_BLK*(512<<3);
	rios_superblock.s_firstdatazone = NR_DATA_BLK(rios_superblock);
	rios_superblock.s_specific_blk_nr = 1;	/*free space management stragety: grouping*/
/*initially, the first( counting from 1 ) data block is allocated for specific block.*/
	int total_used_ctrl_blks =  2 + rios_superblock.s_zone_bitmap_blks + \
		 rios_superblock.s_inode_bitmap_blks + rios_superblock.s_inode_blks; 
	format_superblock(rios_superblock);
	format_zone_bitmap_blks(rios_superblock,total_used_ctrl_blks);
	format_inode_bitmap_blks(rios_superblock);
	format_inode_blks(rios_superblock);
}
