//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999 Alexander Batalov
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
//  Some os2 pm func.
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gmemdbg.h>
#include <gutlos.h>

#define INCL_DOSMODULEMGR
#define INCL_WINWINDOWMGR
#define INCL_DOSPROCESS
#define INCL_WIN
#define INCL_OS2MM
#define INCL_WINFRAMEMGR
#include <os2.h>
#include <os2me.h>

//  ------------------------------------------------------------------

HAB      APIENTRY (*pfnWinInitialize)(ULONG);
BOOL     APIENTRY (*pfnWinTerminate)(HAB);
HMQ      APIENTRY (*pfnWinCreateMsgQueue)(HAB, LONG);
BOOL     APIENTRY (*pfnWinDestroyMsgQueue)(HMQ);
BOOL     APIENTRY (*pfnWinEmptyClipbrd)(HAB);
BOOL     APIENTRY (*pfnWinOpenClipbrd)(HAB);
BOOL     APIENTRY (*pfnWinCloseClipbrd)(HAB);
BOOL     APIENTRY (*pfnWinSetClipbrdData)(HAB, ULONG, ULONG, ULONG);
ULONG    APIENTRY (*pfnWinQueryClipbrdData)(HAB, ULONG);
ULONG    APIENTRY (*pfnWinChangeSwitchEntry)(HSWITCH, SWCNTRL *);
BOOL     APIENTRY (*pfnWinSetWindowText)(HWND, PSZ);
BOOL     APIENTRY (*pfnWinQueryClipbrdFmtInfo)(HAB, ULONG, PULONG);
ULONG    APIENTRY (*pfnWinQuerySwitchEntry)(HSWITCH, PSWCNTRL);
HSWITCH  APIENTRY (*pfnWinQuerySwitchHandle)(HWND, PID);
LONG     APIENTRY (*pfnWinQueryWindowText)(HWND, LONG, char *);
MRESULT  APIENTRY (*pfnWinSendMsg)(HWND, ULONG, MPARAM, MPARAM);
HPOINTER APIENTRY (*pfnWinLoadPointer)(HWND, HMODULE, ULONG);
BOOL     APIENTRY (*pfnWinInvalidateRect)(HWND, PRECTL, BOOL);
DWORD    APIENTRY (*pfnmciSendString)(LPSTR, LPSTR, WORD, HWND, WORD);
DWORD    APIENTRY (*pfnmciGetErrorString)(DWORD, LPSTR, WORD);

static BOOL       access_pm_clipboard(void);
static void       leave_pm_clipboard(int mode);


//  ------------------------------------------------------------------

HAB               ge_os2_hab = 0;
HMQ               ge_os2_hmq = 0;
HSWITCH           ge_os2_hsw = 0;
HWND              ge_os2_hwndframe = 0;
HMODULE           ge_os2_hmte = 0;
HMODULE           ge_os2_hmte2 = 0;
HMODULE           ge_os2_mdmHandle = 0;
SWCNTRL           ge_os2_swolddata;
SWCNTRL           ge_os2_swdata;
SZ                ge_os2_loaderr[256];
char              ge_os2_coldtitle[GMAXTITLE] = "";
HPOINTER          ge_os2_coldicon;
char              ge_os2_title[GMAXTITLE] = "";
int               ge_os2_ext_title;
ULONG             ge_os2_oldicon;


//  ------------------------------------------------------------------

static struct {
  PPIB ppib;
  HAB hab;
  HMQ hmq;
  ULONG savedtype;
  BOOL opened;
} pminfo;


//  ------------------------------------------------------------------
// Init os/2 pm functions

