#ifndef __filewrap_h
#define __filewrap_h

#if defined(_MSC_VER)
#include <io.h>
#else
#include <gfilutil.h>
#endif

#define sopen(fn,access,share) (sopen)((fn),(access),(share),S_IREAD|S_IWRITE)

#endif // __filewrap_h
