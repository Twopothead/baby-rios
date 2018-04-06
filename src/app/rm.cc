#include <rios/app/iapp.h>
#include <rios/memory.h>
#include <rios/file.h>
extern struct m_inode iroot;
extern struct task_struct * current;
void rm_service(char* cmd_buffer,int cmd_buffer_index){
	 char tmp[80*25];char name[50];
	    strcpy(tmp,cmd_buffer); 
	    // struct m_inode * saved_pwd = current->pwd;
	    int before_ino = current->pwd->i_ino;
	    if(equal_to(tmp,"rm /")){
	          iget(&iroot,0);
	        *(current -> pwd) = iroot;
	        kprintf("\n root directory CANNOT be removed.");
	        return;
	    }else{ 
	            char * thisname = (char *)NULL;thisname = tmp;
	            char * basename = (char *)get_path_basename(get_cmd_basename(tmp));
	            char * _s_rm = strtok((char*)tmp,(char *)" ");
	            if(strlen(basename)==0)_panic(" FBI WARNING:PLEASE INPUT A VALID DIR NAME!!!");
	            if(thisname == basename){
	                kprintf("\n invalid command.");
	            }else if(!basename){
	                kprintf("\n please input a valid directory name.");
	            }else{ 
	/* currently, rmdir doesn't support 'rmdir -p dir1/dir2/dir3' :( */                
	                thisname = strtok((char*)NULL,(char *)"/");
	                rm(basename,NORMAL_FILE);        
	            }
	    }
	    iget(current->pwd,before_ino);
}

/* 'rm' is stupid ,it will remove the file under current directory */
void rm(const char *name,u8 mode)
{
	int fd = open(name);if(fd==-1)return;
	int contents_len = current->filp[fd]->f_inode->i_size;
	if( current->filp[fd]->f_inode->i_size==0){
		kprintf("\n rm: '%s': not a valid file.",name);
		return;
	}
	if(current->filp[fd]->f_inode->i_nlinks!=0)
		kprintf("\nrm: nlinks:%d",current->filp[fd]->f_inode->i_nlinks);
	u16 ino = current->filp[fd]->f_inode->i_ino;
	int length = current->filp[fd]->f_inode->i_size;
	close(fd);
	if(ino==0) _panic(" FBI WARNING:rm:i_ino = 0 !!! ");/* will destroy root directory */
/* free blocks and then free inode,finally remove this record from the directory */	
	u8 sector[512]={0};int buffer_offset=0;
	int total_sectors = (length+511)/512;
	struct m_inode rm_inode;
	iget(&rm_inode,ino);

	memset(sector,0x00,sizeof(sector));
	if(total_sectors<=7*SECTOR_PER_BLOCK){
/* @#0.1 zone[0~6]: direct block 直接寻址,大概7kB*/
		for(int i=0; i<total_sectors; i++){
			int blk_i = get_zone_blks(i+1)-1;
			if(i%2==0){
				IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[blk_i]));
			}else{
				IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[blk_i])+1);
				if(rm_inode.i_zone[blk_i]!=0)free_block(rm_inode.i_zone[blk_i]);	
			}
		}
	}else if(total_sectors<=7*SECTOR_PER_BLOCK+512*SECTOR_PER_BLOCK){
/* @#1.1 zone[0~6]: direct block 直接寻址,大概7kB*/		
		for(int i=0; i<7*SECTOR_PER_BLOCK; i++){
			int blk_i = get_zone_blks(i+1)-1;
			if(i%2==0){
				IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[blk_i]));
			}else{
				IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[blk_i])+1);	
				if(rm_inode.i_zone[blk_i]!=0)free_block(rm_inode.i_zone[blk_i]);
			}
		}
