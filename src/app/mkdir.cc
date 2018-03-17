#include <rios/app/mkdir.h>
extern struct m_inode iroot;
void mkdir(const char *name,u8 mode){
	struct m_inode newinode;
	newinode.i_ino =  new_inode();		/*allocate inode number*/
	newinode.i_zone[0] = (u16)new_block();	/*allocate block number*/
	newinode.i_size = 1 * sizeof(struct dir_entry);
	newinode.i_mode = DIR_FILE;/* mode */
	iput(&newinode,newinode.i_ino);
	struct dir_entry *de = (struct dir_entry *)NULL;
	u8 sector[512]={0};
/*currently, "namei" hasn't been implemented,i will mkdir under '/' root directory.   */
	IDE_read_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(iroot.i_zone[0]));	
	de = (struct dir_entry*)sector; 
	for(int i=0;i<iroot.i_size/sizeof(struct dir_entry);i++)de++;	/*point to correct position*/
/*we should control the length of dir name,otherwise may run into problem*/
	if( strlen(name) > MAX_NAME_LEN)_panic("FBI WARNNING:length of dir name must under 14 chars!\n halt...");//MAX_NAME_LEN	
	strcpy((char *)de->name,name);de->inode = newinode.i_ino;
	IDE_write_sector((void *)&sector, DATA_BLK_NR_TO_SECTOR_NR(iroot.i_zone[0]));	
/*ok, update current directory file's filesize, because we add a record.*/	
	iroot.i_size += newinode.i_size;
	iput(&iroot,iroot.i_ino);
}