# Microsoft Visual Studio 6 makefile.
# Usage:
#
# NMAKE /f "golded.mak" CFG="golded - Win32 Release"
#
# or
#
# NMAKE /f "golded.mak" CFG="golded - Win32 Debug"
#
#

!IF "$(CFG)" == ""
CFG=golded - Win32 Release
!MESSAGE No configuration specified. Defaulting to golded - Win32 Release.
!ENDIF

!IF "$(CFG)" != "golded - Win32 Release" && "$(CFG)" != "golded - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "golded.mak" CFG="golded - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "golded - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "golded - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF


!IF  "$(CFG)" == "golded - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\gedwin.exe" "$(OUTDIR)\rddtwin.exe" "$(OUTDIR)\gnwin.exe"

CLEAN :
    -@erase "$(INTDIR)\fexist.obj"
    -@erase "$(INTDIR)\ffind.obj"
    -@erase "$(INTDIR)\fptools.obj"
    -@erase "$(INTDIR)\gbmh.obj"
    -@erase "$(INTDIR)\gcalst.obj"
    -@erase "$(INTDIR)\gcarea.obj"
    -@erase "$(INTDIR)\gccfgg.obj"
    -@erase "$(INTDIR)\gccfgg0.obj"
    -@erase "$(INTDIR)\gccfgg1.obj"
    -@erase "$(INTDIR)\gccfgg2.obj"
    -@erase "$(INTDIR)\gccfgg3.obj"
    -@erase "$(INTDIR)\gccfgg4.obj"
    -@erase "$(INTDIR)\gccfgg5.obj"
    -@erase "$(INTDIR)\gccfgg6.obj"
    -@erase "$(INTDIR)\gccfgg7.obj"
    -@erase "$(INTDIR)\gccfgg8.obj"
    -@erase "$(INTDIR)\gcharset.obj"
    -@erase "$(INTDIR)\gckeys.obj"
    -@erase "$(INTDIR)\gclang.obj"
    -@erase "$(INTDIR)\gcmisc.obj"
    -@erase "$(INTDIR)\gcrc16tb.obj"
    -@erase "$(INTDIR)\gcrc32tb.obj"
    -@erase "$(INTDIR)\gcrchash.obj"
    -@erase "$(INTDIR)\gcrckeyv.obj"
    -@erase "$(INTDIR)\gcrcm16.obj"
    -@erase "$(INTDIR)\gcrcm32.obj"
    -@erase "$(INTDIR)\gcrcs16.obj"
    -@erase "$(INTDIR)\gcrcs32.obj"
    -@erase "$(INTDIR)\gdbgerr.obj"
    -@erase "$(INTDIR)\gdbgtrk.obj"
    -@erase "$(INTDIR)\gdirposx.obj"
    -@erase "$(INTDIR)\gealst.obj"
    -@erase "$(INTDIR)\gearea.obj"
    -@erase "$(INTDIR)\gecarb.obj"
    -@erase "$(INTDIR)\gecmfd.obj"
    -@erase "$(INTDIR)\gectnr.obj"
    -@erase "$(INTDIR)\gectrl.obj"
    -@erase "$(INTDIR)\gedacfg.obj"
    -@erase "$(INTDIR)\gedcyg.res"
    -@erase "$(INTDIR)\gedoit.obj"
    -@erase "$(INTDIR)\gedoss.obj"
    -@erase "$(INTDIR)\geedit.obj"
    -@erase "$(INTDIR)\geedit2.obj"
    -@erase "$(INTDIR)\geedit3.obj"
    -@erase "$(INTDIR)\gefile.obj"
    -@erase "$(INTDIR)\gefind.obj"
    -@erase "$(INTDIR)\geglob.obj"
    -@erase "$(INTDIR)\gehdre.obj"
    -@erase "$(INTDIR)\gehdrs.obj"
    -@erase "$(INTDIR)\gehtml.obj"
    -@erase "$(INTDIR)\geinit.obj"
    -@erase "$(INTDIR)\geline.obj"
    -@erase "$(INTDIR)\gelmsg.obj"
    -@erase "$(INTDIR)\gemain.obj"
    -@erase "$(INTDIR)\gemenu.obj"
    -@erase "$(INTDIR)\gemlst.obj"
    -@erase "$(INTDIR)\gemnus.obj"
    -@erase "$(INTDIR)\gemrks.obj"
    -@erase "$(INTDIR)\gemsgid.obj"
    -@erase "$(INTDIR)\gemsgs.obj"
    -@erase "$(INTDIR)\genmsgid.obj"
    -@erase "$(INTDIR)\genode.obj"
    -@erase "$(INTDIR)\geplay.obj"
    -@erase "$(INTDIR)\gepost.obj"
    -@erase "$(INTDIR)\geqwks.obj"
    -@erase "$(INTDIR)\gerand.obj"
    -@erase "$(INTDIR)\geread.obj"
    -@erase "$(INTDIR)\geread2.obj"
    -@erase "$(INTDIR)\gescan.obj"
    -@erase "$(INTDIR)\gesoup.obj"
    -@erase "$(INTDIR)\gesrch.obj"
    -@erase "$(INTDIR)\getpls.obj"
    -@erase "$(INTDIR)\geusrbse.obj"
    -@erase "$(INTDIR)\geutil.obj"
    -@erase "$(INTDIR)\geutil2.obj"
    -@erase "$(INTDIR)\geval.obj"
    -@erase "$(INTDIR)\gevalhum.obj"
    -@erase "$(INTDIR)\gevalrpn.obj"
    -@erase "$(INTDIR)\geview.obj"
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
    -@erase "$(INTDIR)\gmarea.obj"
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
    -@erase "$(INTDIR)\golded3.obj"
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
    -@erase "$(INTDIR)\lzh.obj"
    -@erase "$(INTDIR)\patmat.obj"
    -@erase "$(INTDIR)\regex.obj"
    -@erase "$(INTDIR)\smblib.obj"
    -@erase "$(INTDIR)\uucheck.obj"
    -@erase "$(INTDIR)\uuencode.obj"
    -@erase "$(INTDIR)\uulib.obj"
    -@erase "$(INTDIR)\uunconc.obj"
    -@erase "$(INTDIR)\uuscan.obj"
    -@erase "$(INTDIR)\uustring.obj"
    -@erase "$(INTDIR)\uuutil.obj"
    -@erase "$(INTDIR)\vc60.idb"
    -@erase "$(INTDIR)\goldnode.obj"
    -@erase "$(INTDIR)\rddt.obj"

distclean:
    -@erase "$(OUTDIR)\gedwin.exe"
    -@erase "$(OUTDIR)\gnwin.exe"
    -@erase "$(OUTDIR)\rddtwin.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I ".\golded3" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /c

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
RSC_PROJ= /fo"$(INTDIR)\gedcyg.res" /d "NDEBUG"
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\golded.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib /nologo /subsystem:console /incremental:no /machine:I386
LINK32_OBJS= \
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
    "$(INTDIR)\gdbgtrk.obj" \
    "$(INTDIR)\gdirposx.obj" \
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
    "$(INTDIR)\fexist.obj" \
    "$(INTDIR)\ffind.obj" \
    "$(INTDIR)\genmsgid.obj" \
    "$(INTDIR)\patmat.obj" \
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
    "$(INTDIR)\gcalst.obj" \
    "$(INTDIR)\gcarea.obj" \
    "$(INTDIR)\gccfgg.obj" \
    "$(INTDIR)\gccfgg0.obj" \
    "$(INTDIR)\gccfgg1.obj" \
    "$(INTDIR)\gccfgg2.obj" \
    "$(INTDIR)\gccfgg3.obj" \
    "$(INTDIR)\gccfgg4.obj" \
    "$(INTDIR)\gccfgg5.obj" \
    "$(INTDIR)\gccfgg6.obj" \
    "$(INTDIR)\gccfgg7.obj" \
    "$(INTDIR)\gccfgg8.obj" \
    "$(INTDIR)\gckeys.obj" \
    "$(INTDIR)\gclang.obj" \
    "$(INTDIR)\gcmisc.obj" \
    "$(INTDIR)\gealst.obj" \
    "$(INTDIR)\gearea.obj" \
    "$(INTDIR)\gecarb.obj" \
    "$(INTDIR)\gecmfd.obj" \
    "$(INTDIR)\gectnr.obj" \
    "$(INTDIR)\gectrl.obj" \
    "$(INTDIR)\gedoit.obj" \
    "$(INTDIR)\gedoss.obj" \
    "$(INTDIR)\geedit.obj" \
    "$(INTDIR)\geedit2.obj" \
    "$(INTDIR)\geedit3.obj" \
    "$(INTDIR)\gefile.obj" \
    "$(INTDIR)\gefind.obj" \
    "$(INTDIR)\geglob.obj" \
    "$(INTDIR)\gehdre.obj" \
    "$(INTDIR)\gehdrs.obj" \
    "$(INTDIR)\gehtml.obj" \
    "$(INTDIR)\geinit.obj" \
    "$(INTDIR)\geline.obj" \
    "$(INTDIR)\gelmsg.obj" \
    "$(INTDIR)\gemain.obj" \
    "$(INTDIR)\gemenu.obj" \
    "$(INTDIR)\gemlst.obj" \
    "$(INTDIR)\gemnus.obj" \
    "$(INTDIR)\gemrks.obj" \
    "$(INTDIR)\gemsgid.obj" \
    "$(INTDIR)\gemsgs.obj" \
    "$(INTDIR)\genode.obj" \
    "$(INTDIR)\geplay.obj" \
    "$(INTDIR)\gepost.obj" \
    "$(INTDIR)\geqwks.obj" \
    "$(INTDIR)\gerand.obj" \
    "$(INTDIR)\geread.obj" \
    "$(INTDIR)\geread2.obj" \
    "$(INTDIR)\gescan.obj" \
    "$(INTDIR)\gesoup.obj" \
    "$(INTDIR)\gesrch.obj" \
    "$(INTDIR)\getpls.obj" \
    "$(INTDIR)\geusrbse.obj" \
    "$(INTDIR)\geutil.obj" \
    "$(INTDIR)\geutil2.obj" \
    "$(INTDIR)\geview.obj" \
    "$(INTDIR)\gmarea.obj" \
    "$(INTDIR)\gedcyg.res"

GOLDED_OBJS= \
    "$(INTDIR)\golded3.obj"

GOLDNODE_OBJS= \
    "$(INTDIR)\goldnode.obj"

RDDT_OBJS= \
    "$(INTDIR)\rddt.obj"


"$(OUTDIR)\gedwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS) $(GOLDED_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) /pdb:"$(OUTDIR)\gedwin.pdb" /out:"$(OUTDIR)\gedwin.exe" $(LINK32_OBJS) $(GOLDED_OBJS)
<<

