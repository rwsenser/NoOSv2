//
// primes.h: primes application for NoOS
//

#define PRIMES_APP {"primes",primes_run,"find primes, args <up_to> <-nv>"}

int primes_run(int argc, char** argv) {
  const char nv[] = "-nv";
  int top = 100;
  boolean verbose = TRUE;
  if (argc > 1) {
    if (strcmp(argv[1],nv) == 0) {
      verbose = FALSE;
    } else {
      top = atoi(argv[1]);
    }
  }
  if (argc > 2) {
    if (strcmp(argv[2],nv) == 0) {
      verbose = FALSE;
    }
  }
  long cnt = 0;
  for (int i=3; i <= top; i +=2 ) {
    boolean found = TRUE;
    for (int j=3; j < (i/2); j++) {
      if ((i%j) == 0) {
        found = FALSE;
        break;
      }
    }
    if (found) {
           cnt++;
           if (verbose) {
                   printf("%d\n", i);
           }
    }
  }
  printf("primes to %d\n", top);
  printf("primes found: %d", cnt);
  return cnt;
}
