#include <rios/fs.h>
extern struct m_inode iroot;
extern struct task_struct * current;
void get_file_attrib(m_inode *fd)
{
	if(fd -> i_mode == NORMAL_FILE)
		print(" Normal file. ");
	else if(fd -> i_mode == DIR_FILE)
		print(" dir file.");
}

void init_fs()
{
	current -> root = & iroot;
	current -> pwd = & iroot;
	check_rifs();
}

int open(char * filenamep,u8 mode){

}
int close(unsigned int fd){
	struct file *filp;


}