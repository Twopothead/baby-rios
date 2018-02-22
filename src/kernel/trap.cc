/*
 * RiOS/include/kernel/trap.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/type.h>
#include <rios/trap.h>
#include <rios/irq.h>


void irq_handle(TrapFrame *trapframe)
{
	/*check unique signal,defined in RiOS/src/kernel/idt.S*/
	if(trapframe->err==888)
		keyboard_handler_main();
	/*DONNOT forget to unmask other interrupts*/
}