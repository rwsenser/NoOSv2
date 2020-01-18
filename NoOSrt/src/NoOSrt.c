// kludge home
//

// prefix: noosrt and _noosrt_

#include <NoOSrt.h>
#include <io.h>
#include <tty.h>
// provide hybrid time method (really CPU cycls since start)
// now called ticks()
//
// from https://stackoverflow.com/questions/9887839/how-to-count-clock-cycles-with-rdtsc-in-gcc-x86
#if defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
	    unsigned long long int x;
	        __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
		    return x;
}

#elif defined(__x86_64__)

static __inline__ unsigned long long rdtsc(void)
{
	    unsigned hi, lo;
	        __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
		    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#endif
//
unsigned long long noosrt_ticks (void* _junk) {
	const int scaler = 1; // 14
	unsigned long long value =  rdtsc();
	value = value >> scaler; // 16K
        return value;
}
//
// get system time in seconds
//
unsigned long time (void* _junk) {
  // ports
  const unsigned char port70 = 0x70;
  const unsigned char port71 = 0x71;
  // CMOS address we like
  const unsigned char sec_addr = 0x00;
  const unsigned char min_addr = 0x02;
  const unsigned char hrs_addr = 0x04;
  const unsigned char stb_addr = 0x0b;
  // CMOS values we'll need
  unsigned char sec = 0;
  unsigned char sec2 = 0;
  unsigned char min = 0;
  unsigned char min2 = 0;
  unsigned char hrs = 0;
  unsigned char hrs2 = 0;
  unsigned char stb = 0;
  //
  int cnt;
  unsigned long ret = 0;
  // get the big 4
  // since the RTC can update current time at ANY time and we do not want
  // to try to impact interrupt handling, read twice and compare.
  // reread as needed.
  outb(port70, stb_addr);
  stb = inb(port71);
  cnt = 0;
  do {
    // suspect area, pass 1
    outb(port70, sec_addr);
    sec = inb(port71);
    outb(port70, min_addr);
    min = inb(port71);
    outb(port70, hrs_addr);
    hrs = inb(port71);
    // suspect area, pass 2
    outb(port70, sec_addr);
    sec2 = inb(port71);
    if (sec != sec2) { cnt++; continue;}
    outb(port70, min_addr);
    min2 = inb(port71);
    if (min != min2) { cnt++; continue;}
    outb(port70, hrs_addr);
    hrs2 = inb(port71);
    if (hrs != hrs2) { cnt++; continue;}
    break;
  } while (cnt < 4);
  if ((stb & 0x02) && (cnt < 4)) { // format we know and good data
    // unit is seconds
    ret += (hrs / 16 * 10 + hrs % 16) * 3600;
    ret += (min / 16 * 10 + min % 16) * 60;
    ret += sec / 16 * 10 + sec % 16;
  } else {
    ret = 9999999;
  }
  return ret;
}
// end of file
