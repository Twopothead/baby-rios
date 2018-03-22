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
	setup_fs();

}

void setup_fs(){
	extern int new_rifs;
	if(new_rifs){/* the first time */
		silent_mkdir((char*)"mkdir /usr",strlen("mkdir /usr"));
		silent_cd((char*)"cd /usr",strlen("cd /usr"));	
		#define _silent_mkdir(cmd) 	silent_mkdir((char*) cmd, strlen(cmd))
		#define _silent_cd(cmd)		silent_cd((char*) cmd, strlen(cmd))
		_silent_mkdir("mkdir /bin"),_silent_mkdir("mkdir /lib"),_silent_mkdir("mkdir /man"),_silent_mkdir("mkdir /tmp");
		_silent_cd("cd /");_silent_mkdir("mkdir /lib");
		_silent_mkdir("mkdir /dev/tty"),_silent_mkdir("mkdir /home/qiuri"),_silent_mkdir("mkdir /var");
		_silent_mkdir("mkdir /etc"),_silent_cd("cd /etc");_silent_mkdir("mkdir /getty"),_silent_mkdir("mkdir /passwd");
		_silent_cd("cd ..");
	}
}


int open(char * filenamep,u8 mode){

}
int close(unsigned int fd){
	struct file *filp;


}