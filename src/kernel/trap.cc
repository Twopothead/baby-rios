/*
 * RiOS/include/kernel/trap.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/type.h>
#include <rios/trap.h>
#include <rios/irq.h>
#include <rios/timer.h>

void irq_handle(TrapFrame *trapframe)
{
	/*check unique signal,defined in RiOS/src/kernel/idt.S*/
	if(trapframe->err==888)
		keyboard_handler_main();
	/*DONNOT forget to unmask other interrupts*/
	if(trapframe->err==111)
		hd0_handler_main();
	if(trapframe->err==222)
		hd1_handler_main();
	if(trapframe->err==666)
		timer_8253_handler_main();
/*system call*/	
	if(trapframe->err==0x80)
		_syscall_handler_main(trapframe);
	
/*interal Exception, irq0~irq31*/
	if(trapframe->err==0)
		_irq0handler_main();
	if(trapframe->err==1)
		_irq1handler_main();
	if(trapframe->err==2)
		_irq2handler_main();
	if(trapframe->err==3)
		_irq3handler_main();
	if(trapframe->err==4)
		_irq4handler_main();
	if(trapframe->err==5)
		_irq5handler_main();
	if(trapframe->err==6)
		_irq6handler_main();
	if(trapframe->err==7)
		_irq7handler_main();
	if(trapframe->err==8)
		_irq8handler_main();
	if(trapframe->err==9)
		_irq9handler_main();
	if(trapframe->err==10)
		_irq10handler_main();
	if(trapframe->err==11)
		_irq11handler_main();
	if(trapframe->err==12)
		_irq12handler_main();
	if(trapframe->err==13)
		_irq13handler_main();
	if(trapframe->err==14)
		_irq14handler_main();
	if(trapframe->err==15)
		_irq15handler_main();
	if(trapframe->err==16)
		_irq16handler_main();
	if(trapframe->err==17)
		_irq17handler_main();
	if(trapframe->err==18)
		_irq18handler_main();
	if(trapframe->err==19)
		_irq19to31handler_main();	

}