int g_init_os(int flags) {

  int rc;
  PTIB ptib;

  rc = DosQueryModuleHandle((PSZ)"PMWIN", &ge_os2_hmte);
  memset(&pminfo, 0, sizeof (pminfo));

  DosGetInfoBlocks(&ptib, &pminfo.ppib);
  pminfo.savedtype = pminfo.ppib->pib_ultype;

  // Morph application into PM
  if(not rc) {
    pminfo.ppib->pib_ultype = 3;
    rc = DosLoadModule((PSZ)ge_os2_loaderr, sizeof(ge_os2_loaderr), (PSZ)"PMWIN", &ge_os2_hmte);
    if(rc)
      return 1;

    // Load the functions dinamically
    rc = DosQueryProcAddr(ge_os2_hmte, 707, 0, (PFN*)&pfnWinCloseClipbrd);
    rc = DosQueryProcAddr(ge_os2_hmte, 716, 0, (PFN*)&pfnWinCreateMsgQueue);
    rc = DosQueryProcAddr(ge_os2_hmte, 726, 0, (PFN*)&pfnWinDestroyMsgQueue);
    rc = DosQueryProcAddr(ge_os2_hmte, 733, 0, (PFN*)&pfnWinEmptyClipbrd);
    rc = DosQueryProcAddr(ge_os2_hmte, 763, 0, (PFN*)&pfnWinInitialize);
    rc = DosQueryProcAddr(ge_os2_hmte, 793, 0, (PFN*)&pfnWinOpenClipbrd);
    rc = DosQueryProcAddr(ge_os2_hmte, 806, 0, (PFN*)&pfnWinQueryClipbrdData);
    rc = DosQueryProcAddr(ge_os2_hmte, 807, 0, (PFN*)&pfnWinQueryClipbrdFmtInfo);
    rc = DosQueryProcAddr(ge_os2_hmte, 854, 0, (PFN*)&pfnWinSetClipbrdData);
    rc = DosQueryProcAddr(ge_os2_hmte, 888, 0, (PFN*)&pfnWinTerminate);
    rc = DosQueryProcAddr(ge_os2_hmte, 841, 0, (PFN*)&pfnWinQueryWindowText);
    rc = DosQueryProcAddr(ge_os2_hmte, 877, 0, (PFN*)&pfnWinSetWindowText);
    rc = DosQueryProcAddr(ge_os2_hmte, 920, 0, (PFN*)&pfnWinSendMsg);
    rc = DosQueryProcAddr(ge_os2_hmte, 780, 0, (PFN*)&pfnWinLoadPointer);
    rc = DosQueryProcAddr(ge_os2_hmte, 765, 0, (PFN*)&pfnWinInvalidateRect);

    rc = DosQueryModuleHandle((PSZ)"PMSHAPI", &ge_os2_hmte2);
    if(rc)
      return 1;
    rc = DosLoadModule((PSZ)ge_os2_loaderr, sizeof(ge_os2_loaderr), (PSZ)"PMSHAPI", &ge_os2_hmte2);
    if(rc)
      return 1;
    DosQueryProcAddr(ge_os2_hmte2, 123, 0, (PFN*)&pfnWinChangeSwitchEntry);
    DosQueryProcAddr(ge_os2_hmte2, 124, 0, (PFN*)&pfnWinQuerySwitchEntry);
    DosQueryProcAddr(ge_os2_hmte2, 125, 0, (PFN*)&pfnWinQuerySwitchHandle);

    if(flags & 1) {
      rc = DosQueryModuleHandle((PSZ)"MDM", &ge_os2_hmte);
      if(not rc) {
        rc = DosLoadModule((PSZ)ge_os2_loaderr, sizeof(ge_os2_loaderr), (PSZ)"MDM", &ge_os2_mdmHandle);
        if(not rc) {
          DosQueryProcAddr(ge_os2_mdmHandle, 0, (PSZ)"mciSendString", (PFN*)&pfnmciSendString);
          DosQueryProcAddr(ge_os2_mdmHandle, 0, (PSZ)"mciGetErrorString", (PFN*)&pfnmciGetErrorString);
        }
      }
    }

    // Make queue
    ge_os2_hab = pfnWinInitialize(0);
    ge_os2_hmq = pfnWinCreateMsgQueue(ge_os2_hab, 0);

    // Find our hSw
    ge_os2_hsw = pfnWinQuerySwitchHandle(0, pminfo.ppib->pib_ulpid);
    if(ge_os2_hsw) {
      pfnWinQuerySwitchEntry(ge_os2_hsw, &ge_os2_swolddata);
      ge_os2_swdata = ge_os2_swolddata;
      ge_os2_hwndframe = ge_os2_swdata.hwnd;
      pfnWinQueryWindowText(ge_os2_hwndframe, sizeof(ge_os2_coldtitle), ge_os2_coldtitle);
    }
  }
  DosSetPriority(PRTYS_PROCESS, PRTYC_REGULAR, PRTYD_MINIMUM, 0);
  return 0;
}


//  ------------------------------------------------------------------
// deInit os/2 pm functions

void g_deinit_os(void) {

  if(ge_os2_hwndframe) {
    pfnWinChangeSwitchEntry(ge_os2_hsw, &ge_os2_swolddata);
    g_set_ostitle(ge_os2_coldtitle);
    pfnWinSendMsg(ge_os2_hwndframe, WM_SETICON, (MPARAM) ge_os2_swolddata.hwndIcon, (MPARAM) 0);
  }
  if(ge_os2_hmq)
    pfnWinDestroyMsgQueue(ge_os2_hmq);
  if(ge_os2_hab)
    pfnWinTerminate(ge_os2_hab);
  if(ge_os2_hmte)
    DosFreeModule(ge_os2_hmte);
  if(ge_os2_hmte2)
    DosFreeModule(ge_os2_hmte2);
  if(ge_os2_mdmHandle)
    DosFreeModule(ge_os2_mdmHandle);
}


