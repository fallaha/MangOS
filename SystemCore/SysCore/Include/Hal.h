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



//! DMA Routines provided for driver use
extern	void dma_set_mode(uint8_t channel, uint8_t mode);
extern	void dma_set_read(uint8_t channel);
extern	void dma_set_write(uint8_t channel);
extern	void dma_set_address(uint8_t channel, uint8_t low, uint8_t high);
extern	void dma_set_count(uint8_t channel, uint8_t low, uint8_t high);
extern	void dma_mask_channel(uint8_t channel);
extern	void dma_unmask_channel(uint8_t channel);
extern	void dma_reset_flipflop(int dma);
extern  void dma_enable(uint8_t ctrl, bool e);
extern  void dma_reset(int dma);
extern  void dma_set_external_page_register(uint8_t reg, uint8_t val);
extern  void dma_unmask_all(int dma);


#endif /*_HAL_H_INCLUDE*/