#ifndef _SYSTEM_H
#define _SYSTEM_H
#define sti() __asm__ ("sti"::) /*set interrupt*/
#define cli() __asm__ ("cli"::) /*clear interrupt*/
#define nop() __asm__ ("nop"::)
#define iret() __asm__ ("iret"::)
#endif