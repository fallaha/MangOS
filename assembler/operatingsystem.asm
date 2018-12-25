bits 16
org 0x7c00

start: jmp main


;*************************************************;
; OEM Parameter block
;*************************************************;
TIMES 0Bh-$+start DB 0
bpbBytesPerSector: DW 512
bpbSectorsPerCluster: DB 1
bpbReservedSectors: DW 1
bpbNumberOfFATs: DB 2
bpbRootEntries: DW 224
bpbTotalSectors: DW 2880
bpbMedia: DB 0xF0
bpbSectorsPerFAT: DW 9
bpbSectorsPerTrack: DW 18
bpbHeadsPerCylinder: DW 2
bpbHiddenSectors: DD 0
bpbTotalSectorsBig: DD 0
bsDriveNumber: DB 0
bsUnused: DB 0
bsExtBootSignature: DB 0x29
bsSerialNumber: DD 0xa0a1a2a3
bsVolumeLabel: DB "MOS FLOPPY "
bsFileSystem: DB "FAT12 "
;*************************************************;
; Bootloader Entry Point
;*************************************************;
;----------------------------------------------------

;-----------------
; print a string
;----------------------
print:
push ax
pri:
lodsb
or al,al 
jz endprint
mov ah,0xe
int 10h
jmp pri
endprint:
pop ax
ret



;-------------------------------
; LBA To CHS
; input : ax: logical sector
; output: cl:sector | ch: track | dh: head
;----------------------
LBAtoCHS:
mov dx,0 
div word [bpbSectorsPerTrack] 
mov cl,dl 
inc cl

mov dx,0
div word [bpbHeadsPerCylinder]
mov ch,al 
mov dh,dl
ret



;----------------------
; get chs
;---------------------
Get_CHS:
sub ax,2
add ax,[data_cluster]
ret


;------------------------
; read sector 
; input: dl:drive | dh:head | ch:Track | cl:Sector | al: num sector
;-----------------------
read_sector:
mov ah,2 
mov dl,0; drive
int 13h
jc fault
ret

;---------------------
fault:
mov si,fault_msg
call print
cli
hlt

;-------------------
; load the root directory
;----------------------------

;-------------
; read root dir 
;--------------
read_dir:
mov ax,[first_sector_dir]
call LBAtoCHS
mov al,[size_dir]
mov si,buffer
mov bx,si
call read_sector
ret  

;----------------------
; search root directory
;--------------------------------
search_root:
mov cx,224
mov di,buffer

loopsearch:
push cx
push di
mov si,kernel_name
mov cx,11

repz cmpsb

jz load_fat
pop di
pop cx
add di,0x20 
loop loopsearch
call fault
ret



;-----------------------
; load fat
;---------------------
load_fat:
pop di
pop cx
add di,0x1a
mov bx,[di]
mov [cluster],bx 

mov ax,[first_fat]
call LBAtoCHS
mov si,buffer
mov bx,si
mov al,byte [bpbSectorsPerFAT]
call read_sector

ret

;---------------------------
; load kernel
;---------------------
kernel:
mov bx,0x2000
push bx 

load_kernel:
mov ax,[cluster]
call Get_CHS
call LBAtoCHS
pop bx
mov al,1
call read_sector
add bx,512
push bx

mov ax,[cluster]
mov cx,ax
mov dx,ax 
shr dx,1  ;div 2
add cx,dx 
mov si,buffer
mov bx,si 
add bx,cx 
mov dx,[bx]
test ax,1 
jnz Odd_cluster

Even_Cluster:
and dx,0000111111111111b
jmp Done

Odd_cluster:
mov cl,4
shr dx,cl

Done:
mov word [cluster],dx 
cmp dx,0xff0
jb load_kernel
pop bx
mov si,load_kernel_msg
call print
ret 


;---------------------------------
main:

mov byte[first_sector_dir],19
mov byte[size_dir],14
mov byte[first_fat],1
mov word [data_cluster],33



call read_dir
call search_root
call kernel

mov si,msg
call print
jmp 0x2000



cli
hlt

;-------
; data
;-----
data_cluster dw 0
first_fat dw 0 
first_sector_dir dw 0
size_dir db 0
msg db "Welcome to OS..!",0
fault_msg db "dont work your os :(",0
kernel_name db "KERNEL  BIN"
ok_search_msg db "search ok!",0
cluster dw 0
load_kernel_msg db "Kernel loaded!",0
times 510 - ($-$$) db 0
dw 0xAA55
buffer:
