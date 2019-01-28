/*
* Ali Fallah (C) 97/11/08
* Some Functions Need To Compelte Dont use Now
*/

#include <vfs.h>
#include <stdint.h>
#include <fat12.h>
#include <flopy.h>
#include <string.h>

struct FILE_SYSTEM* FILE_SYSTEMS[VFS_MAX_FILE_SYSTEM];
extern struct FILE_SYSTEM FAT12_FS_DF;

void vfs_open_file(DIRECTORY*file, const char* path){
	char device = 0; /* a */
	if (path){
		
		FILE_SYSTEMS[device]->Open(file,path);
		return ;
	}

	file->flag = 5;
	return ;
}

/*
 * Set Cursor of file on Top
*/
void vfs_rewind(DIRECTORY* dir){
	dir->eof = 0;
	dir->current_cluster = dir->first_cluster;
}

void vfs_read_file(DIRECTORY* dir, char* buffer,int size){
	int i = 0;
	char sector[512];
	
	while (size > 0 && !dir->eof){
		FILE_SYSTEMS[dir->device]->Read(dir, (unsigned char *)sector);
		if (size<512)
			memcpy(buffer + i * 512, sector, size%512);
		else
			memcpy(buffer+i*512, sector, 512);
		i ++;
		size -= 512;
	}
}

int vfsWriteFile(){
	return 1;
}

int vfsCloseFile(){
	return 1;
}

void vfs_mount_fs(FILE_SYSTEM*pfs,char device){
	FILE_SYSTEMS[0] = pfs;
}

int vfsUnMountFS(){
	return 1;
}

DIRECTORY* vfsOpenSubDir(char* name){
	DIRECTORY* doo;
	return doo;
}

void vfs_initialize(){
	fat12_default_device_init();
	
	//FILE_SYSTEMS[0]->UnMount = fat12_UnMount;
}

char* vfs_get_device_name(int device){
	return FILE_SYSTEMS[device]->name;
}

/*
* initialize default Floppy File system
* dont Touch this :)
* i don't know why this function don't work when place in fat12.cpp
*/
void fat12_default_device_init(){
	BOOT_SECTOR_STRUCT* btsect;
	btsect = (BOOT_SECTOR_STRUCT*)flpydsk_read_sector(0);
	memcpy(&FAT12_FS_DF.bpb, &btsect->bpb, sizeof(FAT12_FS_DF.bpb));
	memcpy(FAT12_FS_DF.name, "FAT 12", sizeof(FAT12_FS_DF.name));

	FAT12_FS_DF.Open = fat12_open_file;
	FAT12_FS_DF.Close = fat12_close_file;
	FAT12_FS_DF.Read = fat12_read_one_sector_file;
	FAT12_FS_DF.UnMount = fat12_UnMount;
	vfs_mount_fs(&FAT12_FS_DF, 0);

}
