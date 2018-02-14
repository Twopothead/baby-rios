/*
 * RiOS/src/kmain.c
 *
 * Copyright (C) 2018 qiuri
 * Licence: GPL version 2  http://www.gnu.org/licenses/gpl.html
 */


void RiOS_main(void)
{
	__asm__ __volatile__(
		"movw $0x075b,(0xb80a0)\n\t" 	
		"movw $0x075d,(0xb80aa)\n\t" 	
		"movl $0x0a4b0a4f,(0xb80a4)"
		);/* [ OK ]*/
	

	while(1);
}
