;------------------------
; cmp two String
;---------------------------
cmpstring:
	
	pusha	
	ccmpstring:
	
	.loop:
	
	cmp byte [di],','
	jz .slash
	
	mov al,[si]
	mov bl,[di]
	
	cmp al,bl
	jne .noequal
	cmp al,0
	je .Done
	
	inc si
	inc di
	
	jmp .loop
	
	
	ret


.noequal:
	popa
	clc
	ret

.slash:


.Done: 
	popa
	stc
	ret
	
;--------------------------------------------
; CMP two string 
; in : si:first string | DI:secound string     OUT: if equal two string , carry set 
;-----------------------------------
cmp2str:
pusha 

.loop:
mov al,[si]
mov bl,[di]

cmp al,bl
jne .noequal

cmp al,0 
je .end

inc si
inc di

jmp .loop

.noequal:
clc 
popa 
ret 

.end:
stc 
popa
ret 
	
;----------------------------------------
; Get parametr od commad from user
; IN:   out: si:first cmd ax:secound cmd , bx: therd cmd cx: forch cmd
;----------------------------------
get_par:
push si

mov ax,0
mov bx,0
mov cx,0 

.start1:
lodsb ; write 1 byte ftom si to al 
cmp al,0
jz .endend
cmp al,','
jnz .start1 


dec si
mov byte[si],0
inc si
mov ax,si ; ax is secound command
push ax


.start2:
lodsb ; write 1 byte ftom si to al 
cmp al,0
jz .end
cmp al,','
jnz .start2

dec si
mov byte[si],0
inc si
mov bx,si ; ax is secound command

.start3:
lodsb ; write 1 byte ftom si to al 
cmp al,0
jz .end
cmp al,','
jnz .start3

dec si
mov byte [si],0
inc si
mov cx,si ; ax is secound command

.end:
pop ax
.endend:
pop si
ret		
	

;--------------------------------------------
; Edit file name e.d.g. "YAMIN   JPG" to "YAMIN.JPG"
;in: SI file name
;-------------------------------------------
file_name_ex:

mov bx,si 
add bx,9
mov al,[bx]
cmp al,' '
jz .end


mov cx,0
mov di,file_name

	.start:
	mov al,[si]
	cmp al," "
	je .space
	mov [di],al 
	inc di
	inc si
	inc cx

	cmp cx,8
	je .dot
	cmp cx,11
	je .done_copy
	jmp .start
	
.space:
	inc si 
	inc cx
	cmp cx,8
	je .dot
	jmp .start
	
.dot:
	mov byte [di],'.'
	inc di
	jmp .start
	
.done_copy:

	mov byte [di],0
	mov si,file_name
	clc
	ret

.end:
stc
ret


;------------------------------------------------
; copy si to di
; in: SI: source    DI: Destination  Cx: counter
;--------------------------------------------
cpy2str:
pusha

.loop:
mov al,[si]
mov [di],al 
inc si 
inc di 
loop .loop
mov byte [di],0

popa 
ret
;----------------------------------------------
; Upper the English alphobet e.d.g. ali to ALI
; in: AL:char
;----------------------------------------------
toupperch:
cmp al,96
ja .two 

jmp .end

.two:
cmp al,123
jb .cun

jmp .end

.cun:
sub al,0x20

.end:
ret 

;------------------------------------------------------
; To upper string
;in: ax : addres of string 
;-------------------------------------------------------
toupper:
pusha
	mov si,ax 
	
	.start:
	cmp byte [si],0
	jz .end
	
	cmp byte [si],'a'
	jb .next
	
	cmp  byte [si],'z'
	ja .next 
	
	sub  byte [si],0x20 
	inc si
	jmp .start
	
.next:
inc si 
jmp .start 

.end:
popa
ret 


;-------------------------------------------------
; copy str 1 to end of str 2
; in : si:source | di: destination | cx:counter str 2 
; al: char ex : 0 or ',' | ah : char beetwin str1 and str 2
;----------------------------------------------------
mem_cpy:
mov [.sign],al

.start:
mov al,[di]
cmp al,[.sign]
jz .next
inc di
jmp .start

