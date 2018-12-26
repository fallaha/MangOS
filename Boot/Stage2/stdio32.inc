%ifndef __STDIO32_INC_67343546FDCC56AAB872_INCLUDED__
%define __STDIO32_INC_67343546FDCC56AAB872_INCLUDED__

bits 32
%define VIDADR 0xB8000 ; video memory address
%define		COLS	80			; width and height of screen
%define		LINES	25
%define		CHAR_ATTRIB 14			; character attribute (White text on black background)

    _curX db 0  ; current X location
    _curY db 0  ; current y location


    ; print a character to screen
    ; bl = character
putch32:
    pusha
    ; cheking next Line 
    cmp bl,0x0A ; next Line character
    je .nextRow

    ; Set ax current Location
    xor ax,ax
    mov al,byte [_curY]
    mov cx,COLS
    mul cl
    xor cx,cx
    mov cl,byte[_curX]
    add ax,cx
    shl ax,1
    ; show character
    mov edi,VIDADR
    add edi,eax 
    mov [edi],bl
    mov [edi+1],byte CHAR_ATTRIB

    inc byte [_curX]
    cmp byte [_curX],COLS
    jnz .done

    .nextRow:
    mov byte [_curX],0
    inc byte[_curY]
    .done:
    popa
    ret

    ;   print string
    ;   bx = Adress od input string
puts32:
    pusha
    push ebx
    pop edi
    .loop:
    cmp byte [edi],0
    je .done
    mov bl,[edi]
    call putch32
    inc edi
    jmp .loop
    .done:
    mov	bh, byte [_curY]	; get current position
	mov	bl, byte [_curX]
    call setCursor
    popa 
    ret

setCursor:
	pusha				; save registers (aren't you getting tired of this comment?)
 
	;-------------------------------;
	;   Get current position        ;
	;-------------------------------;
 
	; Here, _CurX and _CurY are relitave to the current position on screen, not in memory.
	; That is, we don't need to worry about the byte alignment we do when displaying characters,
	; so just follow the forumla: location = _CurX + _CurY * COLS
 
	xor	eax, eax
	mov	ecx, COLS
	mov	al, bh			; get y pos
	mul	ecx			; multiply y*COLS
	add	al, bl			; Now add x
	mov	ebx, eax
 
	;--------------------------------------;
	;   Set low byte index to VGA register ;
	;--------------------------------------;
 
	mov	al, 0x0f		; Cursor location low byte index
	mov	dx, 0x03D4		; Write it to the CRT index register
	out	dx, al
 
	mov	al, bl			; The current location is in EBX. BL contains the low byte, BH high byte
	mov	dx, 0x03D5		; Write it to the data register
	out	dx, al			; low byte
 
	;---------------------------------------;
	;   Set high byte index to VGA register ;
	;---------------------------------------;
 
	xor	eax, eax
 
	mov	al, 0x0e		; Cursor location high byte index
	mov	dx, 0x03D4		; Write to the CRT index register
	out	dx, al
 
	mov	al, bh			; the current location is in EBX. BL contains low byte, BH high byte
	mov	dx, 0x03D5		; Write it to the data register
	out	dx, al			; high byte
 
	popa
	ret

clrScr32:
    pusha 
    cld
    mov al,' '
    mov ah,CHAR_ATTRIB
    mov edi,VIDADR
    mov cx,COLS*LINES
    rep stosw
    popa 
    ret 




%endif ;__STDIO32_INC_67343546FDCC56AAB872_INCLUDED__