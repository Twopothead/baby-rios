#ifndef _IRQ_H
#define _IRQ_H

#include <rios/type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <rios/timer.h>
	
void keyboard_handler_main();
void hd_handler_main();
void timer_8253_handler_main();

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif