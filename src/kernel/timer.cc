#include <rios/console.h>
#include <rios/timer.h>
#include <rios/i8259.h>
#include <asm/x86.h>
int jiffies =0;
void do_timer()
{
	jiffies ++;
	outb(PIC0_OCW2,0x60);/*irq acknowledge*/
	outnumxy(0,60,jiffies);
/*inform 8259A that irq0 has been handled*/
	
	//print("A");/*print A all the time*/
}