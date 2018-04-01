#ifndef _FILE_H
#define _FILE_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <rios/file.h>
#include <rios/sched.h>
#include <rios/bitmap.h>
#define MAX_ACTIVE_INODE 64
/* file_table.cc */
extern struct file file_table[NR_FILE];	/* system-wide file table */
/* kmain.cc */
extern struct task_struct * current;	/* 'current' contains  user-wide file table*/
struct active_inode_table{
	struct m_inode inode_table[MAX_ACTIVE_INODE];
};
int search_file_table(char * name);
int simple_creat(const char *name,u8 mode);
int write(int fd, void *buffer, int length);
int read(int fd, void *buffer, int length);
int open(const char *name);
int close(int fd);
int get_zone_blks(int sectors);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif