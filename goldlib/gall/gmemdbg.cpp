//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Memory management routines with debugging features.
//  Based on free code from SNIPPETS 9404 by Walter Bright.
//  ------------------------------------------------------------------

#include <climits>
#include <gdbgerr.h>
#include <gmemdbg.h>
#include <glog.h>
#include <gmemall.h>


//  ------------------------------------------------------------------
//  Global vars

int throw_alloc_extra = 0;

#if defined(GTHROW_LOG)
glog* throw_log = NULL;
#define TLOG if(throw_log) throw_log
#endif


//  ------------------------------------------------------------------


#define gmem_strdup strdup
#define gmem_malloc malloc
#define gmem_calloc calloc
#define gmem_realloc realloc
#define gmem_free free


//  ------------------------------------------------------------------

#if defined(GTHROW_DEBUG)


//  ------------------------------------------------------------------
//  Various values

const dword BEFOREVAL = 0x12345678L;
const dword AFTERVAL  = 0x87654321L;

const byte BADVAL    = 0xFF;
const byte MALLOCVAL = 0xEE;

const size_t BAD_SIZE    = 0xFFFFFFFF;
const size_t MALLOC_SIZE = 0xEEEEEEEE;


//  ------------------------------------------------------------------
//  Struct

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif

struct Throw {
  Throw* next;
  Throw* prev;
  const char* file;
  int    line;
  int    index;
  size_t nbytes;
  dword  beforeval;
  char   data[1];
};

#if defined(GOLD_CANPACK)
#pragma pack()
#endif

//  ------------------------------------------------------------------

struct ThrowIndex {
  Throw* pointer;
};


//  ------------------------------------------------------------------
//  Internal vars

int     throw_inited = 0;
int     throw_count;
ulong   throw_allocations = 0;
Throw   throw_alloclist;
int     throw_max_count = 0;
Throw** throw_index = NULL;
int     throw_index_size = 0;
int     throw_index_free = 0;
int     throw_last_free = -1;
ulong   throw_index_cache_hits = 0;
int     throw_overhead = sizeof(Throw) - 1;

#define throw_index_init_size 1000
#define throw_index_increment 100


//  ------------------------------------------------------------------
//  Inlined functions

inline Throw* throw_ptrtodl(const void* ptr) { return (Throw*)((const char*)ptr-sizeof(Throw)+1); }
inline void* throw_dltoptr(Throw* dl) { return (void*)dl->data; }


//  ------------------------------------------------------------------

int throw_index_find_free() {

  if(throw_last_free != -1) {
    throw_index_cache_hits++;
    return throw_last_free;
  }
  else if(throw_index_free) {
    Throw** i = throw_index;
    for(int n=0; n<throw_index_size; n++,i++) {
      if(*i == NULL) {
        throw_last_free = n;
        return throw_last_free;
      }
    }
  }
  if(throw_index_size) {
    throw_index = (Throw**)realloc(throw_index, (throw_index_size+throw_index_increment)*sizeof(Throw*));
    throw_new(throw_index);
    memset(throw_index+throw_index_size, 0, throw_index_increment*sizeof(Throw*));
    throw_last_free = throw_index_size;
    throw_index_size += throw_index_increment;
    throw_index_free = throw_index_increment;
  }
  else {
    throw_index = (Throw**)calloc(throw_index_init_size, sizeof(Throw*));
    throw_new(throw_index);
    throw_index_size = throw_index_init_size;
    throw_index_free = throw_index_init_size;
    throw_last_free = 0;
  }
  return throw_last_free;
}


//  ------------------------------------------------------------------

int throw_index_add(Throw* pointer) {

  int index = throw_index_find_free();
  throw_index[index] = pointer;
  throw_last_free = -1;
  throw_index_free--;
  return index;
}


//  ------------------------------------------------------------------

void throw_index_remove(int index) {

  throw_index[index] = NULL;
  throw_last_free = index;
  throw_index_free++;
}


