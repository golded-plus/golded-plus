//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
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
//  XXX
//  ------------------------------------------------------------------

#ifndef __gutlos_h
#define __gutlos_h


//  ------------------------------------------------------------------

#include <gdefs.h>


//  ------------------------------------------------------------------

#if defined(__WIN32__) or defined(__OS2__) or defined(__DJGPP__)


//  ------------------------------------------------------------------

#define GUTLOS_FUNCS 1

const int GMAXTITLE = 60;
const int BUFFERSIZE = 128;

//  ------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

int    g_init_os(int flags);
void   g_deinit_os(void);

void   g_increase_priority(void);

int    g_send_mci_string(char*, char*);

void   g_init_title(char *, int);
void   g_set_ostitle(char *);
void   g_set_osicon(void);

bool   g_is_clip_available(void);
char*  g_get_clip_text(void);
int    g_put_clip_text(char *cd);

void   g_get_ostitle_name(char *);
void   g_set_ostitle_name(char *, int);

char   g_tolower(char);
char   g_toupper(char);

#ifdef __cplusplus
}
#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
