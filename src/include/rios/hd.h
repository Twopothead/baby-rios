#ifndef _HD_H
#define _HD_H

#define ATA_PORT_DATA 		0x1f0
#define ATA_PORT_ERROR 		0x1f1
#define ATA_PORT_SECT_COUNT 	0x1f2
#define ATA_PORT_LBA_LOW 	0x1f3
#define ATA_PORT_LBA_MID 	0x1f4
#define ATA_PORT_LBA_HIGH 	0x1f5
#define ATA_PORT_CURRENT 	0x1f6
#define ATA_PORT_STATUS 	0x1f7
#define ATA_PORT_COMMAND 	0x1f7
/**
 *When read, 0x1f7 => status;
 *When write, 0x1f7 => command
 */

#define Get_Device_register(disk_no,sector_no) \
	(0xe0 | ((disk_no & 1) << 4) | ((sector_no >> 24) & 0x0f) )
/* The Device register structure
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
/*@param disk_No: can ONLY be 0 or 1*/
#define switch_to_disk(disk_No) \
	outb(ATA_PORT_CURRENT,Get_Device_register(disk_No,0))	

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


#include <rios/type.h>
#include <asm/x86.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



void init_hd();
void _do_nothing();
void _kdo_hd0();
void do_hd0();
void _kdo_hd1();
void do_hd1();
void judge_disk1_exist();
int IDE_disk_wait(int check_error);
int ATA_read(u32 sector_no, void *dest, int n_sectors);
int ATA_write(u32 sector_no, void *dest, int n_sectors);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif