
; first make : 97/08/04
; sencound change : 97/09/15


bits 16
;org 0x7c00
start: jmp loader

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

fault:
mov si,faulterr
call print
cli
hlt


;*************************************************
;   Print String To Dispaly
;   in: si (as Address of str)
;*************************************************
print:
pusha
.while:
lodsb
or al,al
jz printend
mov ah,0xe
int 0x10
jmp .while
printend:
popa
ret

printch:
mov ah,0xe
int 0x10

LBA2CHS:
div word [bpbSectorsPerTrack] ; ax/18 = head
mov cl,dl ; number sector
add cl,1
xor dx,dx
div word [bpbHeadsPerCylinder]
mov ch,al  ; head/2 = track
mov dh,dl ;; number head error rrrrrrrrrrr
ret



loadFat:
mov ax,1
xor dx,dx ; error div 
call LBA2CHS
mov al,[bpbSectorsPerFAT]
mov bx,0x200
call readSector
ret


;******************************************
;   LOOKup for stage2 file
;******************************************
lookUp:
mov cx,224
mov di,0x200

.search:
push cx 
push di
mov cx,11
mov si,krnldrName
rep cmpsb
jcxz .done
pop di
pop cx
add di,0x20
loop .search
mov si,stage2notfound
call print
ret
.done:
pop di ; address file in root
pop cx
add di,26
mov ax,[ds:di]
mov [file_cluster], ax
ret

;******************************************
;   Load Root
;******************************************
loadRoot:
;Calculate Size sector of Root
; you mul 224 *[32] that it is wrong
xor dx,dx
mov ax, 0x20 ; 32 byte in each entries
mul word [bpbRootEntries] ; 224
div word [bpbBytesPerSector] ;512
push ax ; size of Root directory

;Calculate First Sector 
mov ax,word [bpbNumberOfFATs]
mul word [bpbSectorsPerFAT]
add ax,[bpbReservedSectors]
call LBA2CHS
pop ax
mov bx,0x200
call readSector


ret

;******************************************
;   Read Some Sector and Write to ram
;   al : numbers of sector to read
;   ch : number Track 1
;   cl : number sector 4
;   dh : number head 3
;*******************************************
readSector:
pusha
.reset:
mov ah,0
mov dl,0
int 0x13
jc .reset
popa

.read:
mov ah, 0x02  ; function number
mov dl, 0     ; number Drive (0 : A:\)
int 0x13
jc fault
ret

;*****************************
; load Kernel
;*****************************
loadKernel:
mov ax,0x50
mov es,ax
mov bx,0
push bx

.write:
mov ax,word [file_cluster]
add ax,31
xor dx,dx
call LBA2CHS
mov al,1
pop bx
call readSector
add bx,512
push bx



mov ax,word [file_cluster]
mov cx,ax
shr cx,1
add cx,ax ; dx is index of fat array
mov si,0x200 ; 0x500 where the fat loaded! ;;;;;;;;;;;;;;;;; Last Problem here 
add si,cx   ; realay cx is a index for the Fat table 
mov dx,word [si]
test ax,1 ; it is odd?
jnz .odd 

.even:
and dx,0000111111111111b
jmp .done

.odd:
mov cl,4
shr dx,cl

.done:
mov word [file_cluster],dx
cmp dx,0xFF0
jb  .write
pop bx
ret




;*****************************
;   BOOTLOADER STRAT
;******************************
loader:
;   Set Data Segment (at BottLoader Loaded in RAM)
mov ax,0x7c0
mov ds,ax 
mov es,ax 

call loadRoot
call lookUp
call loadFat
call loadKernel
jmp 0x00:0x500

cli
hlt
stage2notfound db "nfound!",0
file_cluster dw 0
krnldrName db "KLOADER SYS",0 ; UPPER Char
faulterr db "Faulted :(",0
msg db "Welcome",0
times 510 - ($-$$) db 0
dw 0xAA55