/*  #1.2 zone[7]:   single indirect block 一次间址,大概五百kB*/
		u8 two_sectors[1024]={0};/*load indexs in zone[7] to memory 'two_sectors'*/
		IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[7]));
		IDE_read_sector((void *)(two_sectors + 512), DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[7])+1);
		u16 * pzone =(u16 *)&two_sectors;/* that's right */

		for(int i = 7*SECTOR_PER_BLOCK;i<total_sectors;i++){//[7*2+1,7*2+512*2]
			int blk_i = get_zone_blks(i+1)-1;
			u16 zone_index = pzone[blk_i-7];/*zone[0~6]*/
/* MAKE SURE that zone_index!=0. assert(zone_index!=0)*/			
			if(zone_index==0) {
				_panic("FBI WARNNING:rm:zone_index should not be zero!!!");/* will destory root directory!*/			
			}
			if(i%2==0){
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index));
			}else{
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index)+1);
					if(zone_index!=0)free_block(zone_index);
			}
		}

		if(rm_inode.i_zone[7]!=0)free_inode(rm_inode.i_zone[7]);
	}
	else if(total_sectors<=7*SECTOR_PER_BLOCK+512*SECTOR_PER_BLOCK+512*512*SECTOR_PER_BLOCK){
/* @#2.1 zone[0~6]:  direct block 直接寻址,大概7kB*/	
		kprintf("\n  rm: removing direct blocks of file '%s'.",name);
		for(int i=0; i<7*SECTOR_PER_BLOCK; i++){
				int blk_i = get_zone_blks(i+1)-1;
				if(i%2==0){
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[blk_i]));
				}else{
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[blk_i])+1);	
					if(rm_inode.i_zone[blk_i]!=0)free_block(rm_inode.i_zone[blk_i]);
				}
		}
/*  #2.2 zone[7]  :  single indirect block 一次间址,大概五百kB*/
		kprintf("\n  rm: removing single indirect blocks of file '%s'.",name);
		u8 two_sectors[1024]={0};/*load indexs in zone[7] to memory 'two_sectors'*/
		IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[7]));
		IDE_read_sector((void *)(two_sectors + 512), DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[7])+1);
		u16 * pzone =(u16 *)&two_sectors;/* that's right */

		for(int i = 7*SECTOR_PER_BLOCK;i<7*SECTOR_PER_BLOCK+512*SECTOR_PER_BLOCK;i++){//[7*2+1,7*2+512*2]
			int blk_i = get_zone_blks(i+1)-1;
			u16 zone_index = pzone[blk_i-7];/*zone[0~6]*/
/* MAKE SURE that zone_index!=0. assert(zone_index!=0)*/			
			if(zone_index==0) {
				_panic("FBI WARNING:read:zone_index should not be zero!!!");/* will destory root directory!*/			
			}
			if(i%2==0){
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index));
			}else{
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index)+1);
					if(zone_index!=0)free_block(zone_index);
			}
		}
		for(int i=0;i<512;i++){
			if(pzone[i]!=0)
				free_block(pzone[i]);
		}
		if(rm_inode.i_zone[7]!=0)free_inode(rm_inode.i_zone[7]);
