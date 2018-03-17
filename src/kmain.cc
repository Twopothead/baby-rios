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
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern unsigned char *buffer_addr;
extern union Super_Block_Sect rios_superblock;
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
/* OK,here we are switch to hdb(hd1),(hd0 is your Udisk
 * and hdb(hd1) is your PC hard disk. 
 * this operation may format your PC hard disk!
 * Confirm that you know what you are doing!!!
 */	
	init_syscall();
	init_dpt();
	init_fs();
	visit_all_free_blks();
	// extern union free_space_grouping_head specific_block;
	// for(int i=0;i<100;i++){//kprintf("blk:%d ",new_block() );
	// 	int tmp =new_block();kprintf("blk:%d ",tmp );if(tmp==-1)_panic("die!");
	// kprintf(" group_nr:%d",rios_superblock.s_specific_blk_nr_group);
	// kprintf(" %d",specific_block.s_next_free_group_nr);
// }

	// new_block();
	/*_debug_visit_free_group_ctr();*/
	// for(int i=0;i<=127;i++){
	// 	kprintf("%d ",get_nr_free_group(i).s_next_free_group_nr);
	// }
	// kprintf(" %d ", NR_DATA_BLK(rios_superblock));
	//_syscall(_SYS_TESTHD,0,0,0);

	// for(int i=0;i<20;i++)iput(&iroot,i);
	// struct m_inode im;
	// iget( &im, 20);
	// kprintf("%d",im.i_creat_time);
	while(1);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*C and C++ code can call each other by taking advantage of extern "C" */

