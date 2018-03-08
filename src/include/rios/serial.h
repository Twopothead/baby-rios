#ifndef _SERIAL_H
#define _SERIAL_H

#include <asm/x86.h>
#include <rios/type.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define SERIAL_PORT 0x3f8

#define SERIAL_COM1 0x3f8
#define SERIAL_COM2 0x2f8
#define SERIAL_COM3 0x3e8
#define SERIAL_COM4 0x2f8

#define ATA_PORT_DATA 		0x1f0
#define ATA_PORT_ERROR 		0x1f1
#define ATA_PORT_SECT_COUNT 	0x1f2
#define ATA_PORT_LBA_LOW 	0x1f3
#define ATA_PORT_LBA_MID 	0x1f4
#define ATA_PORT_LBA_HIGH 	0x1f5
#define ATA_PORT_CURRENT 	0x1f6
#define ATA_PORT_DEVICE		ATA_PORT_CURRENT
#define ATA_PORT_STATUS 	0x1f7
#define ATA_PORT_COMMAND 	ATA_PORT_STATUS 


/*Bits of HD Status*/
#define ERR_STAT    0x01 /*cmd execute error*/
#define INDEX_STAT  0x02 
#define ECC_STAT    0x04 /*Corrected error,ECC校验错*/
#define DRQ_STAT    0x08 /*Disk request*/
#define SEEK_STAT   0x10
#define WRERR_STAT  0x20 /*Disk error*/
#define READY_STAT  0x40
#define BUSY_STAT   0x80 	

#define SECTOR_SIZE 512  /*a sector is 512 Byte*/	

/*Values for HD command*/
#define HD_RESTORE  0x10
#define HD_READ     0x20
#define HD_WRITE    0x30
#define HD_VERIFY   0x40
#define HD_FORMAT   0x50
#define HD_INIT     0x60
#define HD_SEEK     0x70
#define HD_DIAGNOSE 0x90
#define HD_SPECIFY  0x91

#define HD_IDENTIFY 0xec

void init_serial();

void IDE_disk_wait();
void IDE_write_sector(void *src,int lba);
void IDE_read_sector(void *dest, int lba);




static inline int _in_data32(u16 port) {
	u32 data;
	asm volatile("in %1,%0" : "=a" (data) : "d" (port));
	return data;
}
	
static inline void _out_data32(u16 port, u32 data) {
    asm volatile("out %0,%1" : : "a"(data), "d"(port));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* The Device register structure => port 0x1f7
 * 7	|     1      |	
 * 6	|     L      |   LBA mode
 * 5	|    DRV     |   Drive No.?
 * 4	|     1      |
 * 3	|    HS3     | }
 * 2	|    HS2     | }
 * 1	|    HS1     | }
 * 0	|    HS0     | }
 * 
 *  L 	  : LBA mode 
 * DRV 	  : if DRV = 0,drive 0 (master) is selected,
 *          if DRV = 1,drive 1(slave) is slected
 * HS0~HS3: if L = 0, these 4 bits select the head number
 *          if L = 1,H0~H3 contains bits 24~27 of the LBA (Linear Base Address)
 */

#endif