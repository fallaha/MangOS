
extern int _cdecl main();

// boot Loader start from here
int kernel_entry(){
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
	
	main();

#ifdef ARCH_X86
	_asm {
		cli 
		hlt
	}
#endif

	while (1);

}