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

#include <stdint.h>
char *pVGA= (char *)0xb8000;
static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT =25;
void clear_screen(void)
{
	unsigned int i=0;
	while(i<SCREEN_WIDTH*SCREEN_HEIGHT*2){
		pVGA[i++] = 'R';
		pVGA[i++] = 0b01000000;
	}

}

