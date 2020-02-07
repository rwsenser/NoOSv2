//
// NoOSfs.c for NoOS, uses eefs heavily 
//
// global namespace prefix: noosfs_
// local namespace prefix: _noosfs_
//
#ifdef NOOSRT
#include "NoOSrt.h"
#include "io.h"
#include "tty.h"
#include "kbd.h"
#include "string.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif
#include "NoOS.h"
#include "common_types.h"
#include "eefs_filesys.h"
#include "eefs_fileapi.h"


// eefs fields
#define mySize 5000
unsigned char myBuffer[mySize];
typedef struct {
	 void *BaseAddress;
	 EEFS_FileAllocationTable_t *FileAllocationTable;
} FileSystem_t;

static FileSystem_t FileSystem;
// int32 status;
uint32 EEFSMemoryAddress;
static char DeviceName[32] = "/EEDEV1";
static char eefs_mountpoint[32] = "/eebank1";
// char versionFile [16] = "version";
char eefs_filename[128];
char tempname[128];
// int32 fd;
EEFS_DirectoryDescriptor_t *DirDescriptor = NULL;
EEFS_DirectoryEntry_t *DirEntry = NULL;
EEFS_Stat_t StatBuffer;
int total_used_space;
int total_free_space;
boolean ok = FALSE;
//

int noosfs_init () {
   int32 status;
   int ret = -1;
	//
	// allocate mem for file system
	//
	FileSystem.BaseAddress = (void *) myBuffer; 
	if (FileSystem.BaseAddress <= 0) {
	   printf("malloc failed!\n");
	} else {
	  //
	  // init file system field
	  //
	  memset(FileSystem.BaseAddress, 0,  mySize);
	  FileSystem.FileAllocationTable = FileSystem.BaseAddress;
	  FileSystem.FileAllocationTable->Header.Crc = 0;
	  FileSystem.FileAllocationTable->Header.Magic = EEFS_FILESYS_MAGIC;
	  FileSystem.FileAllocationTable->Header.Version = 1;
	  FileSystem.FileAllocationTable->Header.FreeMemoryOffset = sizeof(EEFS_FileAllocationTable_t);
	  FileSystem.FileAllocationTable->Header.FreeMemorySize = (mySize-sizeof(EEFS_FileAllocationTable_t));
	  FileSystem.FileAllocationTable->Header.NumberOfFiles = 0;
	  // printf("FileAlloc Tab Size: %d\n",sizeof(EEFS_FileAllocationTable_t));
	  // printf("File FreeMemorySize: %d\n",FileSystem.FileAllocationTable->Header.FreeMemorySize);
	  //
	  // actual init 
	  //
	  EEFSMemoryAddress = (uint32)FileSystem.BaseAddress;
	  status = EEFS_InitFS(DeviceName, EEFSMemoryAddress);
	  if (status !=0) {
	    printf("filesystem init failed!(%d)\n",status);
	  } else {
	    printf("NoOS File System(Device Name: %s, Mount Point: %s) ", DeviceName, eefs_mountpoint);
      ret = 0;
    }
  }
  return ret;
}
int noosfs_mount() {
  int32 status;
  int ret = -1;
	status = EEFS_Mount(DeviceName,eefs_mountpoint);
	if (status != 0) {
	  printf("filesystem mount failed!(%d)\n", status);
  } else {
	  printf("Mount OK!\n");
    ret = 0;
  }
  return ret;
}
int noosfs_create(char filename[]) {
  int32 fd;
	strcpy(tempname, eefs_mountpoint);
	strcat(tempname, "/");
	strcat(tempname, filename);
  fd = EEFS_Creat(tempname, 0);
  if (fd < 0) {
    printf("create failed!(%d)\n",fd);
  }
  return fd;
}
int noosfs_open(char filename[]) {
  int32 fd;
	strcpy(tempname, eefs_mountpoint);
	strcat(tempname, "/");
	strcat(tempname, filename);
  fd = EEFS_Open(tempname,O_RDWR);  // was 0
  if (fd < 0) {
    printf("open failed!(%d)\n",fd);
  }
  return fd;
}
int noosfs_read(int32 fd, char buffer[], size_t buffer_size) {
  int32 status;
	status = EEFS_Read(fd,buffer,buffer_size);
	// printf("Read Status: %d\n", status);		      
  return status;
}
int noosfs_write(int32 fd, char buffer[], size_t buffer_size) {
  int32 status;
	status = EEFS_Write(fd,buffer,buffer_size);
	// printf("Write Status: %d\n", status);
  return status;
}
int noosfs_close(int32 fd) {
  int32 status;
	status = EEFS_Close(fd);
	// printf("Close Status: %d\n", status);
  return status;
}
int noosfs_list(char prefix[]) {
  int32 status = 0;
  DirDescriptor = EEFS_OpenDir(eefs_mountpoint);
  if (DirDescriptor != NULL) {
    printf("free memory size: %d\n\n",FileSystem.FileAllocationTable->Header.FreeMemorySize);
    int total_used_space = 0;
    int total_free_space = 0;
    while ( (DirEntry = EEFS_ReadDir(DirDescriptor)) != NULL) {
      if (DirEntry->InUse != 0) {
        printf("%s%s",prefix,DirEntry->Filename);
      }
      strcpy(eefs_filename, eefs_mountpoint);
      strcat(eefs_filename,"/");
      strcat(eefs_filename,DirEntry->Filename);
      status = EEFS_Stat(eefs_filename, &StatBuffer);
      if (status == 0) {
         int size = StatBuffer.FileSize;
         int max = DirEntry -> MaxFileSize;
         total_used_space += size;
         total_free_space += (max - size);
         printf(" (size: %d max: %d)\n", size, max);
      } else {
        printf("\n");
      }
    }
    printf("\n");
    printf("in-file used: %d\n", total_used_space);
    printf("in-file free: %d\n", total_free_space);
    status = EEFS_CloseDir(DirDescriptor);
    if (status != 0) {
       printf("Close Dir failed!\n");
    } 
  } else {
    printf("Dir Open failed!\n");
    status = -1;
  }
  return status;
}
int noosfs_delete(char filename[]) {
  int32 status;
	strcpy(tempname, eefs_mountpoint);
	strcat(tempname, "/");
	strcat(tempname, filename);
  status =  EEFS_Remove(tempname);
  return status;
}
/* end of module */
