.code32
.align 4

.global update_idt
.extern idt_pointer

update_idt:
 	lidt (idt_pointer)
 	ret


.global irq_empty
irq_empty: 
	pushl $-1;/*push unique signal -1*/
	jmp asm_all_trap;

.global keyboard_handler
keyboard_handler:
	push $888;/*push unique signal 888 */
	jmp asm_all_trap /*call keyboard_handler_main*/

.global hd0_handler
hd0_handler:
	push $111;/*push unique signal 111 */
	jmp asm_all_trap /*call hd0_handler_main*/

.global hd1_handler
hd1_handler:
	push $222;/*push unique signal 222 */
	jmp asm_all_trap /*call hd1_handler_main*/	
 
.global timer_8253_handler
timer_8253_handler:
	push $666;/*push unique signal 666 */
	jmp asm_all_trap /*call timer_8253_handler_main*/

/* all traps go here */
.global asm_all_trap
.extern irq_handle
asm_all_trap:
	pushal 
	push %esp
	call irq_handle
	pop %esp
	popal
	add $4,%esp/*to get rig of "push unique signal,eg 888 ,-1" */
	iret



/*Internel Exceptions
 *Here are some stupid codes,but necessary. :)
 */

.global  _isr0_hander
_isr0_hander:
	push $0;/*push unique signal 0 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr1_hander
_isr1_hander:
	push $1;/*push unique signal 1 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr2_hander
_isr2_hander:
	push $2;/*push unique signal 2 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr3_hander
_isr3_hander:
	push $3;/*push unique signal 3 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr4_hander
_isr4_hander:
	push $4;/*push unique signal 4 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr5_hander
_isr5_hander:
	push $5;/*push unique signal 5 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr6_hander
_isr6_hander:
	push $6;/*push unique signal 6 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr7_hander
_isr7_hander:
	push $7;/*push unique signal 7 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr8_hander
_isr8_hander:
	push $8;/*push unique signal 8 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr9_hander
_isr9_hander:
	push $9;/*push unique signal 9 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr10_hander
_isr10_hander:
	push $10;/*push unique signal 10 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr11_hander
_isr11_hander:
	push $11;/*push unique signal 11 */
	jmp asm_all_trap /*call hd1_handler_main*/		

.global  _isr12_hander
_isr12_hander:
	push $12;/*push unique signal 12 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr13_hander
_isr13_hander:
	push $13;/*push unique signal 13 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr14_hander
_isr14_hander:
	push $14;/*push unique signal 14 */
	jmp asm_all_trap /*call hd1_handler_main*/				

.global  _isr15_hander
_isr15_hander:
	push $15;/*push unique signal 15 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr16_hander
_isr16_hander:
	push $16;/*push unique signal 16 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr17_hander
_isr17_hander:
	push $17;/*push unique signal 17 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr18_hander
_isr18_hander:
	push $18;/*push unique signal 18 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr19to31_hander
_isr19to31_hander:
	push $19;/*push unique signal 19 */
	jmp asm_all_trap /*call hd1_handler_main*/						