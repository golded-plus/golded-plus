
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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
//  Derived menu classes.
//  ------------------------------------------------------------------
#ifndef __GEMNUS_H
#define __GEMNUS_H
//  ------------------------------------------------------------------

#include <gmnubase.h>


//  ------------------------------------------------------------------
//  General menu with ASK colors

class GMnuAsk : public GMnu {

public:
  void AskInit(int __row, int __col, char* __title, int __help);
};


//  ------------------------------------------------------------------
//  General YES/NO selection menu

class GMnuYesNo : public GMnuAsk {

public:
  int YesNo(int __row, int __col, char* __title, char* __yes, char* __no, int __help, int __yesno=YES);
};


//  ------------------------------------------------------------------

class GMenuQuit : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuReadonly : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuChange : public GMnuYesNo {

public:
  int Run(char* info);
};


//  ------------------------------------------------------------------

class GMenuLockLoop : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuDelorig : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuDropmsg : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuZonegate : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuForward : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuOverwrite : public GMnuAsk {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuDelete : public GMnuAsk {

public:
  int Run(int all, GMsg* __msg);
};


//  ------------------------------------------------------------------

class GMenuNewarea : public GMnuAsk {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuCMF : public GMnuAsk {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuDomarks : public GMnuAsk {

public:
  int Run(char* dowhat);
};


//  ------------------------------------------------------------------

class GMenuWriteMsg : public GMnuAsk {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuImportTxt : public GMnuAsk {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuMarkMsgs : public GMnuAsk {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuEditfile : public GMnuAsk {

public:
  int Run(GMsg* __msg);
};


//  ------------------------------------------------------------------

class GMenuCarbon : public GMnuAsk {

public:
  int Run(GMsg* __msg);
};


//  ------------------------------------------------------------------

class GMenuCross : public GMnuAsk {

public:
  int Run(GMsg* __msg);
};


//  ------------------------------------------------------------------

class GMnuAreaDo : public GMnuAsk {

public:
  int With(char* __title, char* __all, char* __marked, char* __current, int __full, char* __no, int __help);
};


//  ------------------------------------------------------------------

class GMenuAreaScan : public GMnuAreaDo {

public:
  int Run(int pmscan);
};


//  ------------------------------------------------------------------

class GMenuAreaHeat : public GMnuAreaDo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuAreaZap : public GMnuAreaDo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuAreaCatchup : public GMnuAreaDo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuAreaDropMarks : public GMnuAreaDo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuEditHeader : public GMnu {

public:
  int Run(int mode, GMsg* msg);
};


//  ------------------------------------------------------------------

class GMenuConfirm : public GMnuYesNo {

public:
  int Run();
};


//  ------------------------------------------------------------------

class GMenuNavigate : public GMnu {

public:
  int Run();
};


//  ------------------------------------------------------------------

#if defined(GCFG_SPELL_INCLUDED)
class GMenuSChecker : public GMnu
{
public:
  int Run(CSpellChecker &schecker, const char *word);
};
#endif


//  ------------------------------------------------------------------

#endif
