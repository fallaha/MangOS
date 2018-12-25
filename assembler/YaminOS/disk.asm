;---------------------------
; Directory File show
;------------------------------
show_dir:
call reset_disk
call load_fat_table
call get_file_name
mov si,file_name_dir
call print_dir

ret 



;-----------------------------
; get chs
; input : al : logical sector
; out put : cl : Sector | ch : TRACK | dh : Head
;---------------------------
Get_CHS:
push ax

mov dx,0 
div word [bpbSectorsPerTrack] ; 18
inc dl 
mov cl,dl 

mov dx,0 
div word [bpbHeadsPerCylinder]
mov dh,dl ;head
mov ch,al

pop ax
ret


;------------------------------------
; get LBA from CHS
; input : ax
;---------------------------------
LBAtoCHS:
sub ax,2
add ax,[first_data_cluster]
ret


;--------------------------------------
; reset disk
; output : none
;--------------------------------------
reset_disk:
mov dl,[bsDriveNumber]
mov ah,0
int 13h
jnc endreset
call fault
endreset:
ret

;--------------------------------------
; read sector from disk to memory
; input : AL : Sectors To Read Count | CH : Cylinder | CL : num of Sector | DH : num of Head | DL : Drive | ES:BX : Buffer Address Pointer
; output : AH : 0 sucssecful | AL: 	Actual Sectors Read Count
;-----------------------------------
read_sector:
mov dl,[bsDriveNumber]
mov ah,2
;stc
int 13h
jc read_sector_fault
ret

;--------------------------------
; write 1 sector
; input: AL: num of sector to write | BX:addres ram | CL: NO sector of disk to write | ch: numver of track | DL: number of drive | DH: Head
; IN: ax:number sector to write | bx:addres ram that want to wite to disk
;-------------------------------------------
write_sector:
push bx
call Get_CHS
mov dl,[bsDriveNumber]
mov al,1
mov ah,3 
pop bx 
int 13h
jc .dont
ret
.dont:
call read_sector_fault
ret 


;--------------------------------
; write directory
; input: AL: num of sector to write | BX:addres ram | CL: NO sector of disk to write | ch: numver of track | DL: number of drive | DH: Head
; IN: ax:number sector to write | bx:addres ram that want to wite to disk
;-------------------------------------------
write_dir:
mov ax,19
call Get_CHS
mov dl,[bsDriveNumber]
mov al,14
mov ah,3 
mov bx,word [directory]
int 13h
jc .dont
ret
.dont:
stc
ret 


;--------------------------------
; write FAT
; input: AL: num of sector to write | BX:addres ram | CL: NO sector of disk to write | ch: numver of track | DL: number of drive | DH: Head
; IN: ax:number sector to write | bx:addres ram that want to wite to disk
;-------------------------------------------
write_fat:
mov ax,1
call Get_CHS
mov dl,[bsDriveNumber]
mov al,9
mov ah,3 
mov bx,buffer_fat
int 13h
jc .dont
ret
.dont:
stc
ret 


;-------------------------------
; Load root directory
;------------------------------
load_root:
mov ax, word [first_sector_dir]
call Get_CHS
mov al,byte [size_dir]
mov si,word [directory]
mov bx,si
call read_sector
ret

;--------------------------------------
; Search in root Director form 0x200 
; in: SI:addres name of file
;----------------------------------------
search_root:
mov di,word [directory]

search_dir:
mov cx,224
Loop_search:

push cx
push di
push si

mov cx,11

rep cmpsb

jz .end
pop si
pop di
pop cx 
add di,0x20 
loop Loop_search

stc
ret

.end:
pop si
pop di
pop cx 
clc
ret


;-------------------------------
; Load FAT
;------------------------------
load_FAT:
pop di
pop cx
add di,0x1a ;26
mov bx,di
mov ax,[bx]
mov [cluster_kernel],ax
mov ax,1
call Get_CHS
mov al,18
mov si,buffer_fat
mov bx,si
call read_sector
ret


