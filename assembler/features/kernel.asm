;org 0x2000
start : jmp main

;-----------------
; main Function
;-----------------
main:

	cli				; Clear interrupts
	mov ax, 0
	mov ss, ax			; Set stack segment and pointer
	mov sp, 0FFFFh
	sti				; Restore interrupts

	cld				; The default direction for string operations
					; will be 'up' - incrementing address in RAM

	mov ax, 2000h			; Set all segments to match where kernel is loaded
	mov ds, ax			; After this, we don't need to bother with
	;mov es, ax			; segments ever again, as MikeOS and its programs
	mov fs, ax			; live entirely in 64K
	mov gs, ax

	
cmp dl, 0
	je no_change
	mov [bsDriveNumber], dl		; Save boot device number
	mov ah, 8			; Get drive parameters
	int 13h
	jc fault
	and cx, 3Fh			; Maximum sector number
	mov [bpbSectorsPerTrack], cx	; Sector numbers start at 1
	movzx dx, dh			; Maximum head number
	add dx, 1			; Head numbers start at 0 - add 1 for total
	mov [bpbHeadsPerCylinder], dx
	
	
	no_change:

	mov ax, 1003h			; Set text output with certain attributes
	mov bx, 0			; to be bright, and not blinking
	int 10h

	
	
	
start_kernel:

	
	
call load_root
mov word [cluster_dir],19
call basic_screen

mov bh,007H ; bg / fg
call clear_screen

mov si,welcome_msg  ; show welcome msg
call print


mov si,enter_msg  ; show enter a cmd msg
call print


cmd:
call printe
mov si,dir_name
call print

mov al,' '
call printch

mov al,'>'
call printch

call Get_CMD
jmp cmd


cli
hlt

;----------------------------
; Dta of Kernel
;-----------------------------
enterType db 0DH,0AH,0
welcome_msg db "Welcome to Yamin OS!",0XD,0XA,0
graphic_mode db 0
bpbHeadsPerCylinder dw 2
bpbSectorsPerTrack dw 18
bsDriveNumber db 0 
directory_name times 12 db 0
buffer times 64 db 0  ; get cmd from user





%INCLUDE "YaminOS/screen.asm"
%INCLUDE "YaminOS/string.asm"
%INCLUDE "YaminOS/cmd.asm"
%INCLUDE "YaminOS/disk.asm"


dir_name times 110 db "/" ,0
buffer_fat times 4608 db 0