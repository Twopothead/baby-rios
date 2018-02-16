/*
 *  RiOS/src/kernel/gas/gas.S
 * 
 *  Copyright (C) 2018 Frank Curie (邱日)
 */

.global write_port
.global read_port

.code32

write_port:
	mov 4(%esp),%edx /*@parameter1 value*/
	mov 8(%esp),%al  /*@parameter2 port */
	out %al,%dx
	ret

read_port:
	mov 4(%esp),%edx /*@parameter1 port*/
	in %dx,%al
	ret
/*
 * write_port(value,port)
 *    out 21h,al |  out %al,$0x21
 * read_port(port)
 *    in al,21h  |  in $0x21,%al
 */

