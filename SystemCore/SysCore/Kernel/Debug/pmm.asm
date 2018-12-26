; Listing generated by Microsoft (R) Optimizing Compiler Version 18.00.40629.0 

	TITLE	C:\Users\Ali\Desktop\MangOS\SystemCore\SysCore\Kernel\pmm.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB MSVCRTD
INCLUDELIB OLDNAMES

_BSS	SEGMENT
__pmm_memory_size DD 01H DUP (?)
__pmm_used_blocks DD 01H DUP (?)
__pmm_max_blocks DD 01H DUP (?)
__pmm_memory_map DD 01H DUP (?)
_BSS	ENDS
PUBLIC	?pmm_initialize@@YAXII@Z			; pmm_initialize
PUBLIC	?pmm_set_region@@YAXII@Z			; pmm_set_region
PUBLIC	?pmm_clr_region@@YAXII@Z			; pmm_clr_region
PUBLIC	?pmm_alloc_block@@YAPAXXZ			; pmm_alloc_block
PUBLIC	?pmm_free_block@@YAXPAI@Z			; pmm_free_block
PUBLIC	?pmm_alloc_block_s@@YAPAXI@Z			; pmm_alloc_block_s
PUBLIC	?pmm_free_block_s@@YAXPAII@Z			; pmm_free_block_s
PUBLIC	?pmm_get_memory_size@@YAIXZ			; pmm_get_memory_size
PUBLIC	?pmm_get_max_blocks@@YAIXZ			; pmm_get_max_blocks
PUBLIC	?pmm_get_free_block_count@@YAIXZ		; pmm_get_free_block_count
PUBLIC	?pmm_get_used_block_count@@YAIXZ		; pmm_get_used_block_count
PUBLIC	?pmm_map_set_block@@YAXI@Z			; pmm_map_set_block
PUBLIC	?pmm_map_clr_block@@YAXI@Z			; pmm_map_clr_block
PUBLIC	?pmm_first_free@@YAHXZ				; pmm_first_free
PUBLIC	?pmm_first_free_s@@YAHI@Z			; pmm_first_free_s
EXTRN	?memset@@YAPAXPAXDI@Z:PROC			; memset
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
_size$ = 8						; size = 4
?pmm_first_free_s@@YAHI@Z PROC				; pmm_first_free_s

; 48   : int pmm_first_free_s(size_t size){

	push	ebx

; 49   : 	uint32_t counter=0;
; 50   : 	if (size == 0)

	mov	ebx, DWORD PTR _size$[esp]
	xor	eax, eax
	test	ebx, ebx
	jne	SHORT $LN13@pmm_first_

; 51   : 		return -1;

	or	eax, -1
	pop	ebx

; 68   : }

	ret	0
$LN13@pmm_first_:

; 52   : 
; 53   : 	if (size == 1)

	cmp	ebx, 1
	jne	SHORT $LN12@pmm_first_
	pop	ebx

; 54   : 		return pmm_first_free();

	jmp	?pmm_first_free@@YAHXZ			; pmm_first_free
$LN12@pmm_first_:
	push	ebp

; 55   : 
; 56   : 	for (uint32_t i = 0; i < pmm_get_max_blocks() / 32; i++)

	mov	ebp, DWORD PTR __pmm_max_blocks
	push	esi
	shr	ebp, 5
	xor	esi, esi
	push	edi
	test	ebp, ebp
	je	SHORT $LN27@pmm_first_
	mov	ecx, DWORD PTR __pmm_memory_map
$LL11@pmm_first_:

