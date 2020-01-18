//
// malloctst.h: simple malloc use to testNoOS
//


#define MALLOCTST_APP {"malloctst",malloctst_run,"malloc test util, argrs <size>"}

int malloctst_run(int argc, char** argv) {

#define __MALLOC__MEM_SIZE 1024 * 4
  static unsigned char mem[__MALLOC__MEM_SIZE];
  int size = 128;
  void *ptr;
#ifndef NOOSRT
  printf("\nmode: not NOOSRT\n");
#endif
  if (argc > 1) {
    size = atoi(argv[1]);
  }
  printf("size: %d\n", size);
  if (size > __MALLOC__MEM_SIZE) {
    printf("%d > %d, too big error\n", size, __MALLOC__MEM_SIZE);
  } else {
    ptr = malloc(size);
    if (ptr == NULL) {
      printf("malloc of %d failed!\n", size);
    } else {
      printf("malloc of %d OK!\n", size);
      // test
      unsigned char *c_ptr = (unsigned char *) ptr;
      for (int i=0; i < size; i++) {
        c_ptr[i] = 0x77;
      }
      int barf = FALSE;
      for (int i=0; i < size; i++) {
        if (c_ptr[i]  != 0x77) {
          printf("malloc verify error at offset %d\n", i);
          barf = TRUE;
        }
      }
      if (barf) {
        printf("Malloc verify failed!\n");
      } else {
        printf("Malloc verify OK!\n");
      }
      free(ptr);
    }
  }
  printf("exit.\n");
  return 0;
}
