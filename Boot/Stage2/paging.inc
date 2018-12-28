; Ali Fallah (c) 97/10/06
%ifndef _PAGING_INC
%define _PAGING_INC
%define PT_FIRST_4MB 0x9D000
%define PT_KERNEL_SPACE_3GB 0x9E000
%define PAGE_DIRECTORY_ADDRESS 0x9C000

paging_init:
;-------------------- Map 1st 4Mb (phys) to 1st 4Mb (virt) - identity map
    mov ebx,0x00 ; phys address
    or ebx,3 ; writable and present page
    mov eax,PT_FIRST_4MB ; Address PageTable that we want set page descriptor to it
    mov cx,1024
    .loop1:
    mov dword [eax],ebx 
    add ebx,0x1000 ; 4096 dec or 1000000000000 bin
    add eax,4 ; next entry
    loop .loop1
        ; now we have a page table that describe (1st) 4mb of memory

;-------------------- Map 1Mb (phys) to 3Gb (virt) - Kernel Address Space
    mov ebx,0x100000 ; phys address
    or ebx,3 ; writable and present page
    mov eax,PT_KERNEL_SPACE_3GB ; Address PageTable that we want set page descriptor to it
    mov cx,1024
    .loop2:
    mov dword [eax],ebx 
    add ebx,0x1000 ; 4096 dec or 1000000000000 bin
    add eax,4 ; next entry
    loop .loop2

;-------------------- set Page table to page directory

    ; set 1st 4Mb pageTable to pageDirectory
    mov ebx,PT_FIRST_4MB ; phys address
    or ebx,3 ;  writable and present page
    mov eax,PAGE_DIRECTORY_ADDRESS
    mov dword [eax],ebx 

    ; set Kernel pageTable to pageDirectory
    mov ebx,PT_KERNEL_SPACE_3GB ; phys address
    or ebx,3 ;  writable and present page
    mov eax,PAGE_DIRECTORY_ADDRESS
    add eax,768*4 ; 0xC0000000 =  b11x00 b00x00 x000 -> 1100000000b = 768th entry of Page directory
    mov dword [eax],ebx 

;-------------------- Set Page Directory Base Address
    mov eax,PAGE_DIRECTORY_ADDRESS
    mov cr3,eax
;-------------------- paging Enable
    mov eax,cr0
    or eax,0x80000000 ; Bit 6 (PG) : Enables Memory Paging.
    mov cr0,eax
    ret
%endif