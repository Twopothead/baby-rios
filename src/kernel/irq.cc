/*
 * RiOS/include/kernel/irq.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/irq.h>
#include <rios/console.h>
#include <rios/keyboard.h>

void keyboard_handler_main()
{
	do_keyboard();
	//clear_screen();
}