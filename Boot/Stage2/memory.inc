; 1397/10 (c) AliFallah - powerd by : www.brokenthorn.com
%ifndef __MEMORY_INC_67343546FDCC56AAB872_INCLUDED__
%define __MEMORY_INC_67343546FDCC56AAB872_INCLUDED__
bits 16
%include "stdio.inc"
%include "multiboot.inc"
memory_err_msg db "Cant Obtain Memory!",0
memory_map_err_msg db "we Cant Get MemoryMap!",0

struc	MemoryMapEntry
	.baseAddress		resq	1
	.length				resq	1
	.type				resd	1
	.acpi_null			resd	1
endstruc

;*************************
;   out: EAX = Extended memory between 1MB and 16MB in KB
;   out: EBX = Extended memory above 16MB, in 64KB blocks
;*************************
get_memory_size:
    push ecx
    push edx
    xor ecx,ecx
    xor edx,edx
    mov eax,0xE801
    int 15h
    jc .problem
    cmp ax,0x80
    jz .problem
    cmp ax,0x86
    jz .problem
    jcxz .use_ax
    mov eax,ecx
    mov ebx,edx
    .use_ax:
    pop edx
    pop ecx
    ret
    .problem:
    mov si,memory_err_msg
    call Puts16
    cli
    hlt

;*************************************
;   in : ES:DI = Buffer for result
;   out : bp = mumber of entry
;*************************************
get_memory_map:
    xor ebx,ebx
    xor ebp,ebp
    .start:
    mov eax,0xe820
    mov ecx,24
    mov edx,'PAMS' ; SMAP
    int 15h
    jc .problem
    cmp eax,'PAMS'
    jnz .problem
    test ecx,ecx 
    jz .start
    test ebx,ebx
    jz .compelet
    .next:
    add di,24
    inc ebp
    jmp .start
    .compelet:
    ret
    .problem:
    mov si,memory_map_err_msg
    call Puts16
    cli
    hlt 









%endif