#ifndef _TRAP_H
#define _TRAP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct TrapFrame{
/*registers that pushed by "pushal",xxxesp is useless */	
	u32 edi,esi,ebp,xxxesp,ebx,edx,ecx,eax;
/*below are defined by x86 hardware:eip cs .. eflags*/
	u32 err;/*irq*/
	u32 eip; 
	u16 cs; u16 padding;
	u32 eflags;
};

void irq_handle(TrapFrame *trapframe);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif