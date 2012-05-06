//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  Copyright (C) 2012 Golded+ team
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Template handling definitions.
//  ------------------------------------------------------------------

// top lines of built-in template
#define TPL_BUILTIN_TOP_MAIN \
  "@header= @oecho (@caddr) @align{79}{=}\n" \
  "@header Msg  : @msgno of @msgs@align{44}@attr\n"

// second lines of built-in template, only for email/news areas
#define TPL_BUILTIN_TOP_INTERNET \
  "@header From : @ofrom@align{60}@odtime\n" \
  "@header To   : @oto\n"

// second lines of built-in template, only for FTN areas
#define TPL_BUILTIN_TOP_FTN_FROM \
  "@header From : @oname@align{44}@oaddr@align{60}@odtime\n"

// third lines of built-in template, only for FTN NETMAIL areas
#define TPL_BUILTIN_TOP_FTN_TO_NETMAIL \
  "@header To   : @dname@align{44}@daddr\n"

// third lines of built-in template, only for FTN non-NETMAIL areas
#define TPL_BUILTIN_TOP_FTN_TO_NOTNETMAIL \
  "@header To   : @dname\n" \

// alternate lines of built-in template, only for FTN (all areas)
#define TPL_BUILTIN_TOP_FTN \
  "@header From : @oname@align{44}@oaddr@align{60}@odtime\n" \
  "@net@header To   : @dname@align{44}@daddr\n" \
  "@echo@header To   : @dname\n" \
  "@local@header To   : @dname\n"


// rest lines of built-in template, for all cases
#define TPL_BUILTIN_BODY \
  "@header Subj : @subject\n" \
  "@header@align{79}{=}\n" \
  "@moved* Replying to a msg in @oecho (@odesc)\n@moved\n" \
  "@changed* Changed by @cname (@caddr), @cdate @ctime.\n" \
  "@changed\n" \
  "@forward* Forwarded from @oecho by @fname (@faddr).\n" \
  "@forward* Originally by: @oname (@oaddr), @odate @otime.\n" \
  "@forward* Originally to: @dname{}{}{all}.\n" \
  "@forward\n" \
  "@message\n" \
  "@forward\n" \
  "\n" \
  "   Hello @pseudo{}{}{everybody}!\n" \
  "@new\n" \
  "@position\n" \
  "@replyReplying to a msg dated @odate @otime, from @oname{me}{you} to @dname{me}{you}{all}.\n" \
  "@reply@position\n" \
  "@quoted@odate @otime, @oname{I}{you} wrote to @dname{me}{you}{all}:\n" \
  "@quoted@position\n" \
  "@comment@odate @otime, @oname{I}{you} wrote to @dname{me}{you}{all}:\n" \
  "@comment@position\n" \
  "@quotebuf\n" \
  "@quotebuf@odate @otime, @oname{I}{you} wrote to @dname{me}{you}{all}:\n" \
  "@quotebuf\n" \
  "@quote\n\n" \
  "@cfname\n\n" \
  "\n"

