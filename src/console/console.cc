/*
 * RiOS/src/console/console.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/console.h>
char *pVGA= (char *)0xb8000;
static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT =25;
static const int Bytes_each_box = 2; /*attr , text*/
static u32 Vx,Vy;
static u32 top,bottom;
static u32 pos;
static unsigned char attr = 0x07; /*white text,black background*/


/*static inline u8 VGA_color_code(enum Color foreground,enum Color background );*/
class foo{public :void func(){};};

int strlen(const char * str)
{
	int length = 0;
	while(str[length]) length++;
	return length;
}

void putch(u8 ch)
{

	__asm__ ("movb %1,%%ah\n\t"
		 "movw %%ax,%2\n\t"
		 ::"a"(ch),"m"(attr),"m"(*(short *)pos)
		);
	pos += Bytes_each_box;
}

void puts(char *str)
{


}
void print(const char *str)
{
	/*set_text_attr(LightGray,Black);*/
	for(int i = 0; i< strlen(str); i++)
		putch(str[i]);

}
void outtextxy(int x,int y,u8 *textstring)
{

}

void set_text_attr(enum Color foreground,enum Color background)
{
	 attr = VGA_color_code(foreground,background);
}

void outtext(char* textstring)
{

}

void init_console(void)
{
	pos = 0xb8000;
	pos+=2 * (SCREEN_WIDTH * Bytes_each_box);
	set_text_attr(LightGray,Black);
	msg_gdt_ok();
}

void clear_screen(void)
{
	unsigned int i=0;
	while(i<SCREEN_WIDTH*SCREEN_HEIGHT*2){
		pVGA[i++] = ' ';
		pVGA[i++] = VGA_color_code(LightGray,Black);
	}

}

void nextline(void)
{
	double i=double((pos - 0xb8000))/double(SCREEN_WIDTH * Bytes_each_box);
	if(i>0)pos=0xb8000+(int)(i+1)*SCREEN_WIDTH * Bytes_each_box;
}
/*
 *Bit(s)	Value
 *0-7		ASCII code point
 *8-11		Foreground color
 *12-14		Background color
 *15		Blink
*/
/*
|	 7 6 5 4 3 2 1 0	|
|       BL R G B I R G B	|
|__________Back____Fore_________|
	BL:Blink, I:Highligh
*/	