; 57   : 		if (_pmm_memory_map[i] != 0xFFFFFFFF){

	mov	edi, DWORD PTR [ecx+esi*4]
	cmp	edi, -1
	je	SHORT $LN8@pmm_first_

; 58   : 			for (uint32_t j = 0; j < 32; j++){

	xor	ecx, ecx
	npad	3
$LL7@pmm_first_:

; 59   : 				if (!(_pmm_memory_map[i] & (1 << j)))

	mov	edx, 1
	shl	edx, cl
	test	edx, edi
	jne	SHORT $LN4@pmm_first_

; 60   : 					counter++;

	inc	eax
	jmp	SHORT $LN3@pmm_first_
$LN4@pmm_first_:

; 61   : 				else counter = 0;

	xor	eax, eax
$LN3@pmm_first_:

; 62   : 				if (counter == size)

	cmp	eax, ebx
	je	SHORT $LN20@pmm_first_

; 58   : 			for (uint32_t j = 0; j < 32; j++){

	inc	ecx
	cmp	ecx, 32					; 00000020H
	jb	SHORT $LL7@pmm_first_

; 64   : 			}
; 65   : 		}
; 66   : 		else counter = 0;

	mov	ecx, DWORD PTR __pmm_memory_map
	jmp	SHORT $LN10@pmm_first_
$LN8@pmm_first_:
	xor	eax, eax
$LN10@pmm_first_:

; 55   : 
; 56   : 	for (uint32_t i = 0; i < pmm_get_max_blocks() / 32; i++)

	inc	esi
	cmp	esi, ebp
	jb	SHORT $LL11@pmm_first_
$LN27@pmm_first_:
	pop	edi
	pop	esi
	pop	ebp

; 67   : 	return -1;

	or	eax, -1
	pop	ebx

; 68   : }

	ret	0
$LN20@pmm_first_:

; 63   : 					return i * 32 + (j - (size - 1)); /*i test this function in another IDE*/

	shl	esi, 5
	lea	eax, DWORD PTR [ecx+1]
	sub	esi, ebx
	pop	edi
	add	eax, esi
	pop	esi
	pop	ebp
	pop	ebx

; 68   : }

	ret	0
?pmm_first_free_s@@YAHI@Z ENDP				; pmm_first_free_s
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
?pmm_first_free@@YAHXZ PROC				; pmm_first_free

; 38   : int pmm_first_free (){

	push	ebx
	push	esi
	push	edi

; 39   : 	for (uint32_t i = 0; i < pmm_get_max_blocks() / 32; i++)

	mov	edi, DWORD PTR __pmm_max_blocks
	xor	edx, edx
	shr	edi, 5
	test	edi, edi
	je	SHORT $LN22@pmm_first_
	mov	ebx, DWORD PTR __pmm_memory_map
$LL8@pmm_first_:

; 40   : 		if (_pmm_memory_map[i] != 0xFFFFFFFF){

	mov	esi, DWORD PTR [ebx+edx*4]
	cmp	esi, -1
	je	SHORT $LN7@pmm_first_

; 41   : 			for (uint32_t j = 0; j < 32; j++)

	xor	ecx, ecx
$LL4@pmm_first_:

; 42   : 				if (!(_pmm_memory_map[i] & (1 << j)))

	mov	eax, 1
	shl	eax, cl
	test	eax, esi
	je	SHORT $LN15@pmm_first_

; 41   : 			for (uint32_t j = 0; j < 32; j++)

	inc	ecx
	cmp	ecx, 32					; 00000020H
	jb	SHORT $LL4@pmm_first_
$LN7@pmm_first_:

; 39   : 	for (uint32_t i = 0; i < pmm_get_max_blocks() / 32; i++)

	inc	edx
	cmp	edx, edi
	jb	SHORT $LL8@pmm_first_
$LN22@pmm_first_:
	pop	edi
	pop	esi

; 44   : 		}
; 45   : 	return -1;

	or	eax, -1
	pop	ebx

; 46   : }

	ret	0
$LN15@pmm_first_:

; 43   : 					return i * 32 + j;

	shl	edx, 5
	pop	edi
	pop	esi
	pop	ebx
	lea	eax, DWORD PTR [ecx+edx]

; 46   : }

	ret	0
?pmm_first_free@@YAHXZ ENDP				; pmm_first_free
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
;	COMDAT ?pmm_map_clr_block@@YAXI@Z
_TEXT	SEGMENT
_frame$ = 8						; size = 4
?pmm_map_clr_block@@YAXI@Z PROC				; pmm_map_clr_block, COMDAT

