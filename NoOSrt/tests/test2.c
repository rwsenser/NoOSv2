//
// test2.c:  changeable small test for NoOSrt functions -- intended for full OS use
//
//
#include <stdio.h>
int ltoa2(long, char *, int);
int ltoa1(int, char *, int);

int main() {
  int c;
  do {
    c = getchar();
    printf("%c %d %x\n", c, c, c);
  } while (c != 'x');
}
