#ifndef __filewrap_h
#define __filewrap_h

#include <gfilutil.h>

#define sopen(fn,access,share) (sopen)((fn),(access),(share),S_IREAD|S_IWRITE)

#endif // __filewrap_h
