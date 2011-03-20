# Microsoft Developer Studio Generated NMAKE File, Based on goldlib.dsp
!IF "$(CFG)" == ""
CFG=goldlib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to goldlib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "goldlib - Win32 Release" && "$(CFG)" != "goldlib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "goldlib.mak" CFG="goldlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "goldlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "goldlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "goldlib - Win32 Release"

OUTDIR=.\bin\release\win32
INTDIR=.\obj\release\win32
# Begin Custom Macros
OutDir=.\bin\release\win32
# End Custom Macros

ALL : "$(OUTDIR)\goldlib.lib"


CLEAN :
	-@erase "$(INTDIR)\affentry.obj"
	-@erase "$(INTDIR)\affixmgr.obj"
	-@erase "$(INTDIR)\csutil.obj"
	-@erase "$(INTDIR)\dictmgr.obj"
	-@erase "$(INTDIR)\fptools.obj"
	-@erase "$(INTDIR)\gbmh.obj"
	-@erase "$(INTDIR)\gcharset.obj"
	-@erase "$(INTDIR)\gcrc16tb.obj"
	-@erase "$(INTDIR)\gcrc32tb.obj"
	-@erase "$(INTDIR)\gcrchash.obj"
	-@erase "$(INTDIR)\gcrckeyv.obj"
	-@erase "$(INTDIR)\gcrcm16.obj"
	-@erase "$(INTDIR)\gcrcm32.obj"
	-@erase "$(INTDIR)\gcrcs16.obj"
	-@erase "$(INTDIR)\gcrcs32.obj"
	-@erase "$(INTDIR)\gdbgerr.obj"
	-@erase "$(INTDIR)\gdbgexit.obj"
	-@erase "$(INTDIR)\gdbgtrk.obj"
	-@erase "$(INTDIR)\gdirposx.obj"
	-@erase "$(INTDIR)\gedacfg.obj"
	-@erase "$(INTDIR)\gespell.obj"
	-@erase "$(INTDIR)\geval.obj"
	-@erase "$(INTDIR)\gevalhum.obj"
	-@erase "$(INTDIR)\gevalrpn.obj"
	-@erase "$(INTDIR)\gfile.obj"
	-@erase "$(INTDIR)\gfilport.obj"
	-@erase "$(INTDIR)\gfilutl1.obj"
	-@erase "$(INTDIR)\gfilutl2.obj"
	-@erase "$(INTDIR)\gftnaddr.obj"
	-@erase "$(INTDIR)\gftnnl.obj"
	-@erase "$(INTDIR)\gftnnlfd.obj"
	-@erase "$(INTDIR)\gftnnlfu.obj"
	-@erase "$(INTDIR)\gftnnlge.obj"
	-@erase "$(INTDIR)\gftnnlv7.obj"
	-@erase "$(INTDIR)\gfuzzy.obj"
	-@erase "$(INTDIR)\ghdrmime.obj"
	-@erase "$(INTDIR)\gkbdbase.obj"
	-@erase "$(INTDIR)\gkbdgetm.obj"
	-@erase "$(INTDIR)\gkbdwait.obj"
	-@erase "$(INTDIR)\glog.obj"
	-@erase "$(INTDIR)\gmemdbg.obj"
	-@erase "$(INTDIR)\gmemutil.obj"
	-@erase "$(INTDIR)\gmoarea.obj"
	-@erase "$(INTDIR)\gmoezyc1.obj"
	-@erase "$(INTDIR)\gmoezyc2.obj"
	-@erase "$(INTDIR)\gmoezyc3.obj"
	-@erase "$(INTDIR)\gmoezyc4.obj"
	-@erase "$(INTDIR)\gmoezyc5.obj"
	-@erase "$(INTDIR)\gmofido1.obj"
	-@erase "$(INTDIR)\gmofido2.obj"
	-@erase "$(INTDIR)\gmofido3.obj"
	-@erase "$(INTDIR)\gmofido4.obj"
	-@erase "$(INTDIR)\gmofido5.obj"
	-@erase "$(INTDIR)\gmohuds.obj"
	-@erase "$(INTDIR)\gmojamm1.obj"
	-@erase "$(INTDIR)\gmojamm2.obj"
	-@erase "$(INTDIR)\gmojamm3.obj"
	-@erase "$(INTDIR)\gmojamm4.obj"
	-@erase "$(INTDIR)\gmojamm5.obj"
	-@erase "$(INTDIR)\gmopcbd1.obj"
	-@erase "$(INTDIR)\gmopcbd2.obj"
	-@erase "$(INTDIR)\gmopcbd3.obj"
	-@erase "$(INTDIR)\gmopcbd4.obj"
	-@erase "$(INTDIR)\gmopcbd5.obj"
	-@erase "$(INTDIR)\gmosmb1.obj"
	-@erase "$(INTDIR)\gmosmb2.obj"
	-@erase "$(INTDIR)\gmosqsh1.obj"
	-@erase "$(INTDIR)\gmosqsh2.obj"
	-@erase "$(INTDIR)\gmosqsh3.obj"
	-@erase "$(INTDIR)\gmosqsh4.obj"
	-@erase "$(INTDIR)\gmosqsh5.obj"
	-@erase "$(INTDIR)\gmoubase.obj"
	-@erase "$(INTDIR)\gmowcat1.obj"
	-@erase "$(INTDIR)\gmowcat2.obj"
	-@erase "$(INTDIR)\gmowcat3.obj"
	-@erase "$(INTDIR)\gmowcat4.obj"
	-@erase "$(INTDIR)\gmowcat5.obj"
	-@erase "$(INTDIR)\gmoxbbs1.obj"
	-@erase "$(INTDIR)\gmoxbbs2.obj"
	-@erase "$(INTDIR)\gmoxbbs3.obj"
	-@erase "$(INTDIR)\gmoxbbs4.obj"
	-@erase "$(INTDIR)\gmoxbbs5.obj"
	-@erase "$(INTDIR)\gmsgattr.obj"
	-@erase "$(INTDIR)\gprnutil.obj"
	-@erase "$(INTDIR)\gregex.obj"
	-@erase "$(INTDIR)\gsearch.obj"
	-@erase "$(INTDIR)\gsnd.obj"
	-@erase "$(INTDIR)\gsndwrap.obj"
	-@erase "$(INTDIR)\gsrchmgr.obj"
	-@erase "$(INTDIR)\gstrbags.obj"
	-@erase "$(INTDIR)\gstrctyp.obj"
	-@erase "$(INTDIR)\gstrmail.obj"
	-@erase "$(INTDIR)\gstrname.obj"
	-@erase "$(INTDIR)\gstrutil.obj"
	-@erase "$(INTDIR)\gtimjuld.obj"
	-@erase "$(INTDIR)\gtimutil.obj"
	-@erase "$(INTDIR)\gtxtpara.obj"
	-@erase "$(INTDIR)\gusrbase.obj"
	-@erase "$(INTDIR)\gusrezyc.obj"
	-@erase "$(INTDIR)\gusrgold.obj"
	-@erase "$(INTDIR)\gusrhuds.obj"
	-@erase "$(INTDIR)\gusrmax.obj"
	-@erase "$(INTDIR)\gusrpcb.obj"
	-@erase "$(INTDIR)\gusrra2.obj"
	-@erase "$(INTDIR)\gusrxbbs.obj"
	-@erase "$(INTDIR)\gutlclip.obj"
	-@erase "$(INTDIR)\gutlcode.obj"
	-@erase "$(INTDIR)\gutlgrp.obj"
	-@erase "$(INTDIR)\gutlmisc.obj"
	-@erase "$(INTDIR)\gutlmtsk.obj"
	-@erase "$(INTDIR)\gutltag.obj"
	-@erase "$(INTDIR)\gutlvers.obj"
	-@erase "$(INTDIR)\gutlwin.obj"
	-@erase "$(INTDIR)\gutlwinm.obj"
	-@erase "$(INTDIR)\gvidbase.obj"
	-@erase "$(INTDIR)\gvidinit.obj"
	-@erase "$(INTDIR)\gwildmat.obj"
	-@erase "$(INTDIR)\gwinbase.obj"
	-@erase "$(INTDIR)\gwindow.obj"
	-@erase "$(INTDIR)\gwinhlp1.obj"
	-@erase "$(INTDIR)\gwinhlp2.obj"
	-@erase "$(INTDIR)\gwininit.obj"
	-@erase "$(INTDIR)\gwinline.obj"
	-@erase "$(INTDIR)\gwinmenu.obj"
	-@erase "$(INTDIR)\gwinmnub.obj"
	-@erase "$(INTDIR)\gwinpckf.obj"
	-@erase "$(INTDIR)\gwinpcks.obj"
	-@erase "$(INTDIR)\gwinpick.obj"
	-@erase "$(INTDIR)\gwinput2.obj"
	-@erase "$(INTDIR)\gxareas.obj"
	-@erase "$(INTDIR)\gxcrash.obj"
	-@erase "$(INTDIR)\gxdb.obj"
	-@erase "$(INTDIR)\gxdutch.obj"
	-@erase "$(INTDIR)\gxezy102.obj"
	-@erase "$(INTDIR)\gxezy110.obj"
	-@erase "$(INTDIR)\gxfd.obj"
	-@erase "$(INTDIR)\gxfecho4.obj"
	-@erase "$(INTDIR)\gxfecho5.obj"
	-@erase "$(INTDIR)\gxfecho6.obj"
	-@erase "$(INTDIR)\gxfidpcb.obj"
	-@erase "$(INTDIR)\gxfm092.obj"
	-@erase "$(INTDIR)\gxfm100.obj"
	-@erase "$(INTDIR)\gxfm116.obj"
	-@erase "$(INTDIR)\gxgecho.obj"
	-@erase "$(INTDIR)\gxhpt.obj"
	-@erase "$(INTDIR)\gximail4.obj"
	-@erase "$(INTDIR)\gximail5.obj"
	-@erase "$(INTDIR)\gximail6.obj"
	-@erase "$(INTDIR)\gxinter.obj"
	-@erase "$(INTDIR)\gxlora.obj"
	-@erase "$(INTDIR)\gxmax3.obj"
	-@erase "$(INTDIR)\gxme2.obj"
	-@erase "$(INTDIR)\gxopus.obj"
	-@erase "$(INTDIR)\gxpcb.obj"
	-@erase "$(INTDIR)\gxportal.obj"
	-@erase "$(INTDIR)\gxprobrd.obj"
	-@erase "$(INTDIR)\gxqecho.obj"
	-@erase "$(INTDIR)\gxqfront.obj"
	-@erase "$(INTDIR)\gxquick.obj"
	-@erase "$(INTDIR)\gxra.obj"
	-@erase "$(INTDIR)\gxraecho.obj"
	-@erase "$(INTDIR)\gxspace.obj"
	-@erase "$(INTDIR)\gxsquish.obj"
	-@erase "$(INTDIR)\gxsuper.obj"
	-@erase "$(INTDIR)\gxsync.obj"
	-@erase "$(INTDIR)\gxtimed.obj"
	-@erase "$(INTDIR)\gxtmail.obj"
	-@erase "$(INTDIR)\gxts.obj"
	-@erase "$(INTDIR)\gxwmail.obj"
	-@erase "$(INTDIR)\gxwtr.obj"
	-@erase "$(INTDIR)\gxxbbs.obj"
	-@erase "$(INTDIR)\gxxmail.obj"
	-@erase "$(INTDIR)\hashmgr.obj"
	-@erase "$(INTDIR)\hunspell.obj"
	-@erase "$(INTDIR)\lzh.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\smblib.obj"
	-@erase "$(INTDIR)\suggmgr.obj"
	-@erase "$(INTDIR)\uucheck.obj"
	-@erase "$(INTDIR)\uuencode.obj"
	-@erase "$(INTDIR)\uulib.obj"
	-@erase "$(INTDIR)\uunconc.obj"
	-@erase "$(INTDIR)\uuscan.obj"
	-@erase "$(INTDIR)\uustring.obj"
	-@erase "$(INTDIR)\uuutil.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\goldlib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /Zi /O1 /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /I "..\goldlib" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "HAVE_STDARG_H" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /c

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\goldlib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\goldlib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gbmh.obj" \
	"$(INTDIR)\gcharset.obj" \
	"$(INTDIR)\gcrc16tb.obj" \
	"$(INTDIR)\gcrc32tb.obj" \
	"$(INTDIR)\gcrchash.obj" \
	"$(INTDIR)\gcrckeyv.obj" \
	"$(INTDIR)\gcrcm16.obj" \
	"$(INTDIR)\gcrcm32.obj" \
	"$(INTDIR)\gcrcs16.obj" \
	"$(INTDIR)\gcrcs32.obj" \
	"$(INTDIR)\gdbgerr.obj" \
	"$(INTDIR)\gdbgexit.obj" \
	"$(INTDIR)\gdbgtrk.obj" \
	"$(INTDIR)\gdirposx.obj" \
	"$(INTDIR)\gespell.obj" \
	"$(INTDIR)\geval.obj" \
	"$(INTDIR)\gevalhum.obj" \
	"$(INTDIR)\gevalrpn.obj" \
	"$(INTDIR)\gfile.obj" \
	"$(INTDIR)\gfilport.obj" \
	"$(INTDIR)\gfilutl1.obj" \
	"$(INTDIR)\gfilutl2.obj" \
	"$(INTDIR)\gftnaddr.obj" \
	"$(INTDIR)\gftnnl.obj" \
	"$(INTDIR)\gftnnlfd.obj" \
	"$(INTDIR)\gftnnlfu.obj" \
	"$(INTDIR)\gftnnlge.obj" \
	"$(INTDIR)\gftnnlv7.obj" \
	"$(INTDIR)\gfuzzy.obj" \
	"$(INTDIR)\ghdrmime.obj" \
	"$(INTDIR)\gkbdbase.obj" \
	"$(INTDIR)\gkbdgetm.obj" \
	"$(INTDIR)\gkbdwait.obj" \
	"$(INTDIR)\glog.obj" \
	"$(INTDIR)\gmemdbg.obj" \
	"$(INTDIR)\gmemutil.obj" \
	"$(INTDIR)\gmoubase.obj" \
	"$(INTDIR)\gmsgattr.obj" \
	"$(INTDIR)\gprnutil.obj" \
	"$(INTDIR)\gregex.obj" \
	"$(INTDIR)\gsearch.obj" \
	"$(INTDIR)\gsnd.obj" \
	"$(INTDIR)\gsndwrap.obj" \
	"$(INTDIR)\gsrchmgr.obj" \
	"$(INTDIR)\gstrbags.obj" \
	"$(INTDIR)\gstrctyp.obj" \
	"$(INTDIR)\gstrmail.obj" \
	"$(INTDIR)\gstrname.obj" \
	"$(INTDIR)\gstrutil.obj" \
	"$(INTDIR)\gtimjuld.obj" \
	"$(INTDIR)\gtimutil.obj" \
	"$(INTDIR)\gtxtpara.obj" \
	"$(INTDIR)\gusrbase.obj" \
	"$(INTDIR)\gusrezyc.obj" \
	"$(INTDIR)\gusrgold.obj" \
	"$(INTDIR)\gusrhuds.obj" \
	"$(INTDIR)\gusrmax.obj" \
	"$(INTDIR)\gusrpcb.obj" \
	"$(INTDIR)\gusrra2.obj" \
	"$(INTDIR)\gusrxbbs.obj" \
	"$(INTDIR)\gutlclip.obj" \
	"$(INTDIR)\gutlcode.obj" \
	"$(INTDIR)\gutlgrp.obj" \
	"$(INTDIR)\gutlmisc.obj" \
	"$(INTDIR)\gutlmtsk.obj" \
	"$(INTDIR)\gutltag.obj" \
	"$(INTDIR)\gutlvers.obj" \
	"$(INTDIR)\gutlwin.obj" \
	"$(INTDIR)\gutlwinm.obj" \
	"$(INTDIR)\gvidbase.obj" \
	"$(INTDIR)\gvidinit.obj" \
	"$(INTDIR)\gwildmat.obj" \
	"$(INTDIR)\gwinbase.obj" \
	"$(INTDIR)\gwindow.obj" \
	"$(INTDIR)\gwinhlp1.obj" \
	"$(INTDIR)\gwinhlp2.obj" \
	"$(INTDIR)\gwininit.obj" \
	"$(INTDIR)\gwinline.obj" \
	"$(INTDIR)\gwinmenu.obj" \
	"$(INTDIR)\gwinmnub.obj" \
	"$(INTDIR)\gwinpckf.obj" \
	"$(INTDIR)\gwinpcks.obj" \
	"$(INTDIR)\gwinpick.obj" \
	"$(INTDIR)\gwinput2.obj" \
	"$(INTDIR)\gedacfg.obj" \
	"$(INTDIR)\gxareas.obj" \
	"$(INTDIR)\gxcrash.obj" \
	"$(INTDIR)\gxdb.obj" \
	"$(INTDIR)\gxdutch.obj" \
	"$(INTDIR)\gxezy102.obj" \
	"$(INTDIR)\gxezy110.obj" \
	"$(INTDIR)\gxfd.obj" \
	"$(INTDIR)\gxfecho4.obj" \
	"$(INTDIR)\gxfecho5.obj" \
	"$(INTDIR)\gxfecho6.obj" \
	"$(INTDIR)\gxfidpcb.obj" \
	"$(INTDIR)\gxfm092.obj" \
	"$(INTDIR)\gxfm100.obj" \
	"$(INTDIR)\gxfm116.obj" \
	"$(INTDIR)\gxgecho.obj" \
	"$(INTDIR)\gxhpt.obj" \
	"$(INTDIR)\gximail4.obj" \
	"$(INTDIR)\gximail5.obj" \
	"$(INTDIR)\gximail6.obj" \
	"$(INTDIR)\gxinter.obj" \
	"$(INTDIR)\gxlora.obj" \
	"$(INTDIR)\gxmax3.obj" \
	"$(INTDIR)\gxme2.obj" \
	"$(INTDIR)\gxopus.obj" \
	"$(INTDIR)\gxpcb.obj" \
	"$(INTDIR)\gxportal.obj" \
	"$(INTDIR)\gxprobrd.obj" \
	"$(INTDIR)\gxqecho.obj" \
	"$(INTDIR)\gxqfront.obj" \
	"$(INTDIR)\gxquick.obj" \
	"$(INTDIR)\gxra.obj" \
	"$(INTDIR)\gxraecho.obj" \
	"$(INTDIR)\gxspace.obj" \
	"$(INTDIR)\gxsquish.obj" \
	"$(INTDIR)\gxsuper.obj" \
	"$(INTDIR)\gxsync.obj" \
	"$(INTDIR)\gxtimed.obj" \
	"$(INTDIR)\gxtmail.obj" \
	"$(INTDIR)\gxts.obj" \
	"$(INTDIR)\gxwmail.obj" \
	"$(INTDIR)\gxwtr.obj" \
	"$(INTDIR)\gxxbbs.obj" \
	"$(INTDIR)\gxxmail.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\gmoarea.obj" \
	"$(INTDIR)\gmoezyc1.obj" \
	"$(INTDIR)\gmoezyc2.obj" \
	"$(INTDIR)\gmoezyc3.obj" \
	"$(INTDIR)\gmoezyc4.obj" \
	"$(INTDIR)\gmoezyc5.obj" \
	"$(INTDIR)\gmofido1.obj" \
	"$(INTDIR)\gmofido2.obj" \
	"$(INTDIR)\gmofido3.obj" \
	"$(INTDIR)\gmofido4.obj" \
	"$(INTDIR)\gmofido5.obj" \
	"$(INTDIR)\gmohuds.obj" \
	"$(INTDIR)\gmojamm1.obj" \
	"$(INTDIR)\gmojamm2.obj" \
	"$(INTDIR)\gmojamm3.obj" \
	"$(INTDIR)\gmojamm4.obj" \
	"$(INTDIR)\gmojamm5.obj" \
	"$(INTDIR)\gmopcbd1.obj" \
	"$(INTDIR)\gmopcbd2.obj" \
	"$(INTDIR)\gmopcbd3.obj" \
	"$(INTDIR)\gmopcbd4.obj" \
	"$(INTDIR)\gmopcbd5.obj" \
	"$(INTDIR)\gmosmb1.obj" \
	"$(INTDIR)\gmosmb2.obj" \
	"$(INTDIR)\gmosqsh1.obj" \
	"$(INTDIR)\gmosqsh2.obj" \
	"$(INTDIR)\gmosqsh3.obj" \
	"$(INTDIR)\gmosqsh4.obj" \
	"$(INTDIR)\gmosqsh5.obj" \
	"$(INTDIR)\gmowcat1.obj" \
	"$(INTDIR)\gmowcat2.obj" \
	"$(INTDIR)\gmowcat3.obj" \
	"$(INTDIR)\gmowcat4.obj" \
	"$(INTDIR)\gmowcat5.obj" \
	"$(INTDIR)\gmoxbbs1.obj" \
	"$(INTDIR)\gmoxbbs2.obj" \
	"$(INTDIR)\gmoxbbs3.obj" \
	"$(INTDIR)\gmoxbbs4.obj" \
	"$(INTDIR)\gmoxbbs5.obj" \
	"$(INTDIR)\lzh.obj" \
	"$(INTDIR)\smblib.obj" \
	"$(INTDIR)\fptools.obj" \
	"$(INTDIR)\uucheck.obj" \
	"$(INTDIR)\uuencode.obj" \
	"$(INTDIR)\uulib.obj" \
	"$(INTDIR)\uunconc.obj" \
	"$(INTDIR)\uuscan.obj" \
	"$(INTDIR)\uustring.obj" \
	"$(INTDIR)\uuutil.obj" \
	"$(INTDIR)\affentry.obj" \
	"$(INTDIR)\affixmgr.obj" \
	"$(INTDIR)\csutil.obj" \
	"$(INTDIR)\dictmgr.obj" \
	"$(INTDIR)\hashmgr.obj" \
	"$(INTDIR)\hunspell.obj" \
	"$(INTDIR)\suggmgr.obj"

