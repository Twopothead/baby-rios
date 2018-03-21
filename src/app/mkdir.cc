#include <rios/app/mkdir.h>
extern struct m_inode iroot;
extern struct task_struct * current;
/* 'mkdir' is stupid ,it will create directory under current directory */
void mkdir(const char *name,u8 mode){
	struct m_inode newinode;
	newinode.i_ino =  new_inode();			/* allocate inode number */
	newinode.i_zone[0] = (u16)new_block();		/* allocate block number */
	newinode.i_size = 2 * sizeof(struct dir_entry); /* at least '.' and '..' */
	newinode.i_mode = DIR_FILE;/* mode */
	iput(&newinode,newinode.i_ino);
	struct dir_entry *de = (struct dir_entry *)NULL;
	u8 sector[512]={0};
/*it will mkdir under current directory.   */
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	de = (struct dir_entry*)sector; 

/* in case that two directory have the same name */
	if(get_dir((char *)name)!=-1){
		kprintf("\n WARNING:a folder with that name already exists.");
		return;
	}
	for(int i=0;i<current->pwd->i_size/sizeof(struct dir_entry);i++)de++;	/*point to correct position*/
/*we should control the length of dir name,otherwise may run into problem*/
	if( strlen(name) > MAX_NAME_LEN)_panic("FBI WARNING:length of dir name must under 14 chars!\n halt...");//MAX_NAME_LEN	
	strcpy((char *)de->name,name);de->inode = newinode.i_ino;
	IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
/*ok, update current directory file's filesize, because we add a record.*/	
	current->pwd->i_size += 1 * sizeof(struct dir_entry);	/* add a new dir*/
	iput(current->pwd,current->pwd->i_ino);
/* handle . and .. here */
	struct m_inode *saved_pino = current->pwd;
	current->pwd = &newinode;
	memset(sector,0x00,512);
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	for(int i=0;i<current->pwd->i_size/sizeof(struct dir_entry);i++)de++;	/*point to correct position*/
	de = (struct dir_entry*)sector; 
	strcpy((char *)de->name,(char*)".");de->inode = newinode.i_ino;
	de++;
	strcpy((char *)de->name,(char*)"..");de->inode = saved_pino ->i_ino;
	de++;
	IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	iput(current->pwd,current->pwd->i_ino);
	current->pwd = saved_pino ;
}

/* 'get_dir' is also stupid. it gets dir by name(not full pathname) under current directory*/
int get_dir(char * partname){
/* before we haven't update pwd here,than pwd is not sync with contents on disk*/	
	int dir_total = current->pwd->i_size/(sizeof(struct dir_entry));
	struct dir_entry * de = (struct dir_entry *)NULL;u8 sector[512]={0};
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	de = (struct dir_entry*)sector; 
	for(int i=0;i<dir_total;i++){
		if(equal_to((char *)de->name,partname))return de->inode;
		de++;
	}
	return -1;
}

char * get_dir_name(u32 inode){
	int dir_total = current->pwd->i_size/(sizeof(struct dir_entry));
	struct dir_entry * de = (struct dir_entry *)NULL;u8 sector[512]={0};
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	de = (struct dir_entry*)sector; 
	for(int i=0;i<dir_total;i++){
		if( de->inode == inode )return (char *)de->name;
		de++;
	}
	return (char *)NULL;
}