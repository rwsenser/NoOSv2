//
// test.c:  changeable small test for NoOSrt functions -- intended for full OS use
//
//a

#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>

int main() {

  int src = 1;
  int dst;   

  asm ("nop \n\t"
      "mov %1, %0\n\t"
      "add $1, %0\n\t"
      "nop" 
      : "=r" (dst) 
      : "r" (src));

  printf("%d\n", dst);

  printf("Done!\n");
}