"$(OUTDIR)\goldlib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "goldlib - Win32 Debug"

OUTDIR=.\bin\debug\win32
INTDIR=.\obj\debug\win32
# Begin Custom Macros
OutDir=.\bin\debug\win32
# End Custom Macros

ALL : "$(OUTDIR)\goldlib.lib"


CLEAN :
	-@erase "$(INTDIR)\affentry.obj"
	-@erase "$(INTDIR)\affixmgr.obj"
	-@erase "$(INTDIR)\csutil.obj"
	-@erase "$(INTDIR)\dictmgr.obj"
	-@erase "$(INTDIR)\fptools.obj"
	-@erase "$(INTDIR)\gbmh.obj"
	-@erase "$(INTDIR)\gcharset.obj"
	-@erase "$(INTDIR)\gcrc16tb.obj"
	-@erase "$(INTDIR)\gcrc32tb.obj"
	-@erase "$(INTDIR)\gcrchash.obj"
	-@erase "$(INTDIR)\gcrckeyv.obj"
	-@erase "$(INTDIR)\gcrcm16.obj"
	-@erase "$(INTDIR)\gcrcm32.obj"
	-@erase "$(INTDIR)\gcrcs16.obj"
	-@erase "$(INTDIR)\gcrcs32.obj"
	-@erase "$(INTDIR)\gdbgerr.obj"
	-@erase "$(INTDIR)\gdbgexit.obj"
	-@erase "$(INTDIR)\gdbgtrk.obj"
	-@erase "$(INTDIR)\gdirposx.obj"
	-@erase "$(INTDIR)\gedacfg.obj"
	-@erase "$(INTDIR)\gespell.obj"
	-@erase "$(INTDIR)\geval.obj"
	-@erase "$(INTDIR)\gevalhum.obj"
	-@erase "$(INTDIR)\gevalrpn.obj"
	-@erase "$(INTDIR)\gfile.obj"
	-@erase "$(INTDIR)\gfilport.obj"
	-@erase "$(INTDIR)\gfilutl1.obj"
	-@erase "$(INTDIR)\gfilutl2.obj"
	-@erase "$(INTDIR)\gftnaddr.obj"
	-@erase "$(INTDIR)\gftnnl.obj"
	-@erase "$(INTDIR)\gftnnlfd.obj"
	-@erase "$(INTDIR)\gftnnlfu.obj"
	-@erase "$(INTDIR)\gftnnlge.obj"
	-@erase "$(INTDIR)\gftnnlv7.obj"
	-@erase "$(INTDIR)\gfuzzy.obj"
	-@erase "$(INTDIR)\ghdrmime.obj"
	-@erase "$(INTDIR)\gkbdbase.obj"
	-@erase "$(INTDIR)\gkbdgetm.obj"
	-@erase "$(INTDIR)\gkbdwait.obj"
	-@erase "$(INTDIR)\glog.obj"
	-@erase "$(INTDIR)\gmemdbg.obj"
	-@erase "$(INTDIR)\gmemutil.obj"
	-@erase "$(INTDIR)\gmoarea.obj"
	-@erase "$(INTDIR)\gmoezyc1.obj"
	-@erase "$(INTDIR)\gmoezyc2.obj"
	-@erase "$(INTDIR)\gmoezyc3.obj"
	-@erase "$(INTDIR)\gmoezyc4.obj"
	-@erase "$(INTDIR)\gmoezyc5.obj"
	-@erase "$(INTDIR)\gmofido1.obj"
	-@erase "$(INTDIR)\gmofido2.obj"
	-@erase "$(INTDIR)\gmofido3.obj"
	-@erase "$(INTDIR)\gmofido4.obj"
	-@erase "$(INTDIR)\gmofido5.obj"
	-@erase "$(INTDIR)\gmohuds.obj"
	-@erase "$(INTDIR)\gmojamm1.obj"
	-@erase "$(INTDIR)\gmojamm2.obj"
	-@erase "$(INTDIR)\gmojamm3.obj"
	-@erase "$(INTDIR)\gmojamm4.obj"
	-@erase "$(INTDIR)\gmojamm5.obj"
	-@erase "$(INTDIR)\gmopcbd1.obj"
	-@erase "$(INTDIR)\gmopcbd2.obj"
	-@erase "$(INTDIR)\gmopcbd3.obj"
	-@erase "$(INTDIR)\gmopcbd4.obj"
	-@erase "$(INTDIR)\gmopcbd5.obj"
	-@erase "$(INTDIR)\gmosmb1.obj"
	-@erase "$(INTDIR)\gmosmb2.obj"
	-@erase "$(INTDIR)\gmosqsh1.obj"
	-@erase "$(INTDIR)\gmosqsh2.obj"
	-@erase "$(INTDIR)\gmosqsh3.obj"
	-@erase "$(INTDIR)\gmosqsh4.obj"
	-@erase "$(INTDIR)\gmosqsh5.obj"
	-@erase "$(INTDIR)\gmoubase.obj"
	-@erase "$(INTDIR)\gmowcat1.obj"
	-@erase "$(INTDIR)\gmowcat2.obj"
	-@erase "$(INTDIR)\gmowcat3.obj"
	-@erase "$(INTDIR)\gmowcat4.obj"
	-@erase "$(INTDIR)\gmowcat5.obj"
	-@erase "$(INTDIR)\gmoxbbs1.obj"
	-@erase "$(INTDIR)\gmoxbbs2.obj"
	-@erase "$(INTDIR)\gmoxbbs3.obj"
	-@erase "$(INTDIR)\gmoxbbs4.obj"
	-@erase "$(INTDIR)\gmoxbbs5.obj"
	-@erase "$(INTDIR)\gmsgattr.obj"
	-@erase "$(INTDIR)\gprnutil.obj"
	-@erase "$(INTDIR)\gregex.obj"
	-@erase "$(INTDIR)\gsearch.obj"
	-@erase "$(INTDIR)\gsnd.obj"
	-@erase "$(INTDIR)\gsndwrap.obj"
	-@erase "$(INTDIR)\gsrchmgr.obj"
	-@erase "$(INTDIR)\gstrbags.obj"
	-@erase "$(INTDIR)\gstrctyp.obj"
	-@erase "$(INTDIR)\gstrmail.obj"
	-@erase "$(INTDIR)\gstrname.obj"
	-@erase "$(INTDIR)\gstrutil.obj"
	-@erase "$(INTDIR)\gtimjuld.obj"
	-@erase "$(INTDIR)\gtimutil.obj"
	-@erase "$(INTDIR)\gtxtpara.obj"
	-@erase "$(INTDIR)\gusrbase.obj"
	-@erase "$(INTDIR)\gusrezyc.obj"
	-@erase "$(INTDIR)\gusrgold.obj"
	-@erase "$(INTDIR)\gusrhuds.obj"
	-@erase "$(INTDIR)\gusrmax.obj"
	-@erase "$(INTDIR)\gusrpcb.obj"
	-@erase "$(INTDIR)\gusrra2.obj"
	-@erase "$(INTDIR)\gusrxbbs.obj"
	-@erase "$(INTDIR)\gutlclip.obj"
	-@erase "$(INTDIR)\gutlcode.obj"
	-@erase "$(INTDIR)\gutlgrp.obj"
	-@erase "$(INTDIR)\gutlmisc.obj"
	-@erase "$(INTDIR)\gutlmtsk.obj"
	-@erase "$(INTDIR)\gutltag.obj"
	-@erase "$(INTDIR)\gutlvers.obj"
	-@erase "$(INTDIR)\gutlwin.obj"
	-@erase "$(INTDIR)\gutlwinm.obj"
	-@erase "$(INTDIR)\gvidbase.obj"
	-@erase "$(INTDIR)\gvidinit.obj"
	-@erase "$(INTDIR)\gwildmat.obj"
	-@erase "$(INTDIR)\gwinbase.obj"
	-@erase "$(INTDIR)\gwindow.obj"
	-@erase "$(INTDIR)\gwinhlp1.obj"
	-@erase "$(INTDIR)\gwinhlp2.obj"
	-@erase "$(INTDIR)\gwininit.obj"
	-@erase "$(INTDIR)\gwinline.obj"
	-@erase "$(INTDIR)\gwinmenu.obj"
	-@erase "$(INTDIR)\gwinmnub.obj"
	-@erase "$(INTDIR)\gwinpckf.obj"
	-@erase "$(INTDIR)\gwinpcks.obj"
	-@erase "$(INTDIR)\gwinpick.obj"
	-@erase "$(INTDIR)\gwinput2.obj"
	-@erase "$(INTDIR)\gxareas.obj"
	-@erase "$(INTDIR)\gxcrash.obj"
	-@erase "$(INTDIR)\gxdb.obj"
	-@erase "$(INTDIR)\gxdutch.obj"
	-@erase "$(INTDIR)\gxezy102.obj"
	-@erase "$(INTDIR)\gxezy110.obj"
	-@erase "$(INTDIR)\gxfd.obj"
	-@erase "$(INTDIR)\gxfecho4.obj"
	-@erase "$(INTDIR)\gxfecho5.obj"
	-@erase "$(INTDIR)\gxfecho6.obj"
	-@erase "$(INTDIR)\gxfidpcb.obj"
	-@erase "$(INTDIR)\gxfm092.obj"
	-@erase "$(INTDIR)\gxfm100.obj"
	-@erase "$(INTDIR)\gxfm116.obj"
	-@erase "$(INTDIR)\gxgecho.obj"
	-@erase "$(INTDIR)\gxhpt.obj"
	-@erase "$(INTDIR)\gximail4.obj"
	-@erase "$(INTDIR)\gximail5.obj"
	-@erase "$(INTDIR)\gximail6.obj"
	-@erase "$(INTDIR)\gxinter.obj"
	-@erase "$(INTDIR)\gxlora.obj"
	-@erase "$(INTDIR)\gxmax3.obj"
	-@erase "$(INTDIR)\gxme2.obj"
	-@erase "$(INTDIR)\gxopus.obj"
	-@erase "$(INTDIR)\gxpcb.obj"
	-@erase "$(INTDIR)\gxportal.obj"
	-@erase "$(INTDIR)\gxprobrd.obj"
	-@erase "$(INTDIR)\gxqecho.obj"
	-@erase "$(INTDIR)\gxqfront.obj"
	-@erase "$(INTDIR)\gxquick.obj"
	-@erase "$(INTDIR)\gxra.obj"
	-@erase "$(INTDIR)\gxraecho.obj"
	-@erase "$(INTDIR)\gxspace.obj"
	-@erase "$(INTDIR)\gxsquish.obj"
	-@erase "$(INTDIR)\gxsuper.obj"
	-@erase "$(INTDIR)\gxsync.obj"
	-@erase "$(INTDIR)\gxtimed.obj"
	-@erase "$(INTDIR)\gxtmail.obj"
	-@erase "$(INTDIR)\gxts.obj"
	-@erase "$(INTDIR)\gxwmail.obj"
	-@erase "$(INTDIR)\gxwtr.obj"
	-@erase "$(INTDIR)\gxxbbs.obj"
	-@erase "$(INTDIR)\gxxmail.obj"
	-@erase "$(INTDIR)\hashmgr.obj"
	-@erase "$(INTDIR)\hunspell.obj"
	-@erase "$(INTDIR)\lzh.obj"
	-@erase "$(INTDIR)\regex.obj"
	-@erase "$(INTDIR)\smblib.obj"
	-@erase "$(INTDIR)\suggmgr.obj"
	-@erase "$(INTDIR)\uucheck.obj"
	-@erase "$(INTDIR)\uuencode.obj"
	-@erase "$(INTDIR)\uulib.obj"
	-@erase "$(INTDIR)\uunconc.obj"
	-@erase "$(INTDIR)\uuscan.obj"
	-@erase "$(INTDIR)\uustring.obj"
	-@erase "$(INTDIR)\uuutil.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\goldlib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /I "..\goldlib" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "HAVE_STDARG_H" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /GZ /c

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\goldlib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\goldlib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\gbmh.obj" \
	"$(INTDIR)\gcharset.obj" \
	"$(INTDIR)\gcrc16tb.obj" \
	"$(INTDIR)\gcrc32tb.obj" \
	"$(INTDIR)\gcrchash.obj" \
	"$(INTDIR)\gcrckeyv.obj" \
	"$(INTDIR)\gcrcm16.obj" \
	"$(INTDIR)\gcrcm32.obj" \
	"$(INTDIR)\gcrcs16.obj" \
	"$(INTDIR)\gcrcs32.obj" \
	"$(INTDIR)\gdbgerr.obj" \
	"$(INTDIR)\gdbgexit.obj" \
	"$(INTDIR)\gdbgtrk.obj" \
	"$(INTDIR)\gdirposx.obj" \
	"$(INTDIR)\gespell.obj" \
	"$(INTDIR)\geval.obj" \
	"$(INTDIR)\gevalhum.obj" \
	"$(INTDIR)\gevalrpn.obj" \
	"$(INTDIR)\gfile.obj" \
	"$(INTDIR)\gfilport.obj" \
	"$(INTDIR)\gfilutl1.obj" \
	"$(INTDIR)\gfilutl2.obj" \
	"$(INTDIR)\gftnaddr.obj" \
	"$(INTDIR)\gftnnl.obj" \
	"$(INTDIR)\gftnnlfd.obj" \
	"$(INTDIR)\gftnnlfu.obj" \
	"$(INTDIR)\gftnnlge.obj" \
	"$(INTDIR)\gftnnlv7.obj" \
	"$(INTDIR)\gfuzzy.obj" \
	"$(INTDIR)\ghdrmime.obj" \
	"$(INTDIR)\gkbdbase.obj" \
	"$(INTDIR)\gkbdgetm.obj" \
	"$(INTDIR)\gkbdwait.obj" \
	"$(INTDIR)\glog.obj" \
	"$(INTDIR)\gmemdbg.obj" \
	"$(INTDIR)\gmemutil.obj" \
	"$(INTDIR)\gmoubase.obj" \
	"$(INTDIR)\gmsgattr.obj" \
	"$(INTDIR)\gprnutil.obj" \
	"$(INTDIR)\gregex.obj" \
	"$(INTDIR)\gsearch.obj" \
	"$(INTDIR)\gsnd.obj" \
	"$(INTDIR)\gsndwrap.obj" \
	"$(INTDIR)\gsrchmgr.obj" \
	"$(INTDIR)\gstrbags.obj" \
	"$(INTDIR)\gstrctyp.obj" \
	"$(INTDIR)\gstrmail.obj" \
	"$(INTDIR)\gstrname.obj" \
	"$(INTDIR)\gstrutil.obj" \
	"$(INTDIR)\gtimjuld.obj" \
	"$(INTDIR)\gtimutil.obj" \
	"$(INTDIR)\gtxtpara.obj" \
	"$(INTDIR)\gusrbase.obj" \
	"$(INTDIR)\gusrezyc.obj" \
	"$(INTDIR)\gusrgold.obj" \
	"$(INTDIR)\gusrhuds.obj" \
	"$(INTDIR)\gusrmax.obj" \
	"$(INTDIR)\gusrpcb.obj" \
	"$(INTDIR)\gusrra2.obj" \
	"$(INTDIR)\gusrxbbs.obj" \
	"$(INTDIR)\gutlclip.obj" \
	"$(INTDIR)\gutlcode.obj" \
	"$(INTDIR)\gutlgrp.obj" \
	"$(INTDIR)\gutlmisc.obj" \
	"$(INTDIR)\gutlmtsk.obj" \
	"$(INTDIR)\gutltag.obj" \
	"$(INTDIR)\gutlvers.obj" \
	"$(INTDIR)\gutlwin.obj" \
	"$(INTDIR)\gutlwinm.obj" \
	"$(INTDIR)\gvidbase.obj" \
	"$(INTDIR)\gvidinit.obj" \
	"$(INTDIR)\gwildmat.obj" \
	"$(INTDIR)\gwinbase.obj" \
	"$(INTDIR)\gwindow.obj" \
	"$(INTDIR)\gwinhlp1.obj" \
	"$(INTDIR)\gwinhlp2.obj" \
	"$(INTDIR)\gwininit.obj" \
	"$(INTDIR)\gwinline.obj" \
	"$(INTDIR)\gwinmenu.obj" \
	"$(INTDIR)\gwinmnub.obj" \
	"$(INTDIR)\gwinpckf.obj" \
	"$(INTDIR)\gwinpcks.obj" \
	"$(INTDIR)\gwinpick.obj" \
	"$(INTDIR)\gwinput2.obj" \
	"$(INTDIR)\gedacfg.obj" \
	"$(INTDIR)\gxareas.obj" \
	"$(INTDIR)\gxcrash.obj" \
	"$(INTDIR)\gxdb.obj" \
	"$(INTDIR)\gxdutch.obj" \
	"$(INTDIR)\gxezy102.obj" \
	"$(INTDIR)\gxezy110.obj" \
	"$(INTDIR)\gxfd.obj" \
	"$(INTDIR)\gxfecho4.obj" \
	"$(INTDIR)\gxfecho5.obj" \
	"$(INTDIR)\gxfecho6.obj" \
	"$(INTDIR)\gxfidpcb.obj" \
	"$(INTDIR)\gxfm092.obj" \
	"$(INTDIR)\gxfm100.obj" \
	"$(INTDIR)\gxfm116.obj" \
	"$(INTDIR)\gxgecho.obj" \
	"$(INTDIR)\gxhpt.obj" \
	"$(INTDIR)\gximail4.obj" \
	"$(INTDIR)\gximail5.obj" \
	"$(INTDIR)\gximail6.obj" \
	"$(INTDIR)\gxinter.obj" \
	"$(INTDIR)\gxlora.obj" \
	"$(INTDIR)\gxmax3.obj" \
	"$(INTDIR)\gxme2.obj" \
	"$(INTDIR)\gxopus.obj" \
	"$(INTDIR)\gxpcb.obj" \
	"$(INTDIR)\gxportal.obj" \
	"$(INTDIR)\gxprobrd.obj" \
	"$(INTDIR)\gxqecho.obj" \
	"$(INTDIR)\gxqfront.obj" \
	"$(INTDIR)\gxquick.obj" \
	"$(INTDIR)\gxra.obj" \
	"$(INTDIR)\gxraecho.obj" \
	"$(INTDIR)\gxspace.obj" \
	"$(INTDIR)\gxsquish.obj" \
	"$(INTDIR)\gxsuper.obj" \
	"$(INTDIR)\gxsync.obj" \
	"$(INTDIR)\gxtimed.obj" \
	"$(INTDIR)\gxtmail.obj" \
	"$(INTDIR)\gxts.obj" \
	"$(INTDIR)\gxwmail.obj" \
	"$(INTDIR)\gxwtr.obj" \
	"$(INTDIR)\gxxbbs.obj" \
	"$(INTDIR)\gxxmail.obj" \
	"$(INTDIR)\regex.obj" \
	"$(INTDIR)\gmoarea.obj" \
	"$(INTDIR)\gmoezyc1.obj" \
	"$(INTDIR)\gmoezyc2.obj" \
	"$(INTDIR)\gmoezyc3.obj" \
	"$(INTDIR)\gmoezyc4.obj" \
	"$(INTDIR)\gmoezyc5.obj" \
	"$(INTDIR)\gmofido1.obj" \
	"$(INTDIR)\gmofido2.obj" \
	"$(INTDIR)\gmofido3.obj" \
	"$(INTDIR)\gmofido4.obj" \
	"$(INTDIR)\gmofido5.obj" \
	"$(INTDIR)\gmohuds.obj" \
	"$(INTDIR)\gmojamm1.obj" \
	"$(INTDIR)\gmojamm2.obj" \
	"$(INTDIR)\gmojamm3.obj" \
	"$(INTDIR)\gmojamm4.obj" \
	"$(INTDIR)\gmojamm5.obj" \
	"$(INTDIR)\gmopcbd1.obj" \
	"$(INTDIR)\gmopcbd2.obj" \
	"$(INTDIR)\gmopcbd3.obj" \
	"$(INTDIR)\gmopcbd4.obj" \
	"$(INTDIR)\gmopcbd5.obj" \
	"$(INTDIR)\gmosmb1.obj" \
	"$(INTDIR)\gmosmb2.obj" \
	"$(INTDIR)\gmosqsh1.obj" \
	"$(INTDIR)\gmosqsh2.obj" \
	"$(INTDIR)\gmosqsh3.obj" \
	"$(INTDIR)\gmosqsh4.obj" \
	"$(INTDIR)\gmosqsh5.obj" \
	"$(INTDIR)\gmowcat1.obj" \
	"$(INTDIR)\gmowcat2.obj" \
	"$(INTDIR)\gmowcat3.obj" \
	"$(INTDIR)\gmowcat4.obj" \
	"$(INTDIR)\gmowcat5.obj" \
	"$(INTDIR)\gmoxbbs1.obj" \
	"$(INTDIR)\gmoxbbs2.obj" \
	"$(INTDIR)\gmoxbbs3.obj" \
	"$(INTDIR)\gmoxbbs4.obj" \
	"$(INTDIR)\gmoxbbs5.obj" \
	"$(INTDIR)\lzh.obj" \
	"$(INTDIR)\smblib.obj" \
	"$(INTDIR)\fptools.obj" \
	"$(INTDIR)\uucheck.obj" \
	"$(INTDIR)\uuencode.obj" \
	"$(INTDIR)\uulib.obj" \
	"$(INTDIR)\uunconc.obj" \
	"$(INTDIR)\uuscan.obj" \
	"$(INTDIR)\uustring.obj" \
	"$(INTDIR)\uuutil.obj" \
	"$(INTDIR)\affentry.obj" \
	"$(INTDIR)\affixmgr.obj" \
	"$(INTDIR)\csutil.obj" \
	"$(INTDIR)\dictmgr.obj" \
	"$(INTDIR)\hashmgr.obj" \
	"$(INTDIR)\hunspell.obj" \
	"$(INTDIR)\suggmgr.obj"

