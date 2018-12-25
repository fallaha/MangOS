bits 16
org 0x7c00
start: jmp main

print:
lodsb
or al,al
jz endprint
mov ah,0xE
int 10h
jmp print
endprint:
ret
  
  
main:


mov si,  wel_msg
call print

cli
hlt

wel_msg db "Welcome To My Operating System!",0
times 510 - ($-$$) db 0
dw 0xAA55