//  ------------------------------------------------------------------

Throw* throw_find_overrun(Throw* pointer) {

  Throw* last_candidate = NULL;
  long last_diff = LONG_MAX;
  Throw** i = throw_index;
  for(int n=0; n<throw_index_size; n++,i++) {
    long diff = (ulong)*i - (ulong)pointer;
    if((diff > 0) and (diff < last_diff)) {
      last_candidate = *i;
      last_diff = diff;
    }
  }
  return last_candidate;
}


//  ------------------------------------------------------------------

Throw* throw_find_underrun(Throw* pointer) {

  Throw* last_candidate = NULL;
  long last_diff = LONG_MAX;
  Throw** i = throw_index;
  for(int n=0; n<throw_index_size; n++,i++) {
    long diff = (ulong)pointer - (ulong)*i;
    if((diff > 0) and (diff < last_diff)) {
      last_candidate = *i;
      last_diff = diff;
    }
  }
  return last_candidate;
}


//  ------------------------------------------------------------------
//  Dump pointer information to the log

#if defined(GTHROW_LOG)

void throw_printdl(Throw* dl) {

  char buf[100];

  char* ptr = (char*)throw_dltoptr(dl);
  TLOG->printf(": Ptr (%p,%u) at [%s,%u].", ptr, (uint)dl->nbytes, CleanFilename(dl->file), (uint)dl->line);
  TLOG->printf(": %s", HexDump16(buf, ptr, dl->nbytes, HEX_DUMP2));

}

#else

inline void throw_printdl(Throw* dl) {}

#endif


//  ------------------------------------------------------------------
//  Debugging new

void throw_new_debug(const void* __ptr, const char* __file, int __line) {

  if(__ptr == NULL) {
    #if defined(GTHROW_LOG)
    TLOG->errmemory(__file, __line);
    TLOG->printf("! A memory allocation failed (out of memory).");
    TLOG->printf("+ Advice: Restart.");
    #endif
    MemoryErrorExit();
  }
}


//  ------------------------------------------------------------------
//  Debugging strdup()

char* throw_strdup_debug(const char* __str, const char* __file, int __line) {

  char* _ptr = (char*)(__str ? throw_calloc_debug(1, strlen(__str)+1, __file, __line) : NULL);
  return _ptr ? strcpy(_ptr, __str) : _ptr;
}


//  ------------------------------------------------------------------
//  Debugging malloc()

void* throw_malloc_debug(size_t __size, const char* __file, int __line) {

  void* _ptr = throw_calloc_debug(1, __size, __file, __line);
  if(_ptr)
    memset(_ptr, MALLOCVAL, __size);
  return _ptr;
}


//  ------------------------------------------------------------------
//  Debugging calloc()

void* throw_calloc_debug(size_t __items, size_t __size, const char* __file, int __line) {

  __size *= __items;
  __size += throw_alloc_extra;

  if(__size == 0) {
    #if defined(GTHROW_LOG)
    TLOG->errmemory(__file, __line);
    TLOG->printf("! Attempted to allocate zero bytes of memory.");
    TLOG->printf("+ Advice: This is a bug. Please report to the Author.");
    #endif
    MemoryErrorExit();
  }

  size_t _siz = sizeof(Throw) + __size + sizeof(AFTERVAL) - 1;

  Throw* dl = (Throw*)calloc(_siz, 1);
  if(dl == NULL) {
    #if defined(GTHROW_LOG)
    TLOG->errmemory(__file, __line);
    TLOG->printf("! A memory allocation failed (out of memory).");
    TLOG->printf(": Needed %u (%Xh) bytes.", (uint)__size, (uint)__size);
    if(__size == BAD_SIZE) {
      TLOG->printf("+ Info: Value could be from free'd data.");
      TLOG->printf("+ Info: This indicates a serious bug.");
      TLOG->printf("+ Advice: Report to the Author immediately.");
    }
    else if(__size == MALLOC_SIZE) {
      TLOG->printf("+ Info: Value could be from uninitialized data");
      TLOG->printf("+ Info: This indicates a serious bug.");
      TLOG->printf("+ Advice: Report to the Author immediately.");
    }
    else {
      TLOG->printf("+ Advice: Restart.");
    }
    #endif
    MemoryErrorExit();
  }

  dl->index = throw_index_add(dl);

  dl->file = __file;
  dl->line = __line;
  dl->nbytes = __size;
  dl->beforeval = BEFOREVAL;
  *(dword*)&(dl->data[__size]) = AFTERVAL;

  dl->next = throw_alloclist.next;
  dl->prev = &throw_alloclist;
  throw_alloclist.next = dl;
  if(dl->next != NULL)
    dl->next->prev = dl;

  throw_count++;
  throw_max_count = maximum_of_two(throw_count, throw_max_count);
  throw_allocations++;

  return throw_dltoptr(dl);
}