"$(OUTDIR)\goldlib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("goldlib.dep")
!INCLUDE "goldlib.dep"
!ELSE 
!MESSAGE Warning: cannot find "goldlib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "goldlib - Win32 Release" || "$(CFG)" == "goldlib - Win32 Debug"
SOURCE=..\goldlib\gall\be_lock.c
SOURCE=..\goldlib\gall\gbmh.cpp

"$(INTDIR)\gbmh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcharset.cpp

"$(INTDIR)\gcharset.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrc16tb.cpp

"$(INTDIR)\gcrc16tb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrc32tb.cpp

"$(INTDIR)\gcrc32tb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrchash.cpp

"$(INTDIR)\gcrchash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrckeyv.cpp

"$(INTDIR)\gcrckeyv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrcm16.cpp

"$(INTDIR)\gcrcm16.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrcm32.cpp

"$(INTDIR)\gcrcm32.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrcs16.cpp

"$(INTDIR)\gcrcs16.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gcrcs32.cpp

"$(INTDIR)\gcrcs32.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gdbgerr.cpp

"$(INTDIR)\gdbgerr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gdbgexit.cpp

"$(INTDIR)\gdbgexit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gdbgtrk.cpp

"$(INTDIR)\gdbgtrk.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gdirposx.cpp

"$(INTDIR)\gdirposx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gespell.cpp

