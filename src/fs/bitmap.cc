#include <rios/bitmap.h>
#include <rios/syscall.h>
#include <rios/fs.h>
#include <rios/console.h>
#define HDB_SUPER_BLOCK_SEC 2
/*sector1:boot sector
 *settor2:superblock;
 */
extern int hdb_sect_total;

void format_disk()
{
	union Super_Block_Sect{
/*利用联合体去占坑位*/
		u8 bytes[512]={0};
		struct {
			u16 s_ninodes;
			u16 s_capacity_blks;		/*capacity count in blocks*/
			u16 s_startsect;
			u16 s_zone_bitmap_blks;		/*in rios.a zone is a sector.*/
			u16 s_inode_bitmap_blks;	/*num of blks that bitmap takes up*/
			u16 s_inode_blks;
			u16 s_firstdatazone;
			u16 s_magic;			/*ri_fs magic:0x8888*/
		};
	}; union Super_Block_Sect rios_superblock;
/*step1:format superblock*/
	rios_superblock.s_startsect = 1;			/*sect0 for MBR and sect1 for superblock*/
	rios_superblock.s_capacity_blks = hdb_sect_total/1;	/*call get_hd_size first*/	
	rios_superblock.s_magic = RIFS_MAGIC_NUMBER;
	rios_superblock.s_inode_bitmap_blks = INODE_BITMAP_BLK;	/*we use a blk for inode bitmap*/
	rios_superblock.s_inode_blks = INODE_BLKS;
	rios_superblock.s_zone_bitmap_blks = (hdb_sect_total+0xfff)>>12;
/** 
 * a zone bitmap can represent 512*8=4096sectors, 
 * 10MB= 20160sectors,need (20160+0xfff)>>12=5setors for zone_bitmap
 */
	rios_superblock.s_ninodes = INODE_BLKS;
	IDE_write_sector((void *)&rios_superblock,BOOT_BLK(rios_superblock));
/*step2:format bitmap*/

/*step3:format root folder*/	

	IDE_write_sector((void *)&rios_superblock,SUPER_BLK(rios_superblock));
	// IDE_write_sector((void *)&rios_superblock,BOOT_BLK(rios_superblock));
	// IDE_write_sector((void *)&rios_superblock,BOOT_BLK(rios_superblock));
	// IDE_write_sector((void *)&rios_superblock,BOOT_BLK(rios_superblock));

	print(" startsect:");putnum(rios_superblock.s_startsect);



}
void check_rifs()
{
	u8 sector[512];
	putnum(hdb_sect_total);/*C*H*S=20160 cylinders=20, heads=16, spt=63*/
	format_disk();
	
}