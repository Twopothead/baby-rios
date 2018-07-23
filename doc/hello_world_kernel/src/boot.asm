 
 ; RiOS/src/boot.asm
 
 ; Copyright (C) 2018 qiuri 
global RiOS_start 
bits 32 
section .text
RiOS_start:
  mov word [0xb8000],0x075b;[
  mov word [0xb800a],0x075d;]
  mov dword [0xb8004],0x0a4b0a4f ;OK
  mov dword [0xb8010],0x07690752;
        mov dword [0xb8014],0x0753074f; RiOS
  ;for more info about VGA resources,
  ;please refer to  https://wiki.osdev.org/Printing_to_Screen
  hlt
 
; for ascii, $ man ascii
 
;|   7 6 5 4 3 2 1 0  |
;|      BL R G B I R G B  |
;|_________Back____Fore_________|
;BL:Blink, I:Highligh
 
 
 
