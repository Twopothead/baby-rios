/*
 * RiOS/include/asm/x86.h
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#ifndef _X86_H
#define _X86_H

#include <rios/type.h>


#define sti() __asm__ ("sti"::)
#define cli() __asm__ ("cli"::)
#define nop() __asm__ ("nop"::)
#define iret() __asm__ ("iret"::)

#define outb(port,value) \
  	__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))

#define inb(port) ({ \
  	unsigned char _v; \
  	__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
  	_v; \
  	})



#define outb_wait(port,value) \
__asm__ ("outb %%al,%%dx\n" \
	"\tjmp 1f\n" \
	"1:\tjmp 1f\n" \
	"1:"::"a" (value),"d" (port))


#define inb_wait(port)({ \
	unsigned char _v; \
	__asm__ volatile ("inb %%dx,%%al\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:":"=a" (_v):"d" (port)); \
_v; \
})

/*
 * write_port(value,port)
 *    out 21h,al |  out %al,$0x21
 * read_port(port)
 *    in al,21h  |  in $0x21,%al
 */
/* outb_wait is almost the same as outb,but wait for a while*/
#endif