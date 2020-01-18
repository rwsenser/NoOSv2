#include "../include/string.h"

// this is a kludge   NoOS
//
char* strncpy(char* dest, const char* src, size_t n) {
    char* tmp = dest;

    memset(tmp,0,n); // clear to \0 chars

    while ((dest-tmp < n) && ((*dest++ = *src++) != 0)) ; // ugly!

    return tmp;
}

