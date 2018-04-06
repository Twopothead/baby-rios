#include <rios/fs.h>
#include <rios/app/iapp.h>
#include <rios/file.h>

extern struct m_inode iroot;
extern struct task_struct * current;
void get_file_attrib(m_inode *fd)
{
	if(fd -> i_mode == NORMAL_FILE)
		print(" Normal file. ");
	else if(fd -> i_mode == DIR_FILE)
		print(" dir file.");
}

extern struct file file_table[NR_FILE];
// extern struct active_inode_table_nr active_inode_table;
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
/* first.make some directories */	
		kprintf("\n  setting up file system, waiting...");
		silent_mkdir((char*)"mkdir /usr",strlen("mkdir /usr"));
		silent_cd((char*)"cd /usr",strlen("cd /usr"));	
		#define _silent_mkdir(cmd) 	silent_mkdir((char*) cmd, strlen(cmd))
		#define _silent_cd(cmd)		silent_cd((char*) cmd, strlen(cmd))
		_silent_mkdir("mkdir /bin"),_silent_mkdir("mkdir /lib"),_silent_mkdir("mkdir /man"),_silent_mkdir("mkdir /tmp");
		_silent_cd("cd /");_silent_mkdir("mkdir /lib");
		_silent_mkdir("mkdir /dev/tty"),_silent_mkdir("mkdir /home/qiuri"),_silent_mkdir("mkdir /var");
		_silent_mkdir("mkdir /etc"),_silent_cd("cd /etc");_silent_mkdir("mkdir /getty"),_silent_mkdir("mkdir /passwd");
		_silent_cd("cd ..");
/* Then create file write to file*/	
		int contents_fd = simple_creat("jane.txt",NORMAL_FILE);
		int hamlet_fd = simple_creat("hamlet.txt",NORMAL_FILE);
/* 'create' will do the 'open' job, so we do not need to 'open' */
		#include <rios/hamlet.txt>
		int hamlet_len = strlen(hamlet);
	        write(hamlet_fd, (void *)hamlet, hamlet_len);	
	        char *hamlet_buf = (char *)kmalloc(hamlet_len+512);
	        read(hamlet_fd, (void *)hamlet_buf,hamlet_len);
	        // kprintf("\n%s",hamlet_buf);
	        Ri_free(hamlet_buf,hamlet_len+512),hamlet_buf=(char*)NULL;
	        close(hamlet_fd);
		#include <rios/Jane_Eyre.txt>
		/*kprintf("fd%d",contents_fd);*/
		int contents_len = strlen(Jane_Eyre);
		//contents_len = 511400;
		//contents_len =1020*512;/*以前不能支持超大文件,这只用于调试,以后要注释掉*/
		//contents_len =20*512;
		//kprintf("\n%d",contents_len);/*以前不能支持超大文件*/
	        write(contents_fd, (void *)Jane_Eyre, contents_len);	
	        char *contents = (char *)kmalloc(contents_len+512);
	        read(contents_fd, (void *)contents, contents_len);
	        Ri_free(contents,contents_len+512),contents=(char*)NULL;
	        close(contents_fd);
	        msg_setup_fs();
	}
}


