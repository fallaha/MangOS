#ifndef _HAL_H_INCLUDE
#define _HAL_H_INCLUDE
#include <stdint.h>
extern void __cdecl hal_initialize(void);
extern void __cdecl hal_shutdown(void);
extern void INT(int i);
void outportb(uint16_t port, uint8_t value);
uint8_t inportb(uint16_t port);
void setvect(uint32_t n, void(&irq_handler)());
void intdone(uint8_t n);
void interrupt_enable();
void interrupt_disable();
uint32_t get_tick();

#endif /*_HAL_H_INCLUDE*/