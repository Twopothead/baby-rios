/*
 * RiOS/include/rios/gdt_idt.h
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#ifndef _GDT_IDT_H
#define _GDT_IDT_H

#include <asm/x86.h>
#include <rios/type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*GDT*/
void set_gdt_entry(u8 num, u32 base, u32 limit, u8 access, u8 granularity);
extern void update_gdt();
void init_gdt();

#define INDEX_KERNEL_CODE 1
#define INDEX_KERNEL_DATA 2


/*global descriptor number*/
#define GLOBAL_DES_KERNEL_CODE ((INDEX_KERNEL_CODE) << 3)
#define GLOBAL_DES_KERNEL_DATA ((INDEX_KERNEL_DATA) << 3)

#define RING0 0
#define RING3 3
/*in Linux and Win,Only Ring0 Ring3 are used, 0 for kernel and 3 for user*/

#define Gate_INTERRUPT_TYPE 0b1110
#define Gate_TRAP_TYPE 0b1111
/*赵炯《Linux内核完全剖析》P95中断门和陷阱门TYPE字段分别为14和15*/

/*IDT*/
#define MAX_IDT 256

extern void update_idt();

#pragma pack(1)
struct GATE_DESCRPTER{
	u16 offset_lowerbits		:16; // offset bits 0..15
	u16 selector			:16; // a code segment selector in GDT or LDT
	u8  zero			:8 ; // unused,set to 0
					     // type and attributes, total u8 type_attr;
	u8  seg_type			:4 ;
	u8  storage			:1 ; // set to 0 for interrupt and trap gates 
	u8  descr_privilege_level   	:2 ;
	u8  present			:1 ;
					     //~	
	u16	offset_higherbits	:16; // offset bits 16..31	
};
#pragma pack()
void set_interrupt_gate(struct GATE_DESCRPTER *descr, u16 index ,u32 offset, u8 dpl);
void set_trap_gate(struct GATE_DESCRPTER *descr,u16 index,u32 offset,u8 dpl);

static inline void _lidt(struct GATE_DESCRPTER *p,u16 size){
	volatile u16 data_48b[3];
	data_48b[0] = size - 1;/* 256-1=255 0xff 16bits*/
	data_48b[1] = (u32)p;
	data_48b[2] = (u32)p >> 16;
	/*32b IDT linear base address + 16b table size =>48b idtr */ 
	asm volatile("lidt (%0)" : : "r" (data_48b));
}

extern void write_port(int value,int port);
extern u32 irq_empty();
extern u32 keyboard_handler();
extern u32 timer_8253_handler();
#include <rios/i8259.h>
#include <rios/i8253.h>
void init_idt();
void enable_keyboad();
void disable_all_interrupts();


/*irq*/

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif