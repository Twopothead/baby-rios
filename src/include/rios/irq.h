#ifndef _IRQ_H
#define _IRQ_H

#include <rios/type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/timer.h>
	
void keyboard_handler_main();
void hd0_handler_main();
void hd1_handler_main();
void timer_8253_handler_main();


void _irq0handler_main();
void _irq0handler_main();
void _irq1handler_main();
void _irq2handler_main();
void _irq3handler_main();
void _irq4handler_main();
void _irq5handler_main();
void _irq6handler_main();
void _irq7handler_main();
void _irq8handler_main();
void _irq9handler_main(); 
void _irq10handler_main();
void _irq11handler_main();
void _irq12handler_main();
void _irq13handler_main();
void _irq14handler_main();
void _irq15handler_main();
void _irq16handler_main();
void _irq17handler_main();
void _irq18handler_main();
void _irq19to31handler_main();

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif