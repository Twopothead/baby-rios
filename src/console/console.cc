/*
 * RiOS/src/console/console.cc
 * 
 * Copyright (C) 2018 Frank Curie (邱日)
 */
#include <rios/console.h>
#include <asm/x86.h>
#include <rios/keyboard.h>
char *pVGA= (char *)0xb8000;
static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT =25;
static const int Bytes_each_box = 2; /*attr , text*/
static u32 Vx,Vy;
static u32 top,bottom;
static u32 pos;/*the most important*/
static unsigned char attr = 0x07; /*white text,black background*/
static u32 index;/*about cursor pos*/
static u8 cmd_running = 0;
static u8 cmd_input =0;
#define value2ascii(n) (n+48)
u8 cmd_buffer[80*25];/*keyboard data buffer*/
int cmd_buffer_index = 0;


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
	set_cursor();
	
	
}

void del()
{
	set_cursor();
	if(Vx > 18){/*[root@localhost]# */
		pos -= 1<<1;
		*(char *)pos = ' ';
		set_cursor();
	}
	cmd_buffer[cmd_buffer_index]=' ';
	cmd_buffer_index --;/*delete a char in buffer*/
}

void con_putch(u8 ch)
{ /* put a char in console */
	
	scroll();
	cli();
	if(ch == BACKSPACE_KEYCODE){/*Backspace*/
		del();
	}else if(ch == ENTER_KEYCODE){
			if(cmd_matching((char*)cmd_buffer,"clear")){
				clear_screen();
				clear_cmd_buffer();
				pos = 0xb8000;
			}else if(cmd_matching((char*)cmd_buffer,"help")){
				msg_cmd_help();
			}else if(cmd_matching((char*)cmd_buffer,"logo")){
				print_njau_logo();
			}

			nextline();
			cmd_start();
			cmd_input = 0;	
			clear_cmd_buffer();
	}
	else if (ch == RIGHT_CTRL_KEYCODE){
	/*@debug
	 *press right ctrl key for debug,print used buffer size
	 */
		putnum(cmd_buffer_index);
		
	}else{
		cmd_input = 1;
		putch(ch);
		ch2cmd_buffer(ch);
	}
	sti();
}

void ch2cmd_buffer(u8 ch)
{
	if(cmd_input==1)
	cmd_buffer[cmd_buffer_index] = ch;
	cmd_buffer_index++;
}


void puts(char *str)
{


}
void print(const char *str)
{
	/*set_text_attr(LightGray,Black);*/
	for(int i = 0; i< strlen(str); i++){
		scroll();
		con_putch(str[i]);
	}

}
void outtextxy(int x,int y,u8 *textstring)
{
	int tmp = pos;
	pos = x*80+y;
	print((const char *)textstring);
	pos=tmp;
	
}

void outnumxy(int x,int y,int value)
{
	int tmp = (x*80+y)*2;
	if(value==0){pVGA[tmp]='0';return;}
  	        int raw = value;
  	        int a[32];
  	        int l=0;
  	        while(value){
  	                      a[l++] = value%10;
  	                      value/=10;
  	        }
 /*just like a stack*/
                 if(raw != 0){
  	                      while(--l >= 0)
  	                      pVGA[tmp+=2]=value2ascii(a[l]);
                 }
  		return;
	set_cursor();

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
	set_cursor();
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

int cmd_matching(char *str1,char *str2)
{/*cmd_matching(your_cmd_input,system)*/
	int _l1 = strlen(str1);
	int _l2 = strlen(str2);
	if(_l2 > _l1){
		return 0;
	}else {
		int i = 0;
		while(i < _l2){
			if(str1[i] != str2[i])
				return 0;
			i++;
		}
		return 1;
	}
}

void print_cpqr(const char *str)
{
	scroll();
	/*set_text_attr(LightGray,Black);*/
	for(int i = 0; i< strlen(str); i++){
		if(str[i]=='@'){
			set_text_attr(Yellow,Black);
			con_putch(str[i]);
		}else if(str[i]=='x'){
			set_text_attr(Magenta,Black);
			con_putch(str[i]);
		}else if(str[i]=='.'){
			set_text_attr(Brown,Black);
			con_putch(str[i]);
		}else{
			set_text_attr(LightGray,Black);
			con_putch(str[i]);
		}
	}

}
void print_njau_logo()
{
	nextline();
	#include <rios/cpqr.txt>
	set_text_attr(LightGray,Black);
	//pos=0xb8000;
	clear_cmd_buffer();
	cmd_buffer_index = 0;

}

void set_cursor()
{
	/* more hardware info about cursor: https://wiki.osdev.org/Text_Mode_Cursor*/
	cli();
	u16 xyindex = (pos-0xb8000)/Bytes_each_box;
	Vx = xyindex%80;
	Vy = xyindex/80;
	u16 tmp = (Vy*80)+Vx;
	outb(0x3d4,0x0f);
	outb(0x3d5,(u8)(tmp & 0xff));/*cursor low port: reg15*/
	outb(0x3d4,0x0e);
	outb(0x3d5,(u8)( (tmp >> 8) & 0xff));/*cursor high port:　reg14*/
	/*about 0x3d4 and 0x3d5 port:http://blog.csdn.net/gemini_star/article/details/4438280 */
	sti();
}

void update_cursor()
{
	

}

void scroll()
{
	index = pos - 0xb8000;
	if( index >= 80 *25 *2){
		index = 0;
		while(index < 80 * 24 *2){
			pVGA[index] = pVGA[index + 160];
			pVGA[index+1]=pVGA[index+161];
			index+=2;
		}
		index =80*24*2;pos = index + 0xb8000;
		while(index<80*25*2){
			putch(' ');index = pos - 0xb8000;
		}
		index =80*24*2;pos = index + 0xb8000;
	}
	
}

void cmd_start()
{
 	set_text_attr(Yellow,Black);print("[");
 	set_text_attr(Green,Black);print("root");
 	set_text_attr(Blue,Black);print("@");
 	set_text_attr(Green,Black);print("localhost");
 	set_text_attr(Yellow,Black);print("]");
 	set_text_attr(Green,Black);print("# ");
	set_text_attr(LightGray,Black);
}

void clear_cmd_buffer()
{
	int i=0;
	while(i < 80*25){
		cmd_buffer[i] = '\0';
		i++;
	}
	cmd_buffer_index = 0;


}

void init_Rishell()
{
	cli();
	//print_njau_logo();
	clear_cmd_buffer();
	cmd_buffer_index = 0;
	cmd_input = 0;
	set_cursor();
	sti();
	msg_Rishell_ok();
}

void putnum(int value)
{
                if(value==0){con_putch('0');return;}
  	        int raw = value;
  	        int a[32];
  	        int l=0;
  	        while(value){
  	                      a[l++] = value%10;
  	                      value/=10;
  	        }
 /*just like a stack*/
                 if(raw != 0){
  	                      while(--l >= 0)
  	                      con_putch(value2ascii(a[l]));
                 }
  		return;
 }
void puthex(int value)
{
                if(value==0){con_putch('0');return;}
  	        int raw = value;
  	        int a[32];
  	        int l=0;
  	        while(value){
  	                      a[l++] = value%16;
  	                      value/=16;
  	        }
 /*just like a stack*/
                 if(raw != 0){
  	                      while(--l >= 0){
  	                      	 if(a[l]<10)
  	                      		con_putch(value2ascii(a[l]));else if(a[l]==10)con_putch(0x61);
  	                      	  else if(a[l]==11)con_putch(0x62);
  	                      	  else if(a[l]==12)con_putch(0x63);
  	                      	  else if(a[l]==13)con_putch(0x64);
  	                      	  else if(a[l]==14)con_putch(0x65);
  	                      	   else if(a[l]==15)con_putch(0x64);

  	              		}
                 }
  		return;
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