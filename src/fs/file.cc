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
	if(ino==-1){
		kprintf("\n open: '%s':no such file or directory.",name);
		return -1;
	}
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
		return i;
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
	u8 sector[512]={0};int buffer_offset=0;
	int total_sectors = (length+511)/512;
	
	if(total_sectors<=7*SECTOR_PER_BLOCK){
		for(int i=0; i<total_sectors; i++){
			int blk_i = get_zone_blks(i+1)-1;
			if(i%2==0){
				IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i]));
			}else{
				IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i])+1);	
			}
			memcpy(buffer+buffer_offset,sector,512);buffer_offset+=512;
		}
	}else if(total_sectors<=7*SECTOR_PER_BLOCK+512*SECTOR_PER_BLOCK){
/* zone[0~6]: direct block 直接寻址,大概7kB*/		
		for(int i=0; i<7*SECTOR_PER_BLOCK; i++){
			int blk_i = get_zone_blks(i+1)-1;
			if(i%2==0){
				IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i]));
			}else{
				IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i])+1);	
			}
			memcpy(buffer+buffer_offset,sector,512);buffer_offset+=512;
		}
/* zone[7]:   single indirect block 一次间址,大概五百kB*/
		u8 two_sectors[1024]={0};/*load indexs in zone[7] to memory 'two_sectors'*/
		IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7]));
		IDE_read_sector((void *)(two_sectors + 512), DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7])+1);
		u16 * pzone =(u16 *)&two_sectors;

//for(int j=0;j<263;j++){kprintf("%d ",pzone[j]);;}kprintf("dddddd%d",pzone[262]);_panic("kaj");
		for(int i = 7*SECTOR_PER_BLOCK;i<total_sectors;i++){//[7*2+1,7*2+512*2]
			memcpy(sector,buffer+512*i,512);
			int blk_i = get_zone_blks(i+1)-1;
			u16 zone_index = pzone[blk_i-7];/*zone[0~6]*/
/* MAKE SURE that zone_index!=0. assert(zone_index!=0)*/			
			if(zone_index==0) {
				// zone_index = new_inode();
				kprintf("\n%d\n%d\n",p_ft->f_inode->i_zone[7],blk_i);
				kprintf("%d\n%d",blk_i,i);
				// kprintf("\ntovisit%d\n",p_ft->f_inode->i_zone[7]);
				// u8 two_sectors[1024]={0};
				IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7]));
				IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7])+1);
				u16 *ppp=(u16*)&two_sectors;
				kprintf("");kprintf("%d",ppp[0]);
				kprintf("\npzone%d,ppp:%d\n",pzone[0],ppp[0]);
				for(int j=0;j<512;j++)kprintf("%d ",pzone[j]);
					kprintf("\n");
				// for(int j=7;j<blk_i-7;j++)kprintf("%d ",pzone[blk_i-7]);
			_panic("FBI WARNNING:zone_index should not be zero!!!");/* will destory root directory!*/
			
			}
			if(i%2==0){
					IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index));
			}else{
					IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index)+1);
			}
			memcpy(buffer+buffer_offset,sector,512);buffer_offset+=512;

		}
	}


}

int get_zone_blks(int sectors){
	int tmp = sectors/SECTOR_PER_BLOCK;
	return (tmp*SECTOR_PER_BLOCK == sectors) ? (tmp):(tmp+1);
}

int write(int fd, void *buffer, int length){
/* assume we have 'open'ed before we 'read' */
	file * p_ft =current->filp[fd];
	u8 sector[512]={0};
	int total_sectors = (length+511)/512;
	int total_zone_blks=get_zone_blks(total_sectors);
	p_ft->f_inode->i_size = length; 
	iput(p_ft->f_inode,p_ft->f_inode->i_ino); 

	if(total_sectors<=7*SECTOR_PER_BLOCK){ 
		for(int i=0;i<total_sectors;i++){//[1,7*2]
				memcpy(sector,buffer+512*i,512);
				int blk_i = get_zone_blks(i+1)-1;
				if(p_ft->f_inode->i_zone[blk_i]==0){
		/* allocate new data block when writing */		
					p_ft->f_inode->i_zone[blk_i] = new_block();
					iput(p_ft->f_inode,p_ft->f_inode->i_ino); 	
				}
				if(i%2==0){
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i]));
				}else{
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i])+1);
				}
		}
	}else if(total_sectors<=7*SECTOR_PER_BLOCK+512*SECTOR_PER_BLOCK){/*u16 i_zone[10];*/
/* zone[0~6]: direct block */		
		for(int i=0;i<7*SECTOR_PER_BLOCK;i++){//[1,7*2]
				memcpy(sector,buffer+512*i,512);
				int blk_i = get_zone_blks(i+1)-1;
				if(p_ft->f_inode->i_zone[blk_i]==0){
		/* allocate new data block when writing */		
					p_ft->f_inode->i_zone[blk_i] = new_block();
					iput(p_ft->f_inode,p_ft->f_inode->i_ino); 	
				}
				if(i%2==0){
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i]));
				}else{
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[blk_i])+1);
				}
		}