//  ------------------------------------------------------------------
//  Debugging realloc()

void* throw_realloc_debug(void* __oldptr, size_t __size, const char* __file, int __line) {

  void* _ptr;
  Throw* dl = throw_ptrtodl(__oldptr);

  if(__size == 0) {
    throw_free_debug(__oldptr,__file,__line);
    _ptr = NULL;
  }
  else if(__oldptr == NULL) {
    _ptr = throw_malloc_debug(__size,__file,__line);
  }
  else {
    _ptr = throw_malloc_debug(__size,__file,__line);
    if(dl->nbytes < __size)
      __size = dl->nbytes;
    memcpy(_ptr,__oldptr,__size);
    throw_free_debug(__oldptr,__file,__line);
  }
  return _ptr;
}


//  ------------------------------------------------------------------
//  Debugging free()

void throw_free_debug(void* __ptr, const char* __file, int __line) {

  Throw* dl;
  Throw* overrun_dl = NULL;
  Throw* underrun_dl = NULL;

  if(__ptr == NULL)
    return;
  int inerr = false;
  if(throw_count <= 0) {
    #if defined(GTHROW_LOG)
    TLOG->errmemory(__file, __line);
    TLOG->printf("! More free's than allocs.");
    TLOG->printf("+ Info: This indicates a potentially serious bug.");
    TLOG->printf("+ Advice: Report to the Author immediately.");
    #endif
    goto err;
  }
  dl = throw_ptrtodl(__ptr);
  if(dl->nbytes == BAD_SIZE) {
    #if defined(GTHROW_LOG)
    TLOG->errpointer(__file, __line);
    TLOG->printf("! A memory allocation was already free'd.");
    #endif
    goto err2;
  }
  if(dl->beforeval != BEFOREVAL) {
    #if defined(GTHROW_LOG)
    TLOG->errpointer(__file, __line);
    TLOG->printf("! An allocated memory region was underrun.");
    #endif
    dl->prev = dl->next = NULL;
    dl->file = NULL;
    underrun_dl = throw_find_underrun(dl);
    goto err2;
  }
  if(*(dword*)&dl->data[dl->nbytes] != AFTERVAL) {
    #if defined(GTHROW_LOG)
    TLOG->errpointer(__file, __line);
    TLOG->printf("! An allocated memory region was overrun.");
    #endif
    overrun_dl = throw_find_overrun(dl);
    inerr = true;
  }

  if(inerr)
    goto err2;

  if(dl->prev)
    dl->prev->next = dl->next;
  if(dl->next)
    dl->next->prev = dl->prev;

  throw_index_remove(dl->index);

  memset(dl,BADVAL,sizeof(*dl)+dl->nbytes);
  throw_count--;

  free(dl);
  return;

err2:
  throw_printdl(dl);
  if(underrun_dl) {
    #if defined(GTHROW_LOG)
    TLOG->printf("! Possibly caused by overrun in this allocation:");
    throw_printdl(underrun_dl);
    if(*(dword*)&underrun_dl->data[underrun_dl->nbytes] != AFTERVAL)
      TLOG->printf("! Overrun of previous allocation confirmed.");
    #endif
  }
  if(overrun_dl) {
    #if defined(GTHROW_LOG)
    TLOG->printf("! Possibly cause - Underrun in this allocation:");
    throw_printdl(overrun_dl);
    if(overrun_dl->beforeval != BEFOREVAL)
      TLOG->printf("! Underrun of previous allocation confirmed.");
    #endif
  }
  #if defined(GTHROW_LOG)
  TLOG->printf(": Detected while freeing the allocation.");
  TLOG->printf("+ Info: This indicates a serious bug.");
  TLOG->printf("+ Advice: Report to the Author immediately.");
  #endif
err:
  //PointerErrorExit();
  ;
}


