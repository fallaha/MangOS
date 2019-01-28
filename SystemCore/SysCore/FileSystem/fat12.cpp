/*
 *	Ali Fallah (c) 97/10/20 
 *	complete  in   97/11/07
*/

#include "fat12.h"
#include <flopy.h>
#include <string.h>
#include <vfs.h>
#include "filesystem.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


struct FILE_SYSTEM FAT12_FS_DF;
uint8_t FAT[FS_SECTOR_SIZE* 2];

int get_dir_name(char* dir_name, const char *path){
	int size;
	memcpy(dir_name, path, size = strchr(path, '/') - path);
	return size;
}

/*
 *	Open File By Path
 */
void fat12_open_file(DIRECTORY*file,const char* path){
	char dir_name[50];
	int size = 0;
	DIRECTORY directory;
	// a:/folder/sub/ali.txt
	if (path[1] == ':'){ /* it's Absolute Addressing */
		
		if (!isalpha(path[0])) {
			/* Error Must be a..z char that determine to a device */
			file->flag = FS_FLAG_INVALID;
			return;
		}
		else file->device = FS_GET_DEVICE(path[0]);

		if (path[2] != '/') {
			/* Error in this place expecting '/' char */
			file->flag = FS_FLAG_INVALID;
			return ;
		}

		path += 3;
		
		/* Set Darawer Name */
		size = get_dir_name(dir_name, path);
		dir_name[size] = 0;
		path += size;
		path++; /* this is '/' charachter */

		fat12_get_directory_from_root(file , dir_name);
		/* The Address was the 1st subDirectory (e.g. C:/Folder/ ) */
		if (*path == 0)
			return;
		
		memcpy(&directory, file, sizeof(directory));

		while (1){
			size = get_dir_name(dir_name, path);
			dir_name[size] = 0;
			path += size;
			path++; /* this is '/' charachter */
			memcpy(file, &directory, sizeof(directory));
			fat12_get_directory(&directory,file, dir_name);
			
			if (directory.flag == FS_FLAG_INVALID) /* this directory dosn't exist or ... */
				return ;
			
			if (*path == 0){ /* The Address was the 1st subDirectory (e.g. C:/Folder/ ) */
				memcpy(file, &directory, sizeof(directory));
				return;
			}
		}

	}

	file->flag = FS_FLAG_INVALID;
	return ;
}

/*
 * Read One Sector From Hardware (e.g. HardDisk,Floppy,Flash ...)
 * This will complete!
 */
uint8_t * fat12_read_one_sector_from_device (uint32_t sector, char device){

	if (device == 0) /*a*/
		return flpydsk_read_sector(sector);

}

/*
 * Read one Sector from file
 */
void fat12_read_one_sector_file(DIRECTORY* file,unsigned char * buffer){

	int phys_sector = 31 + file->current_cluster;
	unsigned char* pb = fat12_read_one_sector_from_device(phys_sector, file->device); /* Pointer to Buffer */
	memcpy(buffer, pb, FS_SECTOR_SIZE);

	/* Calculate FAT  */
	
	uint32_t FAT_entry = file->current_cluster + (file->current_cluster / 2);  /* multiply by 1.5 */
	uint32_t FAT_phys_sector = 1 + (FAT_entry / FS_SECTOR_SIZE); /* add 1 for MBR sector! 
																	we have 9 sector for FAT and FS_SECTOR_SIZE
																	determine wich one of FAT Sector ?*/
	/* Read 2 Sector of Fat */
	pb = fat12_read_one_sector_from_device (FAT_phys_sector,file->device);
	memcpy(FAT, pb, FS_SECTOR_SIZE);

	pb = fat12_read_one_sector_from_device(FAT_phys_sector + 1, file->device);
	memcpy(FAT + FS_SECTOR_SIZE, pb, FS_SECTOR_SIZE);

	uint16_t next_cluster = FAT[FAT_entry + 1] << 8 | FAT[FAT_entry];

	if (file->current_cluster & 0x01)  /* if Odd */
		next_cluster >>= 4;
	else
		next_cluster &= 0x0FFF;

	if (next_cluster >= 0XFF8){ /* End of File */
		file->eof = 1;
		return;
	}
	
	if (next_cluster == 0){ /* File Corruption */
		file->eof = 1;
		return;
	}
	
	file->current_cluster = next_cluster;
	return;
}




