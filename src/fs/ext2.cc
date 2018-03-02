/*
 * RiOS/include/fs/ext2.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
/**
 *TODO
 * Struct:
 * - ext2 inode
 * - ext2 superblock
 * - dentry
 * Functions:
 * - mkdir
 * - rmdir
 * - rename
 * - open
 * - delete
 * - cp
 */
/* VFS
 * - superblock => fs
 * - inode => store FCB
 * - dentry => directory
 * - file => file that has been opened 
 */
struct inode{

};
struct dentry{

};
struct file{

};
/*@mode :privilege level*/
int sys_mkdir(struct indoe *dir ,struct dentry *dentry, int mode){

}
int sys_rmdir(struct inode *dir ,struct dentry *dentry){

}

int sys_rename(struct inode *old_dir ,struct dentry *old_dentry, \
	struct inode *new_dir,struct dentry *new_dentry){

}

int sys_open(struct inode *inode ,struct file *fp){

}

int sys_delete_inode(struct inode *inode){
	
} 

void init_fs(){
	

}