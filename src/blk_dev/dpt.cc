#include <rios/dpt.h>
#include <asm/x86.h>
#include <rios/type.h>
#include <rios/hd.h>
/*Disk Partition Table
 *info about hardware:http://www.gotothings.com/unix/disk-partition-table.htm
 */
void set_DPT()
{
/* first partition */
	set_disk_no(1);
	u8 master_boot_sector[512] ={0};
	master_boot_sector[0x1be] = 0x80;			/* boot-flag:active */
	master_boot_sector[0x1be + 0x04] = RIOS_FS;	/* Partition Types: http://www.osdever.net/documents/partitiontypes.php?the_id=35*/
	*(unsigned int*)&master_boot_sector[0x1be + 0x08]=1;		/* starting sector counting from 0 */
	*(u32*)&master_boot_sector[0x1be + 0x0c]=10*1024*2; 	/* nr of sectors in partition (10MB)*/
/* magic number */
	master_boot_sector[0x1fe]=0x55;
	master_boot_sector[0x1ff]=0xaa;
	IDE_write_sector((void *)(master_boot_sector),0);
}
int check_DPT()
{
	int _DPT_detected = 1;
	u8 _tmp_sect_buf[512];
	set_disk_no(1);
	IDE_read_sector((void *)(_tmp_sect_buf),0);
	nextline();
	if(_tmp_sect_buf[0x1be] == 0x80)
		print(" Active partition,");
	else    print(" Inactive partition,"),_DPT_detected = 0;
	if(_tmp_sect_buf[0x1be + 0x04] == RIOS_FS)
		print(" valid system code :RIOS_FS,");
	else    print(" invalid system code,no FS detected! "),_DPT_detected = 0;
	if(_tmp_sect_buf[0x1fe]==0x55 && _tmp_sect_buf[0x1ff]==0xaa)
		print(" 0xAA55 found.");/*magic number.*/
	else    print(" 0xAA55 not found."),_DPT_detected = 0;
	return _DPT_detected;
}

void sys_setup()
{
	// outb(0x3f6,0);	
	// int *buffer;
	// ATA_read(0,(void *)buffer,100);

}
void init_dpt()
{
_check_DPT_again:	
	if(check_DPT()==0){
		nextline(),print(" Started set Disk partition table on hdb(disk1). "),set_DPT();
		goto _check_DPT_again;
	}else
		print(" OK");
}