/* zone[7]:   single indirect block*/

		if(p_ft->f_inode->i_zone[7]==0){/* allocate newblock for  zone[7] */
			p_ft->f_inode->i_zone[7] = new_block();
			iput(p_ft->f_inode,p_ft->f_inode->i_ino); 
		}

		u8 two_sectors[1024]={0};/*load indexs in zone[7] to memory 'two_sectors'*/
		IDE_read_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7]));
		IDE_read_sector((void *)(two_sectors+512), DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7])+1);
		u16 * pzone =(u16 *)&two_sectors;

		for(int i = 7*SECTOR_PER_BLOCK;i<total_sectors;i++){//[7*2+1,7*2+512*2]
			memcpy(sector,buffer+512*i,512);
			int blk_i = get_zone_blks(i+1)-1;
			u16 zone_index = pzone[blk_i-7];/*zone[0~6]*/
			if(zone_index==0){
				zone_index =(u16) new_block();pzone[blk_i-7]=zone_index;
				/* something similar to  'iput'*/
				IDE_write_sector((void *)two_sectors, DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7]));
				IDE_write_sector((void *)(two_sectors+512), DATA_BLK_NR_TO_SECTOR_NR(p_ft->f_inode->i_zone[7])+1);
				// IDE_write_sector((void *)&two_sectors, DATA_BLK_NR_TO_SECTOR_NR(zone_index));
				// IDE_write_sector((void *)(&two_sectors+512), DATA_BLK_NR_TO_SECTOR_NR(zone_index)+1);
				//for(int j=0;j<512;j++)kprintf("%d",pzone[j]);
				// kprintf("sss%d",pzone[0]);
				kprintf("\nnewbl:%d\nindex%d\n",zone_index,blk_i-7);
				// _panic("ddd");
			}
			if(i%2==0){
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index));
			}else{
					IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(zone_index)+1);
			}
		}
	}



}

int close(int fd){
/*clear a entry in each two tables*/
	struct file * filp;
	if (fd >= NR_OPEN)
		return -1;
	if (!(filp = current->filp[fd]))
		return -1;
	current->filp[fd]=(struct file*)0;
	if (filp->f_count == 0)
		_panic("close: file count is 0");
	if (--filp->f_count)
		return (0);
	iput(filp->f_inode,filp->f_inode->i_ino);
	return (0);
}

int search_file_table(char * name){
	file * p_ft = file_table;
	int i;
	for(i=0;i<NR_FILE;i++,p_ft++){
		if(p_ft->f_inode) break;
	}
	if(i>=NR_FILE){// kprintf("\n fail");
		return -1;
	}
	return i;
}

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
/*
zone[0~6]直接寻址: 
	每个块2扇区,7*(2*512)=7168B = 7kB
 length: 0~7*(2*512)B              [0,7168] B
 sector:length/512 	[1,7*2]

zone[7] 一次间址: 
	u16 zone[] u16 => 2B , 一个区可以存(2*512)/2=512 个扇区号,
	512 * (512*2) = 512kB.
 length: 7*(2*512)+1 ~ 7*(2*512)+512*(512*2) 
        [7169,531456]  B
 sector: ⌈length/512⌉    [7*2+1,7*2+512*2]
 	[15,1038]      

zone[8] 两次间址: 512 * 512 * (512 * 2)= 256 MB 
 length:  7*(2*512)+512*(512*2)+1~7*(2*512)+512*(512*2)+512*512*(512*2)
 	[531457,268966912] B
 sector:⌈length/512⌉     [7*2+512*2+1,]	
 	[1039,525326]

zone[9] 三次间址: 512 * 512 * 512 * (512 * 2) = 128 GB 
 length: 7*(2*512)+512*(512*2)+512*512*(512*2)+1 ~ \
 		7*(2*512)+512*(512*2)+512*512*(512*2)+512 * 512 * 512 * (512 * 2)
 	[268966913,137707920384] B
 */