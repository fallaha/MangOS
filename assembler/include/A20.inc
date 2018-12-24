; ---------------------------------------------------- ;
;	Enable A20 Line in Orginal System and Emulator
	
EnableA20:	
	
	; Enable A20 Line With Keyboard (for Orginal System . no similator)
	.loopA20:
	in al,0x64
	test al,2
	jnz .loopA20
	
	mov al,0xDD 
	out 0x64,al 
	
	; Enable A20 for emulator
	mov al,2 
	out 0x92,al 
	
;--------------------------------
;	Return to OS
	ret