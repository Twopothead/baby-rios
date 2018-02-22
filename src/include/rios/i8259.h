#ifndef _i8259_H
#define _i8259_H

/* 8259 port address*/

#define PIC0_IMR 0x21
#define PIC1_IMR 0xA1
/* programmable interrupt controller (PIC) */
/* Interrupt Mask Register (IMR) */
/* The PIC ignores the request of IRQ whose bit is set.*/

/*master PIC*/
#define PIC0_ICW1_PORT 0x20
#define PIC0_ICW2_PORT 0x21
#define PIC0_ICW3_PORT 0x21
#define PIC0_ICW4_PORT 0x21

/*slave PIC*/
#define PIC1_ICW1_PORT 0xa0
#define PIC1_ICW2_PORT 0xa1
#define PIC1_ICW3_PORT 0xa1
#define PIC1_ICW4_PORT 0xa1




#endif