"$(INTDIR)\gespell.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\geval.cpp

"$(INTDIR)\geval.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gevalhum.cpp

"$(INTDIR)\gevalhum.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gevalrpn.cpp

"$(INTDIR)\gevalrpn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gfile.cpp

"$(INTDIR)\gfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gfilport.cpp

"$(INTDIR)\gfilport.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gfilutl1.cpp

"$(INTDIR)\gfilutl1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gfilutl2.cpp

"$(INTDIR)\gfilutl2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gftnaddr.cpp

"$(INTDIR)\gftnaddr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gftnnl.cpp

"$(INTDIR)\gftnnl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gftnnlfd.cpp

"$(INTDIR)\gftnnlfd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gftnnlfu.cpp

"$(INTDIR)\gftnnlfu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gftnnlge.cpp

"$(INTDIR)\gftnnlge.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gftnnlv7.cpp

"$(INTDIR)\gftnnlv7.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gfuzzy.cpp

"$(INTDIR)\gfuzzy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\ghdrmime.cpp

"$(INTDIR)\ghdrmime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gkbdbase.cpp

"$(INTDIR)\gkbdbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gkbdgetm.cpp

"$(INTDIR)\gkbdgetm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gkbdunix.cpp
SOURCE=..\goldlib\gcui\gkbdwait.cpp

