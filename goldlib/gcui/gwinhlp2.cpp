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
//  GCUI: Golded+ Character-oriented User Interface.
//  Help file compiler.
//  ------------------------------------------------------------------

#include <gmemdbg.h>
#include <gutlmisc.h>
#include <gstrall.h>
#include <gwinall.h>
#include <gwinhelp.h>

//  ------------------------------------------------------------------

extern _help_t whelp;

//  ------------------------------------------------------------------

void whelpcompile(const char* helpfile, long& offset) {

  gfile ifp(helpfile, "rb");
  if (ifp.isopen())
  {
    ifp.SetvBuf();

    int count = 0;
    char buf[1024];
    while (ifp.Fgets(buf, sizeof(buf)))
    {
      if(strnieql(buf, "*B ", 3))
        count++;
    }
    ifp.Rewind();

    Hlpr* helpindex = (Hlpr*)throw_xcalloc(count+2, sizeof(Hlpr));

    long relative_offset = 0;

    whelp.fp->Fputs("*I\r\n");
    whelp.fp->Fwrite(helpindex, count+1, sizeof(Hlpr));
    whelp.fp->Fputs("\r\n\r\n");
    relative_offset += 4 + ((count+1)*sizeof(Hlpr)) + 4;

    int counter = 0;
    bool comment = true;
    while (ifp.Fgets(buf, sizeof(buf)))
    {
      if(strnieql(buf, "*B ", 3)) {
        comment = false;
        helpindex[counter].help = atow(buf+3);
        char* ptr = strchr(buf, ',');
        strbtrim(strcpy(helpindex[counter].category, ptr ? ptr+1 : ""));
        helpindex[counter].offset = relative_offset + strlen(buf);
        counter++;
      }
      if (not comment)
      {
        whelp.fp->Fputs(buf);
        relative_offset += strlen(buf);
      }
      if(strnieql(buf, "*E", 2))
        comment = true;
    }
    helpindex[counter].offset = -1L;

    whelp.fp->FseekSet(offset);
    whelp.fp->Fputs("*I\r\n");
    whelp.fp->Fwrite(helpindex, count+1, sizeof(Hlpr));
    offset += relative_offset;
    whelp.fp->FseekSet(offset);

    throw_xfree(helpindex);

    ifp.Fclose();
  }
} /* whelpcompile() */


//  ------------------------------------------------------------------
