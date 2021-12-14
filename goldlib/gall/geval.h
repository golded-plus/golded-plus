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
//  Expression evaluator classes.
//  ------------------------------------------------------------------

#ifndef __geval_h
#define __geval_h


//  ------------------------------------------------------------------

#include <vector>
#include <gdefs.h>


//  ------------------------------------------------------------------
//  Base class

class geval
{

public:

    enum ops
    {
        lowest,
        logic_or,
        logic_and,
        logic_xor,
        not_equal,
        equal,
        greater_or_equal,
        greater,
        lesser_or_equal,
        lesser,
        subtraction,
        addition,
        modulus,
        division,
        multiplication,
        negation,
        logic_not,
        parenthesis_right,
        parenthesis_left,
        highest
    };

protected:

    std::vector<ops> ostk;
    std::vector<int> vstk;

    ops pop_operator();
    int pop_value();

public:

    geval();
    ~geval();

    void push_value(int v);
    void push_operator(ops o);

    int evaluate_op(ops o, int y, int x);
    int evaluate_ops(std::vector<ops>::iterator o, std::vector<int>::iterator y, std::vector<int>::iterator x);

};


//  ------------------------------------------------------------------

inline int geval::evaluate_ops(std::vector<ops>::iterator o, std::vector<int>::iterator y, std::vector<int>::iterator x)
{

    return evaluate_op(*o, *y, *x);
}


//  ------------------------------------------------------------------
//  Reverse Polish Notation expression evaluator.

class gevalrpn : public geval
{

public:

    gevalrpn();
    ~gevalrpn();

    int evaluate();

};


//  ------------------------------------------------------------------
//  Human (left-to-right) expression evaluator.

class gevalhum : public geval
{

public:

    gevalhum();
    ~gevalhum();

    int evaluate();

};


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