"$(INTDIR)\gkbdwait.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\glog.cpp

"$(INTDIR)\glog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gmemdbg.cpp

"$(INTDIR)\gmemdbg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gmemutil.cpp

"$(INTDIR)\gmemutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gmoubase.cpp

"$(INTDIR)\gmoubase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gmsgattr.cpp

"$(INTDIR)\gmsgattr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gprnutil.cpp

"$(INTDIR)\gprnutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gregex.cpp

"$(INTDIR)\gregex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gsearch.cpp

"$(INTDIR)\gsearch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gsigunix.cpp
SOURCE=..\goldlib\gall\gsnd.cpp

"$(INTDIR)\gsnd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gsndwrap.cpp

"$(INTDIR)\gsndwrap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gsrchmgr.cpp

"$(INTDIR)\gsrchmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gstrbags.cpp

"$(INTDIR)\gstrbags.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gstrctyp.cpp

"$(INTDIR)\gstrctyp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gstrmail.cpp

"$(INTDIR)\gstrmail.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gstrname.cpp

"$(INTDIR)\gstrname.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gstrutil.cpp

"$(INTDIR)\gstrutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gtimjuld.cpp

"$(INTDIR)\gtimjuld.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gtimutil.cpp

"$(INTDIR)\gtimutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gtxtpara.cpp

