#ifndef _VFS_H
#define _VFS_H
#include <stdint.h>


#define VFS_MAX_DIR_NAME_LENGTH 11
#define VFS_MAX_DEVICE_NAME_LENGTH 8
#define VFS_MAX_FILE_SYSTEM 26 /*26 device Max*/


struct DIRECTORY
{
	
	uint32_t	offset_cluster;
	uint32_t	length;
	uint32_t	first_cluster;
	uint32_t	current_cluster;
	uint32_t	eof;
	uint32_t	flag;
	uint8_t		device;
	char		name[VFS_MAX_DIR_NAME_LENGTH];
};


struct BIOS_PARAMATER_BLOCK {
	uint8_t			OEMName[8];
	uint16_t		BytesPerSector;
	uint8_t			SectorsPerCluster;
	uint16_t		ReservedSectors;
	uint8_t			NumberOfFats;
	uint16_t		NumDirEntries;
	uint16_t		NumSectors;
	uint8_t			Media;
	uint16_t		SectorsPerFat;
	uint16_t		SectorsPerTrack;
	uint16_t		HeadsPerCyl;
	uint32_t		HiddenSectors;
	uint32_t		LongSectors;
};

struct BIOS_PARAMATER_BLOCK_EXT
{
	uint32_t			SectorsPerFat32;   //sectors per FAT
	uint16_t			Flags;             //flags
	uint16_t			Version;           //version
	uint32_t			RootCluster;       //starting root directory
	uint16_t			InfoCluster;
	uint16_t			BackupBoot;        //location of bootsector copy
	uint16_t			Reserved[6];

};

struct BOOT_SECTOR_STRUCT
{
	uint8_t						ignore[3];		//first 3 bytes are ignored (our jmp instruction)
	BIOS_PARAMATER_BLOCK		bpb;			//BPB structure
	BIOS_PARAMATER_BLOCK_EXT	bpbExt;			//extended BPB info
	uint8_t						filler[448];		//needed to make struct 512 bytes
};


struct FILE_SYSTEM
{
	char name[VFS_MAX_DEVICE_NAME_LENGTH];
	struct BIOS_PARAMATER_BLOCK bpb;
	void(*Mount)      ();
	void(*UnMount)      ();
	void(*Open)       (DIRECTORY*file,const char* FileName);
	void(*Write)      (DIRECTORY* file, unsigned char* Buffer, unsigned int Length);
	void(*Read)       (DIRECTORY* file, unsigned char* Buffer);
	void(*Close)      (DIRECTORY*);
};



void vfs_initialize();
void vfs_open_file(DIRECTORY*file, const char* path);
void vfs_read_file(DIRECTORY* dir, char* buffer, int size);
void vfs_mount_fs(FILE_SYSTEM*pfs, char device);
char* vfs_get_device_name(int device);
void vfs_rewind(DIRECTORY* dir);
#endif