; 31   : 	_pmm_memory_map[frame / 32] &= ~(1 << (frame % 32));

	mov	ecx, DWORD PTR _frame$[esp-4]
	mov	edx, ecx
	mov	eax, DWORD PTR __pmm_memory_map
	and	ecx, 31					; 0000001fH
	shr	edx, 5
	lea	edx, DWORD PTR [eax+edx*4]
	mov	eax, 1
	shl	eax, cl
	not	eax
	and	DWORD PTR [edx], eax

; 32   : }

	ret	0
?pmm_map_clr_block@@YAXI@Z ENDP				; pmm_map_clr_block
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
;	COMDAT ?pmm_map_set_block@@YAXI@Z
_TEXT	SEGMENT
_frame$ = 8						; size = 4
?pmm_map_set_block@@YAXI@Z PROC				; pmm_map_set_block, COMDAT

; 27   : 	_pmm_memory_map[frame / 32] |= (1 << (frame % 32));

	mov	ecx, DWORD PTR _frame$[esp-4]
	mov	edx, ecx
	mov	eax, DWORD PTR __pmm_memory_map
	and	ecx, 31					; 0000001fH
	shr	edx, 5
	lea	edx, DWORD PTR [eax+edx*4]
	mov	eax, 1
	shl	eax, cl
	or	DWORD PTR [edx], eax

; 28   : }

	ret	0
?pmm_map_set_block@@YAXI@Z ENDP				; pmm_map_set_block
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
?pmm_get_used_block_count@@YAIXZ PROC			; pmm_get_used_block_count

; 161  : 	return _pmm_used_blocks;

	mov	eax, DWORD PTR __pmm_used_blocks

; 162  : }

	ret	0
?pmm_get_used_block_count@@YAIXZ ENDP			; pmm_get_used_block_count
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
?pmm_get_free_block_count@@YAIXZ PROC			; pmm_get_free_block_count

; 158  : 	return _pmm_max_blocks-_pmm_used_blocks;

	mov	eax, DWORD PTR __pmm_max_blocks
	sub	eax, DWORD PTR __pmm_used_blocks

; 159  : }

	ret	0
?pmm_get_free_block_count@@YAIXZ ENDP			; pmm_get_free_block_count
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
?pmm_get_max_blocks@@YAIXZ PROC				; pmm_get_max_blocks

; 155  : 	return _pmm_max_blocks;

	mov	eax, DWORD PTR __pmm_max_blocks

; 156  : }

	ret	0
?pmm_get_max_blocks@@YAIXZ ENDP				; pmm_get_max_blocks
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
?pmm_get_memory_size@@YAIXZ PROC			; pmm_get_memory_size

; 152  : 	return _pmm_memory_size;

	mov	eax, DWORD PTR __pmm_memory_size

; 153  : }

	ret	0
?pmm_get_memory_size@@YAIXZ ENDP			; pmm_get_memory_size
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
_pa$ = 8						; size = 4
_count$ = 12						; size = 4
?pmm_free_block_s@@YAXPAII@Z PROC			; pmm_free_block_s

; 140  : 
; 141  : 	physical_addr addr = (physical_addr)pa;
; 142  : 	int frame = addr / PMM_BLOCK_SIZE;

	mov	eax, DWORD PTR _pa$[esp-4]
	push	edi

; 143  : 
; 144  : 	for (uint32_t i = 0; i < count; i++)

	mov	edi, DWORD PTR _count$[esp]
	shr	eax, 12					; 0000000cH
	test	edi, edi
	je	SHORT $LN12@pmm_free_b

; 140  : 
; 141  : 	physical_addr addr = (physical_addr)pa;
; 142  : 	int frame = addr / PMM_BLOCK_SIZE;

	push	ebx
	push	ebp
	mov	ebp, DWORD PTR __pmm_memory_map
	mov	ebx, edi
	push	esi
	npad	5
$LL3@pmm_free_b:

