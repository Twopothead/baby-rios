/*
 *  RiOS/kernel/console.c
 *
 *   (C) 2018 Frank Curie (邱日) 
 */   
#include <rios/type.h>
#define IDT_SIZE 256
struct IDT_DESCR{
	u16 offset_lowerbits; // offset bits 0..15
	u16 selector;		  // a code segment selector in GDT or LDT
	u8 zero;			  // unused,set to 0
	u8 type_attr;		  // type and attributes
	u16	ofset_higherbits; // offset bits 16..31	
};
struct IDT_DESCR idt_descr[IDT_SIZE];

struct GDT_DESCR{


};

/*for more info about interrupt_Descriptor_table
 *https://wiki.osdev.org/Interrupt_Descriptor_Table
 */