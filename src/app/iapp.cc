#include <rios/app/iapp.h>
#include <rios/grouping.h>
#include <rios/console.h>
#include <rios/bitmap.h>
void info_service(char * cmd_buffer)
{
	if(cmd_matching((char*)cmd_buffer,"info superblock")){
		union Super_Block_Sect *sb = get_super();
		kprintf("\n info about superblock :\n\
        u16 s_ninodes                                  :%d\n\
        u16 s_capacity_blks                            :%d\n\	
        u16 s_startsect                                :%d\n\
        u16 s_zone_bitmap_blks (not used any more, bitmap => grouping)\n\		
        u16 s_inode_bitmap_blks                        :%d\n\	
        u16 s_inode_blks                               :%d\n\
        u16 s_firstdatazone                            :%d\n\
        u16 s_specific_blk_nr                          :%d\n\
        u16 s_magic                                    :0x%x\n",
        sb->s_ninodes,sb->s_capacity_blks,sb->s_startsect,\
        sb->s_zone_bitmap_blks,sb->s_inode_blks,sb->s_firstdatazone,\
        sb->s_specific_blk_nr,sb->s_magic);
	}
	if(cmd_matching((char*)cmd_buffer,"info grouping")){
		visit_all_free_blks();
	}
    if(cmd_matching((char*)cmd_buffer,"info constants")){
                kprintf("\nNR_DATA_BLK(rios_superblock):%d.",NR_DATA_BLK(rios_superblock));
            
    }
    if(cmd_matching((char*)cmd_buffer,"info debug")){
                kprintf(" \n DATA_BLK_NR_TO_SECTOR_NR(nr_blk): \n\
     NR_DATA_BLK(rios_superblock)+SECTORS_EACH_DATA_BLK*(nr_blk-1)\n\
 NR_DATA_BLK(rios_superblock):%d, SECTORS_EACH_DATA_BLK:%d \n",NR_DATA_BLK(rios_superblock),SECTORS_EACH_DATA_BLK);
    }
}

void ls_service(char* cmd_buffer){
    if(cmd_matching((char*)cmd_buffer,"ls /")){
        dir_root();
    }

}
void msg_info_superblock()
{
	nextline();
	kprintf("fuck.\n");
}
