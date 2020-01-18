bits 32
global _start
extern kernel_early
extern main

section .text
	align 4
	dd 0x1BADB002		 ; magic
	dd 0x00			 ; flags
	dd - (0x1BADB002 + 0x00) ; checksum

_start:
	cli
	mov esp, stack
	call kernel_early
	; do other stuff
  ; from ://wiki.osdev.org/Text_Mode_Cursor#Disabling_the_Cursor_2
  mov dx,0x3D4
  mov al,0xA
  out dx,al
  inc dx
  mov al,0x20
  out dx,al
  ;
	call main
	hlt

section .bss
resb 8192
stack:
