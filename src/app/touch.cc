#include <rios/app/iapp.h>
#include <rios/file.h>
extern struct m_inode iroot;
extern struct task_struct * current;
extern struct active_inode_table active_inode_table;
/* 'touch' is stupid ,it will create a new file under current directory */
void touch(const char *name,u8 mode)
{
	simple_creat(name,mode);
}
