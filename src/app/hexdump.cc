#include <rios/app/hexdump.h>
void sector_hexdump(u8 *sector)
{
	u16 *p =(u16 *)sector;
	nextline();
	for(int i=0;i<512/2;i++){
		 int head = ((*p)&0x00ff);
		 int tail = ((*p)&0xff00)>>8;
		 if((head&0xf0)==0)print("0");
		 kprintf("%x",head);
		 if((tail&0xf0)==0)print("0");
		 kprintf("%x ",tail);
		/*!NOTE 不可kprintf("%x ",*p);因x86采用小端模式*/
		*p++;
	}
}
/*usage :nr_sector_hexdump(0) 把第0个扇区hexdump
 */
void nr_sector_hexdump(int nr)
{
	u8 sector[512]={0};
	IDE_read_sector((void *)&sector,nr);
	sector_hexdump(sector);
}