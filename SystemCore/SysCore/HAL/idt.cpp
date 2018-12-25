#include <Hal.h>
#include "idt.h"
#include <stdint.h>
#include "..\Kernel\DebugDisplay.h"
struct idtr {
	uint16_t limit; /*Limit of idtr*/
	uint32_t base; /*base of idtr*/
};

struct idt_description{
	uint16_t offsetLow; /*bit 0-15 of interrupt routine (ir) address*/
	uint16_t segSel;/*Segmet Selector*/
	uint8_t reserved; /*must be 0*/
	uint8_t flags; 
	uint16_t offsetHi; /*bit 15-31 of interrupt routine (ir) address*/
};


static struct idtr _idtr;
static struct idt_description _idt[IA32_MAX_INTERRUPT];

/* Set idtr Register*/
static void idt_install(){
#ifdef _MSC_VER
	_asm lidt [_idtr]
#endif
}


void idt_default_handler(){
#ifdef _DEBUG
	DebugClrScreen(0x0f);
	DebugSetColor(0x78);
	DebugPuts("Exception not Define now :(");
#endif
	for (;;);
}


void idt_install_ir(uint32_t i, uint8_t flags, uint16_t segSel, IA32_IRQ_HANDLER irq){
	uint32_t addressIR = (uint32_t) &(*irq);
	_idt[i].offsetLow = (uint16_t) addressIR & 0xffff;
	_idt[i].offsetHi = (uint16_t)(addressIR >> 16) & 0xffff;
	_idt[i].flags = flags;
	_idt[i].reserved = 0;
	_idt[i].segSel = segSel;

}
void idt_initialize(uint16_t segSel){
	_idtr.base = (uint32_t) &_idt[0];
	_idtr.limit = sizeof(struct idt_description) * IA32_MAX_INTERRUPT - 1;;

	for (int i = 0; i < IA32_MAX_INTERRUPT; i++) /*Set default Handler for All interrupt !*/
		idt_install_ir(i, IA32_IDT_DESC_PRESENT | IA32_IDT_DESC_BIT32,
		segSel, (IA32_IRQ_HANDLER )idt_default_handler);


	idt_install();
}