//  ------------------------------------------------------------------
//  Check all allocations

void throw_check_debug(const char* __file, int __line) {

  Throw* dl = throw_alloclist.next;
  while(dl != NULL) {
    throw_checkptr_debug(throw_dltoptr(dl), __file, __line);
    dl = dl->next;
  }
}


//  ------------------------------------------------------------------
//  Check individual allocation

void throw_checkptr_debug(const void* __ptr, const char* __file, int __line) {

  int inerr = false;
  Throw* dl;
  Throw* overrun_dl = NULL;
  Throw* underrun_dl = NULL;
  if(__ptr == NULL) {
    #if defined(GTHROW_LOG)
    TLOG->errpointer(__file, __line);
    TLOG->printf("! Found NULL pointer instead of allocated memory.");
    #endif
    goto err3;
  }
  dl = throw_ptrtodl(__ptr);
  if(dl->beforeval != BEFOREVAL) {
    #if defined(GTHROW_LOG)
    TLOG->errpointer(__file, __line);
    TLOG->printf("! An allocated memory region was underrun.");
    #endif
    dl->prev = dl->next = NULL;
    dl->file = NULL;
    underrun_dl = throw_find_underrun(dl);
    goto err2;
  }
  if(*(dword*)&dl->data[dl->nbytes] != AFTERVAL) {
    #if defined(GTHROW_LOG)
    TLOG->errpointer(__file, __line);
    TLOG->printf("! An allocated memory region was overrun.");
    #endif
    overrun_dl = throw_find_overrun(dl);
    inerr = true;
  }
  if(inerr)
    goto err2;
  return;

err2:
  throw_printdl(dl);
  if(underrun_dl) {
    #if defined(GTHROW_LOG)
    TLOG->printf("! Possibly caused by overrun in this allocation:");
    throw_printdl(underrun_dl);
    if(*(dword*)&underrun_dl->data[underrun_dl->nbytes] != AFTERVAL)
      TLOG->printf("! Overrun of previous allocation confirmed.");
    #endif
  }
  if(overrun_dl) {
    #if defined(GTHROW_LOG)
    TLOG->printf("! Possibly cause - Underrun in this allocation:");
    throw_printdl(overrun_dl);
    if(overrun_dl->beforeval != BEFOREVAL)
      TLOG->printf("! Underrun of previous allocation confirmed.");
    #endif
  }
err3:
  #if defined(GTHROW_LOG)
  TLOG->printf(": Detected while checking the allocation.");
  TLOG->printf("+ Info: This indicates a serious bug.");
  TLOG->printf("+ Advice: Report to the Author immediately.");
  #endif
  PointerErrorExit();
}


//  ------------------------------------------------------------------
//  Termination function

