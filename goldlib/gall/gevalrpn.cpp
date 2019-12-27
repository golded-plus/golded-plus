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
//  Reverse Polish Notation expression evaluator class.
//  ------------------------------------------------------------------

#include <geval.h>


//  ------------------------------------------------------------------

gevalrpn::gevalrpn()
{

}


//  ------------------------------------------------------------------

gevalrpn::~gevalrpn()
{

}


//  ------------------------------------------------------------------

int gevalrpn::evaluate()
{

    ops o;
    int x, y=0;

    while(ostk.size())
    {
        o = pop_operator();
        switch(o)
        {
        case negation:
        case logic_not:
            x = pop_value();
            break;
        default:
            x = pop_value();
            y = pop_value();
        }
        push_value(evaluate_op(o, y, x));
    }

    return pop_value();
}


//  ------------------------------------------------------------------
