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
#include <rios/syscall.h>

void keyboard_handler_main()
{
	do_keyboard();
}

void timer_8253_handler_main()
{
	do_timer();
}

void _syscall_handler_main(TrapFrame *trapframe)
{
	do_syscall(trapframe);
}


void hd0_handler_main()
{
	// do_hd0();
}

void hd1_handler_main()
{
	// do_hd1();
}



void panic(int nr_irq){
/* You can try this by :int a=0;int b=3;b/=a;*/
	set_text_attr(Red,Black);nextline();
	print(" FBI WARNNING:    ");
	if(nr_irq==0)print(" !Division By Zero Exception!");
	if(nr_irq==1)print(" !Debug Exception!");
	if(nr_irq==2)print(" !Non Maskable Interrupt Exception!");
	if(nr_irq==3)print(" !Breakpoint Exception!");
	if(nr_irq==4)print(" !Into Detected Overflow Exception!");
	if(nr_irq==5)print(" !Out of Bounds Exception!");
	if(nr_irq==6)print(" !Invalid Opcode Exception!");
	if(nr_irq==7)print(" !No Coprocessor Exception!");
	if(nr_irq==8)print(" !Double Fault Exception!");
	if(nr_irq==9)print(" !Coprocessor Segment Overrun Exception!");
	if(nr_irq==10)print(" !Bad TSS Exception!");
	if(nr_irq==11)print(" !Segment Not Present Exception!");
	if(nr_irq==12)print(" !Stack Fault Exception!");
	if(nr_irq==13)print(" !General Protection Fault Exception!");
	if(nr_irq==14)print(" !Page Fault Exception!");
	if(nr_irq==15)print(" !Unknown Interrupt Exception!");
	if(nr_irq==16)print(" !Coprocessor Fault Exception!");
	if(nr_irq==17)print(" !Alignment Check Exception (486+)!");
	if(nr_irq==18)print(" !Machine Check Exception (Pentium/586+)!");
	if(nr_irq==19)print(" !Reserved Exceptions!");
	nextline();print(" halt...");
	set_text_attr(LightGray,Black);
	while(1);
}


void _irq0handler_main() { panic(0); }
void _irq1handler_main() { panic(1); }
void _irq2handler_main() { panic(2); }
void _irq3handler_main() { panic(3); }
void _irq4handler_main() { panic(4); }
void _irq5handler_main() { panic(5); }
void _irq6handler_main() { panic(6); }
void _irq7handler_main() { panic(7); }
void _irq8handler_main() { panic(8); }
void _irq9handler_main() { panic(9); }
void _irq10handler_main() { panic(10); }
void _irq11handler_main() { panic(11); }
void _irq12handler_main() { panic(12); }
void _irq13handler_main() { panic(13); }
void _irq14handler_main() { panic(14); }
void _irq15handler_main() { panic(15); }
void _irq16handler_main() { panic(16); }
void _irq17handler_main() { panic(17); }
void _irq18handler_main() { panic(18); }
void _irq19to31handler_main() { panic(19); }