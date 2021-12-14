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
//  FidoNet Technology Network - Basic Data Structures.
//  ------------------------------------------------------------------

#ifndef __gftnall_h
#define __gftnall_h


//  ------------------------------------------------------------------

#include <gdefs.h>
#include <string>
#include <stdlib.h>

//  ------------------------------------------------------------------
//  FTN contants

class ftn
{

public:

    enum
    {
        wildcard_all   = 0xFFFE,  // For wildcard '*'
        wildcard_first = 0xFFFD,  // For wildcard '?'
        domain_first   = 1,       // domain#address
        domain_last    = 2,       // address@domain
        domain_limit   = 41,
        name_limit     = 36
    };
};


//  ------------------------------------------------------------------
//  FTN domain type

typedef char ftn_domain[ftn::domain_limit];


//  ------------------------------------------------------------------
//  FTN name type

typedef char ftn_name[ftn::name_limit];


//  ------------------------------------------------------------------
//  FTN 4D address type

class ftn_addr
{

public:

    word zone;
    word net;
    word node;
    word point;

    ftn_addr() : zone(0), net(0), node(0), point(0) {}
    ftn_addr(uint a);
    ftn_addr(const char* s);
    ftn_addr(const std::string& s);
    ftn_addr(const ftn_addr& a);
    ftn_addr(uint zn, uint nt, uint nd, uint pt);

    bool valid() const
    {
        return net != 0;
    }
    bool invalid() const
    {
        return net == 0;
    }

    void  set_all(uint part);
    void  set_fast(const ftn_addr& a);
    void  set_fast(uint zn=0, uint nt=0, uint nd=0, uint pt=0);
    void  set(uint zn=0, uint nt=0, uint nd=0, uint pt=0);
    void  set(const ftn_addr& a);
    void  set(const void* a);
    char* set(const char* str, char* dom=NULL, int domsizelimit=ftn::domain_limit);
    const char* set(const std::string& str);
    const char* set(const std::string& str, std::string& dom, int domsizelimit=ftn::domain_limit);

    void  reset();
    void  reset_fast();
    void  reset(const std::string& str);
    char* reset(const char* str, char* dom=NULL, int domsizelimit=ftn::domain_limit);
    void  reset(const std::string& str, std::string& dom, int domsizelimit=ftn::domain_limit);

    bool match(const ftn_addr& mask) const;
    bool equals(const ftn_addr& other) const;
    int  compare(const ftn_addr& other) const;

    std::string& make_string(std::string& str) const;
    std::string& make_string(std::string& str, const std::string& dom, int domfmt=ftn::domain_last) const;
    char*    make_string(char* str, const char* dom=NULL, int domfmt=ftn::domain_last) const;

    bool operator==(const ftn_addr& b) const;
    bool operator!=(const ftn_addr& b) const;

    ftn_addr& operator=(int n);
    ftn_addr& operator=(const char* s);
    ftn_addr& operator=(const std::string& s);
    ftn_addr& operator=(const ftn_addr& a);
};


//  ------------------------------------------------------------------

struct ftn_aka
{

    ftn_addr   addr;
    ftn_domain domain;
    word       pointnet;
};


//  ------------------------------------------------------------------

inline void ftn_addr::set_fast(const ftn_addr& a)
{

    zone  = a.zone;
    net   = a.net;
    node  = a.node;
    point = a.point;
}


//  ------------------------------------------------------------------

inline void ftn_addr::set_fast(uint zn, uint nt, uint nd, uint pt)
{

    zone  = (word)zn;
    net   = (word)nt;
    node  = (word)nd;
    point = (word)pt;
}


//  ------------------------------------------------------------------

inline ftn_addr::ftn_addr(uint a) : zone(0), net(0), node(0), point(0)
{

    set_fast(a, a, a, a);
}


//  ------------------------------------------------------------------

inline ftn_addr::ftn_addr(const char* s) : zone(0), net(0), node(0), point(0)
{

    reset(s);
}


//  ------------------------------------------------------------------

inline ftn_addr::ftn_addr(const std::string& s) : zone(0), net(0), node(0), point(0)
{

    reset(s);
}


//  ------------------------------------------------------------------

inline ftn_addr::ftn_addr(const ftn_addr& a) : zone(0), net(0), node(0), point(0)
{

    set_fast(a);
}


//  ------------------------------------------------------------------

inline ftn_addr::ftn_addr(uint zn, uint nt, uint nd, uint pt) : zone(0), net(0), node(0), point(0)
{

    set_fast(zn, nt, nd, pt);
}


//  ------------------------------------------------------------------

inline void ftn_addr::reset_fast()
{

    set_fast();
}


//  ------------------------------------------------------------------

inline bool ftn_addr::equals(const ftn_addr& other) const
{

    return compare(other) == 0;
}


//  ------------------------------------------------------------------

inline bool ftn_addr::operator==(const ftn_addr& b) const
{
    return equals(b);
}
inline bool ftn_addr::operator!=(const ftn_addr& b) const
{
    return !equals(b);
}


//  ------------------------------------------------------------------

inline ftn_addr& ftn_addr::operator=(int n)
{
    set_all(n);
    return *this;
}
inline ftn_addr& ftn_addr::operator=(const char* s)
{
    reset(s);
    return *this;
}
inline ftn_addr& ftn_addr::operator=(const std::string& s)
{
    reset(s);
    return *this;
}
inline ftn_addr& ftn_addr::operator=(const ftn_addr& a)
{
    set_fast(a);
    return *this;
}


//  ------------------------------------------------------------------

class ftn_akamatch
{

public:

    ftn_akamatch& operator=(const ftn_akamatch& o)
    {
        mask=o.mask;
        aka=o.aka;
        return *this;
    }

    ftn_addr mask;
    ftn_addr aka;
};


//  ------------------------------------------------------------------
//  Legacy

typedef ftn_addr    Addr;
typedef ftn_domain  Domn;
typedef ftn_aka     gaka;

#define GFTN_ALL          ftn::wildcard_all
#define GFTN_FIRST        ftn::wildcard_first
#define GFTN_DOMAIN_FIRST ftn::domain_first
#define GFTN_DOMAIN_LAST  ftn::domain_last
#define GFTN_DOMAIN_LIMIT ftn::domain_limit


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
