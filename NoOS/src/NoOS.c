//
// NoOS frontend for EEFS
//
// NoOSrst.c  
// supplies minor run-time routines that work in normal and baremetal mode
//

// prefix noos and _noos_
#ifdef  NOOSRT
// this include is a "bit dirty" but it allows this code to be straight forward
#include "tty.h"
#else
#include <stdio.h>
#endif

void splash(char msg[]) {
  printf("\n");
  printf("  NN     NN          OOOOOOOOO  SSSSSSSS\n");
  printf("  NNN    NN          OO     OO  SS     S\n");
  printf("  NNNN   NN  oooooo  OO     OO  SS      \n");
  printf("  NN NN  NN  o    o  OO     OO  SSSSSSSS\n");
  printf("  NN  NN NN  o    o  OO     OO        SS\n");
  printf("  NN   NNNN  o    o  OO     OO  S     SS\n");
  printf("  NN     NN  oooooo  OOOOOOOOO  SSSSSSSS\n");
  printf("\n");
  printf("                        (version: %s)\n", msg);
  return;
}