; 145  : 		pmm_map_clr_block(frame + i);

	mov	ecx, eax
	mov	edx, 1
	shr	ecx, 5
	lea	esi, DWORD PTR [ecx*4]
	mov	ecx, eax
	and	ecx, 31					; 0000001fH
	inc	eax
	shl	edx, cl
	not	edx
	and	DWORD PTR [esi+ebp], edx
	dec	ebx
	jne	SHORT $LL3@pmm_free_b
	pop	esi
	pop	ebp
	pop	ebx
$LN12@pmm_free_b:

; 146  : 
; 147  : 	_pmm_used_blocks -= count;

	sub	DWORD PTR __pmm_used_blocks, edi
	pop	edi

; 148  : 
; 149  : }

	ret	0
?pmm_free_block_s@@YAXPAII@Z ENDP			; pmm_free_block_s
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
_count$ = 8						; size = 4
?pmm_alloc_block_s@@YAPAXI@Z PROC			; pmm_alloc_block_s

; 121  : 
; 122  : 	if (pmm_get_free_block_count() <= 0)

	mov	eax, DWORD PTR __pmm_max_blocks
	push	ebx
	mov	ebx, DWORD PTR __pmm_used_blocks
	sub	eax, ebx
	jne	SHORT $LN5@pmm_alloc_
	pop	ebx

; 136  : 
; 137  : }

	ret	0
$LN5@pmm_alloc_:
	push	ebp

; 123  : 		return 0;
; 124  : 
; 125  : 	int first_frame = pmm_first_free_s(count);

	mov	ebp, DWORD PTR _count$[esp+4]
	push	edi
	push	ebp
	call	?pmm_first_free_s@@YAHI@Z		; pmm_first_free_s
	mov	edi, eax
	add	esp, 4

; 126  : 
; 127  : 	if (first_frame == -1)

	cmp	edi, -1
	jne	SHORT $LN4@pmm_alloc_

; 128  : 		return 0;

	pop	edi
	pop	ebp
	xor	eax, eax
	pop	ebx

; 136  : 
; 137  : }

	ret	0
$LN4@pmm_alloc_:

; 129  : 
; 130  : 	for (uint32_t i = 0; i < count; i++)

	test	ebp, ebp
	je	SHORT $LN1@pmm_alloc_
	push	esi
	mov	ebx, ebp
	mov	esi, edi
	mov	ebp, DWORD PTR __pmm_memory_map
	npad	3
$LL3@pmm_alloc_:

; 131  : 		pmm_map_set_block(first_frame + i);

	mov	eax, esi
	mov	ecx, esi
	shr	eax, 5
	and	ecx, 31					; 0000001fH
	inc	esi
	lea	edx, DWORD PTR [eax*4]
	mov	eax, 1
	shl	eax, cl
	or	DWORD PTR [edx+ebp], eax
	dec	ebx
	jne	SHORT $LL3@pmm_alloc_
	mov	ebx, DWORD PTR __pmm_used_blocks
	mov	ebp, DWORD PTR _count$[esp+12]
	pop	esi
$LN1@pmm_alloc_:

; 132  : 
; 133  : 	physical_addr addr = first_frame * PMM_BLOCK_SIZE;

	shl	edi, 12					; 0000000cH

; 134  : 	_pmm_used_blocks += count;

	add	ebx, ebp

; 135  : 	return (void*)addr;

	mov	eax, edi
	mov	DWORD PTR __pmm_used_blocks, ebx
	pop	edi
	pop	ebp
	pop	ebx

; 136  : 
; 137  : }

	ret	0
?pmm_alloc_block_s@@YAPAXI@Z ENDP			; pmm_alloc_block_s
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
_pa$ = 8						; size = 4
?pmm_free_block@@YAXPAI@Z PROC				; pmm_free_block

; 113  : 	physical_addr addr = (physical_addr)pa;
; 114  : 	int frame = addr / PMM_BLOCK_SIZE;

	mov	ecx, DWORD PTR _pa$[esp-4]

; 115  : 	pmm_map_clr_block(frame);

	mov	eax, DWORD PTR __pmm_memory_map

