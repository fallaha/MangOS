%ifndef __GDT_INC_67343546FDCC56AAB872_INCLUDED__
%define __GDT_INC_67343546FDCC56AAB872_INCLUDED__
bits 16

init_gdt:
pusha
cli
lgdt [toc]
sti
popa
ret




; Global Descriptor Table
gdt_start:
; null discriptor
    dd 0
    dd 0
; code discriptor
    dw 0xFFFF ; limit
    dw 0    ; base
    db 0    ; base (16-23)
    db 10011010b ; acsess
    db 11001111b
    db 0    ; base high
; data discriptor
    dw 0xFFFF ; limit
    dw 0    ; base
    db 0    ; base (16-23)
    db 10010010b ; acsess
    db 11001111b
    db 0    ; base high
gdt_end:

toc:
    dw   gdt_end-gdt_start-1 ; limit of GDT - size of 
    dd   gdt_start  ; base Adress of GDT

%endif ;__GDT_INC_67343546FDCC56AAB872_INCLUDED__
