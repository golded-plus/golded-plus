//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  Debugging and logging.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//  Only include once!

#ifndef __GDBGERR_H
    #define __GDBGERR_H


    //  ------------------------------------------------------------------
    #include <gdefs.h>


    //  ------------------------------------------------------------------
    //  Error codes

    #define GOLD_NO_ERROR      0


    //  ------------------------------------------------------------------
    //  Debug aid macros

    #define GDEBUGINFO __FILE__, __LINE__
    #define GDEBUGDATA GDEBUGFILE, GDEBUGLINE
    #define GDEBUGPROTO char* GDEBUGFILE, int GDEBUGLINE


    //  ------------------------------------------------------------------
    //  Automatic inline debugging info for use with _halt_()

    #define E_INFO  __FILE__,__LINE__
    #define E_PROTO int _error_, char* _file_, int _line_
    #define E_DATA      _error_,       _file_,     _line_


    //  ------------------------------------------------------------------
    //  Error types for use with _halt_()

    const int ERR_OK       = 0;
    const int ERR_OPEN     = 1;
    const int ERR_READ     = 2;
    const int ERR_SEEK     = 3;
    const int ERR_MEMO     = 4;
    const int ERR_INDX     = 5;
    const int ERR_WRITE    = 6;
    const int ERR_TELL     = 7;
    const int ERR_CLOSE    = 8;
    const int ERR_CONF     = 9;
    const int ERR_BREAK   = 10;
    const int ERR_NOKEY   = 11;
    const int ERR_TEST    = 12;
    const int ERR_ENVR    = 13;
    const int ERR_LOCK    = 14;
    const int ERR_MPTR    = 15;

    #define E_OK      ERR_OK   , E_INFO
    #define E_OPEN    ERR_OPEN , E_INFO
    #define E_READ    ERR_READ , E_INFO
    #define E_SEEK    ERR_SEEK , E_INFO
    #define E_MEMO    ERR_MEMO , E_INFO
    #define E_INDX    ERR_INDX , E_INFO
    #define E_WRITE   ERR_WRITE, E_INFO
    #define E_TELL    ERR_TELL , E_INFO
    #define E_CLOSE   ERR_CLOSE, E_INFO
    #define E_CONF    ERR_CONF , E_INFO
    #define E_BREAK   ERR_BREAK, E_INFO
    #define E_NOKEY   ERR_NOKEY, E_INFO
    #define E_TEST    ERR_TEST , E_INFO
    #define E_ENVR    ERR_ENVR , E_INFO
    #define E_LOCK    ERR_LOCK , E_INFO
    #define E_MPTR    ERR_MPTR , E_INFO


    //  ------------------------------------------------------------------
    //  Prototypes

    extern int errorlevel;
    extern int error_exit;
    extern const char* etext[];
    void ErrorExit(int type);


    //  ------------------------------------------------------------------
    // Return string (static array): Smth. error exit at [__file,__line].
    char* gerrinfo(int __type, const char* __file, int __line);
    // Return string (static array): __message at [__file,__line].
    char* gerrinfo(const char* __message, const char* __file, int __line);
    // Return string (static array): (OS version) reports error ##: strerror.
    char* gerrosinfo();


    //  ------------------------------------------------------------------

    #define gerropen()    gerrinfo(ERR_OPEN,  __FILE__, __LINE__)
    #define gerrread()    gerrinfo(ERR_READ,  __FILE__, __LINE__)
    #define gerrseek()    gerrinfo(ERR_SEEK,  __FILE__, __LINE__)
    #define gerrmemory()  gerrinfo(ERR_MEMO,  __FILE__, __LINE__)
    #define gerrindex()   gerrinfo(ERR_INDX,  __FILE__, __LINE__)
    #define gerrwrite()   gerrinfo(ERR_WRITE, __FILE__, __LINE__)
    #define gerrtell()    gerrinfo(ERR_TELL,  __FILE__, __LINE__)
    #define gerrclose()   gerrinfo(ERR_CLOSE, __FILE__, __LINE__)
    #define gerrconfig()  gerrinfo(ERR_CONF,  __FILE__, __LINE__)
    #define gerrbreak()   gerrinfo(ERR_BREAK, __FILE__, __LINE__)
    #define gerrnokey()   gerrinfo(ERR_NOKEY, __FILE__, __LINE__)
    #define gerrtest()    gerrinfo(ERR_TEST,  __FILE__, __LINE__)
    #define gerrenvr()    gerrinfo(ERR_ENVR,  __FILE__, __LINE__)
    #define gerrlock()    gerrinfo(ERR_LOCK,  __FILE__, __LINE__)
    #define gerrpointer() gerrinfo(ERR_MPTR,  __FILE__, __LINE__)

    #define erropen(f,l)    printf("! %s", gerrinfo(ERR_OPEN,  f,l))
    #define errread(f,l)    printf("! %s", gerrinfo(ERR_READ,  f,l))
    #define errseek(f,l)    printf("! %s", gerrinfo(ERR_SEEK,  f,l))
    #define errmemory(f,l)  printf("! %s", gerrinfo(ERR_MEMO,  f,l))
    #define errindex(f,l)   printf("! %s", gerrinfo(ERR_INDX,  f,l))
    #define errwrite(f,l)   printf("! %s", gerrinfo(ERR_WRITE, f,l))
    #define errtell(f,l)    printf("! %s", gerrinfo(ERR_TELL,  f,l))
    #define errclose(f,l)   printf("! %s", gerrinfo(ERR_CLOSE, f,l))
    #define errconfig(f,l)  printf("! %s", gerrinfo(ERR_CONF,  f,l))
    #define errbreak(f,l)   printf("! %s", gerrinfo(ERR_BREAK, f,l))
    #define errnokey(f,l)   printf("! %s", gerrinfo(ERR_NOKEY, f,l))
    #define errtest(f,l)    printf("! %s", gerrinfo(ERR_TEST,  f,l))
    #define errenvr(f,l)    printf("! %s", gerrinfo(ERR_ENVR,  f,l))
    #define errlock(f,l)    printf("! %s", gerrinfo(ERR_LOCK,  f,l))
    #define errpointer(f,l) printf("! %s", gerrinfo(ERR_MPTR,  f,l))

    #define ErrOpen()       erropen(__FILE__,__LINE__)
    #define ErrRead()       errread(__FILE__,__LINE__)
    #define ErrSeek()       errseek(__FILE__,__LINE__)
    #define ErrMemory()     errmemory(__FILE__,__LINE__)
    #define ErrIndex()      errindex(__FILE__,__LINE__)
    #define ErrWrite()      errwrite(__FILE__,__LINE__)
    #define ErrTell()       errtell(__FILE__,__LINE__)
    #define ErrClose()      errclose(__FILE__,__LINE__)
    #define ErrConfig()     errconfig(__FILE__,__LINE__)
    #define ErrBreak()      errbreak(__FILE__,__LINE__)
    #define ErrNokey()      errnokey(__FILE__,__LINE__)
    #define ErrTest()       errtest(__FILE__,__LINE__)
    #define ErrEnvr()       errenvr(__FILE__,__LINE__)
    #define ErrLock()       errlock(__FILE__,__LINE__)
    #define ErrPointer()    errpointer(__FILE__,__LINE__)
    #define ErrOSInfo()     printf(": %s", gerrosinfo())

    #define OpenErrorExit()    ErrorExit(ERR_OPEN)
    #define ReadErrorExit()    ErrorExit(ERR_READ)
    #define SeekErrorExit()    ErrorExit(ERR_SEEK)
    #define MemoryErrorExit()  ErrorExit(ERR_MEMO)
    #define IndexErrorExit()   ErrorExit(ERR_INDX)
    #define WriteErrorExit()   ErrorExit(ERR_WRITE)
    #define TellErrorExit()    ErrorExit(ERR_TELL)
    #define CloseErrorExit()   ErrorExit(ERR_CLOSE)
    #define ConfigErrorExit()  ErrorExit(ERR_CONF)
    #define BreakErrorExit()   ErrorExit(ERR_BREAK)
    #define NokeyErrorExit()   ErrorExit(ERR_NOKEY)
    #define TestErrorExit()    ErrorExit(ERR_TEST)
    #define EnvrErrorExit()    ErrorExit(ERR_ENVR)
    #define LockErrorExit()    ErrorExit(ERR_LOCK)
    #define PointerErrorExit() ErrorExit(ERR_MPTR)


    //  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
