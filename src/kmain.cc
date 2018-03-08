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
#include <rios/hd.h>
#include <rios/ext2.h>
#include <rios/dpt.h>
#include <rios/serial.h>
#include <rios/syscall.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern unsigned char *buffer_addr;
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
	nextline();println("Oh.");
	//sys_setup();
	init_Exception();
	init_serial();
	//init_hd();
	init_syscall();

	// unsigned char *tmp=buffer_addr;
	// for(int i=0;i<100;i++){
	// 	*(buffer_addr)=1;
	// 	putnum(*(buffer_addr));
	// 	buffer_addr+=8;
	// }
	// buffer_addr=tmp;
	// __asm__ __volatile__("int $0x80");
	_syscall(_SYS_READ,0,0,0);

	// print("begin");
	// for(int i=0;i<100;i++)
	// 	IDE_write_sector((void *)(buffer_addr+512*i),i+1);
	// for(int i=0;i<100;i++){
	// 	putnum(*(buffer_addr));
	// 	buffer_addr+=8;
	// }

	print("haha,Thank god!I do no die.");
	//init_fs();
	/*Divide by zero :*/
	int a=6;int b=0;a/=b;
	print("?");

	while(1);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*C and C++ code can call each other by taking advantage of extern "C" */