;-----------------------------------
; Load Fat Table
;---------------------------------------
load_fat_table:
mov ax,1 
call Get_CHS
mov al,8 
mov si,buffer_fat
mov bx,si
call read_sector
ret 



;---------------------------
; Get File name
;----------------------------
get_file_name:
	mov si,file_name_dir
	mov byte [si],0 
	
	mov cx,224
	mov si,[directory] ; directory
	
	start_set_file_name:
	mov al,[si]
	;cmp al,0 
	;je end_file_name
	
	
	cmp al,48 
	jb .next
	
	cmp al,122 
	ja .next	
	
	pusha
	call Get_file_type
	popa
	jc .next

	
	mov di,filename
	push cx 
	push si 
	push di
	mov cx,11 
	

	
.loop:
	mov al,[si]
	mov [di],al

	inc di
	inc si 
	loop .loop
	
	call set_file_name
	
	.end:
	pop di
	pop si
	pop cx 
	.next:
	add si , 0x20 
	loop start_set_file_name
	

	
	ret
;-------------------------
; set the file name & print it 
;-----------------------
set_file_name:

	mov si,filename
	call file_name_ex
	mov ax,si
	call get_size_string
	mov di,file_name_dir
	mov cx,ax
	mov al,0 
	mov ah,','
	call mem_cpy

	ret
	

end_file_name:
ret

.size_str dw 0
;-----------------
; abort operating system
;---------------------
fault:
mov si,fault_msg
call print
cli
hlt

read_sector_fault:
mov si,read_sector_msg
call print
cli
hlt

;---------------------------------------
; Get attrebute file 
;----------------------------------
fat:
mov ax,[cluster_file]
mov cx,ax 
mov dx,ax
shr dx,1 ;div 2 
add cx,dx 
mov si,buffer_fat
mov bx,si ; addres of Fat 

add bx,cx 
mov dx,[bx]


test ax,1 ; it's odd or Even?
jnz odd_Cluster 	;it is odd

Even_Cluster:
and dx,0000111111111111b ; ger 12 bit low
jmp ok_load

odd_Cluster:
mov cl,4
shr dx,cl ; take high 12 bit

ok_load:
mov [cluster_file],dx
ret


;---------------------------------------
; Write in Fat 0x000
;----------------------------------
write_fat_mem:
pusha
	.loop:
	mov ax,[cluster_file]
	mov cx,ax 
	mov dx,ax
	shr dx,1 ;div 2 
	add cx,dx 
	mov si,buffer_fat
	mov bx,si ; addres of Fat 
	
	add bx,cx 
	mov dx,[bx]
	
	test ax,1 ; it's odd or Even?
	jnz .odd_Cluster 	;it is odd
	
	.Even_Cluster:
	
	push dx
	and dx,0xF000
	mov [bx],dx
	pop dx
	jmp .ok_fat
	
	.odd_Cluster:
	push dx
	and dx,0x000F
	mov [bx],dx
	pop dx
	
	.ok_fat:
	mov [cluster_file],dx
	cmp dx,0xff8
	jae .end
	jmp .loop
	.end:
	popa
	ret
	

;---------------------------
; Get Type File 
; input: 
;----------------------------
file_type:
add si,0x1a ;26 
mov bx,[si]
mov [cluster_file],bx
call fat
cmp dx,0xff0
ja .two

jmp .clc
.two:
cmp dx,0xFF7
jb .setc

.clc:
clc
ret 

.setc:
stc
ret 

;------------------------------------
; Get File Type
; in: si= file addres out: carry set if file was a system file
;-----------------------------------
Get_file_type:
push si
; do we read a delete file ? delete file E5

mov al,[si] 
cmp al,0xE5
jz .next

; Get type file 
	add si,11 
	mov al,[si]
	cmp al,0x0f
	je .next 
	
	cmp al,0x16
	je .next 
	
	pop si
	clc
	ret		