/*  #2.3 zone[8]  :  double indirect block 两次间址,支持大概256MB*/
		kprintf("\n  rm: removing double indirect blocks of file '%s'.",name);
		memset(two_sectors,0x00,sizeof(two_sectors));/*reuse that buffer*/
		memset(sector,0x00,sizeof(sector));
		if(rm_inode.i_zone[8]==0){/* allocate newblock for  zone[8] */
			_panic("FBI WARNING:rm:file's i_zone[8] is NOT allocated!!!"); 
		}
		/*load indexes in zone[8] to memory 'two_sectors'*/
		IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[8]));
		IDE_read_sector((void *)(two_sectors+512), DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[8])+1);
		u16 * p_zone = (u16 *)&two_sectors;
		u8 double_sectors[1024]={0};/* double indirect block buffer*/
		u16 * pd = (u16 *)&double_sectors;
		for(int i=7*SECTOR_PER_BLOCK+512*SECTOR_PER_BLOCK;i<total_sectors;i++){
			/* load single indirect block (zone[8]) to memory, two_sectolrs <= zone[8]  */			
			IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[8]));
			IDE_read_sector((void *)(two_sectors+512), DATA_BLK_NR_TO_SECTOR_NR(rm_inode.i_zone[8])+1);
			int blk_i = get_zone_blks(i+1)-1;
			u16 single_indirect_i =(blk_i-7-512)/512;/*zone[0~6]:7 zone[7]:512*/
			u16 double_indirect_i = (blk_i-7-512) -512*single_indirect_i;

			u16 si_zone_index = p_zone[single_indirect_i];
			if(si_zone_index ==0) _panic("FBI WARNING:read:file's si_zone_index has not been allocated!!!");
			/* get double indirect block from disk */
			IDE_read_sector((void *)double_sectors, DATA_BLK_NR_TO_SECTOR_NR(si_zone_index));
			IDE_read_sector((void *)(double_sectors+512), DATA_BLK_NR_TO_SECTOR_NR(si_zone_index)+1);
			/* ok, double indirect block is now loaded to double_sectors in memory */	
			u16 db_zone_index = pd[double_indirect_i];
  			if(db_zone_index==0) _panic("FBI WARNING:read:file's db_zone_index has not been allocated!!!");
			/*load file contents been from disk*/
			if(i%2==0){
				 IDE_read_sector((void *)&sector , DATA_BLK_NR_TO_SECTOR_NR(db_zone_index));
			}else{
				 IDE_read_sector((void *)&sector , DATA_BLK_NR_TO_SECTOR_NR(db_zone_index)+1);
				 if(db_zone_index!=0)free_block(db_zone_index);
			}
		}
		for(int i=0;i<512;i++){
			if(p_zone[i]!=0)
				free_block(p_zone[i]);
		}
		if(rm_inode.i_zone[8]!=0)free_inode(rm_inode.i_zone[8]);
	}
	else{
		kprintf("\n file size: %d Bytes.",length);
		_panic(" FBI_WARNING:rm:your file is TOO LARGE!!!");
	}

/* ok, after we have freed its data blocks, we can free its inode*/
	memset(&rm_inode,0x00,sizeof(rm_inode));
	iput(&rm_inode,ino);
	free_inode(ino); 
/* remove its infomation from the directory */
	memset(sector,0x00,sizeof(sector));
	struct dir_entry *de = (struct dir_entry *)NULL;
/*it will rm file under current directory.   */
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	de = (struct dir_entry*)sector; 
	/* in case that two directory have the same name */
	if(get_dir((char *)name)==-1){
		kprintf("\n WARNING:rm:the file '%s' does NOT exist.",name);
		return ;
	}
/*we should control the length of file name,otherwise may run into problem*/
	if( strlen(name) > MAX_NAME_LEN) 
		_panic("FBI WARNING:length of dir name must under 14 chars!\n halt...");//MAX_NAME_LEN		
	int i=0;
	for(i=0;i<current->pwd->i_size/sizeof(struct dir_entry);i++){
		if(equal_to((char *)de->name,name)) break ;
		de++;
	}	/*point to correct position*/
	memset(de,0x00,sizeof(struct dir_entry));
	struct dir_entry *de2 = (struct dir_entry *)NULL;
	if(i==current->pwd->i_size/sizeof(struct dir_entry)){
/* if it is the last one */		
		goto writeback;
	}
	de2=de; de2++;
	for(int j=0;j<current->pwd->i_size/sizeof(struct dir_entry)-i;j++){
		* de = * de2;
		de++;de2++;
	}
writeback:
	IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
/*ok, update current directory file's filesize, because we removed a record.*/	
	current->pwd->i_size -= 1 * sizeof(struct dir_entry);	/* remove a dir*/
	iput(current->pwd,current->pwd->i_ino);
	kprintf("\n  rm: file '%s' has been successfully removed.",name);
	return;
}