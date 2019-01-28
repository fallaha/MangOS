#ifndef _FAT12_H
#define _FAT12_H
#include <stdint.h>
#include <vfs.h>
#include "filesystem.h"
#include <ctype.h>

struct DIRECTORY_ENTRY {

	uint8_t   Filename[8];
	uint8_t   Ext[3];
	uint8_t   Attrib;
	uint8_t   Reserved;
	uint8_t   TimeCreatedMs;
	uint16_t  TimeCreated;
	uint16_t  DateCreated;
	uint16_t  DateLastAccessed;
	uint16_t  FirstClusterHiBytes;
	uint16_t  LastModTime;
	uint16_t  LastModDate;
	uint16_t  FirstCluster;
	uint32_t  FileSize;

};

extern void fat12_open_file(DIRECTORY*file, const char* path);
extern uint8_t * fat12_read_one_sector_from_device(uint32_t sector, char device);
extern void fat12_read_one_sector_file(DIRECTORY* file, unsigned char * buffer);
extern void fat12_to_dos_filename(const char* filename, char* fname, unsigned int FNameLength);
extern void fat12_get_directory(DIRECTORY* file, DIRECTORY* drawer, char* name);
extern void fat12_get_directory_from_root(DIRECTORY* file, char* name);
extern void fat12_default_device_init();
extern void fat12_close_file(DIRECTORY* file);
extern void fat12_UnMount();



#endif