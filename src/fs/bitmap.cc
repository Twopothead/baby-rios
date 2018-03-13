#include <rios/bitmap.h>
#include <rios/syscall.h>
#include <rios/fs.h>
#include <rios/console.h>
#include <rios/ramdisk.h>
union Super_Block_Sect rios_superblock;

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

void free_inode(int inode)
{
	u8 sector[512]={0};
	IDE_read_sector((void *)&sector,NR_INODE_BLK(rios_superblock));
	bitmap_clear_bit(inode,sector);
	IDE_write_sector((void *)&sector,NR_INODE_BLK(rios_superblock));
}

int new_inode()
{
	u8 sector[512]={0};
	IDE_read_sector((void *)&sector,NR_INODE_BLK(rios_superblock));
	for(int i = 0; i < 512<<3 ; i++){
		if(bitmap_test_bit(i,sector)==0){
			bitmap_set_bit(i,sector);
			IDE_write_sector((void *)&sector,NR_INODE_BLK(rios_superblock));
		}
	}
	return -1;
}

void init_root_dir(union Super_Block_Sect  rios_superblock)
{
	u8 sector[512]={0};
	IDE_read_sector((void *)&sector,NR_INODE_BLK(rios_superblock));
_again_check_root:	
	if(!bitmap_test_bit(0,sector)){		
		kprintf("\n  no root directory found! init root '\\' now...");
		bitmap_set_bit(0,sector);
		IDE_read_sector((void *)&rios_superblock,HDB_SUPER_BLOCK_SEC);
		rios_superblock.s_startsect = 1;/*m_superblock may not sysc with disk*/
		IDE_write_sector((void *)&sector,NR_INODE_BLK(rios_superblock));


		/*we need to handle struct dir_entry here */

		goto _again_check_root;
	}else{
		nextline(),msg_ok();kprintf("  root dir / detected.");
	}
}

int new_block(){
	/* code here ....*/
	return -1;
}
void free_block(){

}
struct m_inode * iget(int nr){

}

void iput(struct m_inode * inode){

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
	rios_superblock.s_ninodes = INODE_BLKS;
	int total_used_ctrl_blks =  2 + rios_superblock.s_zone_bitmap_blks + \
		 rios_superblock.s_inode_bitmap_blks + rios_superblock.s_inode_blks; 
	format_superblock(rios_superblock);
	format_zone_bitmap_blks(rios_superblock,total_used_ctrl_blks);
	format_inode_bitmap_blks(rios_superblock);
	format_inode_blks(rios_superblock);
}

void set_specific_blk_nr(int i){/*设定第几块是专业块，从０开始计数*/
	rios_superblock.s_startsect = 1;
	IDE_read_sector((void *)&rios_superblock,NR_SUPER_BLK(rios_superblock));
	rios_superblock.s_specific_blk_nr_group = i;
	IDE_write_sector((void *)&rios_superblock,NR_SUPER_BLK(rios_superblock));

}

void check_rifs()
{
	union Super_Block_Sect tmp_sb;
	IDE_read_sector((void *)&tmp_sb,HDB_SUPER_BLOCK_SEC);
_again_check_fs:	
	if(tmp_sb.s_magic != RIFS_MAGIC_NUMBER){
		kprintf("  FS not detected.Formating disk... \n");
		format_disk();
		set_specific_blk_nr(0);/*第一组的第一块（代码上是第０块）*/
		init_free_space_grouping();/*　空闲块成组链接初始化,这里默认是第一组的第一块，(第０块)　*/
		IDE_read_sector((void *)&tmp_sb,HDB_SUPER_BLOCK_SEC);
		goto _again_check_fs;
		//kprintf("\n%x",tmp_sb.s_magic);
	}else{
		IDE_read_sector((void *)&rios_superblock,HDB_SUPER_BLOCK_SEC);
		msg_ok(),kprintf("  FS detected.");
		memset(&tmp_sb,0x00,512);			/* clear tmp_sb with zero */
		kprintf(" total sector:%d. ",hdb_sect_total);	/* init_hd first */
/*C*H*S=20160 cylinders=20, heads=16, spt=63*/		
	}
	init_root_dir(rios_superblock);
}

/* total_used_ctrl_blks = 350
 		(the first 2 sectors are boot sector and superblock blk 
   		350 = 2 + 5 + 1 + 342
 	kprintf(" total used contrl blks are :%d. = 2+ %d * %d * %d",total_used_ctrl_blks, \
 		rios_superblock.s_zone_bitmap_blks , \
 		 rios_superblock.s_inode_bitmap_blks , \
 		  rios_superblock.s_inode_blks);
 	 kprintf(" data blk NO.:%d.",NR_DATA_BLK(rios_superblock)); 351
 */


/*　Free space management :grouping (空闲块成组链接)　*/
#define BLKS_PER_GROUP 		64				/*每组64块*/
#define TOTAL_GROUP 		128				/*一共128组*/
#define SECTOR_PER_BLOCK 	2				/*每个块２个扇区,1KB*/

void init_free_space_grouping()
{
	u8 sector[512]={0};/*a block is 2 sector*/
	u16 free_data_blk[512] = {0};
/*u16 free_data_blk[512] = 2　个　u8 sector[512] */
/*这里注意一下，sector 是８个bit,若要存块号会位数不够，我要用一个16位的东西来存*/
	memset(sector,0x00,sizeof(sector)/sizeof(sector[0]));
	sector[0] = BLKS_PER_GROUP;				/*空闲块计数*/

	int nr_group = 0;					/*0~127*/
	int nr_last = NR_DATA_BLK(rios_superblock) + TOTAL_GROUP*BLKS_PER_GROUP*SECTOR_PER_BLOCK;
	for(int i = NR_DATA_BLK(rios_superblock); i < nr_last ; \
			i += BLKS_PER_GROUP*SECTOR_PER_BLOCK , nr_group++){
		free_data_blk[0] = BLKS_PER_GROUP ;			/*64块划分成一组*/
		if(nr_group != TOTAL_GROUP - 1){
			free_data_blk[0] = BLKS_PER_GROUP;		/*第一项：空闲块计数*/
			free_data_blk[1] = (nr_group +1) * BLKS_PER_GROUP;	
/*第二项：指向下一组空闲盘块BLKS_PER_GROUP号(初始化时是直接＂下一组＂),形成一个链*/
			int _tmp = BLKS_PER_GROUP *(nr_group + 1);
			for(int j = 1; j < BLKS_PER_GROUP; j++){
				free_data_blk[2+(j-1)] = --_tmp;
			}	
/*每组空闲块６４块，但每组第一个是记录信息的那个，故搞６３条记录,初始化时我逆序而记*/
		}else {/*the last free space block*/
			free_data_blk[0] = BLKS_PER_GROUP;
			free_data_blk[1] = 0; /* next free blk doesn't exist.*/
			int _tmp = BLKS_PER_GROUP *(nr_group + 1);
			for(int j = 1; j < BLKS_PER_GROUP; j++){
				free_data_blk[2+(j-1)] = --_tmp;
			}	
		}
/*写入到这一块第一个扇区*/	
     		memset(sector,0x00,sizeof(sector)/sizeof(sector[0]));
		memcpy((void *) sector, (const void *) free_data_blk, 512);
		IDE_write_sector((void *)&sector,i);	
/*写入到这一块第二个扇区*/			
		memset(sector,0x00,sizeof(sector)/sizeof(sector[0]));
		u8 *p = (u8 *)free_data_blk;
		for(int i = 0; i < 512; i++) p++;
		memcpy((void *) sector, (const void *) p, 512);
		IDE_write_sector((void *)&sector,i+1);	
	}

}