"$(OUTDIR)\gnwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS) $(GOLDNODE_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) /pdb:"$(OUTDIR)\gnwin.pdb" /out:"$(OUTDIR)\gnwin.exe" $(LINK32_OBJS) $(GOLDNODE_OBJS)
<<

"$(OUTDIR)\rddtwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS) $(RDDT_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) /pdb:"$(OUTDIR)\rddtwin.pdb" /out:"$(OUTDIR)\rddtwin.exe" $(LINK32_OBJS) $(RDDT_OBJS)
<<

SOURCE="$(InputPath)"


# Begin Custom Macros
OutDir=.\Release
# End Custom Macros


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\gedwind.exe" "$(OUTDIR)\gedwind.bsc" "$(OUTDIR)\rddtwind.exe" "$(OUTDIR)\rddtwind.bsc" "$(OUTDIR)\gnwind.exe" "$(OUTDIR)\gnwind.bsc"

CLEAN :
    -@erase "$(INTDIR)\fexist.obj"
    -@erase "$(INTDIR)\fexist.sbr"
    -@erase "$(INTDIR)\ffind.obj"
    -@erase "$(INTDIR)\ffind.sbr"
    -@erase "$(INTDIR)\fptools.obj"
    -@erase "$(INTDIR)\fptools.sbr"
    -@erase "$(INTDIR)\gbmh.obj"
    -@erase "$(INTDIR)\gbmh.sbr"
    -@erase "$(INTDIR)\gcalst.obj"
    -@erase "$(INTDIR)\gcalst.sbr"
    -@erase "$(INTDIR)\gcarea.obj"
    -@erase "$(INTDIR)\gcarea.sbr"
    -@erase "$(INTDIR)\gccfgg.obj"
    -@erase "$(INTDIR)\gccfgg.sbr"
    -@erase "$(INTDIR)\gccfgg0.obj"
    -@erase "$(INTDIR)\gccfgg0.sbr"
    -@erase "$(INTDIR)\gccfgg1.obj"
    -@erase "$(INTDIR)\gccfgg1.sbr"
    -@erase "$(INTDIR)\gccfgg2.obj"
    -@erase "$(INTDIR)\gccfgg2.sbr"
    -@erase "$(INTDIR)\gccfgg3.obj"
    -@erase "$(INTDIR)\gccfgg3.sbr"
    -@erase "$(INTDIR)\gccfgg4.obj"
    -@erase "$(INTDIR)\gccfgg4.sbr"
    -@erase "$(INTDIR)\gccfgg5.obj"
    -@erase "$(INTDIR)\gccfgg5.sbr"
    -@erase "$(INTDIR)\gccfgg6.obj"
    -@erase "$(INTDIR)\gccfgg6.sbr"
    -@erase "$(INTDIR)\gccfgg7.obj"
    -@erase "$(INTDIR)\gccfgg7.sbr"
    -@erase "$(INTDIR)\gccfgg8.obj"
    -@erase "$(INTDIR)\gccfgg8.sbr"
    -@erase "$(INTDIR)\gcharset.obj"
    -@erase "$(INTDIR)\gcharset.sbr"
    -@erase "$(INTDIR)\gckeys.obj"
    -@erase "$(INTDIR)\gckeys.sbr"
    -@erase "$(INTDIR)\gclang.obj"
    -@erase "$(INTDIR)\gclang.sbr"
    -@erase "$(INTDIR)\gcmisc.obj"
    -@erase "$(INTDIR)\gcmisc.sbr"
    -@erase "$(INTDIR)\gcrc16tb.obj"
    -@erase "$(INTDIR)\gcrc16tb.sbr"
    -@erase "$(INTDIR)\gcrc32tb.obj"
    -@erase "$(INTDIR)\gcrc32tb.sbr"
    -@erase "$(INTDIR)\gcrchash.obj"
    -@erase "$(INTDIR)\gcrchash.sbr"
    -@erase "$(INTDIR)\gcrckeyv.obj"
    -@erase "$(INTDIR)\gcrckeyv.sbr"
    -@erase "$(INTDIR)\gcrcm16.obj"
    -@erase "$(INTDIR)\gcrcm16.sbr"
    -@erase "$(INTDIR)\gcrcm32.obj"
    -@erase "$(INTDIR)\gcrcm32.sbr"
    -@erase "$(INTDIR)\gcrcs16.obj"
    -@erase "$(INTDIR)\gcrcs16.sbr"
    -@erase "$(INTDIR)\gcrcs32.obj"
    -@erase "$(INTDIR)\gcrcs32.sbr"
    -@erase "$(INTDIR)\gdbgerr.obj"
    -@erase "$(INTDIR)\gdbgerr.sbr"
    -@erase "$(INTDIR)\gdbgtrk.obj"
    -@erase "$(INTDIR)\gdbgtrk.sbr"
    -@erase "$(INTDIR)\gdirposx.obj"
    -@erase "$(INTDIR)\gdirposx.sbr"
    -@erase "$(INTDIR)\gealst.obj"
    -@erase "$(INTDIR)\gealst.sbr"
    -@erase "$(INTDIR)\gearea.obj"
    -@erase "$(INTDIR)\gearea.sbr"
    -@erase "$(INTDIR)\gecarb.obj"
    -@erase "$(INTDIR)\gecarb.sbr"
    -@erase "$(INTDIR)\gecmfd.obj"
    -@erase "$(INTDIR)\gecmfd.sbr"
    -@erase "$(INTDIR)\gectnr.obj"
    -@erase "$(INTDIR)\gectnr.sbr"
    -@erase "$(INTDIR)\gectrl.obj"
    -@erase "$(INTDIR)\gectrl.sbr"
    -@erase "$(INTDIR)\gedacfg.obj"
    -@erase "$(INTDIR)\gedacfg.sbr"
    -@erase "$(INTDIR)\gedcyg.res"
    -@erase "$(INTDIR)\gedoit.obj"
    -@erase "$(INTDIR)\gedoit.sbr"
    -@erase "$(INTDIR)\gedoss.obj"
    -@erase "$(INTDIR)\gedoss.sbr"
    -@erase "$(INTDIR)\geedit.obj"
    -@erase "$(INTDIR)\geedit.sbr"
    -@erase "$(INTDIR)\geedit2.obj"
    -@erase "$(INTDIR)\geedit2.sbr"
    -@erase "$(INTDIR)\geedit3.obj"
    -@erase "$(INTDIR)\geedit3.sbr"
    -@erase "$(INTDIR)\gefile.obj"
    -@erase "$(INTDIR)\gefile.sbr"
    -@erase "$(INTDIR)\gefind.obj"
    -@erase "$(INTDIR)\gefind.sbr"
    -@erase "$(INTDIR)\geglob.obj"
    -@erase "$(INTDIR)\geglob.sbr"
    -@erase "$(INTDIR)\gehdre.obj"
    -@erase "$(INTDIR)\gehdre.sbr"
    -@erase "$(INTDIR)\gehdrs.obj"
    -@erase "$(INTDIR)\gehdrs.sbr"
    -@erase "$(INTDIR)\gehtml.obj"
    -@erase "$(INTDIR)\gehtml.sbr"
    -@erase "$(INTDIR)\geinit.obj"
    -@erase "$(INTDIR)\geinit.sbr"
    -@erase "$(INTDIR)\geline.obj"
    -@erase "$(INTDIR)\geline.sbr"
    -@erase "$(INTDIR)\gelmsg.obj"
    -@erase "$(INTDIR)\gelmsg.sbr"
    -@erase "$(INTDIR)\gemain.obj"
    -@erase "$(INTDIR)\gemain.sbr"
    -@erase "$(INTDIR)\gemenu.obj"
    -@erase "$(INTDIR)\gemenu.sbr"
    -@erase "$(INTDIR)\gemlst.obj"
    -@erase "$(INTDIR)\gemlst.sbr"
    -@erase "$(INTDIR)\gemnus.obj"
    -@erase "$(INTDIR)\gemnus.sbr"
    -@erase "$(INTDIR)\gemrks.obj"
    -@erase "$(INTDIR)\gemrks.sbr"
    -@erase "$(INTDIR)\gemsgid.obj"
    -@erase "$(INTDIR)\gemsgid.sbr"
    -@erase "$(INTDIR)\gemsgs.obj"
    -@erase "$(INTDIR)\gemsgs.sbr"
    -@erase "$(INTDIR)\genmsgid.obj"
    -@erase "$(INTDIR)\genmsgid.sbr"
    -@erase "$(INTDIR)\genode.obj"
    -@erase "$(INTDIR)\genode.sbr"
    -@erase "$(INTDIR)\geplay.obj"
    -@erase "$(INTDIR)\geplay.sbr"
    -@erase "$(INTDIR)\gepost.obj"
    -@erase "$(INTDIR)\gepost.sbr"
    -@erase "$(INTDIR)\geqwks.obj"
    -@erase "$(INTDIR)\geqwks.sbr"
    -@erase "$(INTDIR)\gerand.obj"
    -@erase "$(INTDIR)\gerand.sbr"
    -@erase "$(INTDIR)\geread.obj"
    -@erase "$(INTDIR)\geread.sbr"
    -@erase "$(INTDIR)\geread2.obj"
    -@erase "$(INTDIR)\geread2.sbr"
    -@erase "$(INTDIR)\gescan.obj"
    -@erase "$(INTDIR)\gescan.sbr"
    -@erase "$(INTDIR)\gesoup.obj"
    -@erase "$(INTDIR)\gesoup.sbr"
    -@erase "$(INTDIR)\gesrch.obj"
    -@erase "$(INTDIR)\gesrch.sbr"
    -@erase "$(INTDIR)\getpls.obj"
    -@erase "$(INTDIR)\getpls.sbr"
    -@erase "$(INTDIR)\geusrbse.obj"
    -@erase "$(INTDIR)\geusrbse.sbr"
    -@erase "$(INTDIR)\geutil.obj"
    -@erase "$(INTDIR)\geutil.sbr"
    -@erase "$(INTDIR)\geutil2.obj"
    -@erase "$(INTDIR)\geutil2.sbr"
    -@erase "$(INTDIR)\geval.obj"
    -@erase "$(INTDIR)\geval.sbr"
    -@erase "$(INTDIR)\gevalhum.obj"
    -@erase "$(INTDIR)\gevalhum.sbr"
    -@erase "$(INTDIR)\gevalrpn.obj"
    -@erase "$(INTDIR)\gevalrpn.sbr"
    -@erase "$(INTDIR)\geview.obj"
    -@erase "$(INTDIR)\geview.sbr"
    -@erase "$(INTDIR)\gfile.obj"
    -@erase "$(INTDIR)\gfile.sbr"
    -@erase "$(INTDIR)\gfilport.obj"
    -@erase "$(INTDIR)\gfilport.sbr"
    -@erase "$(INTDIR)\gfilutl1.obj"
    -@erase "$(INTDIR)\gfilutl1.sbr"
    -@erase "$(INTDIR)\gfilutl2.obj"
    -@erase "$(INTDIR)\gfilutl2.sbr"
    -@erase "$(INTDIR)\gftnaddr.obj"
    -@erase "$(INTDIR)\gftnaddr.sbr"
    -@erase "$(INTDIR)\gftnnl.obj"
    -@erase "$(INTDIR)\gftnnl.sbr"
    -@erase "$(INTDIR)\gftnnlfd.obj"
    -@erase "$(INTDIR)\gftnnlfd.sbr"
    -@erase "$(INTDIR)\gftnnlfu.obj"
    -@erase "$(INTDIR)\gftnnlfu.sbr"
    -@erase "$(INTDIR)\gftnnlge.obj"
    -@erase "$(INTDIR)\gftnnlge.sbr"
    -@erase "$(INTDIR)\gftnnlv7.obj"
    -@erase "$(INTDIR)\gftnnlv7.sbr"
    -@erase "$(INTDIR)\gfuzzy.obj"
    -@erase "$(INTDIR)\gfuzzy.sbr"
    -@erase "$(INTDIR)\ghdrmime.obj"
    -@erase "$(INTDIR)\ghdrmime.sbr"
    -@erase "$(INTDIR)\gkbdbase.obj"
    -@erase "$(INTDIR)\gkbdbase.sbr"
    -@erase "$(INTDIR)\gkbdgetm.obj"
    -@erase "$(INTDIR)\gkbdgetm.sbr"
    -@erase "$(INTDIR)\gkbdwait.obj"
    -@erase "$(INTDIR)\gkbdwait.sbr"
    -@erase "$(INTDIR)\glog.obj"
    -@erase "$(INTDIR)\glog.sbr"
    -@erase "$(INTDIR)\gmarea.obj"
    -@erase "$(INTDIR)\gmarea.sbr"
    -@erase "$(INTDIR)\gmemdbg.obj"
    -@erase "$(INTDIR)\gmemdbg.sbr"
    -@erase "$(INTDIR)\gmemutil.obj"
    -@erase "$(INTDIR)\gmemutil.sbr"
    -@erase "$(INTDIR)\gmoarea.obj"
    -@erase "$(INTDIR)\gmoarea.sbr"
    -@erase "$(INTDIR)\gmoezyc1.obj"
    -@erase "$(INTDIR)\gmoezyc1.sbr"
    -@erase "$(INTDIR)\gmoezyc2.obj"
    -@erase "$(INTDIR)\gmoezyc2.sbr"
    -@erase "$(INTDIR)\gmoezyc3.obj"
    -@erase "$(INTDIR)\gmoezyc3.sbr"
    -@erase "$(INTDIR)\gmoezyc4.obj"
    -@erase "$(INTDIR)\gmoezyc4.sbr"
    -@erase "$(INTDIR)\gmoezyc5.obj"
    -@erase "$(INTDIR)\gmoezyc5.sbr"
    -@erase "$(INTDIR)\gmofido1.obj"
    -@erase "$(INTDIR)\gmofido1.sbr"
    -@erase "$(INTDIR)\gmofido2.obj"
    -@erase "$(INTDIR)\gmofido2.sbr"
    -@erase "$(INTDIR)\gmofido3.obj"
    -@erase "$(INTDIR)\gmofido3.sbr"
    -@erase "$(INTDIR)\gmofido4.obj"
    -@erase "$(INTDIR)\gmofido4.sbr"
    -@erase "$(INTDIR)\gmofido5.obj"
    -@erase "$(INTDIR)\gmofido5.sbr"
    -@erase "$(INTDIR)\gmohuds.obj"
    -@erase "$(INTDIR)\gmohuds.sbr"
    -@erase "$(INTDIR)\gmojamm1.obj"
    -@erase "$(INTDIR)\gmojamm1.sbr"
    -@erase "$(INTDIR)\gmojamm2.obj"
    -@erase "$(INTDIR)\gmojamm2.sbr"
    -@erase "$(INTDIR)\gmojamm3.obj"
    -@erase "$(INTDIR)\gmojamm3.sbr"
    -@erase "$(INTDIR)\gmojamm4.obj"
    -@erase "$(INTDIR)\gmojamm4.sbr"
    -@erase "$(INTDIR)\gmojamm5.obj"
    -@erase "$(INTDIR)\gmojamm5.sbr"
    -@erase "$(INTDIR)\gmopcbd1.obj"
    -@erase "$(INTDIR)\gmopcbd1.sbr"
    -@erase "$(INTDIR)\gmopcbd2.obj"
    -@erase "$(INTDIR)\gmopcbd2.sbr"
    -@erase "$(INTDIR)\gmopcbd3.obj"
    -@erase "$(INTDIR)\gmopcbd3.sbr"
    -@erase "$(INTDIR)\gmopcbd4.obj"
    -@erase "$(INTDIR)\gmopcbd4.sbr"
    -@erase "$(INTDIR)\gmopcbd5.obj"
    -@erase "$(INTDIR)\gmopcbd5.sbr"
    -@erase "$(INTDIR)\gmosmb1.obj"
    -@erase "$(INTDIR)\gmosmb1.sbr"
    -@erase "$(INTDIR)\gmosmb2.obj"
    -@erase "$(INTDIR)\gmosmb2.sbr"
    -@erase "$(INTDIR)\gmosqsh1.obj"
    -@erase "$(INTDIR)\gmosqsh1.sbr"
    -@erase "$(INTDIR)\gmosqsh2.obj"
    -@erase "$(INTDIR)\gmosqsh2.sbr"
    -@erase "$(INTDIR)\gmosqsh3.obj"
    -@erase "$(INTDIR)\gmosqsh3.sbr"
    -@erase "$(INTDIR)\gmosqsh4.obj"
    -@erase "$(INTDIR)\gmosqsh4.sbr"
    -@erase "$(INTDIR)\gmosqsh5.obj"
    -@erase "$(INTDIR)\gmosqsh5.sbr"
    -@erase "$(INTDIR)\gmoubase.obj"
    -@erase "$(INTDIR)\gmoubase.sbr"
    -@erase "$(INTDIR)\gmowcat1.obj"
    -@erase "$(INTDIR)\gmowcat1.sbr"
    -@erase "$(INTDIR)\gmowcat2.obj"
    -@erase "$(INTDIR)\gmowcat2.sbr"
    -@erase "$(INTDIR)\gmowcat3.obj"
    -@erase "$(INTDIR)\gmowcat3.sbr"
    -@erase "$(INTDIR)\gmowcat4.obj"
    -@erase "$(INTDIR)\gmowcat4.sbr"
    -@erase "$(INTDIR)\gmowcat5.obj"
    -@erase "$(INTDIR)\gmowcat5.sbr"
    -@erase "$(INTDIR)\gmoxbbs1.obj"
    -@erase "$(INTDIR)\gmoxbbs1.sbr"
    -@erase "$(INTDIR)\gmoxbbs2.obj"
    -@erase "$(INTDIR)\gmoxbbs2.sbr"
    -@erase "$(INTDIR)\gmoxbbs3.obj"
    -@erase "$(INTDIR)\gmoxbbs3.sbr"
    -@erase "$(INTDIR)\gmoxbbs4.obj"
    -@erase "$(INTDIR)\gmoxbbs4.sbr"
    -@erase "$(INTDIR)\gmoxbbs5.obj"
    -@erase "$(INTDIR)\gmoxbbs5.sbr"
    -@erase "$(INTDIR)\gmsgattr.obj"
    -@erase "$(INTDIR)\gmsgattr.sbr"
    -@erase "$(INTDIR)\golded3.obj"
    -@erase "$(INTDIR)\golded3.sbr"
    -@erase "$(INTDIR)\gprnutil.obj"
    -@erase "$(INTDIR)\gprnutil.sbr"
    -@erase "$(INTDIR)\gregex.obj"
    -@erase "$(INTDIR)\gregex.sbr"
    -@erase "$(INTDIR)\gsearch.obj"
    -@erase "$(INTDIR)\gsearch.sbr"
    -@erase "$(INTDIR)\gsnd.obj"
    -@erase "$(INTDIR)\gsnd.sbr"
    -@erase "$(INTDIR)\gsndwrap.obj"
    -@erase "$(INTDIR)\gsndwrap.sbr"
    -@erase "$(INTDIR)\gsrchmgr.obj"
    -@erase "$(INTDIR)\gsrchmgr.sbr"
    -@erase "$(INTDIR)\gstrbags.obj"
    -@erase "$(INTDIR)\gstrbags.sbr"
    -@erase "$(INTDIR)\gstrctyp.obj"
    -@erase "$(INTDIR)\gstrctyp.sbr"
    -@erase "$(INTDIR)\gstrmail.obj"
    -@erase "$(INTDIR)\gstrmail.sbr"
    -@erase "$(INTDIR)\gstrname.obj"
    -@erase "$(INTDIR)\gstrname.sbr"
    -@erase "$(INTDIR)\gstrutil.obj"
    -@erase "$(INTDIR)\gstrutil.sbr"
    -@erase "$(INTDIR)\gtimjuld.obj"
    -@erase "$(INTDIR)\gtimjuld.sbr"
    -@erase "$(INTDIR)\gtimutil.obj"
    -@erase "$(INTDIR)\gtimutil.sbr"
    -@erase "$(INTDIR)\gtxtpara.obj"
    -@erase "$(INTDIR)\gtxtpara.sbr"
    -@erase "$(INTDIR)\gusrbase.obj"
    -@erase "$(INTDIR)\gusrbase.sbr"
    -@erase "$(INTDIR)\gusrezyc.obj"
    -@erase "$(INTDIR)\gusrezyc.sbr"
    -@erase "$(INTDIR)\gusrgold.obj"
    -@erase "$(INTDIR)\gusrgold.sbr"
    -@erase "$(INTDIR)\gusrhuds.obj"
    -@erase "$(INTDIR)\gusrhuds.sbr"
    -@erase "$(INTDIR)\gusrmax.obj"
    -@erase "$(INTDIR)\gusrmax.sbr"
    -@erase "$(INTDIR)\gusrpcb.obj"
    -@erase "$(INTDIR)\gusrpcb.sbr"
    -@erase "$(INTDIR)\gusrra2.obj"
    -@erase "$(INTDIR)\gusrra2.sbr"
    -@erase "$(INTDIR)\gusrxbbs.obj"
    -@erase "$(INTDIR)\gusrxbbs.sbr"
    -@erase "$(INTDIR)\gutlclip.obj"
    -@erase "$(INTDIR)\gutlclip.sbr"
    -@erase "$(INTDIR)\gutlcode.obj"
    -@erase "$(INTDIR)\gutlcode.sbr"
    -@erase "$(INTDIR)\gutlgrp.obj"
    -@erase "$(INTDIR)\gutlgrp.sbr"
    -@erase "$(INTDIR)\gutlmisc.obj"
    -@erase "$(INTDIR)\gutlmisc.sbr"
    -@erase "$(INTDIR)\gutlmtsk.obj"
    -@erase "$(INTDIR)\gutlmtsk.sbr"
    -@erase "$(INTDIR)\gutltag.obj"
    -@erase "$(INTDIR)\gutltag.sbr"
    -@erase "$(INTDIR)\gutlvers.obj"
    -@erase "$(INTDIR)\gutlvers.sbr"
    -@erase "$(INTDIR)\gutlwin.obj"
    -@erase "$(INTDIR)\gutlwin.sbr"
    -@erase "$(INTDIR)\gutlwinm.obj"
    -@erase "$(INTDIR)\gutlwinm.sbr"
    -@erase "$(INTDIR)\gvidbase.obj"
    -@erase "$(INTDIR)\gvidbase.sbr"
    -@erase "$(INTDIR)\gvidinit.obj"
    -@erase "$(INTDIR)\gvidinit.sbr"
    -@erase "$(INTDIR)\gwildmat.obj"
    -@erase "$(INTDIR)\gwildmat.sbr"
    -@erase "$(INTDIR)\gwinbase.obj"
    -@erase "$(INTDIR)\gwinbase.sbr"
    -@erase "$(INTDIR)\gwindow.obj"
    -@erase "$(INTDIR)\gwindow.sbr"
    -@erase "$(INTDIR)\gwinhlp1.obj"
    -@erase "$(INTDIR)\gwinhlp1.sbr"
    -@erase "$(INTDIR)\gwinhlp2.obj"
    -@erase "$(INTDIR)\gwinhlp2.sbr"
    -@erase "$(INTDIR)\gwininit.obj"
    -@erase "$(INTDIR)\gwininit.sbr"
    -@erase "$(INTDIR)\gwinline.obj"
    -@erase "$(INTDIR)\gwinline.sbr"
    -@erase "$(INTDIR)\gwinmenu.obj"
    -@erase "$(INTDIR)\gwinmenu.sbr"
    -@erase "$(INTDIR)\gwinmnub.obj"
    -@erase "$(INTDIR)\gwinmnub.sbr"
    -@erase "$(INTDIR)\gwinpckf.obj"
    -@erase "$(INTDIR)\gwinpckf.sbr"
    -@erase "$(INTDIR)\gwinpcks.obj"
    -@erase "$(INTDIR)\gwinpcks.sbr"
    -@erase "$(INTDIR)\gwinpick.obj"
    -@erase "$(INTDIR)\gwinpick.sbr"
    -@erase "$(INTDIR)\gwinput2.obj"
    -@erase "$(INTDIR)\gwinput2.sbr"
    -@erase "$(INTDIR)\gxareas.obj"
    -@erase "$(INTDIR)\gxareas.sbr"
    -@erase "$(INTDIR)\gxcrash.obj"
    -@erase "$(INTDIR)\gxcrash.sbr"
    -@erase "$(INTDIR)\gxdb.obj"
    -@erase "$(INTDIR)\gxdb.sbr"
    -@erase "$(INTDIR)\gxdutch.obj"
    -@erase "$(INTDIR)\gxdutch.sbr"
    -@erase "$(INTDIR)\gxezy102.obj"
    -@erase "$(INTDIR)\gxezy102.sbr"
    -@erase "$(INTDIR)\gxezy110.obj"
    -@erase "$(INTDIR)\gxezy110.sbr"
    -@erase "$(INTDIR)\gxfd.obj"
    -@erase "$(INTDIR)\gxfd.sbr"
    -@erase "$(INTDIR)\gxfecho4.obj"
    -@erase "$(INTDIR)\gxfecho4.sbr"
    -@erase "$(INTDIR)\gxfecho5.obj"
    -@erase "$(INTDIR)\gxfecho5.sbr"
    -@erase "$(INTDIR)\gxfecho6.obj"
    -@erase "$(INTDIR)\gxfecho6.sbr"
    -@erase "$(INTDIR)\gxfidpcb.obj"
    -@erase "$(INTDIR)\gxfidpcb.sbr"
    -@erase "$(INTDIR)\gxfm092.obj"
    -@erase "$(INTDIR)\gxfm092.sbr"
    -@erase "$(INTDIR)\gxfm100.obj"
    -@erase "$(INTDIR)\gxfm100.sbr"
    -@erase "$(INTDIR)\gxfm116.obj"
    -@erase "$(INTDIR)\gxfm116.sbr"
    -@erase "$(INTDIR)\gxgecho.obj"
    -@erase "$(INTDIR)\gxgecho.sbr"
    -@erase "$(INTDIR)\gxhpt.obj"
    -@erase "$(INTDIR)\gxhpt.sbr"
    -@erase "$(INTDIR)\gximail4.obj"
    -@erase "$(INTDIR)\gximail4.sbr"
    -@erase "$(INTDIR)\gximail5.obj"
    -@erase "$(INTDIR)\gximail5.sbr"
    -@erase "$(INTDIR)\gximail6.obj"
    -@erase "$(INTDIR)\gximail6.sbr"
    -@erase "$(INTDIR)\gxinter.obj"
    -@erase "$(INTDIR)\gxinter.sbr"
    -@erase "$(INTDIR)\gxlora.obj"
    -@erase "$(INTDIR)\gxlora.sbr"
    -@erase "$(INTDIR)\gxmax3.obj"
    -@erase "$(INTDIR)\gxmax3.sbr"
    -@erase "$(INTDIR)\gxme2.obj"
    -@erase "$(INTDIR)\gxme2.sbr"
    -@erase "$(INTDIR)\gxopus.obj"
    -@erase "$(INTDIR)\gxopus.sbr"
    -@erase "$(INTDIR)\gxpcb.obj"
    -@erase "$(INTDIR)\gxpcb.sbr"
    -@erase "$(INTDIR)\gxportal.obj"
    -@erase "$(INTDIR)\gxportal.sbr"
    -@erase "$(INTDIR)\gxprobrd.obj"
    -@erase "$(INTDIR)\gxprobrd.sbr"
    -@erase "$(INTDIR)\gxqecho.obj"
    -@erase "$(INTDIR)\gxqecho.sbr"
    -@erase "$(INTDIR)\gxqfront.obj"
    -@erase "$(INTDIR)\gxqfront.sbr"
    -@erase "$(INTDIR)\gxquick.obj"
    -@erase "$(INTDIR)\gxquick.sbr"
    -@erase "$(INTDIR)\gxra.obj"
    -@erase "$(INTDIR)\gxra.sbr"
    -@erase "$(INTDIR)\gxraecho.obj"
    -@erase "$(INTDIR)\gxraecho.sbr"
    -@erase "$(INTDIR)\gxspace.obj"
    -@erase "$(INTDIR)\gxspace.sbr"
    -@erase "$(INTDIR)\gxsquish.obj"
    -@erase "$(INTDIR)\gxsquish.sbr"
    -@erase "$(INTDIR)\gxsuper.obj"
    -@erase "$(INTDIR)\gxsuper.sbr"
    -@erase "$(INTDIR)\gxsync.obj"
    -@erase "$(INTDIR)\gxsync.sbr"
    -@erase "$(INTDIR)\gxtimed.obj"
    -@erase "$(INTDIR)\gxtimed.sbr"
    -@erase "$(INTDIR)\gxtmail.obj"
    -@erase "$(INTDIR)\gxtmail.sbr"
    -@erase "$(INTDIR)\gxts.obj"
    -@erase "$(INTDIR)\gxts.sbr"
    -@erase "$(INTDIR)\gxwmail.obj"
    -@erase "$(INTDIR)\gxwmail.sbr"
    -@erase "$(INTDIR)\gxwtr.obj"
    -@erase "$(INTDIR)\gxwtr.sbr"
    -@erase "$(INTDIR)\gxxbbs.obj"
    -@erase "$(INTDIR)\gxxbbs.sbr"
    -@erase "$(INTDIR)\gxxmail.obj"
    -@erase "$(INTDIR)\gxxmail.sbr"
    -@erase "$(INTDIR)\lzh.obj"
    -@erase "$(INTDIR)\lzh.sbr"
    -@erase "$(INTDIR)\patmat.obj"
    -@erase "$(INTDIR)\patmat.sbr"
    -@erase "$(INTDIR)\regex.obj"
    -@erase "$(INTDIR)\regex.sbr"
    -@erase "$(INTDIR)\smblib.obj"
    -@erase "$(INTDIR)\smblib.sbr"
    -@erase "$(INTDIR)\uucheck.obj"
    -@erase "$(INTDIR)\uucheck.sbr"
    -@erase "$(INTDIR)\uuencode.obj"
    -@erase "$(INTDIR)\uuencode.sbr"
    -@erase "$(INTDIR)\uulib.obj"
    -@erase "$(INTDIR)\uulib.sbr"
    -@erase "$(INTDIR)\uunconc.obj"
    -@erase "$(INTDIR)\uunconc.sbr"
    -@erase "$(INTDIR)\uuscan.obj"
    -@erase "$(INTDIR)\uuscan.sbr"
    -@erase "$(INTDIR)\uustring.obj"
    -@erase "$(INTDIR)\uustring.sbr"
    -@erase "$(INTDIR)\uuutil.obj"
    -@erase "$(INTDIR)\uuutil.sbr"
    -@erase "$(INTDIR)\vc60.idb"
    -@erase "$(INTDIR)\vc60.pdb"
    -@erase "$(OUTDIR)\golded.bsc"
    -@erase "$(OUTDIR)\golded.ilk"
    -@erase "$(OUTDIR)\golded.pdb"
    -@erase "$(INTDIR)\goldnode.obj"
    -@erase "$(OUTDIR)\goldnode.sbr"
    -@erase "$(OUTDIR)\rddt.obj"
    -@erase "$(OUTDIR)\rddt.sbr"

