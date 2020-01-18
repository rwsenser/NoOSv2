#ifndef _STRING_H
#define _STRING_H 1
// atoi loosely from url:  eeksforgeeks.org/write-your-own-atoi 
// NoOS changes
// #include "stddef.h"

// prefix noosrt and _noosrt_
#include "NoOSrt.h"

int memcmp(const void* aptr, const void* bptr, size_t size);
// NoOS added
void* memcpy(void* aptr, const void* bptr, size_t size);
//
void* memset(void* bufptr, int val, size_t size);
size_t strlen(const char*);
char* strcat(char* d, const char* s);
char* strcpy(char* d, const char* s);
int strcmp(const char * s1, const char * s2);
char *strncat(char *dest, const char *src, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strstr(char *s1, const char *s2);
char *strchr(const char *s, int c);
int strncmp(const char * s1, const char * s2, size_t n);
char *ctos(char s[2], const char c);
int atoi(char* str);

// NoOS added
// these are noos uniqe
void noosrt_malloc_init(void *ptr, size_t size); 
size_t noosrt_malloc_get_free_size(); 
// these are general C
void *malloc(size_t );
void free(void *);

#endif

