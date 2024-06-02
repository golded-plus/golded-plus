//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 2024 Vitaliy Aksyonov
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
//  INI file parser.
//  ------------------------------------------------------------------

#ifndef __GINIPRSR_H
#define __GINIPRSR_H

#include <map>
#include <string>

namespace ini
{
    typedef std::map<std::string, std::string> Variables;
    typedef std::map<std::string, Variables>   Sections;
    bool ReadIniFile(const char * fileName, Sections & sections);
} // namespace ini

#endif // __GINIPRSR_H
