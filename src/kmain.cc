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
	char ch = '!';char *str="world";int tmp=1234;int hexnum=0x888;
	kprintf("hello\t%s%d%c\n%x",str,tmp,ch,hexnum);
	
	//_syscall(_SYS_TESTHD,0,0,0);
	// print("haha,Thank god!I do no die.");
	
	while(1);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*C and C++ code can call each other by taking advantage of extern "C" */

