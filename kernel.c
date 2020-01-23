//
// kernel.c for NoOS, with eefs calls
//
//  loosely based on kernel.c from book:  
// 
// to-dos: (2019-12-29):
// 1)  Clean up Makefile
// 2)  Add a open source license
// 3)  Write version 1 document
// 1d)  Done - Keyboard: fix cursor (on baremetal, turn off in start.asm)
// 2d)  Done - Keyboard edits: ^c for start over 
// 3d)  Done - Rationalize f<op>/eefs file support
// 4d)  Done - Comment malloc with mem leak comments
// d5)  DOne - Finish time to read twice
//     Future:
// 1f) Enhance malloc to reuse storage (simple table...)
// 2f) Enhance keyboard edit to have backspace and pre-line repeat
// 3f) Make configurable, things like malloc area, file space size, stack size, etc.
// 4f) Add a reboot command
//
const int version = 2;
char build[16] = "?????";
char dynamic_build[16] = "alpha01";
const char time_msg[] = "current CPU time (secs): %d\n";
const char dur_msg[] = ("\nCPU duration secs: %d\n");
const int max_programs = 20;
#define MAX_ARGS  8
#define _NOOSRT_MEM_SIZE (1024 * 4)

//
#ifdef NOOSRT
#include "NoOSrt.h"
#include "io.h"
#include "tty.h"
#include "kbd.h"
#include "string.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif
#include "NoOS.h"
#include "NoOSfs.h"
#include "common_types.h"
// #include "eefs_filesys.h"
// #include "eefs_fileapi.h"


#ifdef NOOSRT
static unsigned char noosrt_mem[_NOOSRT_MEM_SIZE];
void kernel_early(void) {
	terminal_initialize();
}
#else
void printprompt() {
	printf(">");
}
#define uint8_t unsigned char
#define scan() getchar()
#endif
// NoOS change ... make an explicit buffer for buff to point to
static char command[1024];

// eefs fields
char buildFileName [16] = "build";
char eefs_filename[128];
char tempname[128];
int32 fd;
// boolean ok = FALSE;
//
// NoOS apps here  <-------  AAPPSS HHEERREE
// "external apps"
//
#include "apps/apps.h"

// argv handling
char* largv[MAX_ARGS];
int largc;

void process_args(char* line) {
	int len = strlen(line);
	char* end = line + len;
	for (largc = 0; largc < MAX_ARGS; largc++) {
	  largv[largc]=line;
	  for (;line < end; line++) {
	    if (*line == ' '  || *line == '\0') {
	      *line = '\0';
	      line++;
	      break; /* inner for, end of arugument */
	    }
	  }
	  if (line >= end) {
	    break;  /* outter for exit */
	  }  
	}
	if (largc < MAX_ARGS) largc++;  // this is the count, so up by 1
	return;
}	

int say_goodnight_dick(int argc, char** argv) {
	printf("Goodbye!\n\n");
#ifdef NOOSRT
#else
	exit(0);
#endif
	return 0;
}
int help( int argc, char** argv) {
  printf("good help can be hard to come by...\n");
  printf("  . consider 'exit' or 'quit' to exit\n");
  printf("  . enter 'show-pgms' to see installed programs\n");
  printf("  . c^ (control 'c') to renter a command line (baremetal only)\n");
  printf("  . other control codes and chars are not (yet) implemented (baremetal only)\n");
  printf("\n");
  return 0;
}

//
// internal apps
//
int showargs_run(int argc, char** argv) {
	printf("uses printf() and \%d\n");
	printf("\nargc: %d\n", argc);
	for (int i=0; i < argc; i++) {
	  printf("arg %d: %s\n", i, argv[i]);
	}
	return argc;
}

int showint_run(int argc, char** argv) {
	printf("uses atoi()\n");
	printf("\nargc: %d\n", argc);
	for (int i=0; i < argc; i++) {
	  printf("arg %d: %s = %d\n", i, argv[i], atoi(argv[i]));
	}
	return argc;
}

int showld_run(int argc, char** argv) {
	printf("\nargc: %d\n", argc);
	for (int i=1; i < argc; i++) {
    long long val = 0;
    for (int j=0; j < atoi(argv[i]); j++) {
      val = val * 10 + 7;
    } 
	  printf("arg %d: %s = %ld\n", i, argv[i], val);
	}
	return argc;
}

int oddtest_run(int argc, char** argv) {
  long long value = 7777777777;
  printf("value with ld is %ld\n", value);
  return 0;
}

int loop19_run(int argc, char** argv) {
	long long value = 0;
	int i = 0;
	for (i=0; i < 19; i++) {
		value = value * 10 + 7;
		printf("%d, %d, %ld\n", i+1, ((int) value), value);
	}
	return i;
}
// end of internal apps

// command table
struct command_table {
	char name[17];
	int (*func) (int, char **);
  char comment[65];
};

struct command_table comd_tab[] = {
				    {"loop19", loop19_run, "list range 7 to 7(19) via %ld"},
				    {"show-args", showargs_run, "show arguments as %s, args <a1, a2, ...>"},
				    {"show-int", showint_run, "show arguments as %d via atoi(), args <n1, n2 ...>"},
				    {"show-ld", showld_run, "show generated numbers to arg digits, args <n1>"},
				    {"oddtest", oddtest_run, "test frame for odd tests"},
// 
// include to link apps into command table
//
#include "apps/appsnames.h"
				    {"*", NULL}
};

