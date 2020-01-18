//
// showmem.h: showmem application for NoOS
//


#define SHOWMEM_APP {"show-mem",showmem_run,"show/view RAM util"}

uint8_t __mem_getchar__() {
  uint8_t chr;
#ifdef NOOSRT
  do {
    chr =  normalmap[scan()]; // kinda like getchar() using busy waiting;
  } while (chr == 0);
  printf("%c", chr);
#else
  chr = getchar();
#endif
  return chr;
}

void __mem_chompcr__() {
#ifdef NOOSRT
  __mem_getchar__();  // yum yum
  printf("\n");
#else
  __mem_getchar__();  // yum yum
#endif
  return;
}

long __mem_getHex__() {
  long val = 0;
  uint8_t chr;
  for (int i=0; i <= 8; i++) {
    chr = __mem_getchar__();
    if (chr == ' ') continue;
    if (chr < '0' || chr > 'f') break;
    val *= 16;
    if (chr <= '9') val += (chr - '0');
    else val += 10 + (chr - 'a');
  }
  return val;
}

int showmem_run(int argc, char** argv) {

  long address;
  int width = 2;
  static int data = 32;
  int size;
  uint8_t cmd;
  uint8_t value;
  uint8_t *ptr;
  int error;
  address = (long) &data;
#ifndef NOOSRT
  printf("\nmode: not NOOSRT\n");
#endif
  while (1) {
    printf("\nCommands: a(ddress), d(isplay), l(ist), p(robe), s(et), w(idth), x(it)\n");
    printf(".");
    cmd = __mem_getchar__();
    __mem_chompcr__(); // eat enter key
    if (cmd == 'a') {
      printf("hex: ");
      address = __mem_getHex__();
      printf("new address: %x\n", address);
    } else if (cmd == 'd') {
      printf("address: %x\n", address);
      printf("width: %x\n", width);
      ptr = (uint8_t *) address;
      for (int i = 0; i < width; i++) {
        if (i > 0) printf(" ");
        value = *ptr++;
        if (value < 17) printf("0"); // my kingdome for a leading zero (%02x is not supported)
        printf("%x", value);  
      }
      printf("\n");
    } else if (cmd == 'l') {
      printf("address: %x\n", address);
      printf("width: %x\n", width);
    } else if (cmd == 'p') {
      printf("address: %x\n", address);
      printf("width: %x\n", width);
      ptr = (uint8_t *) address;
      value = *ptr;
      error = FALSE;
      *ptr = 0;
      if (*ptr != 0) error = TRUE;
      *ptr = 0xff;
      if (*ptr != 0xff) error = TRUE;
      *ptr = value;
      if (error) {
        printf("probe failed!\n");
      } else {
        printf("probe OK!\n");
      } 
    } else if (cmd == 's') {
      printf("address: %x\n", address);
      printf("1 byte hex: ");
      value = __mem_getHex__();
      ptr = (uint8_t *) address;
      *ptr = value;
      printf("set value: %x\n", *ptr);
    } else if (cmd == 'x') {
      break;
    } else if (cmd == 'w') {
      printf("hex: ");
      width = __mem_getHex__();
      if (width < 1) width = 2;
      if (width > 16) width = 16;
      printf("new width: %x\n", width);
    }
  }
  printf("exit.\n");
  return 0;
}
