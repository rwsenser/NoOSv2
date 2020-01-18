//
// NoOSrt
//
// this helps stand in for stdio.h, stdlib.h and string.h
//

// prefixes: noosrt and _noosrt_

#ifndef NoOSRT_H
#define NoOSRT_H

#define size_t int
#define time_t long

// kludges for now
#define BIG_ENDIAN 2
#define LITTLE_ENDIAN 1

#define O_RDONLY 1
#define O_WRONLY 2
#define O_RDWR 3
#define O_TRUNC 4
#define O_CREAT 5
#define O_ACCMODE 6

#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

#define FILE int32

// this is dangerous but needed for now 
// this is 'standard' so use the C API name
unsigned long time(void* _junk); // returns seconds
// this is not standard, so prefix with noosrt 
unsigned long long noosrt_ticks(void* _junk); // returns CPU ticks


#endif