; 116  : 	_pmm_used_blocks--;

	dec	DWORD PTR __pmm_used_blocks
	shr	ecx, 12					; 0000000cH
	mov	edx, ecx
	and	ecx, 31					; 0000001fH
	shr	edx, 5
	lea	edx, DWORD PTR [eax+edx*4]
	mov	eax, 1
	shl	eax, cl
	not	eax
	and	DWORD PTR [edx], eax

; 117  : 
; 118  : }

	ret	0
?pmm_free_block@@YAXPAI@Z ENDP				; pmm_free_block
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
?pmm_alloc_block@@YAPAXXZ PROC				; pmm_alloc_block

; 100  : 	if (pmm_get_free_block_count() <= 0)

	mov	eax, DWORD PTR __pmm_max_blocks
	push	edi
	mov	edi, DWORD PTR __pmm_used_blocks
	sub	eax, edi
	jne	SHORT $LN2@pmm_alloc_
	pop	edi

; 109  : 
; 110  : }

	ret	0
$LN2@pmm_alloc_:
	push	esi

; 101  : 		return 0;
; 102  : 	int frame = pmm_first_free();

	call	?pmm_first_free@@YAHXZ			; pmm_first_free
	mov	esi, eax

; 103  : 	if (frame == -1)

	cmp	esi, -1
	jne	SHORT $LN1@pmm_alloc_

; 104  : 		return 0;

	pop	esi
	xor	eax, eax
	pop	edi

; 109  : 
; 110  : }

	ret	0
$LN1@pmm_alloc_:

; 105  : 	pmm_map_set_block(frame);

	mov	eax, DWORD PTR __pmm_memory_map
	mov	ecx, esi
	shr	ecx, 5

; 106  : 	physical_addr addr = frame*PMM_BLOCK_SIZE;
; 107  : 	_pmm_used_blocks++;

	inc	edi
	lea	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR __pmm_used_blocks, edi
	mov	ecx, esi
	mov	eax, 1
	and	ecx, 31					; 0000001fH
	shl	esi, 12					; 0000000cH
	shl	eax, cl
	or	DWORD PTR [edx], eax

; 108  : 	return (void*)addr;

	mov	eax, esi
	pop	esi
	pop	edi

; 109  : 
; 110  : }

	ret	0
?pmm_alloc_block@@YAPAXXZ ENDP				; pmm_alloc_block
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
_base$ = 8						; size = 4
_limit$ = 12						; size = 4
?pmm_clr_region@@YAXII@Z PROC				; pmm_clr_region

; 89   : void pmm_clr_region(uint32_t base, uint32_t limit){

	push	esi

; 90   : 	uint32_t frame = base / PMM_BLOCK_SIZE;

	mov	esi, DWORD PTR _base$[esp]
	push	edi

; 91   : 	uint32_t size = limit / PMM_BLOCK_SIZE;

	mov	edi, DWORD PTR _limit$[esp+4]
	shr	edi, 12					; 0000000cH
	shr	esi, 12					; 0000000cH

; 92   : 	for (uint32_t i = 0; i<size; i++)

	test	edi, edi
	je	SHORT $LN14@pmm_clr_re

; 90   : 	uint32_t frame = base / PMM_BLOCK_SIZE;

	push	ebx
	mov	ebx, DWORD PTR __pmm_memory_map
	mov	eax, 1
	mov	ecx, esi
	push	ebp
	rol	eax, cl
	mov	ebp, edi
$LL3@pmm_clr_re:

; 93   : 		pmm_map_clr_block(frame++);

	mov	ecx, esi
	inc	esi
	shr	ecx, 5
	lea	edx, DWORD PTR [ebx+ecx*4]
	mov	ecx, eax
	not	ecx
	rol	eax, 1
	and	DWORD PTR [edx], ecx
	dec	ebp
	jne	SHORT $LL3@pmm_clr_re

; 94   : 	_pmm_used_blocks -= size;

	sub	DWORD PTR __pmm_used_blocks, edi

; 95   : 	/*why?*/
; 96   : 	pmm_map_set_block(0);

	or	DWORD PTR [ebx], 1
	pop	ebp
	pop	ebx
	pop	edi
	pop	esi

; 97   : }

	ret	0
