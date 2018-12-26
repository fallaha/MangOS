#include "DebugDisplay.h"
#include <Hal.h>
#include "exception.h"
#include <bootinfo.h>
#include "pmm.h"

//! format of a memory region
struct memory_region {
	uint32_t	startLow;
	uint32_t	startHigh;
	uint32_t	sizeLow;
	uint32_t	sizeHigh;
	uint32_t	type;
	uint32_t	acpi_3_0;
};

int _cdecl main(multiboot_info* info) {
	uint32_t kernel_img_size;
	_asm mov word ptr[kernel_img_size], dx /*Get Kernel Image Size form Bootloader*/
	hal_initialize();

	//! install our exception handlers
	setvect(0, (void(__cdecl &)(void))divide_by_zero_fault);
	setvect(1, (void(__cdecl &)(void))single_step_trap);
	setvect(2, (void(__cdecl &)(void))nmi_trap);
	setvect(3, (void(__cdecl &)(void))breakpoint_trap);
	setvect(4, (void(__cdecl &)(void))overflow_trap);
	setvect(5, (void(__cdecl &)(void))bounds_check_fault);
	setvect(6, (void(__cdecl &)(void))invalid_opcode_fault);
	setvect(7, (void(__cdecl &)(void))no_device_fault);
	setvect(8, (void(__cdecl &)(void))double_fault_abort);
	setvect(10, (void(__cdecl &)(void))invalid_tss_fault);
	setvect(11, (void(__cdecl &)(void))no_segment_fault);
	setvect(12, (void(__cdecl &)(void))stack_fault);
	setvect(13, (void(__cdecl &)(void))general_protection_fault);
	setvect(14, (void(__cdecl &)(void))page_fault);
	setvect(16, (void(__cdecl &)(void))fpu_fault);
	setvect(17, (void(__cdecl &)(void))alignment_check_fault);
	setvect(18, (void(__cdecl &)(void))machine_check_abort);
	setvect(19, (void(__cdecl &)(void))simd_fpu_fault);

	interrupt_enable(); /*Enabling interrupt*/
	DebugClrScreen(0x1f);
	DebugSetColor(0x78);
	DebugPuts("    Mango  Operating System is Loading ....                                     ");
	DebugSetColor(0x19);
	DebugPuts("we are optimizing th operating system WindowsX!\n");

	unsigned int memory_amount = 1024;
	memory_amount += info->m_memoryLo;
	memory_amount += info->m_memoryHi * 64;
	DebugPrintf("ram Amount : %d", memory_amount);
	pmm_initialize(memory_amount,0x100000+kernel_img_size*512);

	DebugGotoXY(0, 24);
	DebugSetColor(0x78);
	DebugPuts("CTRL+ALT+DEL to restart                                                         ");


	memory_region *mem_mep = (memory_region*)0x1000;
	DebugGotoXY(0, 3);
	for (int i = 0; i < 15; i++){
		if (i>0 && mem_mep[i].startLow == 0)
			break;
		DebugPrintf("Base Frame : %d  and limit Block %d type: %d \n",
			mem_mep[i].startLow / 4096, mem_mep[i].sizeLow / 4096,
			mem_mep[i].type);
		if (mem_mep[i].type == 1)	/*We Can Use it region*/
			 pmm_clr_region(mem_mep[i].startLow, mem_mep[i].sizeLow);
	}
	pmm_set_region(0x100000,kernel_img_size*512);
	
	DebugPrintf("Kernel Block : %d \n", kernel_img_size);


	DebugPrintf("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n",
		pmm_get_max_blocks(), pmm_get_used_block_count(), pmm_get_free_block_count());

	//! allocating and deallocating memory examples...

	DebugSetColor(0x12);

	uint32_t* p = (uint32_t*)pmm_alloc_block();
	DebugPrintf("\np allocated at frame : %d", uint32_t(p) / 4096);

	uint32_t* p2 = (uint32_t*)pmm_alloc_block_s(2);
	DebugPrintf("\nallocated 2 blocks for p2 at frame : %d", uint32_t(p2) / 4096);

	uint32_t* p3 = (uint32_t*)pmm_alloc_block();
	DebugPrintf("\np allocated at frame : %d", uint32_t(p3) / 4096);

	pmm_free_block(p);
	p = (uint32_t*)pmm_alloc_block();
	DebugPrintf("\nUnallocated p to free block 1. p is reallocated to frame : %d", uint32_t(p) / 4096);

	pmm_free_block(p);
	pmm_free_block_s(p2, 2);


	for (;;){
		DebugGotoXY(0, 23);
		DebugPrintf("Count = %d", get_tick());
	}

	return 0;
}



