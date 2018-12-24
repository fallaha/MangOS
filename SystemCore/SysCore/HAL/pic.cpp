#include <Hal.h>
#include "pic.h"



void IA32_pic_send_data(uint8_t icw, uint8_t n){
	uint16_t port;
	port = n == 0 ? IA32_PIC0_DATA_REG : IA32_PIC1_DATA_REG;
	outportb(port, icw);
}

void IA32_pic_send_cmd(uint8_t icw, uint8_t n){
	uint16_t port;
	port = n == 0 ? IA32_PIC0_CMD_REG : IA32_PIC1_CMD_REG;
	outportb(port, icw);
}

uint8_t IA32_pic_read_data(uint8_t n){
	uint16_t port;
	port = n == 0 ? IA32_PIC0_DATA_REG : IA32_PIC1_DATA_REG;
	return inportb(port);
}

void IA32_pic_eoi(uint8_t intno){
	uint8_t icw = 0;
	icw = 0x20; /*set bit 5 of OCW 2 - End of Interrupt (EOI) request*/
	IA32_pic_send_cmd(icw, 0);
	if (intno > 8)
		IA32_pic_send_cmd(icw, 1);
}

void IA32_pic_initialize(uint8_t offset1, uint8_t offset2){
	uint8_t icw = 0;

	/*	ICW 1 Set Initialize ... */
	icw = (icw & ~IA32_PIC_ICW1_SNGL) | IA32_PIC_ICW1_IC4 | IA32_PIC_ICW1_INIT;
	IA32_pic_send_cmd(icw, 0);
	IA32_pic_send_cmd(icw, 1);

	/*	ICW 2 Set offset of interrupt in IDT */
	IA32_pic_send_data(offset1, 0);
	IA32_pic_send_data(offset2, 1);

	/*	ICW 3 Set pin of Connector between 2 pic */
	IA32_pic_send_data(0x04, 0);	/*IRQ = 2 -> 0b00000100*/
	IA32_pic_send_data(0x02, 1);	/*IRQ = 2 -> 0x2*/

	/* ICW 4 Only set 8086 Mode */
	icw = IA32_PIC_ICW4_x86; /*If set (1), it is in 80x86 mode. Cleared if MCS-80/86 mode*/
	IA32_pic_send_data(icw, 0);
	IA32_pic_send_data(icw, 1);

}