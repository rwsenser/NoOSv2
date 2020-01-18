//
// test.c:  changeable small test for NoOSrt functions -- intended for full OS use
//
//
#include <stdio.h>
int ltoa2(long, char *, int);
int ltoa1(int, char *, int);

int main() {
	// long n = 12345678901234567890;
	int n = 7777777777;
	char buffer[64] = { '\0' };
	ltoa1(n, buffer, sizeof(buffer));
	// printf("n: %ld, a: %s\n", n, buffer);
	printf("n: %d, a: %s\n", n, buffer);
  printf("size of int: %d\n", sizeof(int));
}
