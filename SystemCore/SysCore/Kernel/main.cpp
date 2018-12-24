#include "DebugDisplay.h"
#include <Hal.h>
#include "exception.h"


int _cdecl main() { 
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
	interrupt_enable();
	DebugClrScreen(0x1f);
	DebugSetColor(0x78);
	DebugPuts("    Mango  Operating System is Loading ....                                     ");
	DebugSetColor(0x19);
	DebugPuts("we are optimizing th operating system WindowsX!\n");
	DebugGotoXY(0, 24);
	DebugSetColor(0x78);
	DebugPuts("CTRL+ALT+DEL to restart                                                         ");
	//INT(16);
	int a = 0;
	int b = 1;
	int c = 0;
	c = b / a;
	for (;;){
		DebugGotoXY(0, 12);
		DebugPrintf("Count = %d       ", get_tick());
	}

	return 0;
}