"$(INTDIR)\gtxtpara.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrbase.cpp

"$(INTDIR)\gusrbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrezyc.cpp

"$(INTDIR)\gusrezyc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrgold.cpp

"$(INTDIR)\gusrgold.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrhuds.cpp

"$(INTDIR)\gusrhuds.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrmax.cpp

"$(INTDIR)\gusrmax.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrpcb.cpp

"$(INTDIR)\gusrpcb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrra2.cpp

"$(INTDIR)\gusrra2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gusrxbbs.cpp

"$(INTDIR)\gusrxbbs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlbeos.cpp
SOURCE=..\goldlib\gall\gutlclip.cpp

"$(INTDIR)\gutlclip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlcode.cpp

"$(INTDIR)\gutlcode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutldos.cpp
SOURCE=..\goldlib\gall\gutlgrp.cpp

"$(INTDIR)\gutlgrp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlmisc.cpp

"$(INTDIR)\gutlmisc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlmtsk.cpp

"$(INTDIR)\gutlmtsk.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlos2.cpp
SOURCE=..\goldlib\gall\gutlos2m.cpp
SOURCE=..\goldlib\gall\gutltag.cpp

"$(INTDIR)\gutltag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlunix.cpp
SOURCE=..\goldlib\gall\gutlvers.cpp

