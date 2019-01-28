; Listing generated by Microsoft (R) Optimizing Compiler Version 18.00.40629.0 

	TITLE	C:\Users\Ali\Desktop\MangOS\SystemCore\SysCore\Kernel\vfs.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB MSVCRT
INCLUDELIB OLDNAMES

PUBLIC	?FILE_SYSTEMS@@3PAPAUFILE_SYSTEM@@A		; FILE_SYSTEMS
_BSS	SEGMENT
?FILE_SYSTEMS@@3PAPAUFILE_SYSTEM@@A DD 01aH DUP (?)	; FILE_SYSTEMS
_BSS	ENDS
CONST	SEGMENT
$SG2820	DB	'FAT 12', 00H
CONST	ENDS
PUBLIC	?vfs_initialize@@YAXXZ				; vfs_initialize
PUBLIC	?vfs_open_file@@YAXPAUDIRECTORY@@PBD@Z		; vfs_open_file
PUBLIC	?vfs_read_file@@YAXPAUDIRECTORY@@PADH@Z		; vfs_read_file
PUBLIC	?vfs_mount_fs@@YAXPAUFILE_SYSTEM@@D@Z		; vfs_mount_fs
PUBLIC	?vfs_get_device_name@@YAPADH@Z			; vfs_get_device_name
PUBLIC	?vfs_rewind@@YAXPAUDIRECTORY@@@Z		; vfs_rewind
PUBLIC	?fat12_default_device_init@@YAXXZ		; fat12_default_device_init
PUBLIC	?vfsWriteFile@@YAHXZ				; vfsWriteFile
PUBLIC	?vfsCloseFile@@YAHXZ				; vfsCloseFile
PUBLIC	?vfsUnMountFS@@YAHXZ				; vfsUnMountFS
PUBLIC	?vfsOpenSubDir@@YAPAUDIRECTORY@@PAD@Z		; vfsOpenSubDir
EXTRN	?fat12_open_file@@YAXPAUDIRECTORY@@PBD@Z:PROC	; fat12_open_file
EXTRN	?fat12_read_one_sector_file@@YAXPAUDIRECTORY@@PAE@Z:PROC ; fat12_read_one_sector_file
EXTRN	?fat12_close_file@@YAXPAUDIRECTORY@@@Z:PROC	; fat12_close_file
EXTRN	?fat12_UnMount@@YAXXZ:PROC			; fat12_UnMount
EXTRN	?flpydsk_read_sector@@YAPAEH@Z:PROC		; flpydsk_read_sector
EXTRN	?memcpy@@YAPAXPAXPBXI@Z:PROC			; memcpy
EXTRN	?FAT12_FS_DF@@3UFILE_SYSTEM@@A:BYTE		; FAT12_FS_DF
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfsOpenSubDir@@YAPAUDIRECTORY@@PAD@Z
_TEXT	SEGMENT
_doo$ = -4						; size = 4
_name$ = 8						; size = 4
?vfsOpenSubDir@@YAPAUDIRECTORY@@PAD@Z PROC		; vfsOpenSubDir, COMDAT

; 58   : DIRECTORY* vfsOpenSubDir(char* name){

	push	ecx

; 59   : 	DIRECTORY* doo;
; 60   : 	return doo;

	mov	eax, DWORD PTR _doo$[esp+4]

; 61   : }

	pop	ecx
	ret	0
?vfsOpenSubDir@@YAPAUDIRECTORY@@PAD@Z ENDP		; vfsOpenSubDir
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfsUnMountFS@@YAHXZ
_TEXT	SEGMENT
?vfsUnMountFS@@YAHXZ PROC				; vfsUnMountFS, COMDAT

; 55   : 	return 1;

	mov	eax, 1

; 56   : }

	ret	0
?vfsUnMountFS@@YAHXZ ENDP				; vfsUnMountFS
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfsCloseFile@@YAHXZ
_TEXT	SEGMENT
?vfsCloseFile@@YAHXZ PROC				; vfsCloseFile, COMDAT

; 47   : 	return 1;

	mov	eax, 1

; 48   : }

	ret	0
?vfsCloseFile@@YAHXZ ENDP				; vfsCloseFile
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfsWriteFile@@YAHXZ
_TEXT	SEGMENT
?vfsWriteFile@@YAHXZ PROC				; vfsWriteFile, COMDAT

; 43   : 	return 1;

	mov	eax, 1

; 44   : }

	ret	0
?vfsWriteFile@@YAHXZ ENDP				; vfsWriteFile
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?fat12_default_device_init@@YAXXZ
_TEXT	SEGMENT
?fat12_default_device_init@@YAXXZ PROC			; fat12_default_device_init, COMDAT

; 79   : 	BOOT_SECTOR_STRUCT* btsect;
; 80   : 	btsect = (BOOT_SECTOR_STRUCT*)flpydsk_read_sector(0);

	push	0
	call	?flpydsk_read_sector@@YAPAEH@Z		; flpydsk_read_sector