static void throw_term(void) {

  if(throw_inited) {
    #if defined(GTHROW_LOG)
    if(throw_count)
      TLOG->printf("! Detected %i unfree'd memory allocation%s.", throw_count, throw_count==1?"":"s");
    if(not error_exit) {
      Throw* dl = throw_alloclist.next;
      int count = throw_count;
      for(; dl; ) {
        Throw* dl_next = dl->next;
        throw_printdl(dl);
        throw_free_debug(throw_dltoptr(dl), __FILE__, __LINE__);
        dl = dl_next;
      }
      throw_count = count;
      if(throw_count) {
        TLOG->printf("+ Info: The memory should have been free'd before exit.");
        TLOG->printf("+ Info: This indicates a potentially serious bug.");
        TLOG->printf("+ Advice: Report to the Author immediately.");
      }
    }
    #endif
    free(throw_index);
    throw_inited = 0;
  }
}


//  ------------------------------------------------------------------
//  Init function

void throw_init() {

  if(throw_inited == 0) {
    throw_count = 0;
    throw_alloclist.next = NULL;
    throw_alloclist.prev = NULL;
    throw_alloclist.file = __FILE__;
    throw_alloclist.line = __LINE__;
    throw_alloclist.nbytes = 0;
    throw_alloclist.beforeval = BEFOREVAL;
    throw_alloclist.data[0] = 0xFF;
    throw_inited++;
    atexit(throw_term);
  }
}

//  ------------------------------------------------------------------

#ifdef gmem_strdup
#undef gmem_strdup
#define gmem_strdup(a) throw_strdup_debug(a, file, line)
#undef gmem_malloc
#define gmem_malloc(a) throw_malloc_debug(a, file, line)
#undef gmem_calloc
#define gmem_calloc(a,b) throw_calloc_debug(a, b, file, line)
#undef gmem_realloc
#define gmem_realloc(a,b) throw_realloc_debug(a, b, file, line)
#undef gmem_free
#define gmem_free(a) throw_free_debug(a, file, line)
#endif

#endif


//  ------------------------------------------------------------------

void* throw_outofmem_report(const char* file, int line, uint size) {

  #if defined(GTHROW_LOG)
  TLOG->errmemory(file, line);
  if(size == 0) {
    TLOG->printf("! Attempted to allocate zero bytes of memory.");
    TLOG->printf("+ Advice: This is a bug. Please report to the Author.");
  }
  else {
    TLOG->printf("! A memory allocation failed (out of memory).");
    TLOG->printf(": Needed %u (%Xh) bytes.", size, size);
    TLOG->printf("+ Advice: Restart.");
  }
  #else
  NW(file);
  NW(line);
  NW(size);
  #endif
  MemoryErrorExit();
  return NULL;
}


//  ------------------------------------------------------------------

void throw_xnew_debug(void* ptr, const char* file, int line) {

  if(ptr == NULL)
    throw_outofmem_report(file, line, 0);
}


//  ------------------------------------------------------------------

char* throw_xstrdup_debug(const char* str, const char* file, int line) {

  char* s = gmem_strdup(str);
  return s ? s : (char*)throw_outofmem_report(file, line, strlen(str));
}


//  ------------------------------------------------------------------

void* throw_xmalloc_debug(size_t size, const char* file, int line) {

  void* p = gmem_malloc(size+throw_alloc_extra);
  return p ? p : throw_outofmem_report(file, line, size);
}


//  ------------------------------------------------------------------

void* throw_xcalloc_debug(size_t items, size_t size, const char* file, int line) {

  void* p = gmem_calloc(items, size+throw_alloc_extra);
  return p ? p : throw_outofmem_report(file, line, items*size);
}


//  ------------------------------------------------------------------

void* throw_xrealloc_debug(void* ptr, size_t size, const char* file, int line) {

  void* p = gmem_realloc(ptr, size+throw_alloc_extra);
  return p ? p : throw_outofmem_report(file, line, size);
}


//  ------------------------------------------------------------------

void throw_xfree_debug(void* ptr, const char* file, int line) {

  if(ptr)
    gmem_free(ptr);
}


//  ------------------------------------------------------------------
