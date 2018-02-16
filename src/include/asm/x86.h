#ifndef _X86_H
#define _X86_H

#include <rios/type.h>

#define inb(port) ({ \
u8 char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port));\
_v; \
})

/* 
 * static inline u8
 * inb(u8 port)
 * {
 *	u8 data;
 *	asm volatile("in %1,%0" : "=a"(data) : "d" (port));
 *  return data;
 * }
 */
static inline void 
outb(u16 port,u8 data)
{
	asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

#endif