/*
 * RiOS/src/console/msg.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/type.h>
#include <rios/console.h>
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
	println("GNU ri_shell,version 0.0.1-debug(x86_i386,x86_64-pc-rios)");
	println("These shell commands are defined internally.  Type `help' to see this list.");
	println(" - help :: print this help manual.");
	println(" - clear :: clear the screen.");
	println(" - ls :: list directory contents.");
	println("      'ls' Usage:  ls [OPTION]... [FILE]...");
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
  		print("disk1 exists.");
 	}else print("disk1 does not exist!");
}