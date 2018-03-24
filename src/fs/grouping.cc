#include <rios/grouping.h>
/* free disk space management : grouping */
/*　Free space management :grouping (空闲块成组链接)　*/

void init_free_space_grouping()
{
	union Super_Block_Sect *sb = get_super();
	union free_space_grouping_head g_head;
	int nr_group = 1;int nr_blk=1;
/* nr_group counts from 1 ,block nr counts from 1*/	
	for(int i = 1; i <= TOTAL_GROUP; i++,nr_group++)
	{
		int sector_num = DATA_BLK_NR_TO_SECTOR_NR(nr_blk);
		nr_blk += BLKS_PER_GROUP;
		if( i!= TOTAL_GROUP){
			g_head.s_free = BLKS_PER_GROUP;//64
			g_head.s_free_blk_nr[0] = i*BLKS_PER_GROUP+1;
			int tmp = g_head.s_free_blk_nr[0];
			for(int j = 0; j <= g_head.s_free; j ++){
				g_head.s_free_blk_nr[j] = tmp--;  
			}

		}else{
			g_head.s_free = BLKS_PER_GROUP-1;//63
			int tmp =BLKS_PER_GROUP*i;
			for(int j = 0; j <= g_head.s_free; j ++){
				g_head.s_free_blk_nr[j] = tmp--;  
			}
			g_head.s_free_blk_nr[0] = 0;/* next group doesn't exist */
		}
/*in rios, a data block contains 2 sectors,this the first*/		
		u8 * p1 = (u8 *)&g_head ;IDE_write_sector((void *)p1,sector_num );
/*then the second.*/
		u8 * p2 = (u8 *)&g_head + 512;IDE_write_sector((void *)p2,sector_num +1);
/*!attention here, p and sect are pointers,we SHOULD NOT use (void*)p , but should use (void *)p */		
	}
	set_specific_blk_nr(1);/*set group [1] as the specific block*/
}



union free_space_grouping_head get_blk_nr_free_group(int nr_blk)
{/* group_nr counts from 1 */
	union Super_Block_Sect *sb = get_super();
	union free_space_grouping_head g_head;
	int i = DATA_BLK_NR_TO_SECTOR_NR(nr_blk);
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

void set_blk_nr_free_group(union free_space_grouping_head g_head,int nr_blk)
{/* group_nr counts from 1 */
	union Super_Block_Sect *sb = get_super();	
	int i =  DATA_BLK_NR_TO_SECTOR_NR(nr_blk);
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

union Super_Block_Sect * get_super()
{
	union Super_Block_Sect *sb = &rios_superblock;
	IDE_read_sector((void *)sb,HDB_SUPER_BLOCK_SEC);
	return sb;
}

/* set_super is neccesary, it writes rios_superblock(in memory) back to disk */
void set_super()
{/*set superblock by using global rios_superblock */
	IDE_write_sector((void *)&rios_superblock,HDB_SUPER_BLOCK_SEC);
}


void free_blk_traverse(){
	union Super_Block_Sect *sb = get_super();
	union free_space_grouping_head g_head;
	int nr_blk = sb->s_specific_blk_nr;
	int sector_num = DATA_BLK_NR_TO_SECTOR_NR(nr_blk);
	nextline();
	g_head = get_blk_nr_free_group(nr_blk);
	int i;int nr_group =1;/*nr_group counts from 1*/
next_group:	
	kprintf("group[%d]:",nr_group++);
	for(i=g_head.s_free;i>1;i--){
		int index = i-1;
		nr_blk = g_head.s_free_blk_nr[index];
		if(index==g_head.s_free-1||index==g_head.s_free-2||index==3||index==2||index==1||index==0){
			kprintf(" [%d]%d ",index,g_head.s_free_blk_nr[index]);
			if(index==g_head.s_free-2)kprintf("...");
		}

	}
	if(i==1){
		int index = i-1;
		nr_blk = g_head.s_free_blk_nr[index];
		g_head = get_blk_nr_free_group(nr_blk);kprintf("\n");
		if(g_head.s_free_blk_nr[0]!=0)goto next_group;
	}
}

/* 'visit_all_free_blks' can be used after first initialize(mainly for debug) ,because it
 * assumes that free block groups distribute in disk in a regular way,
 * Comparing to it,'free_blk_traverse' is more recommended,because the latter works like 
 * visiting linked list.
 */
void visit_all_free_blks()
{/* free space management : grouping */
/* this function prints all free block group's contrl info */	
	union Super_Block_Sect *sb = get_super();
	union free_space_grouping_head g_head;
	int nr_group = 1;int nr_blk = 1;
/* nr_group counts from 1 ,nr_blk counts from 1*/	
	for(int i = 1; i <= 128; i++, nr_group++)
	{
		
		int sector_num = DATA_BLK_NR_TO_SECTOR_NR(nr_blk);
		nr_blk += BLKS_PER_GROUP;
/*in rios, a data block contains 2 sectors,this the first*/		
		u8 * p1 = (u8 *)&g_head ;
		IDE_read_sector((void *)p1,sector_num);
/*then the second.*/
		u8 * p2 = (u8 *)&g_head + 512;
		IDE_read_sector((void *)p2,sector_num+1);
/*!attention here, p and sect are pointers,we SHOULD NOT use (void*)p , but should use (void *)p */		
		kprintf("\n%d:",nr_group);
		kprintf(" group[ ] s_free:%d [0]%d [1]%d [2]%d ...[%d]%d ", g_head.s_free, \
			g_head.s_free_blk_nr[0],g_head.s_free_blk_nr[1],g_head.s_free_blk_nr[2],\
			g_head.s_free-1,g_head.s_free_blk_nr[g_head.s_free-1]);
	}
}
