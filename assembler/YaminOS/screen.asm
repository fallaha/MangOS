; ---------------------------------------------
; Basick screen
;--------------------------------


basic_screen:
mov byte [graphic_mode],1

mov ax,word [cluster_dir]
cmp ax,19 
jz .load_root_dir
mov [cluster_file],ax
pusha 
call load_fat_table
popa
mov bx,word [directory]
call load_file_cluster
jmp .condir

.load_root_dir:
call load_root

.condir:
call reset_disk
call load_fat_table


mov bh,0x1f ;bg/fg
call clear_screen


;------------- Welcome massege -----------
mov ch,0 
mov cl,0 
mov dl,79 
mov dh,0
mov bh,0x7f
call color_scr

mov dl,30
mov dh,0
call set_cursor
mov si,welcome_msg
call print

;------------- help massege -----------
mov ch,24  ; سطر گوشه بالا سمت چپ
mov cl,0 ; ستون گوشه بالا سمت جپ
mov dh,24 ; سطر گوشه پایین سمت راست
mov dl,79 ; ستون گوشه پاییت سمت راست
mov bh,0x71
call color_scr

mov dl,0
mov dh,24
call set_cursor
mov si,.help_msg
call print

;---------------- morabba --------------------
mov ch,4  ; سطر گوشه بالا سمت چپ
mov cl,4 ; ستون گوشه بالا سمت جپ
mov dh,19 ; سطر گوشه پایین سمت راست
mov dl,74 ; ستون گوشه پاییت سمت راست
mov bh,0xf0
call color_scr






.first:
mov ch,5  ; سطر گوشه بالا سمت چپ
mov cl,5 ; ستون گوشه بالا سمت جپ
mov dh,5 ; سطر گوشه پایین سمت راست
mov dl,73 ; ستون گوشه پاییت سمت راست
mov bh,0x3F
mov bl,0



pusha

.start:

pusha
call get_file_name
popa

popa 


pusha
mov ch,4  ; سطر گوشه بالا سمت چپ
mov cl,4 ; ستون گوشه بالا سمت جپ
mov dh,19 ; سطر گوشه پایین سمت راست
mov dl,74 ; ستون گوشه پاییت سمت راست
mov bh,0xf0
call color_scr
popa

pusha
call color_scr


mov dl,6
mov dh,5
call set_cursor

push bx
mov bl,0 

mov si,file_name_dir
.loop1:
lodsb
cmp al,','
jz .next1

or al,al
jz .end1

mov ah,0xe
int 10h
jmp .loop1
.end1:

jmp .continue1

.next1:
inc bl 
inc dh 
inc ch
call set_cursor
jmp .loop1

.continue1:
dec bl
mov [count_item],bl 
pop bx

mov dl,0
mov dh,0
xor cx,cx
call set_cursor
mov al,bl 
add al,0x30
call printch


;call get_select_item
;call print

call hide_cursor

popa
pusha

call get_key ; al char input
cmp ah,80 ; down
jz .next
cmp ah,72 ; up
jz .back
cmp al,27 ; esc
jz .end
cmp al,13 ;enter
jz .enter_press
cmp ah,77 ;right
jz .right
cmp ah,75 ;left
jz .left
cmp ah,83 ;delete
jz .del
cmp ah,60 ;rename
jz .ren
cmp ah,42 ;rename
jz .tab

jmp .start

.back:
popa
cmp bl,0
jz .end_back
dec bl
dec dh
dec ch
pusha
jmp .start

.end_back:
pusha
jmp .start

.next:
popa
cmp bl,[count_item]
jz .end_back
inc bl
inc dh
inc ch
pusha
jmp .start

.enter_press:
call get_select_item
;call box_msg
call ret_file_type_dir
jc .right
mov bx,32768
call load_file
jnc 32768
mov si,.dir_err
call box_msg
jmp .start

.right:
pusha
call get_select_item
mov ax,si
call ret_file_type_dir
jnc .right_end
push si 
call graphic_cd
pop si
;mov di,directory_name
;mov cx,11
;call cpy2str

;mov dl,68
;mov dh,0
;call set_cursor
;mov si,directory_name
;call print
popa
jmp .first

.right_end:
mov si,.dir_err
call box_msg
popa 
jmp .start

.dir_err db "it isn't directory!",0



.left:
pusha
call cd_ret
popa
jmp .first

.del:
pusha
call get_select_item
mov ax,si
call print
call remove_file
jc .dont_delete
mov si,cmd_del_msg
call box_msg
popa
jmp basic_screen

.dont_delete:
mov si,dont_delete_msg
call box_msg
popa
jmp basic_screen


.ren:
pusha
call get_select_item
mov ax,si 
push ax
mov dl,37
mov dh,12
call set_cursor

call get_name_box

mov dl,12
mov dh,10
call set_cursor
mov di,file_name
call get_name_file
pop ax
mov bx,file_name
call rename
popa
jmp basic_screen

.tab:

jmp .start


.end:
popa
mov byte [graphic_mode],0
ret 

.help_msg db " please Enter a key for Setup",0
.number db 0

get_select_item:

mov si,file_name_dir
cmp bl,0 
jz .cont

mov cx,0 
mov cl,bl
mov al,','
call mem_chr
inc si

