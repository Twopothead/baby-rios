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

/*
 * write_port(value,port)
 *    out 21h,al |  out %al,$0x21
 * read_port(port)
 *    in al,21h  |  in $0x21,%al
 */
/* outb_wait is almost the same as outb,but wait for a little while*/
#define inb_wait(port)({ \
	unsigned char _v; \
	__asm__ volatile ("inb %%dx,%%al\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:":"=a" (_v):"d" (port)); \
_v; \
})

/*for hd.cc*/
#define port_read(port,buffer,num) \
 __asm__ ("cld; rep; insw"::"d"(port),"D"(buffer),"c"(num))

#define port_write(port,buffer,num) \
 __asm__ ("cld; rep; outsw"::"d"(port),"S"(buffer),"c"(num))

#ifndef NULL
#define NULL ((void *) 0)
#endif

/*
 * S => Si ,D => Di
 *Here is a mannual online of Inline assembly for x86 
 *	https://www.ibm.com/developerworks/library/l-ia/index.html
 *and here is a comparision of GAS and NASM
 * 	https://www.ibm.com/developerworks/library/l-gas-nasm/index.html
 */



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*set and clear bitmap's bit*/	
static inline void bitmap_set_bit(unsigned int nr, void * addr){
	u8 *tmp = (u8*)addr;
	tmp += nr >> 3;
	*tmp |= 1<<(7-(nr%8));
}

static inline int bitmap_test_bit(unsigned int nr, void * addr){
	u8 *tmp = (u8*)addr;
	tmp += nr >> 3;
	return (*tmp &(1<<(7-(nr%8))))!=0;
}

// int testb(void *s, unsigned int i) {
//     unsigned char *v =(unsigned char *) s;
//     v += i>>3;
//     return (*v&(1<<(7-(i%8)))) !=0;
// }

/*the (nr)th bit of address is set to 1*/
#ifdef __cplusplus
}
#endif

#define bitmap_clear_bit(nr, addr)({ \
register int _res; \
__asm__ volatile("btrl %2,%3\n\tsetnb %%al": \
"=a"(_res):"a"(0),"r"(nr),"m"(addr)); \
_res;})
/*the (nr)th bit of address is cleared to 0*/






#endif