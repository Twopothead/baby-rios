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