.cont:
push si 
mov al,','
mov ah,0 
mov cl,1 
mov ch,0
call search_place_string
pop si


ret
.addres_str dw 0

;---------------- morabba --------------------
draw_mo:
mov ch,4  ; سطر گوشه بالا سمت چپ
mov cl,4 ; ستون گوشه بالا سمت جپ
mov dh,19 ; سطر گوشه پایین سمت راست
mov dl,74 ; ستون گوشه پاییت سمت راست
mov bh,0xf0
call color_scr
ret

;--------------------- box massege --------------
; in : si : addres of massege (limit 59 char)
box_msg:
push si

mov ch,8  ; سطر گوشه بالا سمت چپ
mov cl,9 ; ستون گوشه بالا سمت جپ
mov dh,13 ; سطر گوشه پایین سمت راست
mov dl,69 ; ستون گوشه پاییت سمت راست
mov bh,0x4f
call color_scr

mov dl,10
mov dh,9
call set_cursor
pop si
call print



mov ch,12  ; سطر گوشه بالا سمت چپ
mov cl,34 ; ستون گوشه بالا سمت جپ
mov dh,12 ; سطر گوشه پایین سمت راست
mov dl,42 ; ستون گوشه پاییت سمت راست
mov bh,0xCf
call color_scr

mov dl,37
mov dh,12
call set_cursor
mov si,.ok_msg
call print

.enter:
call get_key
cmp al,13
jnz .enter

ret
.ok_msg db "ok!",0
.msg_help db "In The name of God...!",0

;----------------------------------
; Get name box
;------------------------------------
get_name_box:

; main box 
mov ch,8  ; سطر گوشه بالا سمت چپ
mov cl,9 ; ستون گوشه بالا سمت جپ
mov dh,13 ; سطر گوشه پایین سمت راست
mov dl,69 ; ستون گوشه پاییت سمت راست
mov bh,0x4f
call color_scr

; name kadr
mov ch,10  ; سطر گوشه بالا سمت چپ
mov cl,11 ; ستون گوشه بالا سمت جپ
mov dh,10; سطر گوشه پایین سمت راست
mov dl,67 ; ستون گوشه پاییت سمت راست
mov bh,0xf1
call color_scr


mov dl,10
mov dh,9
call set_cursor
mov si,.enter_msg
call print


; ok bottum
mov ch,12  ; سطر گوشه بالا سمت چپ
mov cl,34 ; ستون گوشه بالا سمت جپ
mov dh,12 ; سطر گوشه پایین سمت راست
mov dl,42 ; ستون گوشه پاییت سمت راست
mov bh,0xCf
call color_scr

mov dl,37
mov dh,12
call set_cursor
mov si,.ok_msg
call print

ret
.ok_msg db "ok!",0
.enter_msg db "Please Enter New File name",0

count_item db 0 

;---------------------------
; Develop code
;--------------------------
dev_code:
call get_key
mov al,ah 
call printch
jmp dev_code
;-----------------------
; select Screen mode 80*25
;-----------------------------
screnn_mode8025:
push ax
mov ah,0 
mov al,3
int 10h 
pop ax
ret 

;--------------------------
; Hide the cursor 
;--------------------------
hide_cursor:
	pusha

	mov ch, 32
	mov ah, 1
	mov al, 3			; Must be video mode for buggy BIOSes!
	int 10h

	popa

ret
;--------------------------
; Clear Screen with a color
;-----------------------

clear_screen:
mov ah,6 
mov al,0
mov dh,29
mov dl,79
xor cx,cx
int 10h

mov ah,2 
mov dh,0 
mov dl,0 
mov bh,0 
int 10h 


ret

;---------------------------------
; color the screen
; ch:
;------------------------------
color_scr:
mov ah,6 
mov al,0
int 10h
ret


;----------------------------
; Ser Cursor
;------------------------
set_cursor:
mov bh,0 
mov ah,2
int 10h 
ret

;------------------------------
; Print a string
;--------------------
print:
push ax
.loop:
lodsb
or al,al
jz .end
mov ah,0xe
int 10h
jmp .loop
.end:
pop ax
ret

;------------------------------
; Print string with curser
;-------------------------
printxy:

.start:
lodsb
cmp al,0 
jz .end 
mov ah,0xa 
mov bh,0 
mov cx,1 
int 10h
inc dl 
int 10h
jmp .start

.end:
ret

;----------------
; Print a string with size of string
;--------------------
prints:
push ax

.loop:
lodsb

mov ah,0xe
int 10h
loop .loop

pop ax
ret

;----------------
; Print a string
;--------------------
noprint:
mov cx,200
loopprint:
lodsb
;or al,al
;jz endofprint1
mov ah,0xE
int 10h
loop loopprint

ret


;----------------------
; Print Enter
;---------------------
printe:
push ax
mov ah,0xE
mov al,0xD
int 10h

mov al,0xA
int 10h
pop ax
ret 




;---------------------
; print a character
;------------------
printch:
	push ax
	mov ah,0xE 
	int 10h 
	pop ax
	ret

	
;-------------------------------------
; Get Time from System
;----------------------------------
get_time:
mov ah,2 
int 1Ah

mov al,cl 
call printch

ret 