distclean:
    -@erase "$(OUTDIR)\gedwind.exe"
    -@erase "$(OUTDIR)\gedwind.bsc"
    -@erase "$(OUTDIR)\gnwind.exe"
    -@erase "$(OUTDIR)\gnwind.bsc"
    -@erase "$(OUTDIR)\rddtwind.exe"
    -@erase "$(OUTDIR)\rddtwind.bsc"


"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Progra~1\Micros~2\VC98\Include\\" /I ".\golded3" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /GZ /c

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
RSC_PROJ= /fo"$(INTDIR)\gedcyg.res" /d "_DEBUG"
BSC32=bscmake.exe
BSC32_FLAGS=/nologo
BSC32_SBRS= \
    "$(INTDIR)\gbmh.sbr" \
    "$(INTDIR)\gcharset.sbr" \
    "$(INTDIR)\gcrc16tb.sbr" \
    "$(INTDIR)\gcrc32tb.sbr" \
    "$(INTDIR)\gcrchash.sbr" \
    "$(INTDIR)\gcrckeyv.sbr" \
    "$(INTDIR)\gcrcm16.sbr" \
    "$(INTDIR)\gcrcm32.sbr" \
    "$(INTDIR)\gcrcs16.sbr" \
    "$(INTDIR)\gcrcs32.sbr" \
    "$(INTDIR)\gdbgerr.sbr" \
    "$(INTDIR)\gdbgtrk.sbr" \
    "$(INTDIR)\gdirposx.sbr" \
    "$(INTDIR)\geval.sbr" \
    "$(INTDIR)\gevalhum.sbr" \
    "$(INTDIR)\gevalrpn.sbr" \
    "$(INTDIR)\gfile.sbr" \
    "$(INTDIR)\gfilport.sbr" \
    "$(INTDIR)\gfilutl1.sbr" \
    "$(INTDIR)\gfilutl2.sbr" \
    "$(INTDIR)\gftnaddr.sbr" \
    "$(INTDIR)\gftnnl.sbr" \
    "$(INTDIR)\gftnnlfd.sbr" \
    "$(INTDIR)\gftnnlfu.sbr" \
    "$(INTDIR)\gftnnlge.sbr" \
    "$(INTDIR)\gftnnlv7.sbr" \
    "$(INTDIR)\gfuzzy.sbr" \
    "$(INTDIR)\ghdrmime.sbr" \
    "$(INTDIR)\gkbdbase.sbr" \
    "$(INTDIR)\gkbdgetm.sbr" \
    "$(INTDIR)\gkbdwait.sbr" \
    "$(INTDIR)\glog.sbr" \
    "$(INTDIR)\gmemdbg.sbr" \
    "$(INTDIR)\gmemutil.sbr" \
    "$(INTDIR)\gmoubase.sbr" \
    "$(INTDIR)\gmsgattr.sbr" \
    "$(INTDIR)\gprnutil.sbr" \
    "$(INTDIR)\gregex.sbr" \
    "$(INTDIR)\gsearch.sbr" \
    "$(INTDIR)\gsnd.sbr" \
    "$(INTDIR)\gsndwrap.sbr" \
    "$(INTDIR)\gsrchmgr.sbr" \
    "$(INTDIR)\gstrbags.sbr" \
    "$(INTDIR)\gstrctyp.sbr" \
    "$(INTDIR)\gstrmail.sbr" \
    "$(INTDIR)\gstrname.sbr" \
    "$(INTDIR)\gstrutil.sbr" \
    "$(INTDIR)\gtimjuld.sbr" \
    "$(INTDIR)\gtimutil.sbr" \
    "$(INTDIR)\gtxtpara.sbr" \
    "$(INTDIR)\gusrbase.sbr" \
    "$(INTDIR)\gusrezyc.sbr" \
    "$(INTDIR)\gusrgold.sbr" \
    "$(INTDIR)\gusrhuds.sbr" \
    "$(INTDIR)\gusrmax.sbr" \
    "$(INTDIR)\gusrpcb.sbr" \
    "$(INTDIR)\gusrra2.sbr" \
    "$(INTDIR)\gusrxbbs.sbr" \
    "$(INTDIR)\gutlclip.sbr" \
    "$(INTDIR)\gutlcode.sbr" \
    "$(INTDIR)\gutlgrp.sbr" \
    "$(INTDIR)\gutlmisc.sbr" \
    "$(INTDIR)\gutlmtsk.sbr" \
    "$(INTDIR)\gutltag.sbr" \
    "$(INTDIR)\gutlvers.sbr" \
    "$(INTDIR)\gutlwin.sbr" \
    "$(INTDIR)\gutlwinm.sbr" \
    "$(INTDIR)\gvidbase.sbr" \
    "$(INTDIR)\gvidinit.sbr" \
    "$(INTDIR)\gwildmat.sbr" \
    "$(INTDIR)\gwinbase.sbr" \
    "$(INTDIR)\gwindow.sbr" \
    "$(INTDIR)\gwinhlp1.sbr" \
    "$(INTDIR)\gwinhlp2.sbr" \
    "$(INTDIR)\gwininit.sbr" \
    "$(INTDIR)\gwinline.sbr" \
    "$(INTDIR)\gwinmenu.sbr" \
    "$(INTDIR)\gwinmnub.sbr" \
    "$(INTDIR)\gwinpckf.sbr" \
    "$(INTDIR)\gwinpcks.sbr" \
    "$(INTDIR)\gwinpick.sbr" \
    "$(INTDIR)\gwinput2.sbr" \
    "$(INTDIR)\gedacfg.sbr" \
    "$(INTDIR)\gxareas.sbr" \
    "$(INTDIR)\gxcrash.sbr" \
    "$(INTDIR)\gxdb.sbr" \
    "$(INTDIR)\gxdutch.sbr" \
    "$(INTDIR)\gxezy102.sbr" \
    "$(INTDIR)\gxezy110.sbr" \
    "$(INTDIR)\gxfd.sbr" \
    "$(INTDIR)\gxfecho4.sbr" \
    "$(INTDIR)\gxfecho5.sbr" \
    "$(INTDIR)\gxfecho6.sbr" \
    "$(INTDIR)\gxfidpcb.sbr" \
    "$(INTDIR)\gxfm092.sbr" \
    "$(INTDIR)\gxfm100.sbr" \
    "$(INTDIR)\gxfm116.sbr" \
    "$(INTDIR)\gxgecho.sbr" \
    "$(INTDIR)\gxhpt.sbr" \
    "$(INTDIR)\gximail4.sbr" \
    "$(INTDIR)\gximail5.sbr" \
    "$(INTDIR)\gximail6.sbr" \
    "$(INTDIR)\gxinter.sbr" \
    "$(INTDIR)\gxlora.sbr" \
    "$(INTDIR)\gxmax3.sbr" \
    "$(INTDIR)\gxme2.sbr" \
    "$(INTDIR)\gxopus.sbr" \
    "$(INTDIR)\gxpcb.sbr" \
    "$(INTDIR)\gxportal.sbr" \
    "$(INTDIR)\gxprobrd.sbr" \
    "$(INTDIR)\gxqecho.sbr" \
    "$(INTDIR)\gxqfront.sbr" \
    "$(INTDIR)\gxquick.sbr" \
    "$(INTDIR)\gxra.sbr" \
    "$(INTDIR)\gxraecho.sbr" \
    "$(INTDIR)\gxspace.sbr" \
    "$(INTDIR)\gxsquish.sbr" \
    "$(INTDIR)\gxsuper.sbr" \
    "$(INTDIR)\gxsync.sbr" \
    "$(INTDIR)\gxtimed.sbr" \
    "$(INTDIR)\gxtmail.sbr" \
    "$(INTDIR)\gxts.sbr" \
    "$(INTDIR)\gxwmail.sbr" \
    "$(INTDIR)\gxwtr.sbr" \
    "$(INTDIR)\gxxbbs.sbr" \
    "$(INTDIR)\gxxmail.sbr" \
    "$(INTDIR)\regex.sbr" \
    "$(INTDIR)\gmoarea.sbr" \
    "$(INTDIR)\gmoezyc1.sbr" \
    "$(INTDIR)\gmoezyc2.sbr" \
    "$(INTDIR)\gmoezyc3.sbr" \
    "$(INTDIR)\gmoezyc4.sbr" \
    "$(INTDIR)\gmoezyc5.sbr" \
    "$(INTDIR)\gmofido1.sbr" \
    "$(INTDIR)\gmofido2.sbr" \
    "$(INTDIR)\gmofido3.sbr" \
    "$(INTDIR)\gmofido4.sbr" \
    "$(INTDIR)\gmofido5.sbr" \
    "$(INTDIR)\gmohuds.sbr" \
    "$(INTDIR)\gmojamm1.sbr" \
    "$(INTDIR)\gmojamm2.sbr" \
    "$(INTDIR)\gmojamm3.sbr" \
    "$(INTDIR)\gmojamm4.sbr" \
    "$(INTDIR)\gmojamm5.sbr" \
    "$(INTDIR)\gmopcbd1.sbr" \
    "$(INTDIR)\gmopcbd2.sbr" \
    "$(INTDIR)\gmopcbd3.sbr" \
    "$(INTDIR)\gmopcbd4.sbr" \
    "$(INTDIR)\gmopcbd5.sbr" \
    "$(INTDIR)\gmosmb1.sbr" \
    "$(INTDIR)\gmosmb2.sbr" \
    "$(INTDIR)\gmosqsh1.sbr" \
    "$(INTDIR)\gmosqsh2.sbr" \
    "$(INTDIR)\gmosqsh3.sbr" \
    "$(INTDIR)\gmosqsh4.sbr" \
    "$(INTDIR)\gmosqsh5.sbr" \
    "$(INTDIR)\gmowcat1.sbr" \
    "$(INTDIR)\gmowcat2.sbr" \
    "$(INTDIR)\gmowcat3.sbr" \
    "$(INTDIR)\gmowcat4.sbr" \
    "$(INTDIR)\gmowcat5.sbr" \
    "$(INTDIR)\gmoxbbs1.sbr" \
    "$(INTDIR)\gmoxbbs2.sbr" \
    "$(INTDIR)\gmoxbbs3.sbr" \
    "$(INTDIR)\gmoxbbs4.sbr" \
    "$(INTDIR)\gmoxbbs5.sbr" \
    "$(INTDIR)\fexist.sbr" \
    "$(INTDIR)\ffind.sbr" \
    "$(INTDIR)\genmsgid.sbr" \
    "$(INTDIR)\patmat.sbr" \
    "$(INTDIR)\lzh.sbr" \
    "$(INTDIR)\smblib.sbr" \
    "$(INTDIR)\fptools.sbr" \
    "$(INTDIR)\uucheck.sbr" \
    "$(INTDIR)\uuencode.sbr" \
    "$(INTDIR)\uulib.sbr" \
    "$(INTDIR)\uunconc.sbr" \
    "$(INTDIR)\uuscan.sbr" \
    "$(INTDIR)\uustring.sbr" \
    "$(INTDIR)\uuutil.sbr" \
    "$(INTDIR)\gcalst.sbr" \
    "$(INTDIR)\gcarea.sbr" \
    "$(INTDIR)\gccfgg.sbr" \
    "$(INTDIR)\gccfgg0.sbr" \
    "$(INTDIR)\gccfgg1.sbr" \
    "$(INTDIR)\gccfgg2.sbr" \
    "$(INTDIR)\gccfgg3.sbr" \
    "$(INTDIR)\gccfgg4.sbr" \
    "$(INTDIR)\gccfgg5.sbr" \
    "$(INTDIR)\gccfgg6.sbr" \
    "$(INTDIR)\gccfgg7.sbr" \
    "$(INTDIR)\gccfgg8.sbr" \
    "$(INTDIR)\gckeys.sbr" \
    "$(INTDIR)\gclang.sbr" \
    "$(INTDIR)\gcmisc.sbr" \
    "$(INTDIR)\gealst.sbr" \
    "$(INTDIR)\gearea.sbr" \
    "$(INTDIR)\gecarb.sbr" \
    "$(INTDIR)\gecmfd.sbr" \
    "$(INTDIR)\gectnr.sbr" \
    "$(INTDIR)\gectrl.sbr" \
    "$(INTDIR)\gedoit.sbr" \
    "$(INTDIR)\gedoss.sbr" \
    "$(INTDIR)\geedit.sbr" \
    "$(INTDIR)\geedit2.sbr" \
    "$(INTDIR)\geedit3.sbr" \
    "$(INTDIR)\gefile.sbr" \
    "$(INTDIR)\gefind.sbr" \
    "$(INTDIR)\geglob.sbr" \
    "$(INTDIR)\gehdre.sbr" \
    "$(INTDIR)\gehdrs.sbr" \
    "$(INTDIR)\gehtml.sbr" \
    "$(INTDIR)\geinit.sbr" \
    "$(INTDIR)\geline.sbr" \
    "$(INTDIR)\gelmsg.sbr" \
    "$(INTDIR)\gemain.sbr" \
    "$(INTDIR)\gemenu.sbr" \
    "$(INTDIR)\gemlst.sbr" \
    "$(INTDIR)\gemnus.sbr" \
    "$(INTDIR)\gemrks.sbr" \
    "$(INTDIR)\gemsgid.sbr" \
    "$(INTDIR)\gemsgs.sbr" \
    "$(INTDIR)\genode.sbr" \
    "$(INTDIR)\geplay.sbr" \
    "$(INTDIR)\gepost.sbr" \
    "$(INTDIR)\geqwks.sbr" \
    "$(INTDIR)\gerand.sbr" \
    "$(INTDIR)\geread.sbr" \
    "$(INTDIR)\geread2.sbr" \
    "$(INTDIR)\gescan.sbr" \
    "$(INTDIR)\gesoup.sbr" \
    "$(INTDIR)\gesrch.sbr" \
    "$(INTDIR)\getpls.sbr" \
    "$(INTDIR)\geusrbse.sbr" \
    "$(INTDIR)\geutil.sbr" \
    "$(INTDIR)\geutil2.sbr" \
    "$(INTDIR)\geview.sbr" \
    "$(INTDIR)\gmarea.sbr"

