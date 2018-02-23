/*
 * RiOS/src/include/rios/console.h
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#ifndef _CONSOLE_H
#define _CONSOLE_H


#include <rios/type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


enum Color{
	Black	       	= 0,
	Blue	        = 1,
	Green           = 2,
	Cyam            = 3,
	Red             = 4,
	Magenta         = 5,
	Brown           = 6,
	LightGray       = 7,
	DarkGray        = 8,
	LightBlue       = 9,
	LightGreen      = 10,
	LightCyan 	= 11,
	LightRed 	= 12,
	Pink		= 13,
	Yellow 		= 14,
	White		= 15,
};
void puts(char *str);
void set_text_attr(enum Color foreground,enum Color background);
void outtextxy(int x,int y,char *textstring);
void outtext(char* textstring);
void init_console(void);
void putch(u8 ch);
int strlen(const char * str);
void print(const char *str);
static inline u8 VGA_color_code(enum Color foreground,enum Color background ){
	return (background<<4 | foreground);
}
void clear_screen(void);
void nextline();
void con_putch(u8 ch);


void print_cpqr(const char *str);
void msg_ok();
void msg_gdt_ok();
void msg_idt_ok();
void msg_keyboard_ok();
void msg_trapframe_ok();
void print_njau_logo();
void set_cursor();
void scroll();
void cmd_start();
int cmd_matching(char *str1,char *str2);
void init_Rishell();
void putnum(int value);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif