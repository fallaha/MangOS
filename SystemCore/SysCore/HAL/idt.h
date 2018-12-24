#ifndef _IDT_H_INCLUDE
#define _IDT_H_INCLUDE
#include <stdint.h>
#include <Hal.h>

typedef void(*IA32_IRQ_HANDLER)();

#define IA32_MAX_INTERRUPT 256


/*Define Flags*/ /*Desc == descriptor*/
#define IA32_IDT_DESC_PRESENT 0x80
#define IA32_IDT_DESC_BIT32 0x0E
#define IA32_IDT_DESC_BIT16 0x06

/*Function*/
void idt_install_ir(uint32_t i, uint8_t flags, uint16_t segSel, IA32_IRQ_HANDLER irq);
void idt_initialize(uint16_t segSel);

#endif /*	_IDT_H_INCLUDE  */