GOLDED_SBRS= \
    "$(INTDIR)\golded3.sbr"

GOLDNODE_SBRS= \
    "$(INTDIR)\goldnode.sbr"

RDDT_SBRS= \
    "$(INTDIR)\rddt.sbr"

"$(OUTDIR)\gedwin.bsc" : "$(OUTDIR)" $(BSC32_SBRS) $(GOLDED_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS)  /o"$(OUTDIR)\gedwin.bsc" $(BSC32_SBRS) $(GOLDED_SBRS)
<<

"$(OUTDIR)\gnwin.bsc" : "$(OUTDIR)" $(BSC32_SBRS) $(GOLDNODE_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) /o"$(OUTDIR)\gnwin.bsc" $(BSC32_SBRS) $(GOLDNODE_SBRS)
<<

"$(OUTDIR)\rddtwin.bsc" : "$(OUTDIR)" $(BSC32_SBRS) $(RDDT_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) /o"$(OUTDIR)\rddtwin.bsc" $(BSC32_SBRS) $(RDDT_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib /nologo /subsystem:console /incremental:yes /debug /machine:I386 /pdbtype:sept
LINK32_OBJS= \
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
    "$(INTDIR)\gdbgtrk.obj" \
    "$(INTDIR)\gdirposx.obj" \
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
    "$(INTDIR)\fexist.obj" \
    "$(INTDIR)\ffind.obj" \
    "$(INTDIR)\genmsgid.obj" \
    "$(INTDIR)\patmat.obj" \
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
    "$(INTDIR)\gcalst.obj" \
    "$(INTDIR)\gcarea.obj" \
    "$(INTDIR)\gccfgg.obj" \
    "$(INTDIR)\gccfgg0.obj" \
    "$(INTDIR)\gccfgg1.obj" \
    "$(INTDIR)\gccfgg2.obj" \
    "$(INTDIR)\gccfgg3.obj" \
    "$(INTDIR)\gccfgg4.obj" \
    "$(INTDIR)\gccfgg5.obj" \
    "$(INTDIR)\gccfgg6.obj" \
    "$(INTDIR)\gccfgg7.obj" \
    "$(INTDIR)\gccfgg8.obj" \
    "$(INTDIR)\gckeys.obj" \
    "$(INTDIR)\gclang.obj" \
    "$(INTDIR)\gcmisc.obj" \
    "$(INTDIR)\gealst.obj" \
    "$(INTDIR)\gearea.obj" \
    "$(INTDIR)\gecarb.obj" \
    "$(INTDIR)\gecmfd.obj" \
    "$(INTDIR)\gectnr.obj" \
    "$(INTDIR)\gectrl.obj" \
    "$(INTDIR)\gedoit.obj" \
    "$(INTDIR)\gedoss.obj" \
    "$(INTDIR)\geedit.obj" \
    "$(INTDIR)\geedit2.obj" \
    "$(INTDIR)\geedit3.obj" \
    "$(INTDIR)\gefile.obj" \
    "$(INTDIR)\gefind.obj" \
    "$(INTDIR)\geglob.obj" \
    "$(INTDIR)\gehdre.obj" \
    "$(INTDIR)\gehdrs.obj" \
    "$(INTDIR)\gehtml.obj" \
    "$(INTDIR)\geinit.obj" \
    "$(INTDIR)\geline.obj" \
    "$(INTDIR)\gelmsg.obj" \
    "$(INTDIR)\gemain.obj" \
    "$(INTDIR)\gemenu.obj" \
    "$(INTDIR)\gemlst.obj" \
    "$(INTDIR)\gemnus.obj" \
    "$(INTDIR)\gemrks.obj" \
    "$(INTDIR)\gemsgid.obj" \
    "$(INTDIR)\gemsgs.obj" \
    "$(INTDIR)\genode.obj" \
    "$(INTDIR)\geplay.obj" \
    "$(INTDIR)\gepost.obj" \
    "$(INTDIR)\geqwks.obj" \
    "$(INTDIR)\gerand.obj" \
    "$(INTDIR)\geread.obj" \
    "$(INTDIR)\geread2.obj" \
    "$(INTDIR)\gescan.obj" \
    "$(INTDIR)\gesoup.obj" \
    "$(INTDIR)\gesrch.obj" \
    "$(INTDIR)\getpls.obj" \
    "$(INTDIR)\geusrbse.obj" \
    "$(INTDIR)\geutil.obj" \
    "$(INTDIR)\geutil2.obj" \
    "$(INTDIR)\geview.obj" \
    "$(INTDIR)\gmarea.obj" \
    "$(INTDIR)\gedcyg.res"

GOLDED_OBJS= \
    "$(INTDIR)\golded3.obj"

GOLDNODE_OBJS= \
    "$(INTDIR)\goldnode.obj"

RDDT_OBJS= \
    "$(INTDIR)\rddt.obj"


"$(OUTDIR)\gedwind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS) $(GOLDED_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) /pdb:"$(OUTDIR)\gedwind.pdb" /out:"$(OUTDIR)\gedwind.exe" $(LINK32_OBJS) $(GOLDED_OBJS)
<<

"$(OUTDIR)\gnwind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS) $(GOLDNODE_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) /pdb:"$(OUTDIR)\gnwind.pdb" /out:"$(OUTDIR)\gnwind.exe" $(LINK32_OBJS) $(GOLDNODE_OBJS)
<<

"$(OUTDIR)\rddtwind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS) $(RDDT_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) /pdb:"$(OUTDIR)\rddtwind.pdb" /out:"$(OUTDIR)\rddtwind.exe" $(LINK32_OBJS) $(RDDT_OBJS)
<<


SOURCE="$(InputPath)"


!ENDIF


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("golded.dep")
!INCLUDE "golded.dep"
!ELSE
!MESSAGE Warning: cannot find "golded.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "golded - Win32 Release" || "$(CFG)" == "golded - Win32 Debug"
SOURCE=.\golded3\gcalst.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcalst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcalst.obj"  "$(INTDIR)\gcalst.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gcarea.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcarea.obj"  "$(INTDIR)\gcarea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg.obj"  "$(INTDIR)\gccfgg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg0.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg0.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg0.obj" "$(INTDIR)\gccfgg0.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg1.obj" "$(INTDIR)\gccfgg1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg2.obj" "$(INTDIR)\gccfgg2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg3.obj" "$(INTDIR)\gccfgg3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg4.obj" "$(INTDIR)\gccfgg4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg5.obj" "$(INTDIR)\gccfgg5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg6.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg6.obj" "$(INTDIR)\gccfgg6.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg7.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg7.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg7.obj" "$(INTDIR)\gccfgg7.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg8.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gccfgg8.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gccfgg8.obj" "$(INTDIR)\gccfgg8.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gckeys.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gckeys.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gckeys.obj"  "$(INTDIR)\gckeys.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gclang.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gclang.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gclang.obj"  "$(INTDIR)\gclang.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gcmisc.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcmisc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcmisc.obj"  "$(INTDIR)\gcmisc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gealst.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gealst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gealst.obj"  "$(INTDIR)\gealst.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gearea.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gearea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gearea.obj"  "$(INTDIR)\gearea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gecarb.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gecarb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gecarb.obj"  "$(INTDIR)\gecarb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gecmfd.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gecmfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gecmfd.obj"  "$(INTDIR)\gecmfd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gectnr.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gectnr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gectnr.obj"  "$(INTDIR)\gectnr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gectrl.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gectrl.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gectrl.obj"  "$(INTDIR)\gectrl.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gedoit.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gedoit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gedoit.obj"  "$(INTDIR)\gedoit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gedoss.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gedoss.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gedoss.obj"  "$(INTDIR)\gedoss.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geedit.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geedit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geedit.obj"  "$(INTDIR)\geedit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geedit2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geedit2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geedit2.obj" "$(INTDIR)\geedit2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geedit3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geedit3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geedit3.obj" "$(INTDIR)\geedit3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gefile.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gefile.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gefile.obj"  "$(INTDIR)\gefile.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gefind.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gefind.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gefind.obj"  "$(INTDIR)\gefind.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geglob.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geglob.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geglob.obj"  "$(INTDIR)\geglob.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gehdre.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gehdre.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gehdre.obj"  "$(INTDIR)\gehdre.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gehdrs.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gehdrs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gehdrs.obj"  "$(INTDIR)\gehdrs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gehtml.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gehtml.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gehtml.obj"  "$(INTDIR)\gehtml.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geinit.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geinit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geinit.obj"  "$(INTDIR)\geinit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geline.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geline.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geline.obj"  "$(INTDIR)\geline.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gelmsg.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gelmsg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gelmsg.obj"  "$(INTDIR)\gelmsg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemain.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gemain.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gemain.obj"  "$(INTDIR)\gemain.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemenu.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gemenu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gemenu.obj"  "$(INTDIR)\gemenu.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemlst.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gemlst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gemlst.obj"  "$(INTDIR)\gemlst.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemnus.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gemnus.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gemnus.obj"  "$(INTDIR)\gemnus.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemrks.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gemrks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gemrks.obj"  "$(INTDIR)\gemrks.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemsgid.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gemsgid.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gemsgid.obj" "$(INTDIR)\gemsgid.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemsgs.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gemsgs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gemsgs.obj"  "$(INTDIR)\gemsgs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\genode.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\genode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\genode.obj"  "$(INTDIR)\genode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geplay.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geplay.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geplay.obj"  "$(INTDIR)\geplay.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gepost.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gepost.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gepost.obj"  "$(INTDIR)\gepost.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geqwks.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geqwks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geqwks.obj"  "$(INTDIR)\geqwks.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gerand.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gerand.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gerand.obj"  "$(INTDIR)\gerand.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geread.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geread.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geread.obj"  "$(INTDIR)\geread.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geread2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geread2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geread2.obj" "$(INTDIR)\geread2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gescan.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gescan.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gescan.obj"  "$(INTDIR)\gescan.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gesoup.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gesoup.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gesoup.obj"  "$(INTDIR)\gesoup.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gesrch.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gesrch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gesrch.obj"  "$(INTDIR)\gesrch.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\getpls.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\getpls.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\getpls.obj"  "$(INTDIR)\getpls.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geusrbse.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geusrbse.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geusrbse.obj"    "$(INTDIR)\geusrbse.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geutil.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geutil.obj"  "$(INTDIR)\geutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geutil2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geutil2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geutil2.obj" "$(INTDIR)\geutil2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geview.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geview.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geview.obj"  "$(INTDIR)\geview.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gmarea.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmarea.obj"  "$(INTDIR)\gmarea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\golded3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\golded3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\golded3.obj" "$(INTDIR)\golded3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\be_lock.c
SOURCE=.\goldlib\gall\gbmh.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gbmh.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gbmh.obj"    "$(INTDIR)\gbmh.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcharset.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcharset.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcharset.obj"    "$(INTDIR)\gcharset.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrc16tb.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrc16tb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrc16tb.obj"    "$(INTDIR)\gcrc16tb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrc32tb.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrc32tb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrc32tb.obj"    "$(INTDIR)\gcrc32tb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrchash.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrchash.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrchash.obj"    "$(INTDIR)\gcrchash.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrckeyv.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrckeyv.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrckeyv.obj"    "$(INTDIR)\gcrckeyv.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcm16.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrcm16.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrcm16.obj" "$(INTDIR)\gcrcm16.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcm32.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrcm32.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrcm32.obj" "$(INTDIR)\gcrcm32.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcs16.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrcs16.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrcs16.obj" "$(INTDIR)\gcrcs16.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcs32.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gcrcs32.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gcrcs32.obj" "$(INTDIR)\gcrcs32.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gdbgerr.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gdbgerr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gdbgerr.obj" "$(INTDIR)\gdbgerr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gdbgexit.cpp
SOURCE=.\goldlib\gall\gdbgtrk.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gdbgtrk.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gdbgtrk.obj" "$(INTDIR)\gdbgtrk.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gdirposx.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gdirposx.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gdirposx.obj"    "$(INTDIR)\gdirposx.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\geval.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\geval.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\geval.obj"   "$(INTDIR)\geval.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gevalhum.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gevalhum.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gevalhum.obj"    "$(INTDIR)\gevalhum.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gevalrpn.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gevalrpn.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gevalrpn.obj"    "$(INTDIR)\gevalrpn.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfile.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gfile.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gfile.obj"   "$(INTDIR)\gfile.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfilport.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gfilport.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gfilport.obj"    "$(INTDIR)\gfilport.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfilutl1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gfilutl1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gfilutl1.obj"    "$(INTDIR)\gfilutl1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfilutl2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gfilutl2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gfilutl2.obj"    "$(INTDIR)\gfilutl2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnaddr.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gftnaddr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gftnaddr.obj"    "$(INTDIR)\gftnaddr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnl.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gftnnl.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gftnnl.obj"  "$(INTDIR)\gftnnl.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlfd.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gftnnlfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gftnnlfd.obj"    "$(INTDIR)\gftnnlfd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlfu.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gftnnlfu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gftnnlfu.obj"    "$(INTDIR)\gftnnlfu.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlge.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gftnnlge.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gftnnlge.obj"    "$(INTDIR)\gftnnlge.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlv7.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gftnnlv7.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gftnnlv7.obj"    "$(INTDIR)\gftnnlv7.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfuzzy.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gfuzzy.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gfuzzy.obj"  "$(INTDIR)\gfuzzy.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\ghdrmime.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\ghdrmime.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\ghdrmime.obj"    "$(INTDIR)\ghdrmime.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gkbdbase.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gkbdbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gkbdbase.obj"    "$(INTDIR)\gkbdbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gkbdgetm.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gkbdgetm.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gkbdgetm.obj"    "$(INTDIR)\gkbdgetm.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gkbdunix.cpp
SOURCE=.\goldlib\gall\gkbdwait.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gkbdwait.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gkbdwait.obj"    "$(INTDIR)\gkbdwait.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\glog.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\glog.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\glog.obj"    "$(INTDIR)\glog.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmemdbg.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmemdbg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmemdbg.obj" "$(INTDIR)\gmemdbg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmemutil.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmemutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmemutil.obj"    "$(INTDIR)\gmemutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmoubase.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoubase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoubase.obj"    "$(INTDIR)\gmoubase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmsgattr.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmsgattr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmsgattr.obj"    "$(INTDIR)\gmsgattr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gprnutil.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gprnutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gprnutil.obj"    "$(INTDIR)\gprnutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gregex.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gregex.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gregex.obj"  "$(INTDIR)\gregex.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsearch.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gsearch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gsearch.obj" "$(INTDIR)\gsearch.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsigunix.cpp
SOURCE=.\goldlib\gall\gsnd.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gsnd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gsnd.obj"    "$(INTDIR)\gsnd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsndwrap.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gsndwrap.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gsndwrap.obj"    "$(INTDIR)\gsndwrap.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsrchmgr.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gsrchmgr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gsrchmgr.obj"    "$(INTDIR)\gsrchmgr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrbags.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gstrbags.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gstrbags.obj"    "$(INTDIR)\gstrbags.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrctyp.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gstrctyp.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gstrctyp.obj"    "$(INTDIR)\gstrctyp.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrmail.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gstrmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gstrmail.obj"    "$(INTDIR)\gstrmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrname.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gstrname.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gstrname.obj"    "$(INTDIR)\gstrname.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrutil.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gstrutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gstrutil.obj"    "$(INTDIR)\gstrutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gtimjuld.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gtimjuld.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gtimjuld.obj"    "$(INTDIR)\gtimjuld.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gtimutil.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gtimutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gtimutil.obj"    "$(INTDIR)\gtimutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gtxtpara.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gtxtpara.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gtxtpara.obj"    "$(INTDIR)\gtxtpara.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrbase.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrbase.obj"    "$(INTDIR)\gusrbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrezyc.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrezyc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrezyc.obj"    "$(INTDIR)\gusrezyc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrgold.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrgold.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrgold.obj"    "$(INTDIR)\gusrgold.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrhuds.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrhuds.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrhuds.obj"    "$(INTDIR)\gusrhuds.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrmax.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrmax.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrmax.obj" "$(INTDIR)\gusrmax.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrpcb.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrpcb.obj" "$(INTDIR)\gusrpcb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrra2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrra2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrra2.obj" "$(INTDIR)\gusrra2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrxbbs.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gusrxbbs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gusrxbbs.obj"    "$(INTDIR)\gusrxbbs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlbeos.cpp
SOURCE=.\goldlib\gall\gutlclip.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlclip.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlclip.obj"    "$(INTDIR)\gutlclip.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlcode.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlcode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlcode.obj"    "$(INTDIR)\gutlcode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutldos.cpp
SOURCE=.\goldlib\gall\gutlgrp.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlgrp.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlgrp.obj" "$(INTDIR)\gutlgrp.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlmisc.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlmisc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlmisc.obj"    "$(INTDIR)\gutlmisc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlmtsk.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlmtsk.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlmtsk.obj"    "$(INTDIR)\gutlmtsk.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlos2.cpp
SOURCE=.\goldlib\gall\gutlos2m.cpp
SOURCE=.\goldlib\gall\gutltag.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutltag.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutltag.obj" "$(INTDIR)\gutltag.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlunix.cpp
SOURCE=.\goldlib\gall\gutlvers.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlvers.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlvers.obj"    "$(INTDIR)\gutlvers.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlwin.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlwin.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlwin.obj" "$(INTDIR)\gutlwin.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlwinm.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gutlwinm.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gutlwinm.obj"    "$(INTDIR)\gutlwinm.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gvidbase.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gvidbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gvidbase.obj"    "$(INTDIR)\gvidbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gvidinit.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gvidinit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gvidinit.obj"    "$(INTDIR)\gvidinit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwildmat.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwildmat.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwildmat.obj"    "$(INTDIR)\gwildmat.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinbase.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinbase.obj"    "$(INTDIR)\gwinbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwindow.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwindow.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwindow.obj" "$(INTDIR)\gwindow.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinhlp1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinhlp1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinhlp1.obj"    "$(INTDIR)\gwinhlp1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinhlp2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinhlp2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinhlp2.obj"    "$(INTDIR)\gwinhlp2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwininit.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwininit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwininit.obj"    "$(INTDIR)\gwininit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinline.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinline.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinline.obj"    "$(INTDIR)\gwinline.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinmenu.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinmenu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinmenu.obj"    "$(INTDIR)\gwinmenu.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinmnub.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinmnub.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinmnub.obj"    "$(INTDIR)\gwinmnub.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinpckf.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinpckf.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinpckf.obj"    "$(INTDIR)\gwinpckf.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinpcks.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinpcks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinpcks.obj"    "$(INTDIR)\gwinpcks.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinpick.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinpick.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinpick.obj"    "$(INTDIR)\gwinpick.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinput2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gwinput2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gwinput2.obj"    "$(INTDIR)\gwinput2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gedacfg.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gedacfg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gedacfg.obj" "$(INTDIR)\gedacfg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxareas.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxareas.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxareas.obj" "$(INTDIR)\gxareas.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxcrash.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxcrash.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxcrash.obj" "$(INTDIR)\gxcrash.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxdb.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxdb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxdb.obj"    "$(INTDIR)\gxdb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxdutch.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxdutch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxdutch.obj" "$(INTDIR)\gxdutch.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxezy102.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxezy102.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxezy102.obj"    "$(INTDIR)\gxezy102.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxezy110.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxezy110.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxezy110.obj"    "$(INTDIR)\gxezy110.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfd.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfd.obj"    "$(INTDIR)\gxfd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfecho4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfecho4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfecho4.obj"    "$(INTDIR)\gxfecho4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfecho5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfecho5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfecho5.obj"    "$(INTDIR)\gxfecho5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfecho6.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfecho6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfecho6.obj"    "$(INTDIR)\gxfecho6.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfidpcb.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfidpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfidpcb.obj"    "$(INTDIR)\gxfidpcb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfm092.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfm092.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfm092.obj" "$(INTDIR)\gxfm092.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfm100.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfm100.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfm100.obj" "$(INTDIR)\gxfm100.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfm116.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxfm116.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxfm116.obj" "$(INTDIR)\gxfm116.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxgecho.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxgecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxgecho.obj" "$(INTDIR)\gxgecho.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxhpt.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxhpt.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxhpt.obj"   "$(INTDIR)\gxhpt.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gximail4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gximail4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gximail4.obj"    "$(INTDIR)\gximail4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gximail5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gximail5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gximail5.obj"    "$(INTDIR)\gximail5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gximail6.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gximail6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gximail6.obj"    "$(INTDIR)\gximail6.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxinter.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxinter.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxinter.obj" "$(INTDIR)\gxinter.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxlora.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxlora.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxlora.obj"  "$(INTDIR)\gxlora.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxmax3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxmax3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxmax3.obj"  "$(INTDIR)\gxmax3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxme2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxme2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxme2.obj"   "$(INTDIR)\gxme2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxopus.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxopus.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxopus.obj"  "$(INTDIR)\gxopus.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxpcb.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxpcb.obj"   "$(INTDIR)\gxpcb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxportal.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxportal.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxportal.obj"    "$(INTDIR)\gxportal.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxprobrd.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxprobrd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxprobrd.obj"    "$(INTDIR)\gxprobrd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxqecho.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxqecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxqecho.obj" "$(INTDIR)\gxqecho.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxqfront.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxqfront.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxqfront.obj"    "$(INTDIR)\gxqfront.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxquick.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxquick.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxquick.obj" "$(INTDIR)\gxquick.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxra.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxra.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxra.obj"    "$(INTDIR)\gxra.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxraecho.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxraecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxraecho.obj"    "$(INTDIR)\gxraecho.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxspace.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxspace.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxspace.obj" "$(INTDIR)\gxspace.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxsquish.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxsquish.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxsquish.obj"    "$(INTDIR)\gxsquish.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxsuper.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxsuper.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxsuper.obj" "$(INTDIR)\gxsuper.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxsync.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxsync.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxsync.obj"  "$(INTDIR)\gxsync.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxtimed.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxtimed.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxtimed.obj" "$(INTDIR)\gxtimed.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxtmail.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxtmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxtmail.obj" "$(INTDIR)\gxtmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxts.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxts.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxts.obj"    "$(INTDIR)\gxts.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxwmail.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxwmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxwmail.obj" "$(INTDIR)\gxwmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxwtr.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxwtr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxwtr.obj"   "$(INTDIR)\gxwtr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxxbbs.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxxbbs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxxbbs.obj"  "$(INTDIR)\gxxbbs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxxmail.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gxxmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gxxmail.obj" "$(INTDIR)\gxxmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\glibc\dummy.c
SOURCE=.\goldlib\glibc\fnmatch.c
SOURCE=.\goldlib\glibc\glob.c
SOURCE=.\goldlib\glibc\regex.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\regex.obj"   "$(INTDIR)\regex.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoarea.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoarea.obj" "$(INTDIR)\gmoarea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoezyc1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoezyc1.obj"    "$(INTDIR)\gmoezyc1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoezyc2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoezyc2.obj"    "$(INTDIR)\gmoezyc2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoezyc3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoezyc3.obj"    "$(INTDIR)\gmoezyc3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoezyc4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoezyc4.obj"    "$(INTDIR)\gmoezyc4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoezyc5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoezyc5.obj"    "$(INTDIR)\gmoezyc5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmofido1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmofido1.obj"    "$(INTDIR)\gmofido1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmofido2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmofido2.obj"    "$(INTDIR)\gmofido2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmofido3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmofido3.obj"    "$(INTDIR)\gmofido3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmofido4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmofido4.obj"    "$(INTDIR)\gmofido4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmofido5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmofido5.obj"    "$(INTDIR)\gmofido5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmohuds.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmohuds.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmohuds.obj" "$(INTDIR)\gmohuds.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmohuds1.cpp
SOURCE=.\goldlib\gmb3\gmohuds2.cpp
SOURCE=.\goldlib\gmb3\gmohuds3.cpp
SOURCE=.\goldlib\gmb3\gmohuds4.cpp
SOURCE=.\goldlib\gmb3\gmohuds5.cpp
SOURCE=.\goldlib\gmb3\gmojamm1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmojamm1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmojamm1.obj"    "$(INTDIR)\gmojamm1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmojamm2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmojamm2.obj"    "$(INTDIR)\gmojamm2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmojamm3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmojamm3.obj"    "$(INTDIR)\gmojamm3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmojamm4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmojamm4.obj"    "$(INTDIR)\gmojamm4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmojamm5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmojamm5.obj"    "$(INTDIR)\gmojamm5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmopcbd1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmopcbd1.obj"    "$(INTDIR)\gmopcbd1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmopcbd2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmopcbd2.obj"    "$(INTDIR)\gmopcbd2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmopcbd3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmopcbd3.obj"    "$(INTDIR)\gmopcbd3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmopcbd4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmopcbd4.obj"    "$(INTDIR)\gmopcbd4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmopcbd5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmopcbd5.obj"    "$(INTDIR)\gmopcbd5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosmb1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmosmb1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmosmb1.obj" "$(INTDIR)\gmosmb1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosmb2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmosmb2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmosmb2.obj" "$(INTDIR)\gmosmb2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmosqsh1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmosqsh1.obj"    "$(INTDIR)\gmosqsh1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmosqsh2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmosqsh2.obj"    "$(INTDIR)\gmosqsh2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmosqsh3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmosqsh3.obj"    "$(INTDIR)\gmosqsh3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmosqsh4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmosqsh4.obj"    "$(INTDIR)\gmosqsh4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmosqsh5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmosqsh5.obj"    "$(INTDIR)\gmosqsh5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmowcat1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmowcat1.obj"    "$(INTDIR)\gmowcat1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmowcat2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmowcat2.obj"    "$(INTDIR)\gmowcat2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmowcat3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmowcat3.obj"    "$(INTDIR)\gmowcat3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmowcat4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmowcat4.obj"    "$(INTDIR)\gmowcat4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmowcat5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmowcat5.obj"    "$(INTDIR)\gmowcat5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoxbbs1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoxbbs1.obj"    "$(INTDIR)\gmoxbbs1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoxbbs2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoxbbs2.obj"    "$(INTDIR)\gmoxbbs2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoxbbs3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoxbbs3.obj"    "$(INTDIR)\gmoxbbs3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoxbbs4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoxbbs4.obj"    "$(INTDIR)\gmoxbbs4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gmoxbbs5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gmoxbbs5.obj"    "$(INTDIR)\gmoxbbs5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\fexist.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\fexist.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\fexist.obj"  "$(INTDIR)\fexist.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\ffind.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\ffind.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\ffind.obj"   "$(INTDIR)\ffind.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\genmsgid.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\genmsgid.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\genmsgid.obj"    "$(INTDIR)\genmsgid.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\patmat.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\patmat.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\patmat.obj"  "$(INTDIR)\patmat.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\smblib\lzh.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\lzh.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\lzh.obj" "$(INTDIR)\lzh.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\smblib\smblib.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\smblib.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\smblib.obj"  "$(INTDIR)\smblib.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\fptools.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\fptools.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\fptools.obj" "$(INTDIR)\fptools.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uucheck.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\uucheck.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\uucheck.obj" "$(INTDIR)\uucheck.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uuencode.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\uuencode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\uuencode.obj"    "$(INTDIR)\uuencode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uulib.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\uulib.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\uulib.obj"   "$(INTDIR)\uulib.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uunconc.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\uunconc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\uunconc.obj" "$(INTDIR)\uunconc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uuscan.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\uuscan.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\uuscan.obj"  "$(INTDIR)\uuscan.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uustring.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\uustring.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\uustring.obj"    "$(INTDIR)\uustring.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uuutil.c

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\uuutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\uuutil.obj"  "$(INTDIR)\uuutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gedcyg.rc

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gedcyg.res" : $(SOURCE) "$(INTDIR)"
    $(RSC) /fo"$(INTDIR)\gedcyg.res" /i "golded3" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gedcyg.res" : $(SOURCE) "$(INTDIR)"
    $(RSC) /fo"$(INTDIR)\gedcyg.res" /i "golded3" /d "_DEBUG" $(SOURCE)


!ENDIF


SOURCE=.\rddt\rddt.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

"$(INTDIR)\rddt.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

"$(INTDIR)\rddt.obj"  "$(INTDIR)\rddt.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ENDIF

SOURCE=.\goldnode\goldnode.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

"$(INTDIR)\goldnode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

"$(INTDIR)\goldnode.obj"  "$(INTDIR)\goldnode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ENDIF

!ENDIF

