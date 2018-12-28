#ifndef _PMM_H
#define _PMM_H
#include <stdint.h>
typedef uint32_t physical_addr;

void pmm_initialize(uint32_t mem_size, physical_addr bitmap);
void pmm_set_region(uint32_t base, uint32_t limit);
void pmm_clr_region(uint32_t base, uint32_t limit);
void* pmm_alloc_block();
void pmm_free_block(void *pa);
void* pmm_alloc_block_s(uint32_t count);
void pmm_free_block_s(physical_addr *pa, uint32_t count);
uint32_t pmm_get_memory_size();
uint32_t pmm_get_max_blocks();
uint32_t pmm_get_free_block_count();
uint32_t pmm_get_used_block_count();

#endif