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