.next:
	pop si
	stc
	ret
	
;------------------------------------
; ret File Type
; in: si= file addres out: carry set if file was a system file
;-----------------------------------
ret_file_type_dir:
pusha
; do we read a delete file ? delete file E5
call search_root
; Get type file 
	add di,11 
	cmp byte [di],0x10
	je .dir 
	popa
	clc
	ret		
.dir:
	popa
	stc
	ret
	
;----------------------------------------
; get_name_file
; IN: di:write get code to di
;-----------------------------------------
get_name_file:
	mov cx,0
	.loop:
	mov ah,0
	int 16h
	
	cmp al,0xD ;it is Enter key (pressed)?
	jz .enterkey
	
	cmp al,8 ; back space
	jz .backspace
	
	stosb ; write al to di
		.show:
	mov ah,0xE 
	int 10h
	
	inc cx 
	cmp cx,12
	jnz .loop
	jmp .enterkey
	ret

		
.backspace:
dec cx
cmp cx,0 
jz .loop


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

.enterkey:

mov al,0
stosb
call printe
ret


;-------------------------------------------
; Get Directory (folder) in root directory
;-------------------------------------------
dir_folder:
;call load_root
	mov si,file_name_dir
	mov byte [si],0 
	
mov di,word [directory]


add di,11
mov cx,224
.start:
push cx
cmp byte [di],0x10 
jne .next

push di
sub di,11
mov cx,11 
mov si,filename
.loop:
mov al,[di]
mov [si],al 
inc si
inc di
loop .loop 
mov byte[si],0
mov si,filename
call Get_file_type
jc .next1
call set_file_name
;call printe
.next1:
pop di
.next:
pop cx
dec cx 
cmp cx,0 
je .end
add di,0x20
loop .start

.end:
ret


;----------------------------'-----------
; Remove the file
;--------------------------------------
remove_file:
mov word[.file_name_remove],ax

call load_root
call load_fat_table


mov ax,[.file_name_remove]
call file_name_convert

mov si,ax ; buffer is cmd that user enterd
call search_root ; out put is di that di is addres file in root directory
jc .dont_write

mov bx,di
add bx,0x1A
mov ax,[bx]
mov word [cluster_file],ax
mov al,0xE5
mov [di],al 

mov ax,19 
mov bx,word[directory]
call write_dir
jc .dont_write

call write_fat_mem
mov ax,1 ; first sector of fat 
mov bx,buffer_fat
call write_fat
jc .dont_write
mov ax,[.file_name_remove]
clc
ret

.dont_write:
mov ax,[.file_name_remove]
stc	
ret

.file_name_remove dw 0




;-----------------------------------------
; load the file to ram
; in:  si:addresa of name file | Bx: address of ram for transfor file to ram
;--------------------------------------------
load_file:
push bx

push si
call load_fat_table
pop si

mov ax,si
call file_name_convert
mov si,ax ; addres of new file name

pusha 
mov di,ax 
mov si,kernelname
call cmp2str
jc kernel_error
popa

call search_root
jc notfound_search_root

mov bx,di ; di is addres of file in root directory
add bx,0x1B ; first cluster file
mov ah,[bx] 
dec bx 
mov al,[bx]
mov word [cluster_file],ax
mov word [cluster_dir_search],ax
pop bx
load_file_cluster:

push bx

.loop:
mov ax,[cluster_file]
call LBAtoCHS
call Get_CHS
pop bx
mov al,1 
call read_sector
add bx,512
push bx

mov ax,[cluster_file]
mov cx,ax 
mov dx,ax
shr dx,1 ;div 2 
add cx,dx 
mov si,buffer_fat
mov bx,si ; addres of Fat 
add bx,cx 
mov dx,[bx]

test ax,1 ; it's odd or Even?
jnz .odd_Cluster 	;it is odd

