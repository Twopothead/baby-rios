#include <rios/fs.h>
void get_file_attrib(m_inode *fd)
{
	if(fd -> i_mode == NORMAL_FILE)
		print(" Normal file. ");
	else if(fd -> i_mode == DIR_FILE)
		print(" dir file.");
}

void init_fs()
{
	check_rifs();
	// for(int i=0;i<100;i++)
	// 	kprintf(" %d ",new_inode());
	// for(int i=34;i<40;i++)free_inode(i);

}