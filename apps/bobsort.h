//
// bobsort.h: bobsort application for NoOS
//

#define BOBSORT_APP {"bobsort",bobsort_run,"bobsort, args <size> <-a>"}
// large size fails in actual baremetal execution?  module too large??
// #define max_sort_size 500000
#define max_sort_size 50000
int sortdata[max_sort_size];

int bobsort_run(int argc, char** argv) {
  printf("bobsort start\n");
  int size = 500;
  int ascending = FALSE;
  int sorted = FALSE;
  int hold;
  int swaps = 0;
#if 1
  if(argc > 1) {
    size = atoi(argv[1]);
  }
  if (argc > 2) {
    if (strcmp(argv[2], "-a") == 0) {
      ascending = TRUE;
   }
  }
#endif
  if (size > max_sort_size) {
    printf("sort aborted, size too large.\n");
    return 0;
  }
  printf("size: %d; ascending: %d\n", size, ascending);
  for (int i=0; i < size; i++) {
    if (ascending) {
      sortdata[i] = i;
    } else {
      sortdata[i] = size - i - 1;
    }
  }
  while (!sorted) {
    sorted = TRUE;
    for (int i=0; i < (size-1); i++) {
      if (sortdata[i] > sortdata[i+1]) {
        sorted = FALSE;
        hold = sortdata[i];
        sortdata[i] = sortdata[i+1];
        sortdata[i+1] = hold;
        swaps++;
      }
    }
  }
  printf("bobsort end with %d swaps\n", swaps);
  return swaps;
}

