//
// fileman.h: file manager  application for NoOS
//

// prefix: fman

#define FILEMAN_APP {"fileman",fileman_run,"file manager util"}

// needed:
// 1)  Ability to input a full string
// 2)  Addition to noosfs to list existing files
// 3)  Addition to noosfs to remove/kill a file
// 4)  Addition to noosfs to open file for rewrite
//

uint8_t _fman_getchar() {
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

#ifdef NOOSRT
  char *_fman_gets(char *s) {
    // printf("gets() not implimented\n");
    char *ptr = s;
    *ptr = '\0';
    char c;
    do {
      c = _fman_getchar();
      if (c >= ' ' && c < 0x7f) {
        *ptr = c;
        ptr++;
        *ptr = '\0';
      } else {
        break;
      }
    } while (1);
    return s;
  }
#else
  char *gets(char *);
  char *_fman_gets(char *s) {
    return gets(s);
  }
#endif

void _fman_chompcr() {
#ifdef NOOSRT
  _fman_getchar();  // yum yum
  printf("\n");
#else
  _fman_getchar();  // yum yum
#endif
  return;
}

long _fman_getHex() {
  long val = 0;
  uint8_t chr;
  for (int i=0; i <= 8; i++) {
    chr = _fman_getchar();
    if (chr == ' ') continue;
    if (chr < '0' || chr > 'f') break;
    val *= 16;
    if (chr <= '9') val += (chr - '0');
    else val += 10 + (chr - 'a');
  }
  return val;
}

char *_fman_getFileName(char * ptr){
  printf("filename:");
  _fman_gets(ptr);
}

int fileman_run(int argc, char** argv) {

  long address;
  int width = 2;
  static int data = 32;
  int size;
  int len;
  uint8_t cmd;
  uint8_t value;
  uint8_t *ptr;
  char filename[255];
  char aline[255];
  int status;
  int fd;
  address = (long) &data;
  while (1) {
    printf("\nCommands: a(dd), k(ill), l(ist), r(ewrite), s(how), x(it)\n");
    printf(":");
    cmd = _fman_getchar();
    _fman_chompcr(); // eat enter key
    if (cmd == 'a' || cmd == 'r') {
      // add file
      _fman_getFileName(filename);
      printf("to exit input mode, enter a single dot on a line.\n");
      if (cmd == 'a') {
        fd = noosfs_create(filename);
      } else {
        // rewrite
        fd = noosfs_open(filename);
      }
      if (fd >= 0) {
        while (1) {
          printf(">");
          _fman_gets(aline);
           len = strlen(aline);
          if (len > 0) {
            if (len == 1 && aline[0] == '.') {
              break;
            } else {
              aline[len] = '\n';
              len++;
              status = noosfs_write(fd, aline, len);
            }
          } else {
            break; /* for now */
          }
        }
        status =  noosfs_close(fd);
      }
    } else if (cmd == 'k') {
      // kill file
      _fman_getFileName(filename);
      fd = noosfs_delete(filename);
    } else if (cmd == 'l') {
      // list all file names
      noosfs_list("  >>");   // outputs via printf()
    } else if (cmd == 'r') {
      // rewrite existing file contents
    } else if (cmd == 's') {
      // show file
      _fman_getFileName(filename);
      fd = noosfs_open(filename);
      if (fd >= 0) {
        while (1) {
          status =noosfs_read(fd, aline, 1);
          if (status > 0) {
            printf("%c",aline[0]);
          } else {
            break;
          }
        }
        status =  noosfs_close(fd);
      }
    } else if (cmd == 'x') {
      break;
    }
  }
  printf("exit.\n");
  return 0;
}