"$(INTDIR)\gutlvers.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlwin.cpp

"$(INTDIR)\gutlwin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gutlwinm.cpp

"$(INTDIR)\gutlwinm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gvidbase.cpp

"$(INTDIR)\gvidbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gvidinit.cpp

"$(INTDIR)\gvidinit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gall\gwildmat.cpp

"$(INTDIR)\gwildmat.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinbase.cpp

"$(INTDIR)\gwinbase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwindow.cpp

"$(INTDIR)\gwindow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinhlp1.cpp

"$(INTDIR)\gwinhlp1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinhlp2.cpp

"$(INTDIR)\gwinhlp2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwininit.cpp

"$(INTDIR)\gwininit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinline.cpp

"$(INTDIR)\gwinline.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinmenu.cpp

"$(INTDIR)\gwinmenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinmnub.cpp

"$(INTDIR)\gwinmnub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinpckf.cpp

"$(INTDIR)\gwinpckf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinpcks.cpp

"$(INTDIR)\gwinpcks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinpick.cpp

"$(INTDIR)\gwinpick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcui\gwinput2.cpp

"$(INTDIR)\gwinput2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gedacfg.cpp

"$(INTDIR)\gedacfg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxareas.cpp

"$(INTDIR)\gxareas.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxcrash.cpp

