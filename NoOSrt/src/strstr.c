#include "../include/string.h"

char *strstr(const char *s1, const char *s2)
{
    size_t n = strlen(s2);
    char *s3 = (char *) s1; // kludge
    while (*s3)
        if (!memcmp(s3++, s2, n))
            return s3 - 1;
    return 0;
}

