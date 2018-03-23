#include <rios/app/iapp.h>
#include <rios/memory.h>
extern struct m_inode iroot;
extern struct task_struct * current;
void cat_service(char* cmd_buffer,int cmd_buffer_index){
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

/* 'cat' is stupid ,it will print contents of a new file under current directory */
void cat(const char *name,u8 mode)
{
	// int cat_fd = open(name);

	// cat_contents_len = 
	// char *cat_contents = (char *)kmalloc(cat_contents_len);
	// read(int fd, void *buffer, int length);
	// kfree_size(cat_contents,cat_contents_len);
	// kprintf("\nhello");
}