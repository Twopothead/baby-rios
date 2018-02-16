/*
 *  RiOS/kernel/console.c
 *
 *   (C) 2018 Frank Curie (邱日) 
 */   

/*
 *  console.c
 *
 *  This module implements the console io functions
 */ 
#include <rios/type.h>
#include <rios/gas.h>
extern void write_port(int value,int port);
char *pVGA= (char *)0xb8000;
static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT =25;
static unsigned long x,y;
static unsigned long top,bottom;
static unsigned long pos;
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
static inline u8 VGA_color_code(enum Color foreground,enum Color background )
{
	return (background<<4 | foreground);
}

class foo{
	public :void func(){};
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void clear_screen(void)
{
	unsigned int i=0;
	while(i<SCREEN_WIDTH*SCREEN_HEIGHT*2){
		pVGA[i++] = 'R';
		pVGA[i++] = 0b01000000;
	}

	foo foo1;
	foo1.func();
	write_port(0x21,0x20);
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

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
