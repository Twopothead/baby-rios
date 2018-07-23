 
 ; RiOS/src/multiboot.asm
 
 ; Copyright (C) 2018 qiuri
 
bits 32 
section .multiboot_header
multiboot_start:
;for more info about Multiboot Specification for GRUB2 
;http://nongnu.askapache.com/grub/phcoder/multiboot.pdf
  dd 0xe85250d6  ;u32,"magic number" for grub2
  dd 0    ;u32,"architecture" 0 => i386 protected mod, 4 => MIPS
  dd multiboot_end - multiboot_start ;u32,"header length" 
  dd 0x100000000 - (0xe85250d6+0+(multiboot_end-multiboot_start))
  ;u32,"checksum", magic number+architecture+header_length should be zero
 
 
  ;"end tag" (u16 type,u16 flags,u32 size) (0,0,8)
  dw 0  ;u16,"type" 
  dw 0  ;u16,"flags"
  dd 8  ;u32,"size"
 
multiboot_end:
 

