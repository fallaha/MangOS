#ifndef _PIT_H_INCLUDE
#define _PIT_H_INCLUDE
#include <stdint.h>

/*	Bit Map */
#define IA32_PIT_BITMAP_BINARY_COUNTER 0X01 /* First bit */
#define IA32_PIT_BITMAP_OPERATING_MODE 0X0E 
#define IA32_PIT_BITMAP_READ_LOAD_MODE 0X30


#define IA32_PIT_BINARY_COUNTER_BINARY 0X00 
#define IA32_PIT_BINARY_COUNTER_BCD 0X01 
#define IA32_PIT_OPERATING_MODE_RATE_GENERATOR 0X04
#define IA32_PIT_OPERATING_MODE_SQUARE_WAVE_GENERATOR 0X06
#define IA32_PIT_READ_LOAD_MODE_LSB_MSB 0X30
#define IA32_PIT_CW_REG 0x43 /*Control Word Register*/
#define IA32_PIT_INPUT_FREQ 1193180  /*Orginal Frequency Cristal in x86 PC*/
#define IA32_PIT_COUNTER0_REG 0x40  
#define IA32_PIT_COUNTER1_REG 0x41  
#define IA32_PIT_COUNTER2_REG 0x42  

void IA32_pit_set_counter(uint8_t cntnum, uint16_t freq, uint8_t mode);
uint32_t IA32_pit_get_tick_count();
uint32_t IA32_pit_get_tick_count();
void IA32_pit_initialize();
#endif