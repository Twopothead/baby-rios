#include <rios/serial.h>
#include <rios/console.h>
unsigned char *buffer_addr=(unsigned char *)0x8888888;/*mm 0x666666*/
void init_serial()
{
	outb_wait(SERIAL_PORT + 1, 0x00); /*Disable all interrupt*/
	outb_wait(SERIAL_PORT + 3, 0x80); 
	outb_wait(SERIAL_PORT + 0, 0x03); 
	outb_wait(SERIAL_PORT + 1, 0x00); 
	outb_wait(SERIAL_PORT + 3, 0x03); 
	outb_wait(SERIAL_PORT + 2, 0xc7); /*enable FIFO*/
	outb_wait(SERIAL_PORT + 3, 0x0b); /*enable irq*/
}

void IDE_disk_wait()
{
	while ( ( inb_wait(ATA_PORT_STATUS) & (BUSY_STAT|READY_STAT) )!=  READY_STAT )
	    ;
	print("wait...ok ");
}

void IDE_write_sector(void *src,int lba)
{
	IDE_disk_wait();
	outb_wait(ATA_PORT_SECT_COUNT,1);/*outb(0x1f2,1);*/
	outb_wait(ATA_PORT_LBA_LOW ,lba);/*outb(0x1f3,lba);*/
	outb_wait(ATA_PORT_LBA_MID ,lba >> 8);/*outb(0x1f4,lba>>8)*/
	outb_wait(ATA_PORT_LBA_HIGH ,lba >> 16);/*outb(0x1f5,lba>>16)*/
	outb_wait(ATA_PORT_DEVICE , 0xe0 | (lba >> 24));/*outb(0x1f5,lba>>16)*/
	outb_wait(ATA_PORT_STATUS, HD_WRITE);
	IDE_disk_wait();
	for(int i = 0; i < SECTOR_SIZE/4 ; i++){
		 _out_data32(ATA_PORT_DATA,((u32*)src)[i]);
	}
}

void IDE_read_sector(void *dest, int lba)
{
	IDE_disk_wait();
	outb_wait(ATA_PORT_SECT_COUNT,1);/*outb(0x1f2,1);*/
	outb_wait(ATA_PORT_LBA_LOW ,lba);/*outb(0x1f3,lba);*/
	outb_wait(ATA_PORT_LBA_MID ,lba >> 8);/*outb(0x1f4,lba>>8)*/
	outb_wait(ATA_PORT_LBA_HIGH ,lba >> 16);/*outb(0x1f5,lba>>16)*/
	outb_wait(ATA_PORT_DEVICE , 0xe0 | (lba >> 24));/*outb(0x1f5,lba>>16)*/
	outb_wait(ATA_PORT_STATUS, HD_READ );
	IDE_disk_wait();
	for(int i = 0; i < SECTOR_SIZE/4 ; i++){
		 ((u32*)dest)[i]=_in_data32(ATA_PORT_DATA);
	}
}


	
/*about serial ports:https://wiki.osdev.org/Serial_Ports*/