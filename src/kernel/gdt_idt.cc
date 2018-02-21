/*
 * RiOS/include/kernel/gdt_idt.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/gdt_idt.h>
#include <rios/type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct GDT_entry 
{
	u16 limit1;	/*0-15*/		
	u16 base1;	/*16-31*/
	u8 base2;	/*32-39*/
	u8 access;	/*40-47*/
	u8 granularity; /*48-55*/
	u8 base3;	/*56-63*/
} __attribute__((packed));
							
	
struct GDT_pointer
{
	u16 limit;
	u32 base;
} __attribute__((packed));

struct GDT_entry gdt[3];
struct GDT_pointer gdt_pointer;

#ifdef __cplusplus
}
#endif /* __cplusplus */


void set_get_entry(u8 num, u32 base, u32 limit, u8 access, u8 granularity)
{
	gdt[num].base1 = base & 0xffff;		/*u16*/
	gdt[num].base2 = (base >> 16 ) & 0x00ff;/*u8*/
	gdt[num].base3 = (base >> 24 ) & 0xffff;/*u8*/

	gdt[num].limit1 = limit & 0xffff;	/*u16*/
	gdt[num].granularity =( ( (limit >> 16) & 0x0f ) | (granularity & 0xf0) );
	gdt[num].access = access;
}

void init_gdt()
{

	gdt_pointer.limit = ( sizeof(struct GDT_entry) * 3) -1 ; /*3 records in gdt,limit counts from zero*/
	gdt_pointer.base = (unsigned long)& gdt;
	set_get_entry(0,0,0,0,0);			/*NULL*/
	set_get_entry(1,0,0xffffffff,0x9a,0xcf);/*0-4GB code seg ring0*/
	set_get_entry(2,0,0xffffffff,0x92,0xcf);/*0-4GB data seg ring0*/
	update_gdt();

}


/* 1.GDT的图可见《从实模式到保护模式》Ｐ201
 * granularity粒度位这里设为１，
 * GDT高32位
 * |23 | 22 | 21 | 20 | 19 - 16|
 * |G  |D/B | L  |AVL | 段界限　｜
 *   
 *  1   1     0     
 * AVL (系统可用位)
 * https://en.wikipedia.org/wiki/Global_Descriptor_Table　
 * 2.赵炯《Linux内核完全剖析》Ｐ91 数据段和代码段在TYPE段的不同编码
 *				    |<-  TYPE ->｜		
 *		｜　15 ｜ 14-13 ｜12 ｜ 11       8｜7 - 0 
 *data segment	｜　p　｜  DPL  ｜ 1 ｜ 0 E W A   ｜
 *code segment  ｜　p　｜  DPL  ｜ 1 ｜ 1 C R A   ｜
 * E:扩展方向 　W：可写 A：已访问 
 * C:一致代码段　R：可读 A：已访问 
 *
 *https://wiki.osdev.org/GDT_Tutorial
 *0x0000000000000000 NULL
 *0x00CF9A000000FFFF GDT_CODE_PL0
 *0x00CF92000000FFFF GDT_DATA_PL0
 *0x00CFFA000000FFFF GDT_CODE_PL3
 *0x00CFF2000000FFFF GDT_CODE_PL3
 */