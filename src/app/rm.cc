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
	    if(equal_to(tmp,"rmdir /")){
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
	                cat(basename,NORMAL_FILE);        
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
		kprintf("\n cat: '%s': not a valid file.",name);
		return;
	}
	char * cat_contents = (char *)kmalloc(contents_len+512);
	read(fd, (void *)cat_contents, contents_len);
	kprintf("\n%s",cat_contents);
	Ri_free(cat_contents,contents_len+512),cat_contents=(char*)NULL;
	return;
}