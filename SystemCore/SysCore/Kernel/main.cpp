#include "DebugDisplay.h"
#include <Hal.h>
#include "exception.h"
#include <bootinfo.h>
#include "pmm.h"
#include "vmm.h"
#include <kbrd.h>
#include <flopy.h>
#include <vfs.h>
#include <pm.h>

/*** BGA Support ****************************************/

/* We'll be needing this. */
void outportw(unsigned short portid, unsigned short value) {
	_asm {
		mov		ax, word ptr[value]
			mov		dx, word ptr[portid]
			out		dx, ax
	}
}

/* Definitions for BGA. Reference Graphics 1. */
#define VBE_DISPI_IOPORT_INDEX          0x01CE
#define VBE_DISPI_IOPORT_DATA           0x01CF
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_LFB_ENABLED           0x40

/* writes to BGA port. */
void VbeBochsWrite(uint16_t index, uint16_t value) {
	outportw(VBE_DISPI_IOPORT_INDEX, index);
	outportw(VBE_DISPI_IOPORT_DATA, value);
}

/* sets video mode. */
void VbeBochsSetMode(uint16_t xres, uint16_t yres, uint16_t bpp) {
	VbeBochsWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
	VbeBochsWrite(VBE_DISPI_INDEX_XRES, xres);
	VbeBochsWrite(VBE_DISPI_INDEX_YRES, yres);
	VbeBochsWrite(VBE_DISPI_INDEX_BPP, bpp);
	VbeBochsWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);
}

/* video mode info. */
#define WIDTH           800
#define HEIGHT          600
#define BPP             32
#define BYTES_PER_PIXEL 4

/* BGA LFB is at LFB_PHYSICAL. */
#define LFB_PHYSICAL 0xE0000000
#define LFB_VIRTUAL  0x300000
#define PAGE_SIZE    0x1000

/* map LFB into current process address space. */
void* VbeBochsMapLFB() {
	int pfcount = WIDTH*HEIGHT*BYTES_PER_PIXEL / PAGE_SIZE;
	for (int c = 0; c <= pfcount; c++)
		vmm_mapPhysicalAddress(vmm_get_cur_dir(), LFB_VIRTUAL + c * PAGE_SIZE, LFB_PHYSICAL + c * PAGE_SIZE, 7);
	return (void*)LFB_VIRTUAL;
}

/* clear screen to white. */
void fillScreen32() {
	uint32_t* lfb = (uint32_t*)LFB_VIRTUAL;
	for (uint32_t c = 0; c<WIDTH*HEIGHT; c++)
		lfb[c] = 0xffffffff;
}


//! format of a memory region
struct memory_region {
	uint32_t	startLow;
	uint32_t	startHigh;
	uint32_t	sizeLow;
	uint32_t	sizeHigh;
	uint32_t	type;
	uint32_t	acpi_3_0;
};

uint32_t kernel_img_size;
void _cdecl kernel_initialize(multiboot_info* info) {
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
	pmm_initialize(memory_amount, 0x100000 + kernel_img_size * 512);


	memory_region *mem_mep = (memory_region*)0x1000;
	//DebugGotoXY(0, 3);
	for (int i = 0; i < 15; i++){
		if (i>0 && mem_mep[i].startLow == 0)
			break;
		if (mem_mep[i].type == 1)	/*We Can Use it region*/
			pmm_clr_region(mem_mep[i].startLow, mem_mep[i].sizeLow);
	}
	pmm_set_region(0x100000, kernel_img_size * 512);
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

void gets(char *s, int max){
	int i = 0;
	do{
		s[i] = getch();
		DebugPutc(s[i++]);
	} while (s[i - 1] != '\r');
	s[i - 1] = 0;
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


#include "..\ProcessesManagement\PCB.h"
#include "..\HAL\gdt.h"

PROCESS pp1;


void p1();
void p2();
void p3();

/* render rectangle in 32 bpp modes. */
void rect32(int x, int y, int w, int h, int col) {
	uint32_t* lfb = (uint32_t*)LFB_VIRTUAL;
	for (uint32_t k = 0; k < h; k++)
		for (uint32_t j = 0; j < w; j++)
			lfb[(j + x) + (k + y) * WIDTH] = col;
}


/* thread cycles through colors of red. */
void p1() {
	int col = 0;
	bool dir = true;
	while (1) {
		rect32(200, 250, 100, 100, col << 16);
		if (dir) {
			if (col++ == 0xfe)
				dir = false;
		}
		else
			if (col-- == 1)
				dir = true;
	}
}

/* thread cycles through colors of green. */
void p2() {
	int col = 0;
	bool dir = true;
	while (1) {
		rect32(350, 250, 100, 100, col << 8);
		if (dir) {
			if (col++ == 0xfe)
				dir = false;
		}
		else
			if (col-- == 1)
				dir = true;
	}
}

/* thread cycles through colors of blue. */
void p3() {
	int col = 0;
	bool dir = true;
	while (1) {
		rect32(500, 250, 100, 100, col);
		if (dir) {
			if (col++ == 0xfe)
				dir = false;
		}
		else
			if (col-- == 1)
				dir = true;
	}
}


__declspec(align(16)) char _kernel_stack[8096];

void _cdecl main(multiboot_info* bootinfo) {

	kernel_initialize(bootinfo);

	/* adjust stack. */
	_asm lea esp, dword ptr[_kernel_stack + 8096];



	//DebugClrScreen(0x1f);
	/* set video mode and map framebuffer. */
	VbeBochsSetMode(WIDTH, HEIGHT, BPP);
	VbeBochsMapLFB();
	fillScreen32();

	create_thread(p1, &pp1.thread[0]);
	create_thread(p2, &pp1.thread[1]);
	create_thread(p3, &pp1.thread[2]);

	pp1.thread[0].t_next = &pp1.thread[1];
	pp1.thread[1].t_next = &pp1.thread[2];
	pp1.thread[2].t_next = &pp1.thread[0];

	pm_set_current_task(&pp1.thread[0]);

	start_scheduler();
	_asm cli

	task_exe();


}


