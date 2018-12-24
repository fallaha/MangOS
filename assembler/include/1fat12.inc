%ifndef __FAT12_INC_67343546FDCC56AAB872_INCLUDED__
%define __FAT12_INC_67343546FDCC56AAB872_INCLUDED__





;Define 
%define ROOT_OFFSET 0x2e00
%define FAT_SEG 0x2c0
%define ROOT_SEG 0x2e0


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


;--------------------------------------
; Search in root Director form 0x200 
; in: SI:addres name of file
;----------------------------------------
search_root:
mov di,[directory]

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

;-----------------------------------------
; load the file to ram
; in:  si:addresa of name file | Bx: address of ram for transfor file to ram
;--------------------------------------------
load_file:
push bx

push si
call load_fat_table
call load_root
pop si



call search_root
jc notfound_search_root

mov bx,di ; di is addres of file in root directory
add bx,0x1B ; first cluster file
mov ah,[bx] 
dec bx 
mov al,[bx]
mov word [cluster_file],ax
pop bx

load_file_cluster:
push bx

.loop:
mov es,0
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
stc
ret 

notfound_search_root:
pop bx
stc
ret 



%endif		;__FAT12_INC_67343546FDCC56AAB872_INCLUDED__

directory dw 25600
cluster_file dw 0