.Even_Cluster:
and dx,0000111111111111b ; ger 12 bit low
jmp .Done

.odd_Cluster:
mov cl,4
shr dx,cl ; take high 12 bit

 
.Done:
mov [cluster_file],dx
cmp dx,0xff0
jb .loop
pop bx

clc
ret

kernel_error:
mov si,kernel_err_msg
call box_msg 
ret 

notfound_search_root:
pop bx
mov si,notfound_msg
call box_msg
stc
ret 


;---------------------------------------------
; rename the file ali.bin -> ahmad.txt
; in : ax:old file name | bx:new file name
;------------------------------------------
rename:
mov [.file_name],ax

;push ax 
;push bx
;call load_root
;pop bx 
;pop ax 


call file_name_convert ; input : ax


mov si,ax
call search_root
jc .notfound
mov dx,di

mov ax,bx ; input : ax
call file_name_convert 
mov si,ax
mov cx,11 
mov di,dx 

.loop:
mov al,[si]
mov [di],al 
inc si 
inc di 
loop .loop

call write_dir
jc .dont_write

clc
ret

.notfound:
mov si,[.file_name]
call print
mov si,file_notfound_msg
cmp byte [graphic_mode],0 
jnz .graph
call print
stc
ret 


.dont_write:
mov si,dont_rename_file
cmp byte [graphic_mode],0 
jnz .graph
call print
stc
ret 

.graph:
call box_msg
ret

.file_name dw 0


;---------------------------------------
; change directory 
; AX: addres of directory name
;--------------------------------------
cdirectory:
pusha
mov si,ax
call get_size_string
mov cx,ax
mov [.size_name],cx
mov di,.dir_name
call cpy2str
popa

call dir_name_converter
mov si,ax

mov bx,word[directory] ; load directory in 25600 (25k) ram
call load_file
jc .error

mov di,dir_name
mov si,.dir_name
mov cx,0
mov cx,[.size_name]
mov al,0
mov ah,'/'
call mem_cpy


ret

.error:

mov si,dir_err
call print

ret 
.dir_name times 12 db 0 
.size_name dw 0

;---------------------------------------
; Graphic changr dir
;------------------------------------
graphic_cd:
mov bx,word[directory] ; load directory in 25600 (25k) ram
call load_file
mov ax,word [cluster_dir_search]
jc .error
mov word [cluster_dir],ax
ret

.error:
mov si,dir_err
call print
ret

;-----------------------------------
; return frome new directory to old dir :)
;---------------------------------------
cd_ret:
mov si,word [directory]
add si,0x3A
mov ax,[si]
cmp ax,0x00 ; if 0x3a was 0 back directory is root
jnz .next
call load_root
mov word [cluster_dir],19
jmp .end

.next:
mov word [cluster_dir],ax
mov [cluster_file],ax
pusha 
call load_fat_table
popa
mov bx,word [directory]
call load_file_cluster
jc .error

.end:


mov si,dir_name
mov al,'/'
mov ah,0
mov cl,2 
mov ch,1
call search_place_string

cmp byte [dir_name],0
je .slash
ret

.slash:
mov byte [dir_name],'/'
ret

.error:
mov si,dir_err
call print
ret 



;------------------------------------------------------

dont_write_msg db "dont write on disk",0
first_data_cluster dw 33
size_dir dw 14 ; size of directory
first_sector_dir dw 19 ; first sector Of directory
fault_msg db "abort os..!",0
read_sector_msg db "dont read",0
kernelname db "KERNEL  BIN"
load_kernel_location db 0
cluster_kernel db 0
cluster_file dw 0
file_name_msg db "Please Enter file name (then press Enter Key): ",0
vfolder db 0x10,0
dir_err db "don't load directory :(",0XD,0XA,0
directory dw 25600
cluster_dir_search dw 0 
cluster_dir dw 0


filename times 13 db 0
file_name times 14 db 0
file_name_dir times 2048 db 0