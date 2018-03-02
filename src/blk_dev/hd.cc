#include <rios/hd.h>
#include <rios/type.h>
#include <rios/gdt_idt.h>
#include <asm/x86.h>
#include <rios/console.h>

extern struct GATE_DESCRPTER idt_descr[MAX_IDT];
extern u32 hd_handler;
struct hd_struct{
	u32 head,sect,cyl;
};
/*IDE : Integrated Drive Electronics, ATA*/
void wait_ATA()
{
	
}

void judge_disk1_exist()
{
	int havedisk1 =0;
 	outb(0x1f6, 0xe0 | (1<<4));
	int i;
	for(i=0; i<1000; i++){
	    if(inb(0x1f7) != 0){
	      havedisk1 = 1;
	      break;
	    }
	}
	msg_hd1_exist(havedisk1);
}

void init_hd()
{
	set_interrupt_gate(idt_descr+0x2e,INDEX_KERNEL_CODE,\
		(u32)hd_handler,RING0);
	/*keyboard irq1 remap to int 0x21 ,hd irq14 remap to int 0x2e*/
	outb_wait(0x21,inb_wait(0x21)&0xfb);/*irq2 0b11111011*/	
	outb(0xa1,inb_wait(0xa1)&0xbf);/*irq14 0b10111111*/
	/**
	 *IPC slave is connected to IPC master's irq2
	 *our hd interrupt is IPC slave's irq14 
	 *therefore, we should enable irq14 and irq2
	 */
	judge_disk1_exist();
  	
}

void do_hd()
{

}