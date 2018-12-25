bits 16
org 0x7c00
start : jmp bootloader
nop

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


kernel:

mov ax, 50h			; Segment where we'll load the kernel
mov es, ax
mov bx, 0
push bx

load_kernel:	;load Kernel file 
mov ax,[cluster_kernel]
call LBAtoCHS
call Get_CHS
pop bx
mov al,1 
call read_sector
add bx,512
push bx

mov ax,[cluster_kernel]
mov cx,ax 
mov dx,ax
shr dx,1 ;div 2 
add cx,dx 
mov si,buffer
mov bx,si ; addres of Fat 

add bx,cx 
mov dx,[bx]


test ax,1 ; it's odd or Even?
jnz odd_Cluster 	;it is odd

Even_Cluster:
and dx,0000111111111111b ; ger 12 bit low
jmp Done

odd_Cluster:
mov cl,4
shr dx,cl ; take high 12 bit

 
Done:
mov [cluster_kernel],dx
cmp dx,0xff0
jb load_kernel
pop bx
ret




;----------------------------------------------------


print:
push ax
lprint:
lodsb
or al,al
jz endofprint
mov ah,0xE
int 10h
jmp lprint
endofprint:
pop ax
ret


;noprint:
;mov cx,100
;loopprint:
;lodsb
;;or al,al
;jz endofprint1
;mov ah,0xE
;int 10h
;loop loopprint
;endofprint1:
;ret

;printch:
;push ax
;mov ah,0xE
;int 10h
;pop ax
;ret

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

;-------------------------------
; Load root directory
;------------------------------
load_dir:
mov ax, word [first_sector_dir]
call Get_CHS
mov al,byte [size_dir]
mov si,buffer
mov bx,si
call read_sector
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
mov ax,[bpbReservedSectors]
call Get_CHS
mov al,byte[bpbSectorsPerFAT]
mov si,buffer
mov bx,si
call read_sector
ret



;--------------------------------------
; Search in root Director form 0x200 
;----------------------------------------
search_root:

mov cx,224
mov di,buffer

Loop_search:

push cx
mov si,kernelname 
mov cx,11
push di

rep cmpsb

jz load_FAT
pop di
pop cx 
add di,0x20 
loop Loop_search
call fault
ret




;------------------------
; the main progran
;---------------------

bootloader:


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

;-----------------------------------
;	get size of root directory  14
;--------------------------------
mov word [size_dir],14


;--------------------------------
; get first sector of root directory  19
;-------------------------------------


mov word [first_sector_dir],19


mov byte[first_data_cluster],33

call reset_disk ; reset of Disk
call load_dir
call search_root
call kernel

mov dl,byte [bsDriveNumber] 
;mov al,'S'


jmp 0050h:0000h




cli
hlt


;-------------------------
; Data of Program
;--------------------------
first_data_cluster dw 0
size_dir dw 0 ; size of directory
first_sector_dir dw 0 ; first sector Of directory
sucssecful_msg db "Ya Mahdi",0
fault_msg db "abort os..!",0
read_sector_msg db "tdont read",0
kernelname db "BTLDR2  SYS"
load_kernel_location db 0
cluster_kernel db 0

times 510 - ($-$$) db 0
dw 0xAA55
buffer: 
