#include <rios/app/iapp.h>
#include <rios/app/mkdir.h>
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
    if(cmd_matching((char*)cmd_buffer,"info disk")){
                kprintf(" \n\
     |________|________|_______|_______|________|___ __ __ __ __| \n\
     |________|________|_______|_______|________|___ __ __ __ __| \n\
 bootsector       |      bmap    imap    inodes   DATA                \n\
             super_block                                                \n\
     NR_BOOT_BLK:      %d,\n\
     NR_SUPER_BLK:     %d,\n\
     NR_ZONE_MAP_BLK:  %d,\n( At Prof.Jiang's request,for free space management,we will use grouping stragety instead of bitmap.\
therefore,we will not use this field(ZONE_MAP_BLK) anymore. Qiuri )\n\
     NR_INODE_MAP_BLK: %d,\n\
     NR_DATA_BLK:      %d,\n",
     NR_BOOT_BLK(rios_superblock),NR_SUPER_BLK(rios_superblock),\
     NR_ZONE_MAP_BLK(rios_superblock),NR_INODE_MAP_BLK(rios_superblock),\
     NR_DATA_BLK(rios_superblock));
    }    

}

void ls_service(char* cmd_buffer){
    if(cmd_matching((char*)cmd_buffer,"ls /")){
        dir_root();
    }

}
void mkdir_service(char* cmd_buffer,int cmd_buffer_index){
    if(cmd_matching((char*)cmd_buffer,"mkdir /")){
       mkdir("fuck_your_mather",DIR_FILE);
    }

}
void msg_info_superblock()
{
	nextline();
	kprintf("fuck.\n");
}
