;-------------------------
;	Get command
;--------------------
Get_CMD:
	mov di,buffer ; buffer of get cmd
	mov cx,0
	.loop:
	mov ah,0
	int 16h
	
	cmp al,0xD ;it is Enter key (pressed)?
	jz .enterkey
	
	cmp al,8 ; back space
	jz .backspace
	
	
	cmp al,32
	jz .space
	
	stosb ; write al to di
	
	.show:
	mov ah,0xE 
	int 10h
	
	inc cx 
	cmp cx,64
	jnz .loop
	jmp .enterkey
	ret
	
	
.backspace:
cmp cx,0 
jz .loop
dec cx


mov ah,3 ; get now curser
mov bh,0 
int 10h 

dec dl ; set sotoon back 
mov ah,2
int 10h 

mov al,' '
mov ah,0xE
int 10h 


mov ah,2
int 10h 

dec di


jmp .loop

.space:
mov al,','
stosb
mov al,0x20
jmp .show

.enterkey:

mov al,0
stosb
call printe
;jmp cmpstring
jmp cmpcmd
;--------------------------------------------------
; cmd command 
;----------------------------------------------

cmpcmd:

mov si,buffer
call get_par ; Get parametrs

mov di,si

mov si,cmd_name
call cmp2str
jc .name

mov si,cmd_del
call cmp2str
jc .del

mov si,cmd_dir
call cmp2str
jc .dir

mov si,cmd_cls
call cmp2str
jc .cls

mov si,cmd_cd
call cmp2str
jc .cd


mov si,cmd_time
call cmp2str
jc .time

mov si,cmd_read
call cmp2str
jc .read

mov si,cmd_rename
call cmp2str
jc .rename

mov si,cmd_ver
call cmp2str
jc .ver

mov si,cmd_graphic
call cmp2str
jc .graphic

mov si,cmd_help
call cmp2str
jc .help


mov si,buffer 
cmp byte [si],0
je cmd

jmp run_file

;-----------------------------------------------
.name:
cmp ax,0 
jne .cmd_name

mov si,cmd_name_msg
call print
ret

.cmd_name:
mov di,ax 
mov si,cmd_help
call cmp2str
jc .name_help

jmp .nocmd

.name_help:
mov si,cmd_name_help_msg
call print
ret 

;-----------------------------------------------

.del:
cmp ax,0 
jne .cmd_del

mov si,nodel_msg
call print 
ret

.cmd_del:
call remove_file
jc .dont_delete
mov si,cmd_del_msg
call print
ret 

.dont_delete:
mov si,dont_delete_msg
call print
mov si,ax
call print
call printe
ret

;-----------------------------------------------

.cls:
mov bh,007H ; bg / fg
call clear_screen
ret

;-----------------------------------------------
.cd:
cmp ax,0 
jne .cmd_cd
mov si,cmd_cd_err
call print
ret 

.cmd_cd:
mov di,ax 
mov si,cmd_cd_ret
call cmp2str
jc .cd_ret

call cdirectory
ret

.cd_ret:
call cd_ret
ret
;-----------------------------------------------
.time:
call get_time
ret 
;-----------------------------------------------

.read:
cmp ax,0 
jne .cmd_read
mov si,nodel_msg
call print
ret
.cmd_read:
mov si,ax
call read_txt
ret

;------------------------------------------------
.rename:
cmp ax,0 
jne .cmd_rename
mov si,cmd_rename_msg_err
call print 
ret 

.cmd_rename:
cmp bx,0
jne .cmd_rename2
mov si,cmd_rename_msg_err
call print 
ret 

.cmd_rename2:
call rename
jc .end_rename 
mov si,cmd_rename_msg
call print 
ret

.end_rename :
ret
;-------------------------------------------------
.ver:
mov si,cmd_ver_msg
call print 
ret
;------------------------------------------------
.graphic:
jmp start_kernel
ret
;-------------------------------------------------
.help:
mov si,help_msg 
call print
ret

;-----------------------------------------------

.dir:
cmp ax,0 
jne .subdir
call show_dir
ret

.subdir:
mov di,ax 
mov si,cmd_dir_f
call cmp2str
jc .dir_f

jmp .nocmd

.dir_f:
call dir_folder
mov si,file_name_dir
call print_dir
ret

;-----------------------------------------------

.nocmd:
mov si,no_cmd_msg
call print
ret


;----------------------------------------------------------------------------------
; if the IN command wasn't anyone above cmd
; we test it , it may be a file or directory ;)
;-----------------------------------------------
run_file:

mov bx,32768
call load_file
jnc 32768
jmp cmd

kernel_err:
popa
mov si,kernel_err_msg
call print 
jmp cmd

cli
hlt 
jmp start_kernel

cmd2 dw 0
enter_msg db "Please Enter a Command (help)",0XD,0XA,0
help_msg db "command : (dir - cls - del - ren - ver - read - name - help)",0XD,0XA,0
no_cmd_msg db "it no command! (cmd help)",0XD,0XA,0
cmd_name_msg db "In the name of God..!",0XD,0XA,0
cmd_del_msg db "File delete :)",0XD,0XA,0
cmd_name_help_msg db "this is a commad for Rahman ;)",0XD,0XA,0
nodel_msg db "You Forgot Type the file name and format Ex: Trst.bin",0XD,0XA,0
dont_delete_msg db "Sorry. don't remove file ",0XD,0XA,0
notfound_msg db "this isn't a command , file or directory :(",0XD,0XA,0
file_notfound_msg db " not found! :(",0XD,0XA,0
find_file db "find the file :)",0XD,0XA,0
cmd_rename_msg_err db "You should Enter file name and new file name Ex: (rename test.bin ali.bin)",0XD,0XA,0
cmd_rename_msg db "file name renamed...! :|",0XD,0XA,0
dont_rename_file db "Sorry, don't rename file :(",0XD,0XA,0
cmd_ver_msg db "Yamin OS 1.0",0XD,0XA,0
cmd_cd_err db "You should enter directory Ex: cd folder",0XD,0XA,0
kernel_err_msg db "You shouldn't open the Kernel file Agein :'(",0XD,0XA,0
kernelbin db "kernel.bin",0
;--------------------------------
; Cmd commend name
;-----------------------------
cmd_name db "name",0
cmd_dir db "dir",0
cmd_cls db "cls",0
cmd_help db "help",0
cmd_del db "del",0
cmd_read db "read",0
cmd_rename db "ren",0
cmd_ver db "ver",0
cmd_time db "time",0
cmd_dir_f db "-f",0
cmd_cd db "cd",0
cmd_cd_ret db "..",0
cmd_graphic db "graphic",0