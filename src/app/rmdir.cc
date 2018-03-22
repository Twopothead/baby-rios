#include <rios/app/iapp.h>
extern struct m_inode iroot;
extern struct task_struct * current;
/* 'rmdir' is stupid ,it will remove a directory under current directory */
void rmdir(const char *name,u8 mode)
{
	if(get_dir((char*)name)==-1){
		kprintf("\n rmdir: failed to remove '%s': No such file or directory",name);
		return;
	}
	struct m_inode rminode;iget(&rminode,get_dir((char*)name));
	int rminode_ino = rminode.i_ino;
	free_inode(rminode.i_ino);			/* free inode number */
	free_block(rminode.i_zone[0]); 			/* free block number */
	memset(&rminode,0x00,sizeof(struct m_inode));
	iput(&rminode,rminode_ino);			/* flush that area */
	struct dir_entry *de = (struct dir_entry *)NULL;u8 sector[512]={0};
/*it will mkdir under current directory.   */
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(current->pwd->i_zone[0]));	
	de = (struct dir_entry*)sector; 
	
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
}