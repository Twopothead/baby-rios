#include <rios/keyboard.h>
u8 keymap[128]=
{
	0,27,
	'1','2','3','4','5','6','7','8','9','0','-','=',
	'\b','\t', /*Backspace , Tap*/
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',/*Enter Key*/
	0,
	'a','s','d','f','g','h','j','k','l',';','\'','`','\r',
	'\\','z','x','c','v','b','n','m',',','.','/',	'\r',/*Right Shift*/
	0,	/*all other key are not defined */
};
void do_keyboard()
{
	u8 kbscancode;
	outb(0x20,0x20);/*EOI signal (End of interrupt acknowlegment)*/
	/*by writing EOI to PIC, PIC will allow further interrupt requests,otherwise not.*/
	kbscancode = inb(KEYBOARD_DATA_PORT);/*inb(0x60)*/
	if(kbscancode & 0x80)/*if the top bit of the scan code is set => a key has just been released*/
	{
		/*keys like shift,alt .etc are released*/
	}else {
		if(kbscancode == ENTER_KEYCODE){
			print("hello ");
		}
		else{
			con_putch(keymap[kbscancode]);
		}
	}

}