/**
*	Helper function. Converts filename to DOS 8.3 file format
*/
void fat12_to_dos_filename(const char* filename,char* fname,unsigned int FNameLength) {

	unsigned int  i = 0;

	if (FNameLength > 11)
		return;

	if (!fname || !filename)
		return;

	//! set all characters in output name to spaces
	memset(fname, ' ', FNameLength);

	//! 8.3 filename
	for (i = 0; i < strlen(filename) - 1 && i < FNameLength; i++) {

		if (filename[i] == '.' || i == 8)
			break;

		//! capitalize character and copy it over (we dont handle LFN format)
		fname[i] = toupper(filename[i]);
	}

	//! add extension if needed
	if (filename[i] == '.') {

		//! note: cant just copy over-extension might not be 3 chars
		for (int k = 0; k<3; k++) {

			++i;
			if (filename[i])
				fname[8 + k] = filename[i];
		}
	}

	//! extension must be uppercase (we dont handle LFNs)
	for (i = 0; i < 3; i++)
		fname[8 + i] = toupper(fname[8 + i]);
}


/*
 * Get Directory Stracture
 * input    @ file   : pointer to file that return!
			@ drawer : drawer that we want find a dir in that
			@ name   : name of directory in drawer
 * Return Directory 
 */

void fat12_get_directory(DIRECTORY* file, DIRECTORY* drawer, char* name){

	unsigned char* buf;
	unsigned char sector[512];
	DIRECTORY_ENTRY* directory;

	//! get 8.3 directory name
	char DosFileName[11];
	fat12_to_dos_filename(name, DosFileName, 11);
	DosFileName[11] = 0;
	
	while (!drawer->eof) {

		fat12_read_one_sector_file(drawer, sector);
		directory = (DIRECTORY_ENTRY*)sector;

		/* 16 entry per sector */
		for (int entry = 0; entry < 16; entry++){

			char dir_name[11];
			memcpy(dir_name, directory->Filename, 11);
			dir_name[11] = 0;

			if (strcmp(DosFileName, dir_name) == 0) {
				/* Found it! */
				strcpy(file->name, name);
				file->first_cluster = directory->FirstCluster;
				file->current_cluster = directory->FirstCluster;
				file->length = directory->FileSize;
				file->offset_cluster = 0;
				file->eof = 0;

				/* set file type */
				if (directory->Attrib == 0x10)
					file->flag = FS_FLAG_DRAWER;
				else
					file->flag = FS_FLAG_FILE;

				/* return file */
				return;
			}

			/* Goto next Directory */
			directory++;
		}
	}

	/* Return File - File notFound*/
	file->flag = FS_FLAG_INVALID;
	return;
}


/*
 * Get Directory Stracture
 * input    @ drawer : drawer that we want find a dir in that
 * Return Directory
 */
void fat12_get_directory_from_root(DIRECTORY* file, char* name){

	unsigned char* buf;
	DIRECTORY_ENTRY* directory;

	//! get 8.3 directory name
	char DosFileName[11];
	fat12_to_dos_filename(name, DosFileName, 11);
	DosFileName[11] = 0;

	for (int i = 0; i < 14;i++) {
		buf = fat12_read_one_sector_from_device(19 + i, file->device);
		directory = (DIRECTORY_ENTRY*)buf;

		/* 16 entry per sector */
		for (int entry = 0; entry < 16; entry++){

			char dir_name[11];
			memcpy(dir_name, directory->Filename, 11);
			dir_name[11] = 0;

			if (strcmp(DosFileName, dir_name) == 0) {
				/* Found it! */
				strcpy(file->name, name);
				file->first_cluster = directory->FirstCluster;
				file->current_cluster = directory->FirstCluster;
				file->length = directory->FileSize;
				file->offset_cluster = 0;
				file->eof = 0;

				/* set file type */
				if (directory->Attrib == 0x10)
					file->flag = FS_FLAG_DRAWER;
				else
					file->flag = FS_FLAG_FILE;

				/* return */
				return;
			}

			/* Goto next Directory */
			directory++;
		}
	}

	/* Return File - File notFound*/
	file->flag = FS_FLAG_INVALID;
	return;
}

void fat12_UnMount(){

}



/*
 * Close a File
 */
void fat12_close_file(DIRECTORY* file){

	if (file)
		file->flag = FS_FLAG_INVALID;
}