"$(INTDIR)\gxcrash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxdb.cpp

"$(INTDIR)\gxdb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxdutch.cpp

"$(INTDIR)\gxdutch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxezy102.cpp

"$(INTDIR)\gxezy102.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxezy110.cpp

"$(INTDIR)\gxezy110.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfd.cpp

"$(INTDIR)\gxfd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfecho4.cpp

"$(INTDIR)\gxfecho4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfecho5.cpp

"$(INTDIR)\gxfecho5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfecho6.cpp

"$(INTDIR)\gxfecho6.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfidpcb.cpp

"$(INTDIR)\gxfidpcb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfm092.cpp

"$(INTDIR)\gxfm092.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfm100.cpp

"$(INTDIR)\gxfm100.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxfm116.cpp

"$(INTDIR)\gxfm116.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxgecho.cpp

"$(INTDIR)\gxgecho.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxhpt.cpp

"$(INTDIR)\gxhpt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gximail4.cpp

"$(INTDIR)\gximail4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gximail5.cpp

"$(INTDIR)\gximail5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gximail6.cpp

"$(INTDIR)\gximail6.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxinter.cpp

"$(INTDIR)\gxinter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxlora.cpp

"$(INTDIR)\gxlora.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxmax3.cpp

"$(INTDIR)\gxmax3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxme2.cpp

"$(INTDIR)\gxme2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxopus.cpp

"$(INTDIR)\gxopus.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxpcb.cpp

"$(INTDIR)\gxpcb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxportal.cpp

"$(INTDIR)\gxportal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxprobrd.cpp

"$(INTDIR)\gxprobrd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxqecho.cpp

"$(INTDIR)\gxqecho.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxqfront.cpp

"$(INTDIR)\gxqfront.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxquick.cpp

"$(INTDIR)\gxquick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxra.cpp

"$(INTDIR)\gxra.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxraecho.cpp

"$(INTDIR)\gxraecho.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxspace.cpp

"$(INTDIR)\gxspace.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxsquish.cpp

"$(INTDIR)\gxsquish.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxsuper.cpp

"$(INTDIR)\gxsuper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxsync.cpp

"$(INTDIR)\gxsync.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxtimed.cpp

"$(INTDIR)\gxtimed.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxtmail.cpp

"$(INTDIR)\gxtmail.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxts.cpp

"$(INTDIR)\gxts.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxwmail.cpp

"$(INTDIR)\gxwmail.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxwtr.cpp

"$(INTDIR)\gxwtr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxxbbs.cpp

"$(INTDIR)\gxxbbs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gcfg\gxxmail.cpp

"$(INTDIR)\gxxmail.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\glibc\dummy.c
SOURCE=..\goldlib\glibc\fnmatch.c
SOURCE=..\goldlib\glibc\glob.c
SOURCE=..\goldlib\glibc\regex.cpp

"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoarea.cpp

"$(INTDIR)\gmoarea.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoezyc1.cpp

"$(INTDIR)\gmoezyc1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoezyc2.cpp

"$(INTDIR)\gmoezyc2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoezyc3.cpp

"$(INTDIR)\gmoezyc3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoezyc4.cpp

"$(INTDIR)\gmoezyc4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoezyc5.cpp

"$(INTDIR)\gmoezyc5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmofido1.cpp

"$(INTDIR)\gmofido1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmofido2.cpp

"$(INTDIR)\gmofido2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmofido3.cpp

"$(INTDIR)\gmofido3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmofido4.cpp

"$(INTDIR)\gmofido4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmofido5.cpp

"$(INTDIR)\gmofido5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmohuds.cpp

"$(INTDIR)\gmohuds.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmohuds1.cpp
SOURCE=..\goldlib\gmb3\gmohuds2.cpp
SOURCE=..\goldlib\gmb3\gmohuds3.cpp
SOURCE=..\goldlib\gmb3\gmohuds4.cpp
SOURCE=..\goldlib\gmb3\gmohuds5.cpp
SOURCE=..\goldlib\gmb3\gmojamm1.cpp

"$(INTDIR)\gmojamm1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmojamm2.cpp

"$(INTDIR)\gmojamm2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmojamm3.cpp

"$(INTDIR)\gmojamm3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmojamm4.cpp

"$(INTDIR)\gmojamm4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmojamm5.cpp

"$(INTDIR)\gmojamm5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmopcbd1.cpp

"$(INTDIR)\gmopcbd1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmopcbd2.cpp

"$(INTDIR)\gmopcbd2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmopcbd3.cpp

"$(INTDIR)\gmopcbd3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmopcbd4.cpp

"$(INTDIR)\gmopcbd4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmopcbd5.cpp

"$(INTDIR)\gmopcbd5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmosmb1.cpp

"$(INTDIR)\gmosmb1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmosmb2.cpp

"$(INTDIR)\gmosmb2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmosqsh1.cpp

"$(INTDIR)\gmosqsh1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmosqsh2.cpp

"$(INTDIR)\gmosqsh2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmosqsh3.cpp

"$(INTDIR)\gmosqsh3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmosqsh4.cpp

"$(INTDIR)\gmosqsh4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmosqsh5.cpp

"$(INTDIR)\gmosqsh5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmowcat1.cpp

"$(INTDIR)\gmowcat1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmowcat2.cpp

"$(INTDIR)\gmowcat2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmowcat3.cpp

"$(INTDIR)\gmowcat3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmowcat4.cpp

"$(INTDIR)\gmowcat4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmowcat5.cpp

"$(INTDIR)\gmowcat5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoxbbs1.cpp

"$(INTDIR)\gmoxbbs1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoxbbs2.cpp

"$(INTDIR)\gmoxbbs2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoxbbs3.cpp

"$(INTDIR)\gmoxbbs3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoxbbs4.cpp

"$(INTDIR)\gmoxbbs4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\gmb3\gmoxbbs5.cpp

"$(INTDIR)\gmoxbbs5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\smblib\lzh.cpp

"$(INTDIR)\lzh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\smblib\smblib.cpp

"$(INTDIR)\smblib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\fptools.cpp

"$(INTDIR)\fptools.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\uucheck.cpp

"$(INTDIR)\uucheck.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\uuencode.cpp

"$(INTDIR)\uuencode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\uulib.cpp

"$(INTDIR)\uulib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\uunconc.cpp

"$(INTDIR)\uunconc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\uuscan.cpp

"$(INTDIR)\uuscan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\uustring.cpp

"$(INTDIR)\uustring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\uulib\uuutil.cpp

"$(INTDIR)\uuutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\affentry.cxx

"$(INTDIR)\affentry.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\affixmgr.cxx

"$(INTDIR)\affixmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\csutil.cxx

"$(INTDIR)\csutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\dictmgr.cxx

"$(INTDIR)\dictmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\hashmgr.cxx

"$(INTDIR)\hashmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\hunspell.cxx

"$(INTDIR)\hunspell.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\suggmgr.cxx

"$(INTDIR)\suggmgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\goldlib\hunspell\utf_info.cxx

!ENDIF 

