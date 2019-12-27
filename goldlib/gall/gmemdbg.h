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

#ifndef __gdbgtmem_h
    #define __gdbgtmem_h


    //  ------------------------------------------------------------------

    #include <gdefs.h>


    //  ------------------------------------------------------------------
    //  Global variables

    extern int throw_alloc_extra;

    #if defined(GTHROW_LOG)
        class glog;
        extern glog* throw_log;
    #endif


    //  ------------------------------------------------------------------
    //  Prototypes

    void  throw_init();

    void  throw_check();
    void  throw_checkptr(void* ptr);

    char* throw_strdup(const char* str);
    void* throw_malloc(size_t size);
    void* throw_calloc(size_t items, size_t size);
    void* throw_realloc(void* ptr, size_t size);
    void  throw_free(void* ptr);

    char* throw_xstrdup(const char* str);
    void* throw_xmalloc(size_t size);
    void* throw_xcalloc(size_t items, size_t size);
    void* throw_xrealloc(void* ptr, size_t size);
    void  throw_xfree(void* ptr);

    void* throw_outofmem();
    void* throw_outofmem_report(const char* file, int line);


    //  ------------------------------------------------------------------
    //  Debugging prototypes

    void  throw_new_debug(const void* ptr, const char* file, int line);
    char* throw_strdup_debug(const char* str, const char* file, int line);
    void* throw_malloc_debug(size_t size, const char* file, int line);
    void* throw_calloc_debug(size_t items, size_t size, const char* file, int line);
    void* throw_realloc_debug(void* ptr, size_t size, const char* file, int line);
    void  throw_free_debug(void* ptr, const char* file, int line);
    void  throw_check_debug(const char* file, int line);
    void  throw_checkptr_debug(const void* ptr, const char* file, int line);

    void  throw_xnew_debug(void* ptr, const char* file, int line);
    char* throw_xstrdup_debug(const char* str, const char* file, int line);
    void* throw_xmalloc_debug(size_t size, const char* file, int line);
    void* throw_xcalloc_debug(size_t items, size_t size, const char* file, int line);
    void* throw_xrealloc_debug(void* ptr, size_t size, const char* file, int line);
    void  throw_xfree_debug(void* ptr, const char* file, int line);


    //  ------------------------------------------------------------------
    //  Macro functions

    #if defined(GTHROW_DEBUG)
        #define throw_new(p)        throw_new_debug((p),__FILE__,__LINE__)
        #define throw_alloc(p)      throw_new_debug((p),__FILE__,__LINE__)
        #define throw_strdup(p)     throw_strdup_debug((p),__FILE__,__LINE__)
        #define throw_malloc(s)     throw_malloc_debug((s),__FILE__,__LINE__)
        #define throw_calloc(i,s)   throw_calloc_debug((i),(s),__FILE__,__LINE__)
        #define throw_realloc(p,s)  throw_realloc_debug((p),(s),__FILE__,__LINE__)
        #define throw_free(p)       throw_free_debug((p),__FILE__,__LINE__)
        #define throw_check()       throw_check_debug(__FILE__,__LINE__)
        #define throw_checkptr(p)   throw_checkptr_debug((p),__FILE__,__LINE__)
    #else
        #define throw_new(p)        throw_xnew_debug((p),__FILE__,__LINE__)
        #define throw_alloc(p)      throw_xnew_debug((p),__FILE__,__LINE__)
        #define throw_strdup(p)     throw_xstrdup_debug((p),__FILE__,__LINE__)
        #define throw_malloc(s)     throw_xmalloc_debug((s),__FILE__,__LINE__)
        #define throw_calloc(i,s)   throw_xcalloc_debug((i),(s),__FILE__,__LINE__)
        #define throw_realloc(p,s)  throw_xrealloc_debug((p),(s),__FILE__,__LINE__)
        #define throw_free(p)       throw_xfree_debug((p),__FILE__,__LINE__)
        #define throw_check()
        #define throw_checkptr(p)
        #define throw_init()
    #endif

    #define throw_xnew(p)        throw_xnew_debug((p),__FILE__,__LINE__)
    #define throw_xstrdup(p)     throw_xstrdup_debug((p),__FILE__,__LINE__)
    #define throw_xmalloc(s)     throw_xmalloc_debug((s),__FILE__,__LINE__)
    #define throw_xcalloc(i,s)   throw_xcalloc_debug((i),(s),__FILE__,__LINE__)
    #define throw_xrealloc(p,s)  throw_xrealloc_debug((p),(s),__FILE__,__LINE__)
    #define throw_xfree(p)       throw_xfree_debug((p),__FILE__,__LINE__)


    //  ------------------------------------------------------------------
    //  Special purpose macros

    #define throw_release(m) { throw_free(m); (m) = NULL; }
    #define throw_xrelease(m) { throw_xfree(m); (m) = NULL; }

    #define throw_delete(p) { delete (p); (p) = NULL; }
    #define throw_xdelete(p) { if(p) delete (p); (p) = NULL; }

    #define throw_deletearray(p) { delete[] (p); (p) = NULL; }
    #define throw_xdeletearray(p) { if(p) delete[] (p); (p) = NULL; }

    #define throw_mallox(__items, __size, __cache)  throw_malloc((__items+__cache+1)*__size)
    #define throw_callox(__items, __size, __cache)  throw_calloc(__items+__cache+1, __size)
    #define throw_reallox(__ptr, __items, __size, __cache) (((__items % __cache) and __ptr) ? (void*)__ptr : throw_realloc(__ptr, (__items+__cache+1)*__size));

    #define throw_xmallox(__items, __size, __cache)  throw_xmalloc((__items+__cache+1)*__size)
    #define throw_xcallox(__items, __size, __cache)  throw_xcalloc(__items+__cache+1, __size)
    #define throw_xreallox(__ptr, __items, __size, __cache) (((__items % __cache) and __ptr) ? (void*)__ptr : throw_xrealloc(__ptr, (__items+__cache+1)*__size));


    //  ------------------------------------------------------------------

    #if defined(GTHROWCHKPTR_ENABLE)
        #define THROW_CHECKPTR(p)   throw_checkptr(p)
        #define THROW_CHECK()       throw_check()
    #else
        #define THROW_CHECKPTR(p)
        #define THROW_CHECK()
    #endif


    //  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
