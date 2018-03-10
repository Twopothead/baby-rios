#include <rios/dpt.h>
#include <asm/x86.h>
#include <rios/type.h>
/*Disk Partition Table
 *info about hardware:http://www.gotothings.com/unix/disk-partition-table.htm
 */
void set_DPT()
{
/* first partition */
	switch_to_disk(1);
	u8 sector[512] ={0};
	sector[0x1be] = 0x80;			/* boot-flag:active */
	sector[0x1be + 0x04] = NORMAL_FS;	/* Partition Types: http://www.osdever.net/documents/partitiontypes.php?the_id=35*/
	*(unsigned int*)&sector[0x1be + 0x08]=1;		/* starting sector counting from 0 */
	*(u32*)&sector[0x1be + 0x0c]=10*1024*2; 	/* nr of sectors in partition (10MB)*/
/* magic number */
	sector[0x1fe]=0x55;
	sector[0x1ff]=0xaa;
	IDE_write_sector((void *)(sector),0);
}
void check_DPT()
{

}

void sys_setup()
{
	// outb(0x3f6,0);	
	// int *buffer;
	// ATA_read(0,(void *)buffer,100);

}
void init_dpt()
{
	set_DPT();

}