//  ------------------------------------------------------------------

void g_init_title(char *tasktitle, int titlestatus) {

  strxcpy(ge_os2_title, tasktitle, GMAXTITLE);
  ge_os2_ext_title = titlestatus;
}


//  ------------------------------------------------------------------

void g_increase_priority(void) {

  DosSetPriority(PRTYS_PROCESS, PRTYC_REGULAR, PRTYD_MAXIMUM, 0);
}


//  ------------------------------------------------------------------
// Change title

void g_set_ostitle(char *title) {

  if(ge_os2_hsw && ge_os2_hwndframe) {
    strxcpy(ge_os2_swdata.szSwtitle, title, MAXNAMEL);
    pfnWinChangeSwitchEntry(ge_os2_hsw, &ge_os2_swdata);
    pfnWinSetWindowText(ge_os2_hwndframe, (PSZ)title);
  }
}


//  ------------------------------------------------------------------
// Change icon

void g_set_osicon(void) {

  if(ge_os2_hwndframe && ge_os2_hsw) {
    ULONG ulPicture = (ULONG) pfnWinLoadPointer(HWND_DESKTOP, 0, 1);
    if(ulPicture) {
      pfnWinSendMsg(ge_os2_hwndframe, WM_SETICON, (MPARAM) ulPicture, (MPARAM) 0);
      pfnWinInvalidateRect(ge_os2_hwndframe, 0, 1);
      pfnWinChangeSwitchEntry(ge_os2_hsw, &ge_os2_swdata);
    }
  }
}


//  ------------------------------------------------------------------

bool g_is_clip_available(void) {

  return (ge_os2_hab) ? true : false;
}


//  ------------------------------------------------------------------

char *g_get_clip_text(void) {

  char *text = NULL;
  char *cd = NULL;
  BOOL clip_open = false;
  if(ge_os2_hab) {
    if((clip_open = access_pm_clipboard()) != true)
      return NULL;
    if((text = (char *) pfnWinQueryClipbrdData(ge_os2_hab, CF_TEXT)) != NULL)
      cd = throw_strdup(text);
    leave_pm_clipboard(clip_open);
  }
  return cd;
}


//  ------------------------------------------------------------------

void g_get_ostitle_name(char *currtitle) {

  if(ge_os2_hwndframe)
    pfnWinQueryWindowText(ge_os2_hwndframe, GMAXTITLE, currtitle);
}


//  ------------------------------------------------------------------

void g_set_ostitle_name(char *title, int mode) {

  if(mode == 0) {
    char fulltitle[80];
    strcpy(fulltitle, ge_os2_title);
    if(ge_os2_ext_title) {
      int len = strlen(fulltitle);
      if(len < GMAXTITLE-4) {
        if(len)
          strcat(fulltitle, " - ");
        strxcpy(fulltitle+len+3, title, GMAXTITLE-len-3);
      }
    }
    g_set_ostitle(fulltitle);
  }
  else
    g_set_ostitle(title);
}


//  ------------------------------------------------------------------

static BOOL access_pm_clipboard(void) {

  BOOL clip_opened = false;
  if(pfnWinOpenClipbrd(ge_os2_hab) == true)
    clip_opened = true;
  else
    leave_pm_clipboard(clip_opened);
  return clip_opened;
}


//  ------------------------------------------------------------------

static void leave_pm_clipboard(int mode) {

  if(mode == true)
    pfnWinCloseClipbrd(ge_os2_hab);
}


//  ------------------------------------------------------------------

int g_put_clip_text(char *cd) {

  ULONG len;
  void *text;
  int rc = -1;
  BOOL clip_open = false;
  if(ge_os2_hab) {
    if((clip_open = access_pm_clipboard()) != true)
      return rc;
    pfnWinEmptyClipbrd(ge_os2_hab);
    len = strlen(cd);

    if(len) {
      DosAllocSharedMem((void **) &text, 0, len + 1, PAG_READ | PAG_WRITE | PAG_COMMIT | OBJ_GIVEABLE);
      strxcpy((char *) text, cd, len + 1);
      if(pfnWinSetClipbrdData(ge_os2_hab, (ULONG) text, CF_TEXT, CFI_POINTER))
        rc = 0;
    }
    leave_pm_clipboard(clip_open);
  }
  return rc;
}


//  ------------------------------------------------------------------
