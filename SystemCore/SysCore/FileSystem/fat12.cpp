/* Ali Fallah (c) 97/10/20 */
#include "fat12.h"
#include <flopy.h>
#include <string.h>
#include <vfs.h>

#define DEFAULT_FILE_SYSTEM_LET 'a'
extern struct FILE_SYSTEM* FILE_SYSTEMS[VFS_MAX_FILE_SYSTEM];
struct FILE_SYSTEM DEFAULT_FAT12_FS_STRUCT;


DIRECTORY* fat12_open_file(const char* FileName){

}

/* initialize default Floppy File system */
void fat12_default_device_init(){
	BOOT_SECTOR_STRUCT* btsect;
	btsect = (BOOT_SECTOR_STRUCT*) flpydsk_read_sector(0);
	memcpy(&DEFAULT_FAT12_FS_STRUCT.bpb, &btsect->bpb, sizeof(DEFAULT_FAT12_FS_STRUCT.bpb));
	memcpy(DEFAULT_FAT12_FS_STRUCT.name, "fat 12", sizeof(DEFAULT_FAT12_FS_STRUCT.name));
	DEFAULT_FAT12_FS_STRUCT.Open = fat12_open_file;
	FILE_SYSTEMS[DEFAULT_FILE_SYSTEM_LET - 'a'] = &DEFAULT_FAT12_FS_STRUCT;


}

