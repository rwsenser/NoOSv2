//
// showcmos.h: showcmos application for NoOS
//

#define SHOWCMOS_APP {"show-cmos",showcmos_run,"show CMOS date data"}

int showcmos_run(int argc, char** argv) {
#ifdef NOOSRT
  const unsigned char port70 = 0x70;
  const unsigned char port71 = 0x71;
  // address we like
  const unsigned char sec_addr = 0x00;
  const unsigned char min_addr = 0x02;
  const unsigned char hrs_addr = 0x04;
  const unsigned char sta_addr = 0x0a;
  const unsigned char stb_addr = 0x0b;
  unsigned char sec = 0;
  unsigned char min = 0;
  unsigned char hrs = 0;
  unsigned char sta = 0;
  unsigned char stb = 0;
  // get the big 3
  outb(port70, sec_addr);
  sec = inb(port71);
  outb(port70, min_addr);
  min = inb(port71);
  outb(port70, hrs_addr);
  hrs = inb(port71);
  outb(port70, sta_addr);
  sta = inb(port71);
  outb(port70, stb_addr);
  stb = inb(port71);
  printf("RTC values: %x:%x:%x\n", hrs, min, sec);
  printf("status values: %x %x\n", sta, stb);
#else
  printf("\nsupported only in NOOSRT mode!\n");
#endif
  return 0;
}

