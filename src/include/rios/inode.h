#ifndef _INODE_H
#define _INODE_H
#ifdef __cplusplus
#include <rios/type.h>
extern "C" {
#endif /* __cplusplus */
#include <rios/fs.h>
#include <asm/x86.h>
#include <rios/dpt.h>
#include <rios/fs.h>
#include <rios/hd.h>
#include <rios/ramdisk.h>
#include <rios/grouping.h>
#include <rios/syscall.h>
#include <rios/fs.h>
#include <rios/console.h>
#include <rios/ramdisk.h>
#include <asm/x86.h>
#include <rios/format.h>
extern union Super_Block_Sect rios_superblock;
int new_inode();
void free_inode(int inode);
struct m_inode * iget(struct m_inode * inode, int nr);
void iput(struct m_inode * inode, int nr);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif