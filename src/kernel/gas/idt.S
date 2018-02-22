.code32
.align 4

.global update_idt
.extern idt_pointer

update_idt:
 	lidt (idt_pointer)
 	ret


.global irq_empty

irq_empty:
 	pushw %es
 	pushw %ds
	pushal
	movl %esp,%eax
	pushl %eax
	movw %ss,%ax
	movw %ax,%ds
	movw %ax,%es//ensure ss=ds=es
        //call ...
	popl %eax
	popal
	popw %ds
	popw %es
	iret

.global keyboard_handler
.extern keyboard_handler_main

keyboard_handler:
	call keyboard_handler_main
	iret

