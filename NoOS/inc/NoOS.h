//
// NoOS frontend for EEFS
//
// NoOSrst.h now containts baremetal support...
//

// prefix noos and _noosrt_

#ifndef NoOS_H
#define NoOS_H

void splash(char msg[]);

// there map common (not all) eefs api calls to fopen, flcose, etc
#define fopen(filename, mode)			EEFS_Open(filename, mode) // won't work
#define fclose(filename)                        EEFS_Close(filename) // won't work
#define fread(ptr, size, nmemb, FILE)           EEFS_Read(FILE, ptr, size)
#define fwrite(ptr, size, nmemb, FILE)		EEFS_Write(FILE, ptr, size)

#endif

