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
//  Tag number types and set class.
//  ------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <gmemdbg.h>
#include <gutltag.h>


//  ------------------------------------------------------------------
//  Constructor.

GTag::GTag()
{

    granularity = 10;

    tag = NULL;
    allocated = tags = count = 0;
}


//  ------------------------------------------------------------------
//  Destructor.

GTag::~GTag()
{

    Reset();
}


//  ------------------------------------------------------------------
//  Deallocate and reset internal data.

void GTag::Reset()
{

    throw_xrelease(tag);
    allocated = tags = 0;
    // NOTE: Does and must NOT reset the count!
}


//  ------------------------------------------------------------------
//  Resize tag array.
//  Returns NULL if realloc failed.

uint32_t* GTag::Resize(uint __tags)
{

    register uint newsize = 0;

    if(__tags >= allocated)
        newsize = __tags + granularity;
    else if(__tags < allocated)
    {
        if((allocated-__tags) > granularity)
            newsize = __tags + granularity;
    }

    if(newsize)
    {
        tag = (uint32_t*)throw_realloc(tag, newsize*sizeof(uint32_t));
        allocated = newsize;
    }

    count = tags = __tags;
    return tag;
}


//  ------------------------------------------------------------------
//  Appends a new tag number to the end of the tag array.
//  NOTE - Does not check for duplicates or correct sequence!

uint32_t* GTag::Append(uint32_t __tagn)
{

    Resize(tags+1);
    tag[tags-1] = __tagn;
    return tag;
}


//  ------------------------------------------------------------------
//  Add a new tag number at the correct position of the tag array.
//
//  Algorithm dry test situations:
//
//  []        0123456
//  tag       345789x  tags = 7 (after resize)
//  _reln     1234567
//
//  Case 1 - New tag is smaller than first tag in array:
//    __tagn = 2
//    _reln = 1 (changed to 0)
//    tags-_reln-1 = 7-0-1 = 6
//
//  Case 2 - Normal insert in middle:
//    __tagn = 6
//    _reln = 3
//    tags-_reln-1 = 7-3-1 = 3
//
//  Case 3 - New tag is larger than last tag in array:
//    __tagn = 10
//    _reln = 6
//    tags-_reln-1 = 7-6-1 = 0
//
//  Dry test result: Works!

uint32_t* GTag::Add(uint32_t __tagn)
{

    // Find closest tag number
    uint _reln = ToReln(__tagn, TAGN_CLOSEST);

    // Do we have it already?
    if((_reln == RELN_INVALID) or (tag[_reln-1] != __tagn))
    {

        // Resize tag array to make room for the new number
        Resize(tags+1);

        //  General rule:
        //    - tag[_reln-1] is smaller than __tagn
        //
        //  The exceptions to the rule:
        //    - no tags in array (if _reln == RELN_INVALID)
        //    - tag[_reln-1] is larger than __tagn

        if(_reln and (tag[_reln-1] > __tagn))
            _reln--;

        // Move data to make room for the new tag number
        memmove(tag+_reln+1, tag+_reln, (tags-_reln-1)*sizeof(uint32_t));

        // Copy the new tag number to the insert position
        tag[_reln] = __tagn;
    }

    return tag;
}


//  ------------------------------------------------------------------
//  Delete a tag number from the tag array.
//  Returns the relative tag number or RELN_INVALID if missing.
//  NOTE - Does not resize the array.

uint GTag::Del(uint32_t __tagn)
{

    return DelReln(ToReln(__tagn));
}


//  ------------------------------------------------------------------

uint GTag::DelReln(uint __reln)
{

    if(__reln)
    {
        memmove(tag+__reln-1, tag+__reln, (tags-__reln)*sizeof(uint32_t));
        count--;
        tags--;
    }
    return __reln;
}


//  ------------------------------------------------------------------

uint GTag::DelResize(uint32_t __tagn)
{

    uint _reln = Del(__tagn);
    Resize(tags);
    return _reln;
}


//  ------------------------------------------------------------------

static int TagnCmp(const uint32_t* __a, const uint32_t* __b)
{

    return CmpV(*__a, *__b);
}


//  ------------------------------------------------------------------

void GTag::Sort()
{

    qsort(tag, tags, sizeof(uint32_t), (StdCmpCP)TagnCmp);
}

//  ------------------------------------------------------------------

void GTag::ElimDups()
{

    if(tags > 1)
    {
        uint _before = tags;
        uint32_t _last = tag[0];
        for(uint n=1; n<tags; n++)
        {
            if(_last == tag[n])
            {
                memmove(&tag[n-1], &tag[n], (tags-n)*sizeof(uint32_t));
                tags--;
                n--;
            }
            _last = tag[n];
        }
        if(_before != tags)
            Resize(tags);
    }
}


//  ------------------------------------------------------------------
//  Return the tag number corresponding to a relative tag number.
//  Returns TAGN_INVALID, if the requested number did not exist.

uint32_t GTag::CvtReln(uint __reln)
{

    if(tag and __reln and (__reln <= tags))
        return tag[__reln-1];
    else
        return TAGN_INVALID;
}


//  ------------------------------------------------------------------
//  Return the relative tag number corresponding to a tag number.
//  Returns RELN_INVALID if the requested number did not exist.
//  If __closest is true, the closest smaller tag number is returned.
//  If no tags exist at all, RELN_INVALID is returned.

uint GTag::ToReln(uint32_t __tagn, int __closest)
{

    uint _lastreln = RELN_INVALID;

    if(tag)
    {

        if(__tagn and tags)
            if(__tagn > tag[tags-1])
                return __closest ? tags : RELN_INVALID;

        if(tags and __tagn)
        {

            int _mid;
            int _left = 0;
            int _right = tags;

            do
            {
                _mid = (_left+_right)/2;
                if(__tagn < tag[(uint)_mid])
                    _right = _mid - 1;
                else if(__tagn > tag[(uint)_mid])
                    _left = _mid + 1;
                else
                    return (uint)(_mid + 1);
            }
            while(_left < _right);

            _lastreln = (uint)(_left + 1);
            if(__tagn == tag[(uint)_left])
                return _lastreln;
        }
    }

    // Not found
    return __closest ? _lastreln : RELN_INVALID;
}


//  ------------------------------------------------------------------

uint GTag::ToReln(uint32_t __tagn)
{

    return ToReln(__tagn, TAGN_EXACT);
}


//  ------------------------------------------------------------------

void GTag::Load(gfile& fp)
{
    dword val;

    fp.Fread(&val, sizeof(dword));
    count = (uint) val;
    if (count)
    {
        Resize(count);
        fp.Fread(tag, sizeof(uint32_t), count);
    }
}


//  ------------------------------------------------------------------

void GTag::Save(gfile& fp)
{
    dword val = (dword) count;
    fp.Fwrite(&val, sizeof(dword));

    if (tag and count)
        fp.Fwrite(tag, sizeof(uint32_t), count);
}


//  ------------------------------------------------------------------
