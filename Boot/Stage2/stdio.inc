 
%ifndef __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
%define __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
 
;************************************************;
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


waite:
	push eax 
	xor eax ,eax 
	.ss:
	inc eax
	cmp eax,0x100000
	jz .end
	jmp .ss
	.end:
	pop eax
	ret

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Puts16ns:
		pusha				; save registers
.Loop1:
		lodsb				; load next byte from string from SI to AL
		or	al, al			; Does AL=0?
		call waite
		;jz	Puts16Done		; Yep, null terminator found-bail out
		mov	ah, 0eh			; Nope-Print the character
		int	10h			; invoke BIOS
		jmp	.Loop1			; Repeat until null terminator found
.Puts16Done:
		popa				; restore registers
		ret				; we are done, so return

 ;   in: AX
Putn32:
    pusha
    mov ecx,10
    .start:
    or eax,eax 
    jz .end
	mov edx,0 
    div ecx
	push eax
    mov al,dl
    add al,48
    mov	ah, 0eh			; Nope-Print the character
	int	10h			; invoke BIOS
	pop eax
    jmp .start
    .end:
    popa 
    ret

%endif ;__STDIO_INC_67343546FDCC56AAB872_INCLUDED__