;*************************************************
;	stdio.inc
;		-Input/Output routines
;
;	OS Development Series
;*************************************************
 
%ifndef __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
%define __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
 
;************************************************;
;	Puts16 ()
;		-Prints a null terminated string
;	DS=>SI: 0 terminated string
;************************************************;
 
bits	16
 
Puts16:
		pusha				; save registers
.Loop1:
		lodsb				; load next byte from string from SI to AL
		or	al, al			; Does AL=0?
		jz	Puts16Done		; Yep, null terminator found-bail out
		mov	ah, 0eh			; Nope-Print the character
		int	10h			; invoke BIOS
		jmp	.Loop1			; Repeat until null terminator found
Puts16Done:
		popa				; restore registers
		ret				; we are done, so return
 




;************************************************;
;	Putch32 ()
;		-Prints a char in Pmode
;	bl: char for print
;	ah: Column al : Row
;************************************************;
bits 32

%define		VIDMEM	0xB8000			; video memory
%define		COLS	80			; width and height of screen
%define		LINES	25
%define		CHAR_ATTRIB 14			; character attribute (White text on black background)
 
_CurX db 0					; current x/y location
_CurY db 0

Putch32:

	cmp bl,0x0D
	jz .Enter
	
	cmp bl,0xA
	jz .nextY
		
	.write:
	cmp byte[_CurX],COLS
	jnz .start
	inc byte [_CurY]
	
	.start:
	pusha 
	mov al,byte [_CurY]
	mov bh,80
	mul bh ; Row * 80 
	
	add ax, [_CurX] 
	mov cl,2
	mul cl
	
	mov edi , VIDMEM
	
	add di ,ax
	mov dl,bl 
	mov dh,CHAR_ATTRIB
	
	mov [edi],dx
	inc byte [_CurX]
	popa
ret 

.Enter:
mov byte [_CurX],0
ret

.nextY:
inc byte [_CurY]
ret


;************************************************;
;	Puts32 ()
;		-Prints a null terminated string
;	ebx: Address of String to print
;************************************************;
Puts32:

	.start:
	cmp byte [ebx],0 
	jz .end 
	
	push ebx 
	mov al,[ebx]
	mov bl,al
	call Putch32
	pop ebx
	inc ebx
	jmp .start

.end:
	mov	bh, byte [_CurY]	; get current position
	mov	bl, byte [_CurX]
	call	MoveCur			; update cursor
	 
ret 


;**************************************************;
;	MoveCur ()
;		- Update hardware cursor
;	parm/ bh = Y pos
;	parm/ bl = x pos
;**************************************************;

MoveCur:
	pusha 
	mov ax,COLS
	mul bh 
	add al,bl 
	mov bx,ax 
	
;-------------------------------------;
;	        Low Byte Send             ;
;-------------------------------------;
	mov al,0xf 
	mov dx,0x03d4 ; index register 
	out dx,al
	
	mov al,bl 
	mov dx,0x03d5 ; Data Reg
	out dx,al

;-------------------------------------;
;	      High Byte Send              ;
;-------------------------------------;
	mov al,0xE 
	mov dx,0x03d4 ; index register 
	out dx,al
	
	mov al,bh 
	mov dx,0x03d5 ; Data Reg
	out dx,al	
	
	popa 
ret 

;**************************************************;
;	ClearScreen  ()
;		- Update hardware cursor
;	parm/ bh = Y pos
;	parm/ bl = x pos
;**************************************************;
ClrScr32:
	pusha 
	mov edi,VIDMEM
	mov al,' '
	mov ah,CHAR_ATTRIB
	mov cx,2000
	rep stosw
	
	mov	bh, byte [_CurY]	; get current position
	mov	bl, byte [_CurX]
	call	MoveCur			; update cursor
	
	popa 
	ret



	%endif ;__STDIO_INC_67343546FDCC56AAB872_INCLUDED__