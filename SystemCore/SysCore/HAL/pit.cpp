#include <stdint.h>
#include <Hal.h>
#include "pit.h"


/* Global Tick count */
static volatile uint32_t			_pit_ticks = 0;

void IA32_pit_irq(){
	_pit_ticks++;
	intdone(0);
	_asm iretd
}

/* Sets new pit tick count and returns prev. value */
uint32_t IA32_pit_set_tick_count(uint32_t i) {

	uint32_t ret = _pit_ticks;
	_pit_ticks = i;
	return ret;
}


/* returns current tick count */
uint32_t IA32_pit_get_tick_count() {

	return _pit_ticks;
}


void IA32_pit_send_cmd(uint8_t cmd){
	outportb(IA32_PIT_CW_REG, cmd);
}

void IA32_pit_send_data(uint8_t data, uint8_t cntnum){
	uint16_t port;
	if (cntnum == 0) port = IA32_PIT_COUNTER0_REG; 
	else if (cntnum == 1) port = IA32_PIT_COUNTER1_REG; 
	else port = IA32_PIT_COUNTER2_REG;
	outportb(port, data);
}

uint8_t IA32_pit_read_data(uint8_t cntnum){
	uint16_t port;
	if (cntnum == 0) port = IA32_PIT_COUNTER0_REG;
	else if (cntnum == 1) port = IA32_PIT_COUNTER1_REG;
	else port = IA32_PIT_COUNTER2_REG;
	return inportb(port);
}

void IA32_pit_set_counter (uint8_t cntnum,uint16_t freq,uint8_t mode){

	if (cntnum > 2) 
		return;

	uint8_t cwr = 0;
	uint16_t div;

	/* Make Control Word Register */
	cwr = (cwr & ~IA32_PIT_BITMAP_BINARY_COUNTER) | IA32_PIT_BINARY_COUNTER_BINARY;
	cwr |= IA32_PIT_READ_LOAD_MODE_LSB_MSB;
	cwr = (cwr & ~IA32_PIT_BITMAP_OPERATING_MODE) | mode;
	cwr |= uint8_t (cntnum << 6);

	/* Send Command to PIT and init it */
	IA32_pit_send_cmd(cwr);

	div = uint16_t(uint16_t(IA32_PIT_INPUT_FREQ) / freq);

	IA32_pit_send_data(div & 0xff,cntnum);		   /* Send LSB */
	IA32_pit_send_data((div >> 8) & 0xff,cntnum);  /* Send MSB */


	/* initilize tick */
	_pit_ticks = 0;
}

void IA32_pit_initialize(){
	setvect(32, IA32_pit_irq);
}