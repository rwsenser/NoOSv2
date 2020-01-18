#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"

int ltoa1(int value, char* ptr, int size) {
  // return ltoa2(((long long) value), ptr, size);
	const int base = 10;
	char dBuff[32] = { 0 };
	char hBuff[32] = { 0 };
	char adigit[2] = {0, 0};
	char sign[2] = { 0, 0};
	int len = 0;
	// process the sign, if present
	if (value < 0) {
	  sign[0] =  '-';
	  len++;
	  value *= -1; // slow but effective
	}
	// process digits, note reordering
	do {
   	const char digit = (char) (value % base);
	  adigit[0]  = '0' + digit;
	  strcpy(hBuff,adigit);
	  strcat(hBuff,dBuff);
	  strcpy(dBuff,hBuff); // kinda brute force...
	  value = value / base;
    len++;
	} while (value && (len < sizeof(dBuff) -1 ) );
	strcpy(hBuff,sign);
	strcat(hBuff,dBuff);
	strncpy(ptr,hBuff,size);
	ptr[size-1] = '\0';    // just in case of Overflow
	return len;
}

int ltoa2(long long value, char* ptr, int size) {
	const long base = 10;
	char dBuff[32] = { 0 };
	char hBuff[32] = { 0 };
	char adigit[2] = {0, 0};
	char sign[2] = { 0, 0};
	int len = 0;
	// process the sign, if present
	if (value < 0) {
	  sign[0] =  '-';
	  len++;
	  value *= -1; // slow but effective
	}
	// process digits, note reordering
	do {
	  int value2 = ((int) value) & 0xffff;
   	  const char digit = (char) (value2 % base);
	  adigit[0]  = '0' + digit;
	  strcpy(hBuff,adigit);
	  strcat(hBuff,dBuff);
	  strcpy(dBuff,hBuff); // kinda brute force...
	  // value = value / base;
	  value = (long long) (  ((long) value) / base );
	  // double valued = (double) value;
	  // valued /= base;
	  // value = (long long ) valued;
  	  len++;
	} while (value && (len < sizeof(dBuff) -1 ) );
	strcpy(hBuff,sign);
	strcat(hBuff,dBuff);
	strncpy(ptr,hBuff,size);
	ptr[size-1] = '\0';    // just in case of Overflow
	return len;
}
 
