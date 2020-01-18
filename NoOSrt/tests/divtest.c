//
// divtest.c
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

long long __noosrt_posdiv__(long long x, long long y, long long* remainder) {  // pronounced: SSLLOOWW!! :)
  long long quotent = 0;
  if (y != 0) {
    while (x >= y) {
      x -= y;
      quotent++;
    }
  }
  *remainder = x;
  return quotent;
}

int __noosrt_insert_char__(char* ptr, int digit, unsigned int size) {
  char quack[2] = {'\0','\0'};
  unsigned int len = strlen(ptr);
  if (len+1 < size) {
    quack[0] = digit + '0';
    strcat(ptr,quack);
    len++;
  }
  return len;
}

int __noosrt_ltoa2_div10__(long long value, char* ptr, unsigned int size) {
  long long result;
  long long  remainder = 0;
  const unsigned int max = 18;
  const unsigned int divby = 10;
  unsigned int cnt;
	long long factor;
  int skip_all = TRUE;
  int len = 0;
  cnt = max;
  *ptr = '\0';

  // now we count down and process 1 digit at a time, left to right
  while (cnt >= 1) {
    factor = 1;
    for (int i=0; i < cnt; i++) { // could have been pow(...)
      factor *= divby;
    }
    result = __noosrt_posdiv__(value, factor, &remainder);
    // printf("factor:: value: %ld, base: %ld, result: %ld, remainder: %ld\n", value, factor, result, remainder);
    // printf(">> %ld\n", result);
    if (result != 0) {
       skip_all = FALSE;
    }
    if (!skip_all) {
			len = __noosrt_insert_char__(ptr, result, size);
    }
    if (result > 0) {
      value = remainder;
    } 
    cnt--;
  } 
  // printf(">> %ld\n", remainder);
	len = __noosrt_insert_char__(ptr, remainder, size);

  return len;
}

int main() {
  long long value = 123456789012;
  char buffer[32];
  int len;
  len = __noosrt_ltoa2_div10__(value, buffer, sizeof(buffer));
  printf("len: %d\n", len);
  printf("buffer: %s\n", buffer);
  return 0;
}
