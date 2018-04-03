#include <rios/app/iapp.h>
#include <rios/memory.h>
#include <rios/file.h>
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
	int fd = open(name);if(fd==-1)return;
	int contents_len = current->filp[fd]->f_inode->i_size;
	if( current->filp[fd]->f_inode->i_size==0){
		kprintf("fd:%d,%d",fd,current->filp[fd]->f_inode->i_zone[0]);
		kprintf("\n cat: '%s': not a valid file.",name);
		return;
	}
	int tmp =current->filp[fd]->f_inode->i_ino;
/*kprintf("i_ino:%d",tmp);*/
	char * cat_contents = (char *)kmalloc(contents_len+512);
	read(fd, (void *)cat_contents, contents_len);
	 

/* @ 'cat' slow version without any optimization: kprintf*/
	/*kprintf("\n%s",cat_contents);*/


/* actually, ' kprintf("\n%s",str) ' is ok, but that will be too slow,(untolerantable :( )
 * therefore,I have to cut them and print them one after another.
 * then the speed will be tolerantable.
 */	
/* @ 'cat' fast version after optimization: cut cat_contents into pieces and print it */	
	int len = strlen(cat_contents);
	char cat_buf[80*25+1]={0};char *pch=cat_contents;
	int times = len/(80*25);
	if(times*80*25==len){;}else times = times+1;
	kprintf("\n");
	if(times==1||times==0){

			memset(cat_buf,0x00,sizeof(cat_buf));
			memcpy(cat_buf,cat_contents,sizeof(cat_buf));
			cat_buf[80*25]='\0';
			kprintf("%s",cat_buf);
	}else{
		for(int i=0;i<times;i++){
			memset(cat_buf,0x00,sizeof(cat_buf));
			memcpy(cat_buf,pch,sizeof(cat_buf));
			pch+=80*25;
			cat_buf[80*25]='\0';
			kprintf("%s",cat_buf);
		}
	}
/* just for speed of printing to screen */

	Ri_free(cat_contents,contents_len+512),cat_contents=(char*)NULL;
/*kprintf("fd:%d,%d",fd,current->filp[fd]->f_inode->i_zone[0]);*/
	return;
}

void slow_cat(const char *name,u8 mode)
{
	int fd = open(name);if(fd==-1)return;
	int contents_len = current->filp[fd]->f_inode->i_size;
	if( current->filp[fd]->f_inode->i_size==0){
		kprintf("fd:%d,%d",fd,current->filp[fd]->f_inode->i_zone[0]);
		kprintf("\n cat: '%s': not a valid file.",name);
		return;
	}
	int tmp =current->filp[fd]->f_inode->i_ino;
/*kprintf("i_ino:%d",tmp);*/
	char * cat_contents = (char *)kmalloc(contents_len+512);
	read(fd, (void *)cat_contents, contents_len);
	
/* @ 'cat' slow version without any optimization: kprintf*/
	kprintf("\n%s",cat_contents);

	Ri_free(cat_contents,contents_len+512),cat_contents=(char*)NULL;
/*kprintf("fd:%d,%d",fd,current->filp[fd]->f_inode->i_zone[0]);*/
	return;
}

void slow_cat_service(char* cmd_buffer,int cmd_buffer_index){
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
	                slow_cat(basename,NORMAL_FILE);        
	            }
	    }
	    iget(current->pwd,before_ino);
}
