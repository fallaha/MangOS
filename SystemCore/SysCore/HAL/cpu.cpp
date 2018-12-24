#include "gdt.h"
#include "idt.h"
void cpu_initialize(){
	gdt_initialize();
	idt_initialize(0x08);
}
void cpu_shutdown(){

}