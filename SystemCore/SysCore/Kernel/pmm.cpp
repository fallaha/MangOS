/* ALi Fallah (c) 97/10/05 */

#include <stdint.h>
#include "pmm.h"
#include <string.h>

#define PMM_BLOCKS_PER_BYTE 8
#define PMM_BLOCK_SIZE 4096
#define PMM_BLOCK_ALIGN PMM_BLOCK_SIZE

/*	Static Variable	*/
static uint32_t		_pmm_memory_size = 0;
static	uint32_t	_pmm_used_blocks = 0;
static	uint32_t	_pmm_max_blocks  = 0;
static	uint32_t*	_pmm_memory_map  = 0;

/* Private Function */
inline void pmm_map_set_block(uint32_t frame);
inline void pmm_map_clr_block(uint32_t frame);
inline bool pmm_test(uint32_t frame);
int pmm_first_free();
int pmm_first_free_s(size_t size);

/*Body of Functions !*/

inline void pmm_map_set_block(uint32_t frame){
	_pmm_memory_map[frame / 32] |= (1 << (frame % 32));
}

inline void pmm_map_clr_block(uint32_t frame){
	_pmm_memory_map[frame / 32] &= ~(1 << (frame % 32));
}

inline bool pmm_test(uint32_t frame){
	return _pmm_memory_map[frame / 32] & (1 << (frame % 32));
}

int pmm_first_free (){
	for (uint32_t i = 0; i < pmm_get_max_blocks() / 32; i++)
		if (_pmm_memory_map[i] != 0xFFFFFFFF){
			for (uint32_t j = 0; j < 32; j++)
				if (!(_pmm_memory_map[i] & (1 << j)))
					return i * 32 + j;
		}
	return -1;
}

int pmm_first_free_s(size_t size){
	uint32_t counter=0;
	if (size == 0)
		return -1;

	if (size == 1)
		return pmm_first_free();

	for (uint32_t i = 0; i < pmm_get_max_blocks() / 32; i++)
		if (_pmm_memory_map[i] != 0xFFFFFFFF){
			for (uint32_t j = 0; j < 32; j++){
				if (!(_pmm_memory_map[i] & (1 << j)))
					counter++;
				else counter = 0;
				if (counter == size)
					return i * 32 + (j - (size - 1)); /*i test this function in another IDE*/
			}
		}
		else counter = 0;
	return -1;
}

void pmm_initialize(uint32_t mem_size, physical_addr bitmap){
	_pmm_memory_size = mem_size;
	_pmm_max_blocks = _pmm_memory_size * 1024 / PMM_BLOCK_SIZE;
	_pmm_used_blocks = _pmm_max_blocks;
	_pmm_memory_map = (physical_addr*)bitmap;
	/*Set All Bitmap to 1 - it means All Memory are using*/
	memset(_pmm_memory_map, 0xff, _pmm_max_blocks / PMM_BLOCKS_PER_BYTE);
}

void pmm_set_region(uint32_t base,uint32_t limit){
	uint32_t block = base / PMM_BLOCK_SIZE;
	uint32_t size = limit / PMM_BLOCK_SIZE;
	for (uint32_t i = 0; i<size; i++)
		pmm_map_set_block(block + size);
	_pmm_used_blocks += size;
	/*why?*/
	pmm_map_set_block(0);
}

void pmm_clr_region(uint32_t base, uint32_t limit){
	uint32_t frame = base / PMM_BLOCK_SIZE;
	uint32_t size = limit / PMM_BLOCK_SIZE;
	for (uint32_t i = 0; i<size; i++)
		pmm_map_clr_block(frame++);
	_pmm_used_blocks -= size;
	/*why?*/
	pmm_map_set_block(0);
}
/*Return Physical Address of Frame*/
void* pmm_alloc_block(){
	if (pmm_get_free_block_count() <= 0)
		return 0;
	int frame = pmm_first_free();
	if (frame == -1)
		return 0;
	pmm_map_set_block(frame);
	physical_addr addr = frame*PMM_BLOCK_SIZE;
	_pmm_used_blocks++;
	return (void*)addr;

}

/*Get Physical Address and change to frame Number */
void pmm_free_block(void *pa){
	physical_addr addr = (physical_addr)pa;
	int frame = addr / PMM_BLOCK_SIZE;
	pmm_map_clr_block(frame);
	_pmm_used_blocks--;

}

void* pmm_alloc_block_s(uint32_t count){

	if (pmm_get_free_block_count() <= 0)
		return 0;

	int first_frame = pmm_first_free_s(count);

	if (first_frame == -1)
		return 0;

	for (uint32_t i = 0; i < count; i++)
		pmm_map_set_block(first_frame + i);

	physical_addr addr = first_frame * PMM_BLOCK_SIZE;
	_pmm_used_blocks += count;
	return (void*)addr;

}

void pmm_free_block_s(physical_addr *pa,uint32_t count){

	physical_addr addr = (physical_addr)pa;
	int frame = addr / PMM_BLOCK_SIZE;

	for (uint32_t i = 0; i < count; i++)
		pmm_map_clr_block(frame + i);

	_pmm_used_blocks -= count;

}

uint32_t pmm_get_memory_size(){
	return _pmm_memory_size;
}
uint32_t pmm_get_max_blocks(){
	return _pmm_max_blocks;
}
uint32_t pmm_get_free_block_count(){
	return _pmm_max_blocks-_pmm_used_blocks;
}
uint32_t pmm_get_used_block_count(){
	return _pmm_used_blocks;
}