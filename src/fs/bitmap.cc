#include <rios/bitmap.h>
#include <rios/syscall.h>
#include <rios/fs.h>
#include <rios/console.h>
#include <rios/ramdisk.h>
#include <asm/x86.h>
#include <rios/format.h>
union Super_Block_Sect rios_superblock;

/*iget: disk =>memory*/
struct m_inode * iget(struct m_inode * inode, int nr){
/*inode nr counts from 0*/	
	u8 sector[512] = {0};int NR_inode_blk_start = NR_INODE_BLK(rios_superblock);
	int m = nr*sizeof(struct d_inode)/512;
	int _m = (nr+1)*sizeof(struct d_inode)/512;
	int n = nr*sizeof(struct d_inode)%512;
	int _n = (nr+1)*sizeof(struct d_inode)%512;
	if(m==_m){/*即将要取的inode不跨越两个扇区*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m);
		memcpy(  inode,(void *) sector + n, sizeof(struct d_inode));
/*内容上，d_inode是m_inode的子集，这里把m_inode的前半段(即d_inode)抄送磁盘d_inode*/
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
	}else{/*即将要取的inode将跨越两个扇区*/
/*copy 前半截 offset: n ~ 512*/		
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m);
		memcpy( inode, (void *) (sector + n) ,512-n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
/*copy 后半截 offset: 0 ~ _n*/	memset(&sector,0x00,512);/*清零*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m + 1);
		memcpy( (void *)(inode + (512-n)), (void *)(sector + 0),_n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m + 1);
	}
/*内容上，d_inode是m_inode的子集，这里把部分m_inode抄送d_inode*/	
	return inode;
}

/*iput : memory => disk*/
void iput(struct m_inode * inode, int nr)
{/*inode nr counts from 0*/	
	u8 sector[512] = {0};int NR_inode_blk_start = NR_INODE_BLK(rios_superblock);
	int m = nr*sizeof(struct d_inode)/512;
	int _m = (nr+1)*sizeof(struct d_inode)/512;
	int n = nr*sizeof(struct d_inode)%512;
	int _n = (nr+1)*sizeof(struct d_inode)%512;
	if(m==_m){/*即将要存的inode不跨越两个扇区*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m );
		memcpy( (void *)(sector + n), inode , sizeof(struct d_inode));
/*内容上，d_inode是m_inode的子集，这里把m_inode的前半段(即d_inode)抄送磁盘d_inode*/
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
	 }else{/*即将要存的inode将跨越两个扇区*/
/*copy 前半截 offset: n ~ 512*/		
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m);
		memcpy( (void *)(sector + n), inode ,512-n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m);
/*copy 后半截 offset: 0 ~ _n	memset(&sector,0x00,512);*//*清零*/
		IDE_read_sector((void *)&sector,NR_inode_blk_start + m + 1);
		memcpy( (void *)(sector + 0), inode + (512-n) ,_n);
		IDE_write_sector((void *)&sector,NR_inode_blk_start + m + 1);
	}
	return;	
}

void free_inode(int inode)
{
	u8 sector[512]={0};
	IDE_read_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
	bitmap_clear_bit(inode,sector);
	IDE_write_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
}

/*You can hexdump NR_INODE_MAP_BLK(rios_superblock),eg "hexdump 8" to see this zone*/
int new_inode()
{
	u8 sector[512]={0};
	int i = 0;rios_superblock.s_startsect = 1;
	IDE_read_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
	for(i=0;i<512*8;i++){
		if(bitmap_test_bit(i,sector)){
			;
		}else{
			bitmap_set_bit(i,sector);
			IDE_write_sector((void *)&sector,NR_INODE_MAP_BLK(rios_superblock));
			return i;
		}
	}
	return i;
}

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
		/*we need to handle struct dir_entry here */
		iroot.i_zone[0]=(u16)new_block();/*成组链接分配数据区*/
		iput(&iroot,iroot.i_ino);
		struct dir_entry *de = (struct dir_entry *)NULL;
		memset(&sector,0x00,512);
		de=(struct dir_entry*)sector;
		strcpy((char *)de->name,".");de->inode = 0;
		++de;strcpy((char *)de->name,"..");de->inode=-1;/*root doesn't have parent.*/
		IDE_write_sector((void *)&sector, NR_DATA_BLK(rios_superblock)+iroot.i_zone[0]);	
// struct dir_entry *de = NULL;
// iput(&rios_superblock,&iroot,);
// de = (struct dir_entry *) sector;
// (const char *)&de.name=".";
// de++;
// (const char *)&de.name="..";
// de = (struct dir_entry *) sector;
/*通过一个空指针指在内存中扇区数组上达到操纵目的，之后写回到硬盘*/
// if()

		goto _again_check_root;
	}else{
		nextline(),msg_ok();kprintf("  root dir / detected.");
	}
	kprintf(" %d\n",iroot.i_zone[0]);
}

