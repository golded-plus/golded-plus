#ifndef __genwrap_h
#define __genwrap_h

#include <gtimall.h>

#ifndef __BEOS__
#define SLEEP(a) usleep(a)
#else
#include <OS.h>
#define SLEEP(a) snooze(a)
#endif
#endif // __genwrap_h
