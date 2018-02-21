.code32
.align 4
.global update_gdt
.extern gdt_pointer
update_gdt:
	lgdt (gdt_pointer) 
	mov $0x10,%ax	/*gdt[2] data seg*/
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs
	mov %ax,%ss	/*flush registers*/
	ljmp $0x08,$gdt_ok /*$0x08 :code seg offset*/
	/* 0 (selector = 0x0000): null descriptor */
	/* 1 (selector = 0x0008): code descriptor */
	/* 2 (selector = 0x0010): data descriptor */
gdt_ok:
	ret	
/*
 *GDT一条记录64位，８Ｂ
 * 0 -  8,8-  16,16 - 24
 * 0x0  0x8    0x10
 */