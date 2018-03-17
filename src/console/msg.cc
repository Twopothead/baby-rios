/*
 * RiOS/src/console/msg.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/type.h>
#include <rios/console.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void msg_ok()
{
	set_text_attr(LightGray,Black);
	print("[");
	set_text_attr(LightGreen,Black);
	print(" OK ");
	set_text_attr(LightGray,Black);
	print("]");
}

void msg_gdt_ok()
{
	msg_ok();
	print("  Updated Global Descriptor Table.");
}

void msg_idt_ok()
{ 
	nextline();
	msg_ok();
	print("  Updated Interrupt Descriptor Table.");
}

void msg_keyboard_ok()
{
	nextline();
	msg_ok();
	print("  Enabled keyboard.");
}

void msg_trapframe_ok()
{
	nextline();
	msg_ok();
	print("  Started TrapFrame.");
}

void msg_Rishell_ok()
{
	nextline();
	msg_ok();
	print("  Started Shell.");

}

void msg_mm_ok1()
{
	nextline();
	msg_ok();
	print("  Starting memory detecting.");
}

void msg_mm_ok2(int mm_value)
{
	nextline();
	msg_ok();
	print("  There are ");
	putnum(mm_value);
	print(" MB memory available.");
}

void msg_cmd_help()
{
	nextline();
	kprintf("GNU ri_shell,version 0.0.1-debug(x86_i386,x86_64-pc-rios)\n\
These shell commands are defined internally.  Type `help' to see this list.\n\
 - help  :: print this help manual.\n\
 - clear :: clear the screen.\n\
 - hexdump NR :: hexdump the NRth sector,eg.hedump 0.\n\
 - ls    :: list directory contents.\n\
            'ls' Usage:  ls [OPTION]... [FILE]...\n\
 - logo  :: print NJAU logo.\n\
 - info  :: 'info superblock','info grouping','info debug'\n\
 - ls :: 'ls /'  ");
}

void msg_8253_ok()
{
	nextline();
	msg_ok();
	print("  Started Programmable Interrupt Timer 8253/8254.");
}

void msg_hd1_exist(int  havedisk1)
{
	nextline();
	msg_ok();
	print("  Kernel is located at disk0, and ");
	if(havedisk1){
  		print("disk1(hdb) exists.");
 	}else print("disk1(hdb) does not exist!");
}
void msg_danger_hdb()
{
	nextline();
	msg_ok();
	print("  Switching from hda(Udisk) to hdb(PC hard disk)!LOOK OUT!!!");
}

/* total_used_ctrl_blks = 350
 		(the first 2 sectors are boot sector and superblock blk 
   		350 = 2 + 5 + 1 + 342
 	kprintf(" total used contrl blks are :%d. = 2+ %d * %d * %d",total_used_ctrl_blks, \
 		rios_superblock.s_zone_bitmap_blks , \
 		 rios_superblock.s_inode_bitmap_blks , \
 		  rios_superblock.s_inode_blks);
 	 kprintf(" data blk NO.:%d.",NR_DATA_BLK(rios_superblock)); 351
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