int showpgms(int argc, char **argv) {
  char space_buffer[18];
	for (int j=0;j<max_programs;j++) {
  	if (comd_tab[j].func == NULL) {
	  	break;
	  }
    space_buffer[0] = '\0';
    int cnt = 16 - strlen(comd_tab[j].name);
    for (int i=0;i < cnt;i++) {
      strcat(space_buffer," ");
    } 
    space_buffer[16] = '\0';
		printf("%s: %s %s\n", comd_tab[j].name, space_buffer, comd_tab[j].comment);
  }
  return 0;
}

int main(void) {
  //
  // get the file system going
  //
  while (1) { /* outer loop */
    int ret;
    ret = noosfs_init();
    if (ret >= 0) {
      ret = noosfs_mount();
    }
    if (ret >= 0) {
      fd = noosfs_create(buildFileName);
      if (fd >= 0) {
        ret = fd;
      }
    }
    if (ret >= 0) {
      ret = noosfs_write(fd, dynamic_build, strlen(dynamic_build));
    }
    if (ret >= 0) {
      ret = noosfs_close(fd);
    }
    if (ret >= 0) {
      fd = noosfs_open(buildFileName);
      if (fd >= 0) {
        ret = fd;
      }
    }
    if (ret >= 0) {
      ret = noosfs_read(fd, build, sizeof(build));
    }
    if (ret >=0) {
      noosfs_close(fd);
    }
	  char *buff = command;
	  strcpy(&buff[strlen(buff)], "");
	  unsigned long start_cycles;
	  unsigned long end_cycles;
	  unsigned long duration_cycles;
    int ctrl_pressed = 0;
#ifdef NOOSRT
    // init malloc with actual storage and size
    noosrt_malloc_init(((void *) noosrt_mem),_NOOSRT_MEM_SIZE );
    printf("Malloc Free Space: %d\n", noosrt_malloc_get_free_size());
#endif
    splash(version, build);
    printf("\n");
    printf("** for assitance, type 'help' <enter> **\n\n");
	  printf(time_msg, time(NULL));
	  printprompt();
	  //
	  // process commands, 1 char at a time
	  //
    int do_restart = 0;
	  while (1) { /* inner loop */
		  uint8_t byte;
		  while (byte = scan()) { /* input char processor */
#ifdef NOOSRT
        // add very Q&D ctrl-C support to restart input, NOOSRT only
        if (byte == 0x1d) { // ctrl key down
          ctrl_pressed = 1;
          continue;
        } else if (byte == 0x2e && ctrl_pressed) { // and c key down
				  printf("\nctrl-C detected!\n");
				  printprompt();
				  memset(&buff[0], 0, sizeof(buff));
          ctrl_pressed = 0;
          continue;
        } 
        ctrl_pressed = 0;
        // 
#endif 
#ifdef NOOSRT
			  if (byte == 0x1c) {
				  printf("\n");
#else
			  if (byte == '\n') {
#endif	
          // dispatch something
				  duration_cycles = 0;
				  process_args(buff);  // set largc, largv -- largv[0] is required
				  if (strlen(largv[0]) > 0 && strcmp(largv[0], "exit") == 0) {
					  say_goodnight_dick(-1, (char **) NULL);
            do_restart = 1;
            break;
				  } else if (strlen(largv[0]) > 0 && strcmp(largv[0], "quit") == 0) {
					  say_goodnight_dick(-1, (char **) NULL);
            do_restart = 1;
            break;
				  } else if (strlen(largv[0]) > 0 && strcmp(largv[0], "show-pgms") == 0) {
					  showpgms(-1, (char **) NULL);
				  } else if (strlen(largv[0]) > 0 && strcmp(largv[0], "help") == 0) {
					  help(-1, (char **) NULL);
				  } else {
					  // general command dispatcher
					  for (int j=0;j<max_programs;j++) {
						  if (strcmp(largv[0],comd_tab[j].name) == 0) {
							  start_cycles = time(NULL);
							  comd_tab[j].func(largc,(char **) largv); // duck!
							  end_cycles = time(NULL);
							  duration_cycles = end_cycles - start_cycles;
							  break;
						  }
						  if (comd_tab[j].func == NULL) {
							  printf("\nHuh?\n");
							  break;
					  	}
				  	}
			  	}
				  // printf("\nDEBUG: %d\n", start_cycles);
				  // printf("DEBUG: %d\n", end_cycles);
				  printf(dur_msg, duration_cycles);
				  printf(time_msg, time(NULL));
				  printprompt();
				  memset(&buff[0], 0, sizeof(buff));
          ctrl_pressed = 0;
				  break;
		  	} else {
			  	char *s;
#ifdef NOOSRT
			  	char c = normalmap[byte];
			  	s = ctos(s, c);
				  printf("%s", s); // poor man's char echo
#else
				  char frog[4];
				  s = frog;
				  s[0] = byte;
				  s[1] = 0;
#endif
				  strcpy(&buff[strlen(buff)], s);
		  	}
#ifdef NOSRRT
			  move_cursor(get_terminal_row(), get_terminal_col());
#endif			
		  } /* end char processor */
      if (do_restart) {
        break;  // this only works for NOOSRT mode....
                // in NOOSRT mode this will restart kernel
      }
    } /* end inner while */
    // we can never leave!
	} /* outter loop */
	return 0;
}
