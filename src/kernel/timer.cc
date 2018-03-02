#include <rios/console.h>
#include <rios/timer.h>
#include <rios/i8259.h>
#include <asm/x86.h>
void do_timer()
{
	outb(PIC0_OCW2,0x60);
/*inform 8259A that irq0 has been handled*/
	
	//print("A");/*print A all the time*/
}