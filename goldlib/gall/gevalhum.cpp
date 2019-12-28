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
//  Human (left-to-right) expression evaluator class.
//  ------------------------------------------------------------------

#include <gdefs.h>
#include <geval.h>
#include <iterator>


//  ------------------------------------------------------------------

gevalhum::gevalhum()
{

}


//  ------------------------------------------------------------------

gevalhum::~gevalhum()
{

}


//  ------------------------------------------------------------------

int gevalhum::evaluate()
{

    while(vstk.size() and ostk.size())
    {

        while(ostk.size())
        {

            std::vector<int>::iterator vptr = vstk.begin();
            std::vector<ops>::iterator optr = ostk.begin();

            while(optr < ostk.end())
            {

                if(optr < (ostk.end() - 1))
                {

                    if(*optr == parenthesis_left)
                    {
                        if(optr[1] == parenthesis_right)
                        {
                            ostk.erase(optr, optr+2);
                            break;
                        }
                        else
                        {
                            optr++;
                        }
                        continue;
                    }
                    else if(((*optr == negation) or (*optr == logic_not)) and (optr[1] == parenthesis_left))
                    {
                        optr++;
                        continue;
                    }
                    else if((*optr < optr[1]) and (optr[1] != parenthesis_right))
                    {
                        optr++;
                        vptr++;
                        continue;
                    }
                }

                if(ostk.size())
                {
                    if((*optr == negation) or (*optr == logic_not))
                        *vptr = evaluate_ops(optr, vptr, vptr);
                    else
                    {
                        *vptr = evaluate_ops(optr, vptr, vptr+1);
                        if(vptr+1 < vstk.end())
                            vstk.erase(vptr+1);
                    }
                    ostk.erase(optr);
                }
                break;
            }
        }
    }

    return pop_value();
}


//  ------------------------------------------------------------------
