#include <rios/hd.h>
#include <rios/type.h>
#include <rios/gdt_idt.h>
#include <rios/console.h>

extern struct GATE_DESCRPTER idt_descr[MAX_IDT];
extern u32 hd0_handler;
extern u32 hd1_handler;
u8 hd_buf[512*2];
struct hd_struct{
	u32 head,sect,cyl;
};
/*IDE : Integrated Drive Electronics, ATA*/

void judge_disk1_exist()
{
	int havedisk1 =0;
 	outb(ATA_PORT_CURRENT,Get_Device_register(1,0));/*outb(0x1f6, 0xe0 | (1<<4));*/
/*switch_to_disk(1)*/
	int i;
	for(i=0; i<1000; i++){
	    if(inb_wait(ATA_PORT_STATUS) != 0){
	      havedisk1 = 1;
	      break;
	    }
	}
	msg_hd1_exist(havedisk1);
	switch_to_disk(0);/*switch back to disk 0*/


}

void init_hd()
{
	/*irq14 set primary ATA*/
	set_interrupt_gate(idt_descr+0x2e,INDEX_KERNEL_CODE,\
		(u32)hd0_handler,RING0);
	//keyboard irq1 remap to int 0x21 ,hd irq14 remap to int 0x2e
	outb_wait(0x21,inb_wait(0x21)&0xfb);/*irq2 0b11111011*/	
	outb(0xa1,inb_wait(0xa1)&0xbf);/*irq14 0b10111111*/
	/**
	 *IPC slave is connected to IPC master's irq2
	 *our hd interrupt is IPC slave's irq14 
	 *therefore, we should enable irq14 and irq2
	 */
	/*1rq15 set secondary ATA*/
	set_interrupt_gate(idt_descr+0x2f,INDEX_KERNEL_CODE,\
		(u32)hd1_handler,RING0);
	/*keyboard irq1 remap to int 0x21 ,hd irq15 remap to int 0x2f*/
	outb_wait(0x21,inb_wait(0x21)&0xfb);/*irq2 0b11111011*/	
	outb(0xa1,inb_wait(0xa1)&0x7f);/*irq15 0b01111111*/
	//switch_to_disk(1);
	// judge_disk1_exist();
	// get_hd_size();
	
}
int  hd_status;
void _kdo_hd0(){
	print("fuck0");
}
void do_hd0()
{
	
	// __asm__ volatile("xor %edx,%edx\n\t" \
	// 		"xchg _kdo_hd0,%edx\n\t" \
	// 		"testl %edx,%edx\n\t" \
	// 		"jne 1f\n\t" \
	// 		"movl $_do_nothing,%edx\n\t" \
	// 		"1:" \
	// 	);
	outb_wait(0x20,0x20);/*Master EOI*/
	outb_wait(0xa0,0x20);/*Slave EOI*/
	//EOI signal (End of interrupt acknowlegment)
	// __asm__ volatile("call *%edx");
}

void _kdo_hd1(){
	print("fuck1");
}
void _do_nothing(){}
void do_hd1()
{
	
	// __asm__ volatile("xor %edx,%edx\n\t" \
	// 		"xchg _kdo_hd1,%edx\n\t" \
	// 		"testl %edx,%edx\n\t" \
	// 		"jne 1f\n\t" \
	// 		"movl $_do_nothing,%edx\n\t" \
	// 		"1:" \
	// 	);
	outb_wait(0x20,0x20);//EOI signal (End of interrupt acknowlegment)
	outb_wait(0xa0,0x20);
	// __asm__ volatile("call *%edx");
}
static int disk_no = 0;/* disk 0 */
/*wait until IDE disk is ready*/
// @param check_error : =0 => donnot check ,just wait until ok
//  *		       =1 => return status code.
 
int _IDE_disk_wait(int check_error)
{
	int _r;

	while ((_r= inb_wait(ATA_PORT_STATUS)) & (BUSY_STAT|READY_STAT) != READY_STAT)
		; /*judge whether it's busy or ready,wait*/
	if( check_error && (WRERR_STAT|ERR_STAT) != 0)
		return -1;
	return 0;
}

int ATA_read(u32 sector_no, void *dest, int n_sectors)
{
	int ide_status;
	
	_IDE_disk_wait(0);

	outb(ATA_PORT_SECT_COUNT, n_sectors);/*outb(0x1f2,n_sectors)*/
	outb(ATA_PORT_LBA_LOW, sector_no & 0xff);/*outb(0x1f3,n_sectors)*/
	outb(ATA_PORT_LBA_MID, (sector_no >> 8) & 0xff);
	outb(ATA_PORT_LBA_HIGH, (sector_no >>16) &0xff);
	outb(ATA_PORT_STATUS, Get_Device_register(disk_no,sector_no));
	
	outb(ATA_PORT_COMMAND, HD_READ);
	for(;n_sectors>0;n_sectors--,dest+=SECTOR_SIZE){
		if((ide_status = _IDE_disk_wait(1) ) < 0)
			return ide_status;
		port_read(ATA_PORT_DATA,dest,256);/*256*word = 512Byte = a sector*/
	}
	return 0;
}

int ATA_write(u32 sector_no, void *dest, int n_sectors)
{
	int ide_status;
	_IDE_disk_wait(0);
	print("oh");
	outb(ATA_PORT_SECT_COUNT, n_sectors);		/*outb(0x1f2,n_sectors)*/
	outb(ATA_PORT_LBA_LOW, sector_no & 0xff);	/*outb(0x1f3,n_sectors)*/
	outb(ATA_PORT_LBA_MID, (sector_no >> 8) & 0xff);
	outb(ATA_PORT_LBA_HIGH, (sector_no >>16) &0xff);
	outb(ATA_PORT_STATUS, Get_Device_register(disk_no,sector_no));
	outb(ATA_PORT_COMMAND, HD_WRITE);
	for(;n_sectors>0;n_sectors--,dest += SECTOR_SIZE){
		if((ide_status = _IDE_disk_wait(1) ) < 0)
			return ide_status;
		port_write(ATA_PORT_DATA,dest,256);
	}
	return 0;
}

int get_hd_size()
{/*@DEBUG it doesn't work!*/
	switch_to_disk(1);
	int cmd_device = (0xa0 | ((1 & 1) << 4) | ((0 >> 24) & 0x0f)); /*CHS mode*/
	int cmd_command = HD_IDENTIFY;
	
	_IDE_disk_wait(0);

	outb(ATA_PORT_SECT_COUNT, 0);		/*outb(0x1f2,n_sectors)*/
	outb(ATA_PORT_LBA_LOW, 0 & 0xff);	
	outb(ATA_PORT_LBA_MID, (0 >> 8) & 0xff);
	outb(ATA_PORT_LBA_HIGH, (0 >>16) &0xff);
      //  outb(ATA_PORT_STATUS, cmd_device);
	//outb(ATA_PORT_COMMAND, cmd_command);
	port_read(ATA_PORT_DATA,hd_buf,512);
	// nextline();print("Detecting HD size...");
	// u16 * hd_info = (u16*)hd_buf;
	// int sectors = ((int)hd_info[61] << 16) +hd_info[60];
	print("HD size:");//putnum(sectors*512/1000000);
	//putnum(hd_buf);
}