; 81   : 	memcpy(&FAT12_FS_DF.bpb, &btsect->bpb, sizeof(FAT12_FS_DF.bpb));

	push	33					; 00000021H
	add	eax, 3
	push	eax
	push	OFFSET ?FAT12_FS_DF@@3UFILE_SYSTEM@@A+8
	call	?memcpy@@YAPAXPAXPBXI@Z			; memcpy

; 82   : 	memcpy(FAT12_FS_DF.name, "FAT 12", sizeof(FAT12_FS_DF.name));

	push	8
	push	OFFSET $SG2820
	push	OFFSET ?FAT12_FS_DF@@3UFILE_SYSTEM@@A	; FAT12_FS_DF
	call	?memcpy@@YAPAXPAXPBXI@Z			; memcpy
	add	esp, 28					; 0000001cH

; 83   : 
; 84   : 	FAT12_FS_DF.Open = fat12_open_file;

	mov	DWORD PTR ?FAT12_FS_DF@@3UFILE_SYSTEM@@A+49, OFFSET ?fat12_open_file@@YAXPAUDIRECTORY@@PBD@Z ; fat12_open_file

; 85   : 	FAT12_FS_DF.Close = fat12_close_file;

	mov	DWORD PTR ?FAT12_FS_DF@@3UFILE_SYSTEM@@A+61, OFFSET ?fat12_close_file@@YAXPAUDIRECTORY@@@Z ; fat12_close_file

; 86   : 	FAT12_FS_DF.Read = fat12_read_one_sector_file;

	mov	DWORD PTR ?FAT12_FS_DF@@3UFILE_SYSTEM@@A+57, OFFSET ?fat12_read_one_sector_file@@YAXPAUDIRECTORY@@PAE@Z ; fat12_read_one_sector_file

; 87   : 	FAT12_FS_DF.UnMount = fat12_UnMount;

	mov	DWORD PTR ?FAT12_FS_DF@@3UFILE_SYSTEM@@A+45, OFFSET ?fat12_UnMount@@YAXXZ ; fat12_UnMount

; 88   : 	vfs_mount_fs(&FAT12_FS_DF, 0);

	mov	DWORD PTR ?FILE_SYSTEMS@@3PAPAUFILE_SYSTEM@@A, OFFSET ?FAT12_FS_DF@@3UFILE_SYSTEM@@A ; FAT12_FS_DF

; 89   : 
; 90   : }

	ret	0
?fat12_default_device_init@@YAXXZ ENDP			; fat12_default_device_init
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfs_rewind@@YAXPAUDIRECTORY@@@Z
_TEXT	SEGMENT
_dir$ = 8						; size = 4
?vfs_rewind@@YAXPAUDIRECTORY@@@Z PROC			; vfs_rewind, COMDAT

; 23   : 	dir->eof = 0;

	mov	ecx, DWORD PTR _dir$[esp-4]

; 24   : 	dir->current_cluster = dir->first_cluster;

	mov	eax, DWORD PTR [ecx+8]
	mov	DWORD PTR [ecx+16], 0
	mov	DWORD PTR [ecx+12], eax

; 25   : }

	ret	0
?vfs_rewind@@YAXPAUDIRECTORY@@@Z ENDP			; vfs_rewind
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfs_get_device_name@@YAPADH@Z
_TEXT	SEGMENT
_device$ = 8						; size = 4
?vfs_get_device_name@@YAPADH@Z PROC			; vfs_get_device_name, COMDAT

; 70   : 	return FILE_SYSTEMS[device]->name;

	mov	eax, DWORD PTR _device$[esp-4]
	mov	eax, DWORD PTR ?FILE_SYSTEMS@@3PAPAUFILE_SYSTEM@@A[eax*4]

; 71   : }

	ret	0
?vfs_get_device_name@@YAPADH@Z ENDP			; vfs_get_device_name
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfs_mount_fs@@YAXPAUFILE_SYSTEM@@D@Z
_TEXT	SEGMENT
_pfs$ = 8						; size = 4
_device$ = 12						; size = 1
?vfs_mount_fs@@YAXPAUFILE_SYSTEM@@D@Z PROC		; vfs_mount_fs, COMDAT

; 51   : 	FILE_SYSTEMS[0] = pfs;

	mov	eax, DWORD PTR _pfs$[esp-4]
	mov	DWORD PTR ?FILE_SYSTEMS@@3PAPAUFILE_SYSTEM@@A, eax

; 52   : }

	ret	0
