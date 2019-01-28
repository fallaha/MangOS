#include "DebugDisplay.h"
#include <Hal.h>
#include "exception.h"
#include <bootinfo.h>
#include "pmm.h"
#include "vmm.h"
#include <kbrd.h>
#include <flopy.h>
#include <vfs.h>

//! format of a memory region
struct memory_region {
	uint32_t	startLow;
	uint32_t	startHigh;
	uint32_t	sizeLow;
	uint32_t	sizeHigh;
	uint32_t	type;
	uint32_t	acpi_3_0;
};

int _cdecl kernel_initialize (multiboot_info* info) {
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
	kbrd_initilize();


	interrupt_enable(); /*Enabling interrupt*/

	unsigned int memory_amount = 1024;
	memory_amount += info->m_memoryLo;
	memory_amount += info->m_memoryHi * 64;
	pmm_initialize(memory_amount,0x100000+kernel_img_size*512);


	memory_region *mem_mep = (memory_region*)0x1000;
	DebugGotoXY(0, 3);
	for (int i = 0; i < 15; i++){
		if (i>0 && mem_mep[i].startLow == 0)
			break;
		if (mem_mep[i].type == 1)	/*We Can Use it region*/
			 pmm_clr_region(mem_mep[i].startLow, mem_mep[i].sizeLow);
	}
	pmm_set_region(0x100000,kernel_img_size*512);
	vmm_initialize();

	//! set drive 0 as current drive
	flpydsk_set_working_drive(0);

	//! install floppy disk to IR 38, uses IRQ 6
	flpydsk_install(38);
	//! set DMA buffer to 64k
	flpydsk_set_dma(0x8000);

	vfs_initialize();

}


//! sleeps a little bit. This uses the HALs get_tick_count() which in turn uses the PIT
void sleep(int ms) {

	static int ticks = ms + get_tick();
	while (ticks > get_tick())
		;
}


uint8_t getch(){
	uint8_t ch;
	while (kbrd_get_last_std_char() == 0);
	ch = kbrd_make(kbrd_get_last_std_char());
	kbrd_destroy_last_char();
	return ch;
}

void gets (char *s,int max){
	int i = 0; 
	do{
		s[i] = getch();
		DebugPutc(s[i++]);
	} while (s[i-1] != '\r');
	s[i-1] = 0;
}

int stoi(char *s){
	int n = 0;
	int i = 0;
	int sign = 1;

	if (s[i] == '-'){
		sign = -1;
		i++;
	}

	while (s[i] != 0){
		n *= 10;
		n += s[i++];
	}
	n *= sign;
	return n;
}

int _cdecl main (multiboot_info* info) {
	kernel_initialize(info);
	DebugClrScreen(0x1f);
	DebugGotoXY(0, 0);

	DIRECTORY file;

	/* Only Support Absolute Addressing (e.g. a:/f1/f2/f3/f4/f5/f6.txt/ */
	/* note: place '/' in last character! */
	vfs_open_file(&file,"a:/a.txt/");
	DebugPrintf("%d\t", file.flag);
	DebugPrintf("%d\t", file.current_cluster);
	DebugPrintf("%d\t", file.device);
	DebugPrintf("%d\t", file.eof);
	DebugPrintf("%d\t", file.first_cluster);
	DebugPrintf("%d\t", file.length);
	DebugPrintf("%d\t", file.offset_cluster);
	DebugPrintf("%s\n", file.name);
	char buff[1024];
	vfs_read_file(&file, buff, 18);
	DebugPrintf("%s\n", buff);

	DebugPrintf("%d\t", file.flag);
	DebugPrintf("%d\t", file.current_cluster);
	DebugPrintf("%d\t", file.device);
	DebugPrintf("%d\t", file.eof);
	DebugPrintf("%d\t", file.first_cluster);
	DebugPrintf("%d\t", file.length);
	DebugPrintf("%d\t", file.offset_cluster);
	DebugPrintf("%s\n", file.name);
	char buff2[1024];
	vfs_rewind(&file);
	vfs_read_file(&file, buff2, 30);
	DebugPrintf("%s\n", buff2);
	for (;;);


}