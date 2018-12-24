#include "gdt.h"
#include <stdint.h>

struct gdtr {
	uint16_t limit; /* limit of GDT - size of */
	uint32_t base;	/*base Adress of GDT*/
};


struct gdt_descriptor {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t flags;
	uint8_t grand;
	uint8_t base_hi;
};

static struct gdt_descriptor _gdt[MAX_GDT_DESCRIPTOR];
static struct  gdtr _gdtr;

void gdt_install(){
#ifdef _MSC_VER
	_asm lgdt[_gdtr]
#endif // _MSC_VER
}


void gdt_set_descriptor(uint32_t i,uint64_t base,uint32_t limit,uint8_t acsess,uint8_t grand){

	_gdt[i].limit_low = uint16_t(limit & 0xffff);
	_gdt[i].base_low = uint16_t(base & 0xffff);
	_gdt[i].base_mid = uint8_t((base >> 16) & 0xff);
	_gdt[i].base_hi = uint8_t((base >> 24) & 0xff);
	_gdt[i].flags = acsess;
	_gdt[i].grand = uint8_t((limit >> 16)&0xf);
	_gdt[i].grand |= grand & 0xf0;

}

void  gdt_initialize(){
	_gdtr.base = (uint32_t)&_gdt[0]; /* i Forgot here */
	_gdtr.limit = (sizeof(gdt_descriptor)*MAX_GDT_DESCRIPTOR)-1;
	gdt_set_descriptor(0, 0, 0, 0, 0);	/*Null descriptor*/
	gdt_set_descriptor(1, 0X00, 0XFFFFF,/*Code Descriptor*/
		I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	gdt_set_descriptor(2, 0x00, 0xFFFFF, /*Data descriptor*/
		I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	gdt_install();

}


