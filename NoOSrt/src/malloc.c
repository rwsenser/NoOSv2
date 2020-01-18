//
// malloc.c
//
// part of NoOSrt
//
// WARNING:  This first version of malloc and friends does not reuse freed memory
//           It purposely has memory leaks!
//
// namespace prefix:  noosrt and _noosrt_
//
// #include "../inc/string.h"
#include "stddef.h"

static void *_NoOSrt_mem_ptr = NULL;
static size_t _NoOSrt_free_mem = 0;

// use this to provide a malloc shortage area
void noosrt_malloc_init(void *ptr, size_t size) {
  _NoOSrt_mem_ptr = ptr;
  _NoOSrt_free_mem = size;
  return;
}
// use this to get size of remaining freestore
size_t noosrt_malloc_get_free_size() {
  return _NoOSrt_free_mem;
}

void *malloc(size_t size) {
  void *new_ptr = NULL;
  unsigned char *kludge;
//  if (_NoOSrt_mem_ptr == NULL) {
//    _NoOSrt_mem_ptr = (void *) _NoOSrt_mem;
//    _NoOSrt_free_mem = _NOOSRT_MEM_SIZE;
//  }
  if (size <= _NoOSrt_free_mem) {
    _NoOSrt_free_mem -= size;
    new_ptr = _NoOSrt_mem_ptr;
    // brute force for easy debugging!
    kludge = (unsigned char *) _NoOSrt_mem_ptr;
    kludge += size;
    _NoOSrt_mem_ptr = (void *) kludge;
  }
  return new_ptr;
}

//  I the creator of memory leaks!
void free(void *ptr) {
  return;
}
