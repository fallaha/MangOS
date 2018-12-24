%ifndef __A20_INC_126746354254654763645_INCLUDED__
%define __A20_INC_126746354254654763645_INCLUDED__
bits 16

enA20:
    cli
    mov al,0xD0 ;Command : Read Output Port
    out 0x64,al
    call wait_output ; waite to keyboard put data on output
    in al,0x60
    push eax 
    call wait_input ; check do the buffer is empity
    mov al,0xD1 ; 	Command:Write Output Port
    out 0x64,al 
    call wait_input ; check do the buffer is empity
    pop eax
    or al,2
    out 0x60,al ; set new output keybord microcontroller
    sti
    ret


wait_input:     ;Bit 1: Input Buffer Status
    in al,0x64      ;0: Input buffer empty, can be written
    test al,2       ;1: Input buffer full, dont write yet
    jnz wait_input
    ret 

wait_output:   ;Bit 0: Output Buffer Status
    in al,0x64     ;0: Output buffer empty, dont read yet
    test al,1      ;1: Output buffer full, please read me :)
    jz wait_output
    ret

%endif ;__A20_INC_126746354254654763645_INCLUDED__