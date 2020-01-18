
//
// tty.c
// based on version in book:
//
// prefix noosrt and _noosrt_
//
#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"
#include "tty.h"
#include "string.h"

// this is .c and not .cpp
#define FALSE 0
#define TRUE 1

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xb8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_scroll()
{
    int i;
    for (i = 0; i < VGA_HEIGHT; i++){
	int m;
        for (m = 0; m < VGA_WIDTH; m++){
            terminal_buffer[i * VGA_WIDTH + m] = terminal_buffer[(i + 1) * VGA_WIDTH + m];
        }
	terminal_row--;
    }
    terminal_row = VGA_HEIGHT - 1;
}

static inline uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
static inline uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	size_t y;
	for ( y = 0; y < VGA_HEIGHT; y++ ) {
		size_t x;
		for ( x = 0; x < VGA_WIDTH; x++ ) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c) {
	if (c == '\n' || c == '\r') {
		terminal_column = 0;
		terminal_row++;
		if (terminal_row == VGA_HEIGHT)
			terminal_scroll();
		return;
	} else if (c == '\t') {
		terminal_column += 4;
		return;
	} else if (c == '\b') {
		terminal_putentryat(' ', terminal_color, terminal_column--, terminal_row);
		terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
		return;
	}

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if ( ++terminal_column == VGA_WIDTH ) {
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT ) {
			terminal_row = 0;
		}
	}
}
 
void terminal_write(const char* data, size_t size) {
	size_t i;
	for ( i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

int putchar(int ic) {
	char c = (char)ic;
	terminal_write(&c, sizeof(c));
	return ic;
}

static void print(const char* data, size_t data_length) {
	size_t i;
	for ( i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

int ltoa1(int value, char* ptr, int size, int base) {
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
    if (digit < 10) {
	    adigit[0]  = '0' + digit;
    } else { // assume hexa
     adigit[0] = 'a' + (digit - 10);
    }
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

// loosely based on URL: https://www.techiedelight.com/perform-division-two-numbers-without-using-division-operator/
// brute force div by repeated subtraction
long long _noosrt_posdiv_(long long x, long long y, long long* remainder) {  // pronounced: SSLLOOWW!! :)
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

// add int digit, converted to char, to end of string
int _noosrt_insert_char_(char* ptr, int digit, unsigned int size) {
  char quack[2] = {'\0','\0'};
  unsigned int len = strlen(ptr);
  if (len+1 < size) {
    quack[0] = digit + '0';
    strcat(ptr,quack);
    len++;
  }
  return len;
}

// speed up for _noosrt_posdiv_ by using larger denominators
int _noosrt_ltoa2_div10(long long value, char* ptr, unsigned int size) {
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
    for (int i=0; i < cnt; i++) { // could have used pow(...) but baremetal...
      factor *= divby;
    }
    // printf("DEBUG factor cnt: %d\n", cnt);
    result = _noosrt_posdiv_(value, factor, &remainder);
    if (result != 0) {
       skip_all = FALSE;
    }
    if (!skip_all) {
      len = _noosrt_insert_char_(ptr, result, size);
    }
    if (result > 0) {
      value = remainder;
    }
    cnt--;
  }
 
  len = _noosrt_insert_char_(ptr, remainder, size);
  // printf("DEBUG len: %d\n", len);

  return len;
}

int _noosrt_ltoa2(long long value, char* ptr, int size) {
  // use normal function if we can
  if (value >= 0 && value <= 0x7ffffff) {
    return ltoa1(((int) value), ptr, size, 10);
  }
  // use specialized _noosrt_ltoa2 ...
  int len;
  len = _noosrt_ltoa2_div10(value, ptr, size);
#if 0 
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
  // valid width 1 to 18 digits, apprx
  // feeling the need for speed, let's be smarter
  // we know that when the base is 10, which will be common, we can use powers of 10
	do {
    long long digit;
	  // this line caused issues with long long divide when baremetal: value = value / base;
    value = _noosrt_posdiv_(value, base, &digit); // SSLLOOWW!!
    // printf("DEBUGz %d int: %d digit: %d, str: %s\n", len,((int) value), digit, dBuff);
	  adigit[0]  = '0' + ((int) digit);
	  strcpy(hBuff,adigit);
	  strcat(hBuff,dBuff);
	  strcpy(dBuff,hBuff); // kinda brute force...
  	len++;
	} while (value && (len < sizeof(dBuff) -1 ) );
	strcpy(hBuff,sign);
	strcat(hBuff,dBuff);
	strncpy(ptr,hBuff,size);
#endif

	ptr[len-1] = '\0';    // just in case of Overflow
	return len;
}
 
int printf(const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
	size_t amount;
	int rejected_bad_specifier = 0;
 
	while ( *format != '\0' ) {
		if ( *format != '%' ) {
		    print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format;
 
		if ( *(++format) == '%' )
			goto print_c;
 
		if ( rejected_bad_specifier ) {
		    incomprehensible_conversion:
			rejected_bad_specifier = 1;
			format = format_begun_at;
			goto print_c;
		}
 
		if ( *format == 'c' ) {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		// NoOS addition
		} else if ( *format == 'd' || *format == 'x' ) { // int
      int base = 10;
      if (*format == 'x') base = 16;
			format++;
			int value = (int) va_arg(parameters, int);
			char lBuff[32] = { 0 };
			int len = ltoa1(value,lBuff,sizeof(lBuff),base);
			print(lBuff, len);
		} else if ( *format == 'l' ) { // long, note long long  support later ....
			format++;
      // this is not right ...
			if (*format == 'd') {
				format++;
				long long value = (long long) va_arg(parameters, long long);
				char lBuff[32] = { 0 };
				int len = _noosrt_ltoa2(value,lBuff,sizeof(lBuff));
				print(lBuff, len);
			}	
		} else if ( *format == 's' ) {
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		} else {
			goto incomprehensible_conversion;
		}
	}
 
	va_end(parameters);
 
	return written;
}

int get_terminal_row(void)
{
	return terminal_row;
}

int get_terminal_col(void)
{
	return terminal_column;
}
