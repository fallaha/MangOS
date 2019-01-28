#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#define FS_SECTOR_SIZE 512

#define FS_FLAG_FILE 0 /* this's a file */
#define FS_FLAG_DRAWER 1 /* this's a file */
#define FS_FLAG_INVALID 2 /* this's a file */
#define FS_GET_DEVICE(a) a-'a'
#endif