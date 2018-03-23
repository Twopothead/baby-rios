#include <rios/file.h>
#include <rios/app/iapp.h>
extern struct file file_table[NR_FILE];
struct active_inode_table active_inode_table;
int get_active_inode_table_nr(){
	for(int i=0;i<MAX_ACTIVE_INODE;i++){
		if(active_inode_table.inode_table[i].i_size == 0){
/* ok, we can insert our record here. */			
			return i;
		}else {
			return -1;
		}
	}
}

/* 'simple_creat' is simple, it create a new file under current working directory. */
int simple_creat(const char *name,u8 mode)
{

	int fd;
/* find a blank entry in process's file descriptor table */	
	for(fd=0 ; fd <NR_OPEN ; fd++ ){
		if(!current->filp[fd])
			break;
	}
	if(fd>=NR_OPEN)
		return -1;/* overflow */
/* find a blank entry in system file table */	
	file * p_ft = file_table;
	int i;
	for(i=0;i<NR_FILE;i++,p_ft++){
		if(!p_ft->f_count) break;
	}
	if(i>=NR_FILE){
		kprintf("\n failed to create.");
		return -1;
	}
	int valid_file_table_nr = i;

/* ok, let filp[fd] points to a entry in file table,and let f_count+=1 */
(current->filp[fd]=p_ft)->f_count++;	
/* let a 'file' in file_table point to empty inode's space */	
	int active_inode_table_nr=get_active_inode_table_nr();
	file_table[valid_file_table_nr].f_inode = &active_inode_table.inode_table[active_inode_table_nr];
	#define p_valid_inode file_table[valid_file_table_nr].f_inode 
	p_valid_inode->i_ino =  new_inode();			/* allocate inode number */
	p_valid_inode->i_zone[0] = (u16)new_block();		/* allocate block number */
	p_valid_inode->i_size = _BLOCK_SIZE ; 			/* a data block is 2 sectors,1024B*/
	p_valid_inode->i_mode = NORMAL_FILE;/* mode */
	iput(p_valid_inode,p_valid_inode->i_ino);
	struct dir_entry *de = (struct dir_entry *)NULL;
	u8 sector[512]={0};
/*it will mkdir under current directory.   */
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	de = (struct dir_entry*)sector; 
/* in case that two directory have the same name */
	if(get_dir((char *)name)!=-1){
		kprintf("\n WARNING:a file with that name already exists.");
		return -2;
	}
	for(int i=0;i<current->pwd->i_size/sizeof(struct dir_entry);i++)de++;	/*point to correct position*/
/*we should control the length of file name,otherwise may run into problem*/
	if( strlen(name) > MAX_NAME_LEN)_panic("FBI WARNING:length of dir name must under 14 chars!\n halt...");//MAX_NAME_LEN	
	strcpy((char *)de->name,name);de->inode = p_valid_inode->i_ino;
	IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
/*ok, update current directory file's filesize, because we add a record.*/	
	current->pwd->i_size += 1 * sizeof(struct dir_entry);	/* add a new file, a new dir entry*/
	iput(current->pwd,current->pwd->i_ino);
	return fd;
}

/* open is stupid, it will open a file under current directory,
 * and return with a valid file descriptor.
 */
int open(const char *name){
	int fd,i;
	int ino = get_dir((char *)name);
	if(ino==-1)kprintf("\n open: '%s':no such file or directory.",name);
/* search the active inode table, if it's not in this table,just copy.*/	
	for(i=0;i<MAX_ACTIVE_INODE;i++){
		if(active_inode_table.inode_table[i].i_ino != ino)
			break;
	}
	int active_inode_table_nr=get_active_inode_table_nr();
	if(i>=MAX_ACTIVE_INODE){/*no record found,need to copy. */
		iget(&active_inode_table.inode_table[active_inode_table_nr],ino);
	}else{/* it has been opened before, no nead to copy */
	      /* here,we get i */
	}
/* find a blank entry in process's file descriptor table */	
	for(fd=0 ; fd <NR_OPEN ; fd++ ){
		if(!current->filp[fd])
			break;
	}
	if(fd>=NR_OPEN)
		return -1;/* overflow */
/* find a blank entry in system file table */	
	file * p_ft = file_table;
	int j;
	for(j=0;j<NR_FILE;j++,p_ft++){
		if(!p_ft->f_count) break;
	}
	if(j>=NR_FILE){
		kprintf("\n failed to create.");
		return -1;
	}
	int valid_file_table_nr = j;
/* ok, let filp[fd] points to a entry in file table,and let f_count+=1 */
(current->filp[fd]=p_ft)->f_count++;
	return fd;
}

int read(int fd, void *buffer, int length){
/* assume we have 'open'ed before we 'read' */
	file * p_ft =current->filp[fd];
	u8 sector[512]={0};
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[0]));
	memcpy(buffer,sector,512);

}

int write(int fd, void *buffer, int length){
/* assume we have 'open'ed before we 'read' */
	file * p_ft =current->filp[fd];
	u8 sector[512]={0};
	memcpy(sector,buffer,512);
	int total_sectors = (length+511)/512;
	IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[0]));

}

int close(int fd){
/*clear a entry in each two tables*/

	// current->filp[fd]->f_count--;
	// *current->filp[fd]->f_inode.i_count--;
	// if(*current->filp[fd].i_count==0)
	// if(current->filp[fd]->f_count==0){
	// 	memset(current->filp[fd],0x00,sizeof(struct file));
	// }

	// memset(&current->filp[fd],0x00,sizeof(struct *file));
	return 0;
}
// void get_(char * name){
// 	file * p_ft = file_table;
// 	int i;
// 	for(i=0;i<NR_FILE;i++,p_ft++){
// 		if(p_ft->f_inode) break;
// 	}
// 	if(i>=NR_FILE){// kprintf("\n fail");
// 		return -1;
// 	}
// 	return i;
// }
/* in file system management,there are 3 important tables,
 * they are active inode table, system file table and process's file descriptor table
 * active inode table copy inodes from disk,system file table restore files. struct 'file'
 * contains inode pointor that point to active inode table.
 * what's in user's file descriptor table is indexes.
 *     usr(process)                 |       sys file table      |     active inode table
 *     *filp[fd]                    |        files[]            |      inodes[]
 *                                  |        file{*inode,etc};  |
 *
 *    *filp -> files[] ->inodes[](in memory) => inodes[](on disk) 
 */