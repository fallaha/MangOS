#ifndef ARCH_X86
#error "Hal only supported on x86 intel processor"
#endif

#include <Hal.h>
#include "cpu.h"
#include "pic.h"
#include "pit.h"
#include "idt.h"

void __cdecl hal_initialize(){
	cpu_initialize();
	IA32_pic_initialize(0x20,0x28);
	IA32_pit_initialize();
	IA32_pit_set_counter(0, 100, IA32_PIT_OPERATING_MODE_RATE_GENERATOR);
	
}
void __cdecl hal_shutdown(){
	cpu_shutdown();
}

void intdone(uint8_t n){
	IA32_pic_eoi(n);
}

void setvect(uint32_t n, void(&irq_handler)()){
	idt_install_ir(n, IA32_IDT_DESC_PRESENT | IA32_IDT_DESC_BIT32,
		0x08, irq_handler);
}

void INT(int i){
#ifdef _MSC_VER
	_asm{
		mov al, byte ptr [i]
		mov byte ptr[runint+1], al
		jmp runint
	runint:
		int 0

	}
#endif
}

void interrupt_enable (){
	_asm sti
}

void interrupt_disable (){
	_asm cli
}

uint32_t get_tick(){
	return	IA32_pit_get_tick_count();
}
uint8_t inportb (uint16_t port){
	_asm {
		mov dx, word ptr[port]
		in al,dx
		mov byte ptr[port],al
	}
	return (uint8_t)port;
}

void outportb (uint16_t port,uint8_t value){
	_asm {
		mov dx, word ptr[port]
		mov al, byte ptr[value]
		out dx, al
	}
}