?vfs_mount_fs@@YAXPAUFILE_SYSTEM@@D@Z ENDP		; vfs_mount_fs
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfs_read_file@@YAXPAUDIRECTORY@@PADH@Z
_TEXT	SEGMENT
_sector$ = -512						; size = 512
_dir$ = 8						; size = 4
_buffer$ = 12						; size = 4
_size$ = 16						; size = 4
?vfs_read_file@@YAXPAUDIRECTORY@@PADH@Z PROC		; vfs_read_file, COMDAT

; 27   : void vfs_read_file(DIRECTORY* dir, char* buffer,int size){

	sub	esp, 512				; 00000200H
	push	esi

; 29   : 	char sector[512];
; 30   : 	
; 31   : 	while (size > 0 && !dir->eof){

	mov	esi, DWORD PTR _size$[esp+512]
	test	esi, esi
	jle	SHORT $LN11@vfs_read_f

; 28   : 	int i = 0;

	push	ebx
	mov	ebx, DWORD PTR _dir$[esp+516]
	push	edi
	mov	edi, DWORD PTR _buffer$[esp+520]
$LL4@vfs_read_f:

; 29   : 	char sector[512];
; 30   : 	
; 31   : 	while (size > 0 && !dir->eof){

	cmp	DWORD PTR [ebx+16], 0
	jne	SHORT $LN12@vfs_read_f

; 32   : 		FILE_SYSTEMS[dir->device]->Read(dir, (unsigned char *)sector);

	movzx	eax, BYTE PTR [ebx+24]
	lea	ecx, DWORD PTR _sector$[esp+524]
	push	ecx
	push	ebx
	mov	eax, DWORD PTR ?FILE_SYSTEMS@@3PAPAUFILE_SYSTEM@@A[eax*4]
	mov	eax, DWORD PTR [eax+57]
	call	eax
	add	esp, 8

; 33   : 		if (size<512)

	cmp	esi, 512				; 00000200H
	jge	SHORT $LN2@vfs_read_f

; 34   : 			memcpy(buffer + i * 512, sector, size%512);

	mov	eax, esi
	and	eax, 511				; 000001ffH
	push	eax

; 35   : 		else

	jmp	SHORT $LN14@vfs_read_f
$LN2@vfs_read_f:

; 36   : 			memcpy(buffer+i*512, sector, 512);

	push	512					; 00000200H
$LN14@vfs_read_f:
	lea	eax, DWORD PTR _sector$[esp+528]
	push	eax
	push	edi
	call	?memcpy@@YAPAXPAXPBXI@Z			; memcpy

; 37   : 		i ++;
; 38   : 		size -= 512;

	sub	esi, 512				; 00000200H
	add	esp, 12					; 0000000cH
	add	edi, 512				; 00000200H
	test	esi, esi
	jg	SHORT $LL4@vfs_read_f
$LN12@vfs_read_f:
	pop	edi
	pop	ebx
$LN11@vfs_read_f:
	pop	esi

; 39   : 	}
; 40   : }

	add	esp, 512				; 00000200H
	ret	0
?vfs_read_file@@YAXPAUDIRECTORY@@PADH@Z ENDP		; vfs_read_file
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfs_open_file@@YAXPAUDIRECTORY@@PBD@Z
_TEXT	SEGMENT
_file$ = 8						; size = 4
_path$ = 12						; size = 4
?vfs_open_file@@YAXPAUDIRECTORY@@PBD@Z PROC		; vfs_open_file, COMDAT

; 11   : 	char device = 0; /* a */
; 12   : 	if (path){

	mov	eax, DWORD PTR _path$[esp-4]
	test	eax, eax
	je	SHORT $LN1@vfs_open_f

; 13   : 		
; 14   : 		FILE_SYSTEMS[device]->Open(file,path);

	mov	DWORD PTR _path$[esp-4], eax
	mov	eax, DWORD PTR ?FILE_SYSTEMS@@3PAPAUFILE_SYSTEM@@A
	mov	eax, DWORD PTR [eax+49]
	jmp	eax
$LN1@vfs_open_f:

; 15   : 		return ;
; 16   : 	}
; 17   : 
; 18   : 	file->flag = 5;

	mov	eax, DWORD PTR _file$[esp-4]
	mov	DWORD PTR [eax+20], 5

; 19   : 	return ;
; 20   : }

	ret	0
?vfs_open_file@@YAXPAUDIRECTORY@@PBD@Z ENDP		; vfs_open_file
_TEXT	ENDS
; Function compile flags: /Ogtpy
; File c:\users\ali\desktop\mangos\systemcore\syscore\kernel\vfs.cpp
;	COMDAT ?vfs_initialize@@YAXXZ
_TEXT	SEGMENT
?vfs_initialize@@YAXXZ PROC				; vfs_initialize, COMDAT

; 64   : 	fat12_default_device_init();

	jmp	?fat12_default_device_init@@YAXXZ	; fat12_default_device_init
?vfs_initialize@@YAXXZ ENDP				; vfs_initialize
_TEXT	ENDS
END