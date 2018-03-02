/*
 * RiOS/include/kernel/irq.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/irq.h>
#include <rios/console.h>
#include <rios/keyboard.h>
#include <rios/hd.h>
#include <rios/timer.h>

void keyboard_handler_main()
{
	do_keyboard();
	//clear_screen();
}

void hd_handler_main()
{
	do_hd();
}

void timer_8253_handler_main()
{
	do_timer();
	//do_timer();
}