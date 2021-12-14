//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  String bag classes.
//  ------------------------------------------------------------------

#ifndef __gstrbags_h
#define __gstrbags_h


//  ------------------------------------------------------------------

#include <gmemdbg.h>


//  ------------------------------------------------------------------

class GStrBag
{

protected:

    char* bag;
    int bagsize;
    int items;
    int currno;

public:

    GStrBag();
    virtual ~GStrBag();

    int blocksize;

    void Reset();

    int Add(const char* string);
    int Add(const void* data, int length);

    void Change(int index, const char* string);
    void Change(int index, const void* data, int length);

    virtual int Count()
    {
        return items;
    }

    int CurrNo()
    {
        return currno;
    }
    void CurrNo(int c)
    {
        currno = c;
    }

    int& Pos(int i)
    {
        return ((int*)(bag+bagsize))[i];
    }
    const char* Index(int i)
    {
        return bag ? bag + Pos(i) : NULL;
    }

    int First()
    {
        if(Count() < 1) return false;
        currno = 0;
        return true;
    }
    int Next()
    {
        if(currno >= (Count()-1)) return false;
        currno++;
        return true;
    }
    const char* Current()
    {
        return Index(currno);
    }

    const char* operator[](int i);
};


//  ------------------------------------------------------------------

class GStrBag2 : public GStrBag
{

public:

    GStrBag2() : GStrBag() {}

    inline int Count()
    {
        return items / 2;
    }

    inline int Add(const char* str1, const char* str2)
    {
        GStrBag::Add(str1);
        return GStrBag::Add(str2);
    }
    inline int Add(const void* data, int length, const char* string)
    {
        GStrBag::Add(data, length);
        return GStrBag::Add(string);
    }

    inline void Change(int index, const char* str1, const char* str2)
    {
        GStrBag::Change(index-1, str1);
        GStrBag::Change(index, str2);
    }

    inline const char* Index1(int i)
    {
        return Index((i*2));
    }
    inline const char* Index2(int i)
    {
        return Index((i*2)+1);
    }

    inline const char* Current1()
    {
        return Index1(currno);
    }
    inline const char* Current2()
    {
        return Index2(currno);
    }
};


//  ------------------------------------------------------------------

class GStrSet3
{

protected:

    char* set;
    struct
    {
        int size;
        int pos2;
        int pos3;
    } cfg;

public:

    GStrSet3()
    {
        set = NULL;
        cfg.size = 0;
        cfg.pos2 = 0;
        cfg.pos3 = 0;
    }
    ~GStrSet3()
    {
        Reset();
    }

    void Reset()
    {
        throw_xfree(set);
    }

    void Put(const char* s1, const char* s2, const char* s3);

    void Change(const char* s1, const char* s2, const char* s3);

    const char* Change1(const char* s)
    {
        Change(s, Get2(), Get3());
        return s;
    }
    const char* Change2(const char* s)
    {
        Change(Get1(), s, Get3());
        return s;
    }
    const char* Change3(const char* s)
    {
        Change(Get1(), Get2(), s);
        return s;
    }

    const char* Get1() const
    {
        return set ? set : "";
    }
    const char* Get2() const
    {
        return set ? set+cfg.pos2 : "";
    }
    const char* Get3() const
    {
        return set ? set+cfg.pos3 : "";
    }
};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
