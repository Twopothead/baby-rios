/*
 * RiOS/include/kernel/gdt_idt.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/gdt_idt.h>
#include <rios/type.h>
#include <asm/x86.h>
#include <rios/console.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*Global Descriptor Table*/
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

/*Interrupt Descriptor Table*/


struct IDT_pointer
{
	u16 limit;
	u32 base;
} __attribute__((packed));

struct GATE_DESCRPTER idt_descr[MAX_IDT];
struct IDT_pointer idt_pointer;


#ifdef __cplusplus
}
#endif /* __cplusplus */

/*GDT*/
void set_gdt_entry(u8 num, u32 base, u32 limit, u8 access, u8 granularity)
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
	set_gdt_entry(0,0,0,0,0);			/*NULL*/
	set_gdt_entry(1,0,0xffffffff,0x9a,0xcf);/*0-4GB code seg ring0*/
	set_gdt_entry(2,0,0xffffffff,0x92,0xcf);/*0-4GB data seg ring0*/
	update_gdt();

}

/*IDT*/

void set_interrupt_gate(struct GATE_DESCRPTER *descr, u16 index ,u32 offset, u8 dpl)
{
	descr->offset_lowerbits = offset & 0xffff;
	descr->selector = index << 3; 	/*the lower 3 bits is TI(2) and RPI(0,1)*/
	descr->zero = 0x00;
	descr->seg_type = Gate_INTERRUPT_TYPE;/*0x0E*/
	descr->storage = 0b0;
	descr->descr_privilege_level = dpl;
	descr->present =0b1;
	descr->offset_higherbits = (offset & 0xffff0000) >> 16;
}

void set_trap_gate(struct GATE_DESCRPTER *descr,u16 index,u32 offset,u8 dpl)
{
	descr->offset_lowerbits = offset & 0xffff;
	descr->selector = index << 3;	/*the lower 3 bits is TI(2) and RPI(0,1)*/
	descr->zero = 0x00;
	descr->seg_type = Gate_TRAP_TYPE; /*0x0F*/
	descr->storage = 0x0;
	descr->descr_privilege_level = dpl;
	descr->present =0x1;
	descr->offset_higherbits = (offset & 0xffff0000) >> 16;
}

void enable_keyboad()
{	/* enable ONLY IRQ1 (keyboard) */
	outb(PIC0_IMR,  inb_wait(PIC0_IMR)&0xfd);/*bin(0xfd)=0b11111101*/
	/*the keyboad uses IRQ1 */
}
void disable_all_interrupts()
{
	/*mask all interrupts*/
	outb(PIC0_IMR,0xff);/*outb(0x21 , 0xff);*/
	outb(PIC1_IMR,0xff);/*outb(0xA1 , 0xff);*/
}
void init_idt()
{
	int i=0;
	for( i = 0; i < MAX_IDT; i++){
		set_trap_gate(idt_descr + i, INDEX_KERNEL_CODE,\
			(u32)keyboard_handler, RING0);
	}

	set_interrupt_gate(idt_descr+0x21,INDEX_KERNEL_CODE,\
		(u32)keyboard_handler,RING0);/*irq1 keyboard*/
	set_interrupt_gate(idt_descr+0x20,INDEX_KERNEL_CODE,\
		(u32)timer_8253_handler,RING0);/*irq0 timer*/
/*ok,we have set 8253 interrupt gate,but have not enabled it*/	
	idt_pointer.limit = (sizeof (struct GATE_DESCRPTER) * 256 ) - 1;
	idt_pointer.base = (unsigned long)&idt_descr ;

	disable_all_interrupts();/*mask all interrupts*/
	/*ICW1*/
	/*0x11 => edge trigger mode*/
	outb(PIC0_ICW1_PORT,0x11);/*outb(0x20 , 0x11);*/
	outb(PIC1_ICW1_PORT,0x11);/*outb(0xA0 , 0x11);*/
	/*ICW2*/
	/*0x20 => remap IRQ 0-7 to INT20-27*/
	/*0x28 => remap IRQ 8-15 to INT28-2f*/
	/*the first 32(0x20) interrupts are reserved for CPU exceptions*/
	outb(PIC0_ICW2_PORT,0x20);/*outb(0x21 , 0x20);*/
	outb(PIC1_ICW2_PORT,0x28);/*outb(0xA1 , 0x28);*/
	/*ICW3: setup 8259 cascading method*/
	/*PIC1 connect to PIC0 by IRQ2*/
	outb(PIC0_ICW3_PORT,1<<2);/*outb(0x21 , 1<<2);*/
/*here, we enable irq2 for slave 8259A.The way that set ICW3 of master PIC 
 *and slave PIC is different. PIC0:1<<num,PIC1:num 
 */	
	outb(PIC1_ICW3_PORT,2);/*outb(0xA1 , 2);*/
	/*ICW4*/
	/*0x01 => no buffer*/
	outb(PIC0_ICW4_PORT,0x01);/*outb(0x21 , 0x01);*/
	outb(PIC1_ICW4_PORT,0x01);/*outb(0xA1 , 0x01);*/

	update_idt(); 
	sti();
	msg_idt_ok();
	enable_keyboad();/* ONLY enable keyboard interrupt!*/
	msg_keyboard_ok();
	msg_trapframe_ok();
}

void enable_8253()
{
	outb(0x21,inb_wait(0x21)&0xf8);
/* bin(0xf8)='0b11111000',enable irq0(timer),irq1(keyboard),irq2(slave 8259)
 */	
}
void init_8253()
{
	outb(PIT_COMMAND_REG,0x34);
/*bin(0x34)='0b110100'= 00   11    010   0
 *Channel 0 ,Access mode: lobyte/hibyte , Mode 2 (rate generator),16-bit binary 
 */	
	outb(PIT_CHANNEL0_DATA_PORT,0x9c);/*loyte*/
	outb(PIT_CHANNEL0_DATA_PORT,0x2e);/*hibyte*/
/* 1193180/100 = 11930 ,100HZ => a irq every 10ms
 * 11930 = 0b 0010 1110 1001 1010,first write lower bits,then the higher
 * see my guide.md and https://wiki.osdev.org/Programmable_Interval_Timer
 */	
	enable_8253();
	msg_8253_ok();
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

/*for more info about interrupt_Descriptor_table
 *https://wiki.osdev.org/Interrupt_Descriptor_Table
 * The meaning of IDT entry's bit fields:
 * https://wiki.osdev.org/Interrupt_Descriptor_Table#IDT_in_IA-32e_Mode_.2864-bit_IDT.29
 *
 *https://wiki.osdev.org/Descriptor
 *Possible IDT gate types :
 *Type
 *0b0101	0x5	5	80386 32 bit task gate
 *0b0110	0x6	6	80286 16-bit interrupt gate
 *0b0111	0x7	7	80286 16-bit trap gate
 *0b1110	0xE	14	80386 32-bit interrupt gate
 *0b1111	0xF	15	80386 32-bit trap gate
 *
 *Selector
 *https://wiki.osdev.org/Selector
 *Name	Bit	Description
 *RPL	0, 1	Requested Privilege Level. The CPU checks these bits before any selector is changed. Also system calls can be executed in userspace (ring 3, see this) without misfeasance using the ARPL (Adjust Requested Privilege Level) instruction.
 *TI	2	Table index; 0 = GDT, 1 = LDT
 *Index	3..15	Index to a Descriptor of the table.
 *
 * Figure of IDT Gate Descriptors (1.Task Gate 2.Interrupt Gate Trape Gate)
 * http://www.blogfshare.com/introduce-idt.html
 */