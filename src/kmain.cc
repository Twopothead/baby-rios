/*
 * RiOS/src/kmain.c
 *
 * Copyright (C) 2018 qiuri (邱日)
 * Licence: GPL version 2  http://www.gnu.org/licenses/gpl.html
 */
#include <stdint.h>
#include <rios/type.h>
#include <rios/gdt_idt.h>
#include <rios/console.h>
#include <rios/irq.h>
#include <rios/trap.h>
#include <rios/memory.h>
#include <rios/hd.h>
#include <rios/ext2.h>
#include <rios/dpt.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


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
	init_hd();
	nextline();println("Oh.");
	sys_setup();
	init_Exception();
	print("haha,Thank god!I do no die.");
	//init_fs();
	/*Divide by zero :int a=6;int b=0;a/=b;*/
	print("?");

	while(1);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*C and C++ code can call each other by taking advantage of extern "C" */

