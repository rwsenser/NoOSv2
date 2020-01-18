//
// test.c:  changeable small test for NoOSrt functions -- intended for full OS use
//
//a

#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>

void ReadFromCMOS (unsigned char array [])
{
   unsigned char tvalue, index;
   tvalue = 0;
 
   for(index = 0; index < 128; index++)
   {
      _asm
      {
         cli             /* Disable interrupts*/
          mov al, index   /* Move index address*/
         /* since the 0x80 bit of al is not set, NMI is active */
         out 0x70,al     /* Copy address to CMOS register*/
         /* some kind of real delay here is probably best */
         in al,0x71      /* Fetch 1 byte to al*/
         sti             /* Enable interrupts*/
         mov tvalue,al
       }
       array[index] = tvalue;
   }
}
int main() {

  printf("Done!\n");
}
