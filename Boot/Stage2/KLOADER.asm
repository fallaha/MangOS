; 1397/08 (c) AliFallah - powerd by : www.brokenthorn.com
bits 16
org 0x500
jmp main


%include "gdt.inc"
%include "stdio.inc"
%include "A20.inc"
%include "memory.inc"
%include "multiboot.inc"
LoadingMsg db "Preparing to load operating system...", 0x0D, 0x0A, 0x00
KernelName db "KRNL32  EXE",0 ; UPPER Char
kernel_cluster dw 0
kernel_sector_size dw 0
faulterr db "oooh!",0
Kernelnotfound db "Sorry KERNEL.EXE not Found! :(",0
mem_size dd 0

boot_info:
istruc multiboot_info
	at multiboot_info.flags,			dd 0
	at multiboot_info.memoryLo,			dd 0
	at multiboot_info.memoryHi,			dd 0
	at multiboot_info.bootDevice,		dd 0
	at multiboot_info.cmdLine,			dd 0
	at multiboot_info.mods_count,		dd 0
	at multiboot_info.mods_addr,		dd 0
	at multiboot_info.syms0,			dd 0
	at multiboot_info.syms1,			dd 0
	at multiboot_info.syms2,			dd 0
	at multiboot_info.mmap_length,		dd 0
	at multiboot_info.mmap_addr,		dd 0
	at multiboot_info.drives_length,	dd 0
	at multiboot_info.drives_addr,		dd 0
	at multiboot_info.config_table,		dd 0
	at multiboot_info.bootloader_name,	dd 0
	at multiboot_info.apm_table,		dd 0
	at multiboot_info.vbe_control_info,	dd 0
	at multiboot_info.vbe_mode_info,	dw 0
	at multiboot_info.vbe_interface_seg,dw 0
	at multiboot_info.vbe_interface_off,dw 0
	at multiboot_info.vbe_interface_len,dw 0
iend


;*************************************************;
; OEM Parameter block
;*************************************************;
bpbBytesPerSector: DW 512
bpbSectorsPerCluster: DB 1
bpbReservedSectors: DW 1
bpbNumberOfFATs: DB 2
bpbRootEntries: DW 224
bpbSectorsPerFAT: DW 9
bpbSectorsPerTrack: DW 18
bpbHeadsPerCylinder: DW 2

;*************************************************;
; Bootloader Entry Point
;*************************************************;

fault:
mov si,faulterr
call Puts16
cli
hlt

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
mov bx,0x0
push ax
mov ax,0x7c0
mov es,ax
pop ax


call readSector
ret

;   Convert LBA to CHS
LBA2CHS:
    div word [bpbSectorsPerTrack] ; ax/18 = head
    mov cl,dl ; number sector
    add cl,1
    xor dx,dx
    div word [bpbHeadsPerCylinder]
    mov ch,al  ; head/2 = track
    mov dh,dl ;; number head error rrrrrrrrrrr
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



;******************************************
;   LOOKup for Kernel
;******************************************
lookUp:
    mov cx,224
    push ax
    mov ax,0x7c0
    mov es,ax
    pop ax
    mov di,0x00
    .search:
    push cx 
    push di
    mov cx,11
    mov si,KernelName
    rep cmpsb
    jcxz .done
    pop di
    pop cx
    add di,0x20
    loop .search
    mov si,Kernelnotfound
    call Puts16
    call fault
    ret
    .done:
    pop di ; address file in root
    pop cx
    add di,26
    mov ax,[es:di] ;;;;;;;;;;;;;;;;;;;;; this last Problem mov ax,[di]
    mov [kernel_cluster], ax
    ret

;   Load File Allocation Table
loadFat:
    mov ax,1
    xor dx,dx ; error div 
    call LBA2CHS
    mov al,[bpbSectorsPerFAT]
    push ax
    mov ax,0x5c0
    mov es,ax
    pop ax
    mov bx,0x00
    call readSector
    ret



;*****************************
; load Kernel
;*****************************
loadKernel:
    push ax
    mov ax,0x5c0
    mov es,ax
    pop ax
    mov bx,0x2000 ; 0x5c0 : 0x2000 = 0x7c00
    push bx

    .write:
    mov ax,[kernel_cluster]
    add ax,31
    xor dx,dx
    call LBA2CHS
    mov al,1
    pop bx
    call readSector
    add bx,512
    push bx

    mov ax,[kernel_cluster]
    mov cx,ax
    shr cx,1
    add cx,ax ; dx is index of fat array
    mov si,0x000 ; 0x500 where the fat loaded!
    add si,cx   ; realay cx is a index for the Fat table 
    mov dx,word[es:si]
    test ax,1 ; it is odd?
    jnz .odd 
.even:
and dx,0000111111111111b
jmp .done

.odd:
mov cl,4
shr dx,cl
    .done:
    inc word[kernel_sector_size]
    mov [kernel_cluster],dx
    cmp dx,0xFF0
    jb  .write
    pop bx
    ret


main:
; init Segments
    cli
    xor ax,ax 
    mov ds,ax
    mov es,ax
    mov ax,0x9000
    mov ss,ax
    mov sp,0xFFFF
    sti 





   

; Load Kernel!
;load_kernel:
    call loadRoot
    call lookUp
    call loadFat
    call loadKernel


; init GDT
    call init_gdt

;   Enable A20
    call enA20

    ; print initilizing GDT
	mov	si, LoadingMsg
	call	Puts16

    call get_memory_size
    mov word[boot_info+multiboot_info.memoryLo],ax
    mov word[boot_info+multiboot_info.memoryHi],bx


;   ;Get and Save Memory Size (KB)
    ;push eax 
    ;mov eax,ebx 
    ;mov bx,64
    ;mul bx 
    ;mov ebx,eax 
    ;pop eax 
    ;add eax,ebx
    ;add eax,1024 ; For 1 MB Low Memory (Conventional Memory) 
    ;mov dword[mem_size],eax  ; Save it

;   Get MemoryMap

   
	mov		eax, 0x0
    mov es,ax   ; errror wrong mov ds,ax
    mov		di, 0x1000
    call get_memory_map

;goto_pmode:
    cli
    mov eax,cr0
    or eax,1 ; Bit 0 (PE) : Puts the system into protected mode
    mov cr0,eax
    jmp 08h:stage3

    ; note : we dont use sti for reEnable interrupt because it occur GPT








bits 32
%include "stdio32.inc"
%include "paging.inc"
wlcome db 0xA,"             Welcome to Modern operating system! :)",0x0A,0
stage3:
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov esp,0x90000

    cld
    mov esi,0x7c00
    mov edi,0x100000
    xor eax,eax 
    mov ax,word[kernel_sector_size]
    mov ecx,128
    mul cx ; 512/4 (4 because doubleWord Transfer)
    mov ecx,eax 
    rep movsd

    call paging_init

    mov ebx,dword[0x100000+0x3c]
    mov esi,ebx
    add esi,0x100000
    mov edi,PEtext
    cmpsw 
    jnz  kernel_error
    add esi,2+20+4*4 ; Image Base
    mov ebx,dword[esi]
    add ebx,0x100000
    mov ebp,ebx
    cli
    mov dx,word[kernel_sector_size]
    mov eax,0x2BADB002
    mov ebx,0
    push dword boot_info

    call ebp ; in this place last error jmp ebp
kernel_error:
    mov ebx,kernel_PE_error_msg
    call puts32
stop:
    cli
    hlt


PEtext db "PE"
kernel_PE_error_msg db "Kernel Not Supported!",0
mem_map_buff db 's'