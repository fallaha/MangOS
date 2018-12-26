#include <stdint.h>
#include <bootinfo.h> 

extern int _cdecl main(multiboot_info* info);

// boot Loader start from here
int _cdecl kernel_entry(multiboot_info* info){
#ifdef ARCH_X86
	// Set register
	_asm {
		cli 
		mov ax,10h
		mov ds,ax 
		mov es,ax 
		mov fs,ax 
		mov gs,ax 
	}
#endif
	
	main(info);

#ifdef ARCH_X86
	_asm {
		cli 
		hlt
	}
#endif

	while (1);

}