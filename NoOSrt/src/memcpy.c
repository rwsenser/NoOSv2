//
// memcpy.c
//
// created as part of NoOSrt
//
#include "../include/string.h"

void* memcpy(void* bufptr, const void* srcptr, size_t size)
{
	unsigned char* buf = (unsigned char*) bufptr;
	unsigned char* src = (unsigned char*) srcptr;
	size_t i;
	for ( i = 0; i < size; i++ )
		buf[i] = src[i];
	return bufptr;
}

