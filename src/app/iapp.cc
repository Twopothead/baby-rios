#include <rios/app/iapp.h>
#include <rios/app/mkdir.h>
#include <rios/grouping.h>
#include <rios/console.h>
#include <rios/bitmap.h>
#include <rios/string.h>
#define get_path_basename(p)  get_basename(p,'/')
#define get_cmd_basename(p)   get_basename(p,' ')
void info_service(char * cmd_buffer)
{
	if(start_with(cmd_buffer,"info superblock")){
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
	if(start_with(cmd_buffer,"info grouping")){
		//visit_all_free_blks();
        free_blk_traverse();
	}
    if(start_with(cmd_buffer,"info constants")){
                kprintf("\nNR_DATA_BLK(rios_superblock):%d.",NR_DATA_BLK(rios_superblock));

    }
    if(start_with(cmd_buffer,"info debug")){
                kprintf(" \n DATA_BLK_NR_TO_SECTOR_NR(nr_blk): \n\
     NR_DATA_BLK(rios_superblock)+SECTORS_EACH_DATA_BLK*(nr_blk-1)\n\
 NR_DATA_BLK(rios_superblock):%d, SECTORS_EACH_DATA_BLK:%d \n",NR_DATA_BLK(rios_superblock),SECTORS_EACH_DATA_BLK);
    }
    if(start_with(cmd_buffer,"info disk")){
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
     NR_INODE_BLK:     %d,\n\
     NR_DATA_BLK:      %d,\n",
     NR_BOOT_BLK(rios_superblock),NR_SUPER_BLK(rios_superblock),\
     NR_ZONE_MAP_BLK(rios_superblock),NR_INODE_MAP_BLK(rios_superblock),\
     NR_INODE_BLK(rios_superblock),NR_DATA_BLK(rios_superblock));
    }    

}

void ls_service(char* cmd_buffer){
    if(cmd_matching((char*)cmd_buffer,"ls /")){
        dir_root();
    }else{
        ls();
    }

}
extern struct task_struct * current;
extern struct m_inode * iroot;
struct m_inode _work_inode;
struct m_inode _s_ino;
void mkdir_service(char* cmd_buffer,int cmd_buffer_index){
    char tmp[80*25];char name[50];
    strcpy(tmp,cmd_buffer); 
    // struct m_inode * saved_pwd = current->pwd;
    _s_ino = *current->pwd;
    if(equal_to(tmp,"mkdir /")){
        *(current -> pwd) = *iroot;
        kprintf("\n root directory exists.");
        return;
    }else{ 
            char * thisname = (char *)NULL;thisname = tmp;
            char * basename = (char *)get_path_basename(get_cmd_basename(tmp));
            char * _s_cd = strtok((char*)tmp,(char *)" ");
            if(strlen(basename)==0)_panic(" FBI WARNNING:PLEASE INPUT A VALID DIR NAME!!!");
            if(thisname == basename){
                kprintf("\n invalid command.");
            }else if(!basename){
                kprintf("\n please input a valid directory name.");
            }else{ 
    
                while(thisname!= basename){
                    thisname = strtok((char*)NULL,(char *)"/");
                    kprintf("\n Creating directory %s ...",thisname);
                    mkdir(thisname,DIR_FILE);
/*ok. let pwd temporarily point to it, and make 'mkdir -p' work smoothly. */                    
                    iget(&_work_inode,get_dir(thisname));
                    * current->pwd = _work_inode;
/*This is WRONG!!!:iget(current->pwd,get_dir(thisname));*/
                 }
                
            }
    }
    *current->pwd = _s_ino;
    return;
}


void cd_service(char* cmd_buffer,int cmd_buffer_index){
    char tmp[80*25];char name[50];
    strcpy(tmp,cmd_buffer);
    if(equal_to(tmp,"cd /")){
        *(current -> pwd) = *iroot;
/* a little tricky here,current->pwd always points to ipwd,
 * what will be changed is its contents,not the pointer itself.
 */        
    }else if(equal_to(tmp,"cd .")){
                ;
    }else if(equal_to(tmp,"cd ..")){
          iget(&_work_inode,get_dir((char *)".."));
          * current->pwd = _work_inode;
    }else{ 
            char * thisname = (char *)NULL;thisname = tmp;
            char * basename = (char *)get_path_basename(get_cmd_basename(tmp));
            char * _s_cd = strtok((char*)tmp,(char *)" ");
            if(strlen(basename)==0)_panic(" FBI WARNNING:PLEASE INPUT A VALID DIR NAME!!!");
            if(thisname == basename){
                kprintf("\n invalid command.");
            }else if(!basename){
                kprintf("\n please input a valid directory name.");
            }else{ 
                while(thisname!= basename){
                    thisname = strtok((char*)NULL,(char *)"/");
                    kprintf("\n cd %s",thisname);
                    iget(&_work_inode,get_dir(thisname));
                    * current->pwd = _work_inode;
/* This is WRONG!!!:iget(current->pwd,get_dir(thisname));*/
                 }
            }
    }

}

 void pwd_service(){
/*This function has been merged to 'print_pwd' */    
    struct m_inode tmp_node = * current->pwd;
    int s[100]={0};int _index = 0;
    while(current->pwd->i_ino != current->root->i_ino){
        int part_ino = current->pwd->i_ino;s[_index++]=part_ino;
        iget(current->pwd,get_dir(".."));/* switch to parrent folder */
        kprintf("\n%s",get_dir_name(part_ino));
    }
    * current->pwd = * current->root;
    nextline();
    for(int i=_index;i>=0;i--){
        kprintf(" %d ",s[i]);
        if(s[i] == current->root->i_ino){
            ;
        }else if(get_dir_name(s[i])!=(char*)NULL)kprintf("/%s",get_dir_name(s[i]));
        iget(current->pwd,s[i]);
    }
    * current->pwd = tmp_node;
 }
void msg_info_superblock()
{
	nextline();
	kprintf("fuck.\n");
}