$LN14@pmm_clr_re:

; 95   : 	/*why?*/
; 96   : 	pmm_map_set_block(0);

	mov	eax, DWORD PTR __pmm_memory_map
	sub	DWORD PTR __pmm_used_blocks, edi
	pop	edi
	pop	esi
	or	DWORD PTR [eax], 1

; 97   : }

	ret	0
?pmm_clr_region@@YAXII@Z ENDP				; pmm_clr_region
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
_base$ = 8						; size = 4
_limit$ = 12						; size = 4
?pmm_set_region@@YAXII@Z PROC				; pmm_set_region

; 81   : 	uint32_t size = limit / PMM_BLOCK_SIZE;

	mov	edx, DWORD PTR _limit$[esp-4]
	mov	eax, DWORD PTR _base$[esp-4]
	shr	edx, 12					; 0000000cH
	shr	eax, 12					; 0000000cH

; 82   : 	for (uint32_t i = 0; i<size; i++)

	test	edx, edx
	je	SHORT $LN14@pmm_set_re

; 80   : 	uint32_t block = base / PMM_BLOCK_SIZE;

	add	eax, edx
	mov	ecx, eax
	push	esi
	and	ecx, 31					; 0000001fH
	mov	esi, 1
	shl	esi, cl
	mov	ecx, edx
	push	edi
	mov	edi, DWORD PTR __pmm_memory_map
	shr	eax, 5
	lea	eax, DWORD PTR [edi+eax*4]
$LL3@pmm_set_re:

; 83   : 		pmm_map_set_block(block + size);

	or	DWORD PTR [eax], esi
	dec	ecx
	jne	SHORT $LL3@pmm_set_re

; 84   : 	_pmm_used_blocks += size;

	add	DWORD PTR __pmm_used_blocks, edx

; 85   : 	/*why?*/
; 86   : 	pmm_map_set_block(0);

	or	DWORD PTR [edi], 1
	pop	edi
	pop	esi

; 87   : }

	ret	0
$LN14@pmm_set_re:

; 85   : 	/*why?*/
; 86   : 	pmm_map_set_block(0);

	mov	eax, DWORD PTR __pmm_memory_map
	add	DWORD PTR __pmm_used_blocks, edx
	or	DWORD PTR [eax], 1

; 87   : }

	ret	0
?pmm_set_region@@YAXII@Z ENDP				; pmm_set_region
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\pmm.cpp
_TEXT	SEGMENT
_mem_size$ = 8						; size = 4
_bitmap$ = 12						; size = 4
?pmm_initialize@@YAXII@Z PROC				; pmm_initialize

; 71   : 	_pmm_memory_size = mem_size;

	mov	ecx, DWORD PTR _mem_size$[esp-4]

; 72   : 	_pmm_max_blocks = _pmm_memory_size * 1024 / PMM_BLOCK_SIZE;
; 73   : 	_pmm_used_blocks = _pmm_max_blocks;
; 74   : 	_pmm_memory_map = (physical_addr*)bitmap;

	mov	eax, DWORD PTR _bitmap$[esp-4]
	mov	DWORD PTR __pmm_memory_size, ecx
	shl	ecx, 10					; 0000000aH
	shr	ecx, 12					; 0000000cH
	mov	DWORD PTR __pmm_max_blocks, ecx
	mov	DWORD PTR __pmm_used_blocks, ecx

; 75   : 	/*Set All Bitmap to 1 - it means All Memory are using*/
; 76   : 	memset(_pmm_memory_map, 0xff, _pmm_max_blocks / PMM_BLOCKS_PER_BYTE);

	shr	ecx, 3
	push	ecx
	push	-1
	push	eax
	mov	DWORD PTR __pmm_memory_map, eax
	call	?memset@@YAPAXPAXDI@Z			; memset
	add	esp, 12					; 0000000cH

; 77   : }

	ret	0
?pmm_initialize@@YAXII@Z ENDP				; pmm_initialize
_TEXT	ENDS
END