.next:
.loop:
mov al,[si]
mov [di],al 
inc si 
inc di 
loop .loop
mov byte [di],ah
inc di
mov byte [di],0

ret 
.sign db 0

;---------------------------------------
; Get key from keyboard
;-----------------------------
get_key:

	mov ah,0
	int 16h
	

ret 
;-----------------------------------------
; Search and place a char in string 
; in :SI :addres of string
; in: al: char for search ah: char for place | cl : Several char
;ch :if 0 start search from Beginning and 1 start search from ending
;------------------------------------
search_place_string:
cmp ch,0 
jz .start 


push ax 
mov ax,si 
call get_size_string
add si,ax
pop ax

.start1:
cmp [si],al
jz .several
dec si
jmp .start1

.several:
dec cl
cmp cl,0 
jz .next 
dec si
jmp .start1



.start:
cmp [si],al
jz .several1
inc si
jmp .start

.several1:
dec cl
cmp cl,0 
jz .next0
inc si
jmp .start

.next0:
dec si 
.next:
inc si
mov [si],ah 
ret 


;----------------------------------------------
; search and replace char in string 
; in :si: addres of string | al : char for search | ah:char for replace
;-----------------------------------------
search_rep_str:
mov byte[.sign],al 

push ax
mov ax,si 
call get_size_string
mov cx,ax 
pop ax 


.restart:
cmp [si],al 
jz .next 
cmp byte [si],0 
jz .end
inc si
jmp .restart
jmp .end

.next:
mov [si],ah 
inc si
jmp .restart

.end:
ret
.sign db 0

;-------------------------------------------
; print directory
;in:si
;---------------------------
print_dir:
push ax
.loop:
lodsb
cmp al,','
jz .next

or al,al
jz .end

mov ah,0xe
int 10h
jmp .loop
.end:
pop ax
ret

.next:
call printe
jmp .loop

ret
;-------------------------------------------
; get_size_string
; IN: AX:addres of string OUT:AX:Lengh of string
;------------------------------------------
get_size_string:
pusha 
mov cx,0 
mov si,ax 

.loop:
cmp byte [si],0 
jz .end

inc si
inc cx
jmp .loop

.end:
mov word [.lengh],cx
popa
mov ax,word [.lengh]
ret 

.lengh dw 0 


;-------------------------------------------
; serach 1 char to str and return addres 
; in : si: addres of string | cx: sevrel char? | al:char
; out: si:addres of char
;-------------------------------------
mem_chr:

.start:
cmp [si],al 
jz .next 
inc si 
jmp .start

.next:
dec cx 
cmp cx,0 
jz .end 
inc si
jmp .start

.end:
ret 

;---------------------------------------------
; Convert "YAMIN.BIN" to "Yamin   BIN"
; in/out : AX:addres of string for convert
;-------------------------------------------
file_name_convert:
	pusha
	
	call toupper
	mov si,ax 
	call get_size_string ; save in AX
	
	mov [.size_string],ax
	mov di,.file_name
	mov cx,0
	
	.start:
	cmp  byte [si],0 
	jz .end 
	
	cmp  byte [si],'.'
	jz .dot
	
	mov al,[si]
	mov [di],al 
	inc si
	inc di 
	inc cx
	cmp cx,11
	jz .end
	jmp .start

.dot:    
inc si
.loopdot:
	cmp cx,8
	jz .start 
	mov  byte [di],' '
	inc cx
	inc di
	jmp .loopdot

.end:
	mov byte [di],0
	popa 
	mov si,.file_name
	mov ax,si
	ret 
	
	

.file_name times 13 db 0 
.size_string dw 0


;--------------------------------------
; file dir converter
; in/out : AX:addres of string for convert
;----------------------------------------
dir_name_converter:
call toupper

	mov si,ax
	mov di,si
	call get_size_string ; save in AX
	add si,ax 
	mov cx,11 
	sub cx,ax 
	
	.loop:
	mov byte [si],' '
	inc si 
	loop .loop 
	mov byte [si],0
	mov ax,di
	
ret


;---------------------------------
; read text file 
;mov si,addres of file
;--------------------------------
read_txt:
mov bx,32768
call load_file
mov si,32768
call print
ret









