#ifndef __genwrap_h
#define __genwrap_h

#if defined(_MSC_VER)
#include <windows.h>
#include <share.h>
#include <io.h>
#else
#include <gtimall.h>
#endif

#ifndef __BEOS__
 #if defined(_MSC_VER)
  #define SLEEP(a) Sleep(a)
 #else
  #define SLEEP(a) usleep(a)
 #endif
#else
#include <OS.h>
#define SLEEP(a) snooze(a)
#endif

#if defined(_MSC_VER)
int lock(int handle, long offset, long length);
int unlock(int handle, long offset, long length);
#endif

#endif // __genwrap_h