void dir_root(){
	u8 sector[512] = {0};
	iget(&iroot,0);/*read root from data zone's head*/
	struct dir_entry *de = (struct dir_entry*)sector;
	IDE_read_sector((void *)&sector, NR_INODE_BLK(rios_superblock)+iroot.i_zone[0]);	
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
	//init_root_dir(rios_superblock);
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

void init_free_space_grouping()
{
	union Super_Block_Sect *sb = get_super();
	union free_space_grouping_head g_head;
	int nr_group = 1;
/* nr_group counts from 1 */	
	for(int i = 1; i <= TOTAL_GROUP; i++)
	{
		int sector_num = NR_GROUP_SECTOR_NUMBER(i);
		if( i!= TOTAL_GROUP){
			g_head.s_free = BLKS_PER_GROUP;//64
			g_head.s_free_blk_nr[0] = i*BLKS_PER_GROUP+1;
			int tmp = g_head.s_free_blk_nr[0];
			for(int j = 0; j <= g_head.s_free; j ++){
				g_head.s_free_blk_nr[j] = tmp--;  
			}

		}else{
			g_head.s_free = BLKS_PER_GROUP-1;//63
			g_head.s_free_blk_nr[0] = 0;/* next group doesn't exist */
			int tmp =BLKS_PER_GROUP*i;
			for(int j = 0; j <= g_head.s_free; j ++){
				g_head.s_free_blk_nr[j] = tmp--;  
			}
		}
/*in rios, a data block contains 2 sectors,this the first*/		
		u8 * p1 = (u8 *)&g_head ;IDE_write_sector((void *)p1,sector_num );
/*then the second.*/
		u8 * p2 = (u8 *)&g_head + 512;IDE_write_sector((void *)p2,sector_num +1);
/*!attention here, p and sect are pointers,we SHOULD NOT use (void*)p , but should use (void *)p */		
	}
}



union Super_Block_Sect * get_super()
{
	union Super_Block_Sect *sb = &rios_superblock;
	IDE_read_sector((void *)sb,HDB_SUPER_BLOCK_SEC);
	return sb;
}

void set_super(){
/*set superblock by using global rios_superblock */
	IDE_write_sector((void *)&rios_superblock,HDB_SUPER_BLOCK_SEC);
}

union free_space_grouping_head specific_block;/*内存专用块*/
int is_specific_block_set = 0;
int tmp =0;
int new_block()
{
// 	union Super_Block_Sect *p_ri_sb = get_super();
// 	set_super();
// _again:
// 	if(specific_block.s_next_free_group_nr==65535&&specific_block.s_free==0)_panic("FBI WANNING:No free space on disk available!!!");
// 	if(!is_specific_block_set) {
// 		specific_block = get_nr_free_group(p_ri_sb->s_specific_blk_nr_group);set_nr_free_group(specific_block,p_ri_sb->s_specific_blk_nr_group/BLKS_PER_GROUP);
// 		is_specific_block_set = 1;
// 	}
// /*要记得把内存专用是第几块写到磁盘上的超级块*/
// 	if(specific_block.s_free > 1){
// 		specific_block.s_free --;set_nr_free_group(specific_block,p_ri_sb->s_specific_blk_nr_group/BLKS_PER_GROUP);
// 		return specific_block.s_free_blk_nr[specific_block.s_free-1];
// 	}else if(specific_block.s_free == 1){
// 		specific_block.s_free --;set_nr_free_group(specific_block,p_ri_sb->s_specific_blk_nr_group/BLKS_PER_GROUP);
// 		specific_block=get_nr_free_group(specific_block.s_next_free_group_nr/BLKS_PER_GROUP);/*第几号块转化为组号*/
// 		p_ri_sb->s_specific_blk_nr_group = specific_block.s_next_free_group_nr/BLKS_PER_GROUP;
// /*把当前用的是哪个专用块的信息写到磁盘超级块*/	set_super();
// 		goto _again;	
// 	}else if(specific_block.s_free ==0){
// 		kprintf("   %d",specific_block.s_next_free_group_nr);
// _panic("FBI WANNING:No free space on disk available!!!");
// 	}
// 	return -1;
	return tmp++;
}


union free_space_grouping_head get_nr_free_group(int nr)
{/* group_nr counts from 1 */
	union Super_Block_Sect *sb = get_super();
	union free_space_grouping_head g_head;
	int i = NR_GROUP_SECTOR_NUMBER(nr);
	{
/* in RiOS, a data block is consists of 2 sector, the first sector */		
		u8 * p1 = (u8 *)&g_head ;	
		IDE_read_sector((void *)p1,i);
/* then the second sector */	
		u8 * p2 = (u8*)&g_head + 512;
		IDE_read_sector((void *)p2,i+1);
	}
	return g_head;
}

void set_nr_free_group(union free_space_grouping_head g_head,int nr)
{/* group_nr counts from 1 */
	union Super_Block_Sect *sb = get_super();	
	int i =  NR_GROUP_SECTOR_NUMBER(nr);
	{
/* the first sector of a data block */		
		u8 * p1 = (u8 *)&g_head ;
		IDE_write_sector((void *)p1,i);
/* then the second */		
		u8 * p2 = (u8*)&g_head + 512;
		IDE_write_sector((void *)p2,i+1);
	}
	return;
}


void visit_all_free_blks()
{/* free space management : grouping */
/* this function prints all free block group's contrl info */	
	union Super_Block_Sect *sb = get_super();
	union free_space_grouping_head g_head;
	int nr_group = 1;
/* nr_group counts from 1 */	
	for(int i = 1; i <= 10; i++,nr_group++)
	{
		
		int sector_num = NR_GROUP_SECTOR_NUMBER(i);
/*in rios, a data block contains 2 sectors,this the first*/		
		u8 * p1 = (u8 *)&g_head ;
		IDE_read_sector((void *)p1,sector_num);
/*then the second.*/
		u8 * p2 = (u8 *)&g_head + 512;
		IDE_read_sector((void *)p2,sector_num+1);
/*!attention here, p and sect are pointers,we SHOULD NOT use (void*)p , but should use (void *)p */		
		kprintf("\n%d:",(i-1)*BLKS_PER_GROUP+1);
		kprintf(" group[%d] s_free:%d [0]%d [1]%d [2]%d ...[%d]%d ", nr_group, g_head.s_free, \
			g_head.s_free_blk_nr[0],g_head.s_free_blk_nr[1],g_head.s_free_blk_nr[2],\
			g_head.s_free-1,g_head.s_free_blk_nr[g_head.s_free-1]);
	}
}