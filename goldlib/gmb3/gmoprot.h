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
//  Messagebase function prototypes
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

#include <gdefs.h>

//  ------------------------------------------------------------------

void EzycomInit(const char* msgbasepath, const char* userbasepath, int userno);
void EzycomExit();

void EzycomWideOpen();
void EzycomWideClose();


//  ------------------------------------------------------------------

void FidoInit(const char* fidolastread, int fidohwmarks, int fidonullfix, int fidouserno, const char* squishuserpath);
void FidoExit();
void FidoRenumberProgress(const char* s);

void FidoWideOpen();
void FidoWideClose();


//  ------------------------------------------------------------------

void GoldInit(const char* path, const char* syspath, int userno, long sizewarn=0, int ra2usersbbs=0);
void GoldExit();

void GoldWideOpen();
void GoldWideClose();


//  ------------------------------------------------------------------

void HudsInit(const char* path, const char* syspath, int userno, long sizewarn, int ra2usersbbs);
void HudsExit();
void HudsSizewarn();

void HudsWideOpen();
void HudsWideClose();

void HGWarnRebuild();


//  ------------------------------------------------------------------

void JamInit(const char* jampath, int harddelete, int smapihw);
void JamExit();

void JamWideOpen();
void JamWideClose();


//  ------------------------------------------------------------------

void PcbInit(const char* path, int userno);
void PcbExit();
void PcbAdjustArea(uint rec, const char* msgfile);

void PcbWideOpen();
void PcbWideClose();


//  ------------------------------------------------------------------

void SquishInit(const char* userpath, int userno, int direct, int recycle, int squishscan);
void SquishExit();


//  ------------------------------------------------------------------

void SMBInit();
void SMBExit();


//  ------------------------------------------------------------------

void WCatInit(int userno);
void WCatExit();

void WCatWideOpen();
void WCatWideClose();


//  ------------------------------------------------------------------

void XbbsInit(const char* path, int userno);
void XbbsExit();

void XbbsWideOpen();
void XbbsWideClose();


//  ------------------------------------------------------------------
//  When not in OS/2, use DUMMY AdeptXBBS functions

#if not defined(__OS2__)

#ifndef GMB_NOXBBS
inline void XbbsInit(const char*, int) { }
inline void XbbsExit() { }

inline void XbbsWideOpen() { }
inline void XbbsWideClose() { }
#endif

#endif


//  ------------------------------------------------------------------

