/*
 * RiOS/src/kmain.c
 *
 * Copyright (C) 2018 qiuri (邱日)
 * Licence: GPL version 2  http://www.gnu.org/licenses/gpl.html
 */
#include <stdint.h>
#include <asm/x86.h>
#include <rios/type.h>
#include <rios/gdt_idt.h>
#include <rios/console.h>
#include <rios/irq.h>
#include <rios/trap.h>
#include <rios/memory.h>
#include <rios/ext2.h>
#include <rios/dpt.h>
#include <rios/serial.h>
#include <rios/syscall.h>
#include <rios/dpt.h>
#include <rios/fs.h>
#include <rios/app/mkdir.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern unsigned char *buffer_addr;
extern union Super_Block_Sect rios_superblock;
extern struct m_inode iroot;
u8 _global_buf[512]={0};
//unsigned char *buf=(unsigned char *)0x8888888;/*mm 0x666666*/

void RiOS_main(void)
{
	__asm__ __volatile__(
		"movw $0x075b,(0xb80a0)\n\t" 	
		"movw $0x075d,(0xb80aa)\n\t" 	
		"movl $0x0a4b0a4f,(0xb80a4)"
		);/* [ OK ]*/
	init_gdt();
	init_console();
	init_idt();
	init_8253();
	init_mm();
	init_Rishell(); 
	init_Exception();
	//sys_setup();
	init_hd();		/*DANGEROUS!*/
/* OK,here we are switching to hdb(hd1),(hd0 is your Udisk
 * and hdb(hd1) is your PC hard disk. 
 * this operation may format your PC hard disk!
 * Confirm that you know what you are doing!!!
 */	
	init_syscall();
	init_dpt();
	init_fs();
	//visit_all_free_blks();
	nextline();
	// for(int i=0;i<10;i++)kprintf(" %d ",new_block());
	clear_cmd_buffer();	
	//_syscall(_SYS_TESTHD,0,0,0);

		// for(int i=0;i<20;i++)iput(&iroot,i);
		// struct m_inode im;
		// iget( &im, 20);
		// kprintf("%d",im.i_creat_time);
	// kprintf("\n%d",iroot.i_zone[0]);
	mkdir("new_dir",DIR_FILE);
	free_blk_traverse();
	clear_cmd_buffer();	
	while(1);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*C and C++ code can call each other by taking advantage of extern "C" */

