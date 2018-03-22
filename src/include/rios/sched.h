#ifndef _SCHED_H
#define _SCHED_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/type.h>
#include <rios/fs.h>
#include <rios/bitmap.h>
#define NR_OPEN 10
/* currently, we haven't really implement 'multitask', :(
 * but we need to use pwd,gid,etc. in development of file system,
 * hope someone will carry on with my work. 	Frank Curie.
 */
struct task_struct{
	u8 gid;
	u8 uid;
	struct m_inode * pwd;
	struct m_inode * root;
	struct file * filp[NR_OPEN];
/* this is user-wide file table */	
};
	
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif