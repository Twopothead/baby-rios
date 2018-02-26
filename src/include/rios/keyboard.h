/*
 * RiOS/include/rios/keyboard.h
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <rios/type.h>
#include <asm/x86.h>
#include <rios/console.h>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEYCODE	0x1c
#define BACKSPACE_KEYCODE 0x0e
#define RIGHT_CTRL_KEYCODE 0x1d


/*handle keyboard interrupt*/
void do_keyboard();

#ifdef __cplusplus
}
#endif /* __cplusplus */


/*reference:https://baike.baidu.com/item/键盘扫描码
 *	    https://wiki.osdev.org/PS2_Keyboard
 *	    Linux-0.11/kernel/chrdrv/keyboad.s => keymap
 * $ man ascii
 */


#endif