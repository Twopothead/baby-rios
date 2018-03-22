#include <rios/keyboard.h>
#include <rios/console.h>
u8 keymap[128]=
{
	0,27,
	'1','2','3','4','5','6','7','8','9','0','-','=',
	'\b','\t', /*Backspace , Tab*/
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',/*Enter Key*/
	0,
	'a','s','d','f','g','h','j','k','l',';','\'','`','\r',
	'\\','z','x','c','v','b','n','m',',','.','/',	'\r',/*Right Shift*/
	0,	/*all other key are not defined */
};
void do_keyboard()
{
/*
 *outb(0x20,0x20); necessary! EOI signal (End of interrupt acknowlegment)
 *by writing EOI to PIC, PIC will allow further interrupt requests,otherwise not.
 *but if we enabled Auto EOI in 8086/88 mode in 8259A ,we won't bother to do this.
 */
	u8 kb_status;
	u8 kbscancode;
	kb_status = inb(KEYBOARD_STATUS_PORT);
	if (kb_status & 0x01){
		kbscancode = inb(KEYBOARD_DATA_PORT);/*inb(0x60)*/
		if(kbscancode & 0x80)/*if the top bit of the scan code is set => a key has just been released*/
		{
			/*break code*/
			/*keys like shift,alt .etc are released*/
		}else{ 
			// putch(keymap[kbscancode]);
			if(kbscancode == ENTER_KEYCODE){
				con_putch(kbscancode);
			}else if(kbscancode == BACKSPACE_KEYCODE){
				con_putch(kbscancode);
			}else if(kbscancode == RIGHT_CTRL_KEYCODE){
				con_putch(kbscancode);
			}else if(kbscancode == SLASH_KEYCODE){
				//con_putch(kbscancode);
				con_putch(0x5c);/* '\\' */
			}else if(kbscancode == SPACE_KEYCODE){
				con_putch(0x20);/*空格　man ascii */
			}else{
				con_putch(keymap[kbscancode]);
			}
		}
	}
}