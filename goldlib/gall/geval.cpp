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
//  Expression evaluator base class.
//  ------------------------------------------------------------------

#include <gdefs.h>
#include <geval.h>


//  ------------------------------------------------------------------

geval::geval() {

  // nothing to do
}


//  ------------------------------------------------------------------

geval::~geval() {

  // nothing
}


//  ------------------------------------------------------------------

geval::ops geval::pop_operator() {

  ops o = ostk.back();
  ostk.pop_back();
  return o;
}


//  ------------------------------------------------------------------

int geval::pop_value() {

  int v = vstk.back();
  vstk.pop_back();
  return v;
}


//  ------------------------------------------------------------------

void geval::push_value(int v) {

  vstk.push_back(v);
}


//  ------------------------------------------------------------------

void geval::push_operator(geval::ops o) {

  ostk.push_back(o);
}


//  ------------------------------------------------------------------

int geval::evaluate_op(geval::ops o, int y, int x) {

  switch(o) {
    case addition:          return y + x;
    case subtraction:       return y - x;
    case multiplication:    return y * x;
    case division:          return y / x;
    case modulus:           return y % x;
    case negation:          return -x;
    case logic_not:         return (not x) ? 1 : 0;
    case logic_and:         return (y and x) ? 1 : 0;
    case logic_xor:         return (y ^ x) ? 1 : 0;
    case equal:             return (y == x) ? 1 : 0;
    case not_equal:         return (y != x) ? 1 : 0;
    case greater:           return (y > x) ? 1 : 0;
    case greater_or_equal:  return (y >= x) ? 1 : 0;
    case lesser:            return (y < x) ? 1 : 0;
    case lesser_or_equal:   return (y <= x) ? 1 : 0;
    default:                ;
  }

  return 0;
}


//  ------------------------------------------------------------------
