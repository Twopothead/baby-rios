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

.global timer_8253_handler
timer_8253_handler:
	push $666;/*push unique signal 666 */
	jmp asm_all_trap /*call timer_8253_handler_main*/
	
.global hd0_handler
hd0_handler:
	push $111;/*push unique signal 111 */
	jmp asm_all_trap /*call hd0_handler_main*/

.global hd1_handler
hd1_handler:
	push $222;/*push unique signal 222 */
	jmp asm_all_trap /*call hd1_handler_main*/	
 


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


/*system call*/
.global _syscall_handler
_syscall_handler:
	push $0x80 /*push unique signal 0x80 128 */
	jmp asm_all_trap /*call hd1_handler_main*/
	
/*Internel Exceptions
 *Here are some stupid codes,but necessary. :)
 */

.global  _isr0_handler
_isr0_handler:
	push $0;/*push unique signal 0 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr1_handler
_isr1_handler:
	push $1;/*push unique signal 1 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr2_handler
_isr2_handler:
	push $2;/*push unique signal 2 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr3_handler
_isr3_handler:
	push $3;/*push unique signal 3 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr4_handler
_isr4_handler:
	push $4;/*push unique signal 4 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr5_handler
_isr5_handler:
	push $5;/*push unique signal 5 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr6_handler
_isr6_handler:
	push $6;/*push unique signal 6 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr7_handler
_isr7_hander:
	push $7;/*push unique signal 7 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr8_handler
_isr8_handler:
	push $8;/*push unique signal 8 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr9_handler
_isr9_handler:
	push $9;/*push unique signal 9 */
	jmp asm_all_trap /*call hd1_handler_main*/

.global  _isr10_hander
_isr10_handler:
	push $10;/*push unique signal 10 */
	jmp asm_all_trap /*call hd1_handler_main*/
		
.global  _isr11_handler
_isr11_handler:
	push $11;/*push unique signal 11 */
	jmp asm_all_trap /*call hd1_handler_main*/		

.global  _isr12_handler
_isr12_handler:
	push $12;/*push unique signal 12 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr13_handler
_isr13_handler:
	push $13;/*push unique signal 13 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr14_handler
_isr14_handler:
	push $14;/*push unique signal 14 */
	jmp asm_all_trap /*call hd1_handler_main*/				

.global  _isr15_handler
_isr15_handler:
	push $15;/*push unique signal 15 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr16_handler
_isr16_handler:
	push $16;/*push unique signal 16 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr17_handler
_isr17_handler:
	push $17;/*push unique signal 17 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr18_hander
_isr18_handler:
	push $18;/*push unique signal 18 */
	jmp asm_all_trap /*call hd1_handler_main*/	

.global  _isr19to31_handler
_isr19to31_handler:
	push $19;/*push unique signal 19 */
	jmp asm_all_trap /*call hd1_handler_main*/						