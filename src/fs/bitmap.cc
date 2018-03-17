#include <rios/bitmap.h>
union Super_Block_Sect rios_superblock;
struct m_inode iroot;
void init_root_dir(union Super_Block_Sect  rios_superblock)
{
	u8 sector[512]={0};iroot.i_creat_time = 2018;
_again_check_root:	
	IDE_read_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
	if(!bitmap_test_bit(0,sector)){		
		kprintf("\n  no root directory found! init root '\\' now...");
		iroot.i_ino = new_inode();/*bitmap_set_bit(0,sector);*/
		if(iroot.i_ino!=0)_panic("FBI WANNING:iroot's inode number must be 0!!!\n halt...");
/*we need to install root directory here */
		iroot.i_zone[0]=(u16)new_block();/*成组链接分配数据区*/
		iroot.i_size = 2 * sizeof(struct dir_entry);/*. and ..*/
/* ok,let's think about how to caculate how many dir_entry in a directory
 * my solution is :we can get the size of a dir file,then divide the sizeof dir_entry, then we'll get it.
 */
		iput(&iroot,iroot.i_ino);
		struct dir_entry *de = (struct dir_entry *)NULL;
		memset(&sector,0x00,512);
		de=(struct dir_entry*)sector;
		strcpy((char *)de->name,".");de->inode = 0;
		++de;strcpy((char *)de->name,"..");de->inode=-1;/*root doesn't have parent.*/
		IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(iroot.i_zone[0]));	

		goto _again_check_root;
	}else{
/* currently,iroot in memory is nothing,we must load iroot from disk */	
		iget(&iroot, 0);
		nextline(),msg_ok();kprintf("  root dir / detected.");
	}
	kprintf(" %d\n",iroot.i_zone[0]);
}

void dir_root(){
	u8 sector[512] = {0};
	iget(&iroot,0);/*read root from data zone's head*/
	int dir_num = iroot.i_size/(sizeof(struct dir_entry));
	struct dir_entry *de = (struct dir_entry*)sector;
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(iroot.i_zone[0]));	
	for(int i=0; i < dir_num; i++){
		kprintf("\n %s",&de->name);
		de++;
	}
}


void set_specific_blk_nr(int i){
	rios_superblock.s_startsect = 1;
	IDE_read_sector((void *)&rios_superblock,NR_SUPER_BLK(rios_superblock));
	rios_superblock.s_specific_blk_nr = i;/*block nr of specific block, counting from 1*/
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
		init_free_space_grouping();/*　空闲块成组链接初始化,这里默认是第一组的第一块，(第０块)　*/
		IDE_read_sector((void *)&tmp_sb,HDB_SUPER_BLOCK_SEC);
		goto _again_check_fs;
	}else{
		IDE_read_sector((void *)&rios_superblock,HDB_SUPER_BLOCK_SEC);
		msg_ok(),kprintf("  FS detected.");
		memset(&tmp_sb,0x00,512);			/* clear tmp_sb with zero */
		kprintf(" total sector:%d. ",hdb_sect_total);	/* init_hd first */
/*C*H*S=20160 cylinders=20, heads=16, spt=63*/		
	}
	init_root_dir(rios_superblock);
}

union free_space_grouping_head specific_block;/*内存专用块*/
int is_specific_block_set = 0;
int tmp =0;
int new_block()
{
	union Super_Block_Sect *p_ri_sb = get_super();
	set_super();
	if(!is_specific_block_set){
/*initially, the first( counting from 1 ) data block is allocated for specific block.*/		
		p_ri_sb->s_specific_blk_nr = 1;set_super();/*write back to disk*/
		specific_block = get_blk_nr_free_group(p_ri_sb->s_specific_blk_nr);
		is_specific_block_set = 1;
	}
/* remember to write back to disk. */	
	if(specific_block.s_free > 1){
		specific_block.s_free --;
		set_specific_blk_nr(p_ri_sb->s_specific_blk_nr);/*write back*/
		set_blk_nr_free_group(specific_block,p_ri_sb->s_specific_blk_nr);
		return specific_block.s_free_blk_nr[specific_block.s_free];
	}else if(specific_block.s_free == 1){
		specific_block.s_free --;
		int nr = specific_block.s_free_blk_nr[0];

	}

	// return tmp++;
}

void free_block(int block){

}




