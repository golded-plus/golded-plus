# Microsoft Visual Studio 6 makefile.
# Usage:
#
# NMAKE /f "golded.mak" CFG="Release"
#
# or
#
# NMAKE /f "golded.mak" CFG="Debug"
#
#

!IF "$(CFG)" == ""
CFG=Release
!MESSAGE No configuration specified. Defaulting to "Release".
!ENDIF

!IF "$(CFG)" != "Release" && "$(CFG)" != "Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "golded.mak" CFG="Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Debug" (based on "Win32 (x86) Console Application")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

CPP=@cl.exe
CPP=@cl.exe /I "C:\Program Files\Microsoft Visual Studio\VC98\Include"

LINK32=link.exe
LINK32=link.exe /LIBPATH:"C:\Program Files\Microsoft Visual Studio\VC98\Lib"


!IF  "$(CFG)" == "Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\gedwin.exe" "$(OUTDIR)\rddtwin.exe" "$(OUTDIR)\gnwin.exe"

CLEAN :
    -@erase "$(INTDIR)\*.obj"

distclean:
    -@erase "$(OUTDIR)\gedwin.exe"
    -@erase "$(OUTDIR)\gnwin.exe"
    -@erase "$(OUTDIR)\rddtwin.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

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


RSC=rc.exe
RSC_PROJ= /i "golded3" /fo"$(INTDIR)\gedcyg.res" /d "NDEBUG"

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
    "$(INTDIR)\golded3.obj" \
    "$(INTDIR)\gedcyg.res"

GOLDED_OBJS= \
    "$(INTDIR)\gemain.obj"

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


!ELSEIF  "$(CFG)" == "Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\gedwind.exe" "$(OUTDIR)\rddtwind.exe" "$(OUTDIR)\gnwind.exe"

CLEAN :
    -@erase "$(INTDIR)\*.obj"
    -@erase "$(INTDIR)\*.idb"
    -@erase "$(INTDIR)\*.pdb"
    -@erase "$(OUTDIR)\*.ilk"

distclean:
    -@erase "$(OUTDIR)\gedwind.exe"
    -@erase "$(OUTDIR)\gnwind.exe"
    -@erase "$(OUTDIR)\rddtwind.exe"


"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Progra~1\Micros~2\VC98\Include\\" /I ".\golded3" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /GZ /c

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


RSC=rc.exe
RSC_PROJ= /i "golded3" /fo"$(INTDIR)\gedcyg.res" /d "_DEBUG"

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
    "$(INTDIR)\golded3.obj" \
    "$(INTDIR)\gedcyg.res"

GOLDED_OBJS= \
    "$(INTDIR)\gemain.obj"

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


!IF "$(CFG)" == "Release" || "$(CFG)" == "Debug"

SOURCE=.\golded3\gcalst.cpp
"$(INTDIR)\gcalst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gcarea.cpp
"$(INTDIR)\gcarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg.cpp
"$(INTDIR)\gccfgg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg0.cpp
"$(INTDIR)\gccfgg0.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg1.cpp
"$(INTDIR)\gccfgg1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg2.cpp
"$(INTDIR)\gccfgg2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg3.cpp
"$(INTDIR)\gccfgg3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg4.cpp
"$(INTDIR)\gccfgg4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg5.cpp
"$(INTDIR)\gccfgg5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg6.cpp
"$(INTDIR)\gccfgg6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg7.cpp
"$(INTDIR)\gccfgg7.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gccfgg8.cpp
"$(INTDIR)\gccfgg8.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gckeys.cpp
"$(INTDIR)\gckeys.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gclang.cpp
"$(INTDIR)\gclang.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gcmisc.cpp
"$(INTDIR)\gcmisc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gealst.cpp
"$(INTDIR)\gealst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gearea.cpp
"$(INTDIR)\gearea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gecarb.cpp
"$(INTDIR)\gecarb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gecmfd.cpp
"$(INTDIR)\gecmfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gectnr.cpp
"$(INTDIR)\gectnr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gectrl.cpp
"$(INTDIR)\gectrl.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gedoit.cpp
"$(INTDIR)\gedoit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gedoss.cpp
"$(INTDIR)\gedoss.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geedit.cpp
"$(INTDIR)\geedit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geedit2.cpp
"$(INTDIR)\geedit2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geedit3.cpp
"$(INTDIR)\geedit3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gefile.cpp
"$(INTDIR)\gefile.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gefind.cpp
"$(INTDIR)\gefind.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geglob.cpp
"$(INTDIR)\geglob.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gehdre.cpp
"$(INTDIR)\gehdre.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gehdrs.cpp
"$(INTDIR)\gehdrs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gehtml.cpp
"$(INTDIR)\gehtml.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geinit.cpp
"$(INTDIR)\geinit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geline.cpp
"$(INTDIR)\geline.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gelmsg.cpp
"$(INTDIR)\gelmsg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gemain.cpp
"$(INTDIR)\gemain.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gemenu.cpp
"$(INTDIR)\gemenu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gemlst.cpp
"$(INTDIR)\gemlst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gemnus.cpp
"$(INTDIR)\gemnus.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gemrks.cpp
"$(INTDIR)\gemrks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gemsgid.cpp
"$(INTDIR)\gemsgid.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gemsgs.cpp
"$(INTDIR)\gemsgs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\genode.cpp
"$(INTDIR)\genode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geplay.cpp
"$(INTDIR)\geplay.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gepost.cpp
"$(INTDIR)\gepost.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geqwks.cpp
"$(INTDIR)\geqwks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gerand.cpp
"$(INTDIR)\gerand.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geread.cpp
"$(INTDIR)\geread.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geread2.cpp
"$(INTDIR)\geread2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gescan.cpp
"$(INTDIR)\gescan.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gesoup.cpp
"$(INTDIR)\gesoup.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gesrch.cpp
"$(INTDIR)\gesrch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\getpls.cpp
"$(INTDIR)\getpls.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geusrbse.cpp
"$(INTDIR)\geusrbse.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geutil.cpp
"$(INTDIR)\geutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geutil2.cpp
"$(INTDIR)\geutil2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\geview.cpp
"$(INTDIR)\geview.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gmarea.cpp
"$(INTDIR)\gmarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\golded3.cpp
"$(INTDIR)\golded3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gbmh.cpp
"$(INTDIR)\gbmh.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcharset.cpp
"$(INTDIR)\gcharset.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrc16tb.cpp
"$(INTDIR)\gcrc16tb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrc32tb.cpp
"$(INTDIR)\gcrc32tb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrchash.cpp
"$(INTDIR)\gcrchash.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrckeyv.cpp
"$(INTDIR)\gcrckeyv.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrcm16.cpp
"$(INTDIR)\gcrcm16.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrcm32.cpp
"$(INTDIR)\gcrcm32.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrcs16.cpp
"$(INTDIR)\gcrcs16.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gcrcs32.cpp
"$(INTDIR)\gcrcs32.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gdbgerr.cpp
"$(INTDIR)\gdbgerr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gdbgtrk.cpp
"$(INTDIR)\gdbgtrk.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gdirposx.cpp
"$(INTDIR)\gdirposx.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\geval.cpp
"$(INTDIR)\geval.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gevalhum.cpp
"$(INTDIR)\gevalhum.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gevalrpn.cpp
"$(INTDIR)\gevalrpn.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gfile.cpp
"$(INTDIR)\gfile.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gfilport.cpp
"$(INTDIR)\gfilport.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gfilutl1.cpp
"$(INTDIR)\gfilutl1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gfilutl2.cpp
"$(INTDIR)\gfilutl2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gftnaddr.cpp
"$(INTDIR)\gftnaddr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gftnnl.cpp
"$(INTDIR)\gftnnl.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gftnnlfd.cpp
"$(INTDIR)\gftnnlfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gftnnlfu.cpp
"$(INTDIR)\gftnnlfu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gftnnlge.cpp
"$(INTDIR)\gftnnlge.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gftnnlv7.cpp
"$(INTDIR)\gftnnlv7.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gfuzzy.cpp
"$(INTDIR)\gfuzzy.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\ghdrmime.cpp
"$(INTDIR)\ghdrmime.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gkbdbase.cpp
"$(INTDIR)\gkbdbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gkbdgetm.cpp
"$(INTDIR)\gkbdgetm.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gkbdwait.cpp
"$(INTDIR)\gkbdwait.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\glog.cpp
"$(INTDIR)\glog.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gmemdbg.cpp
"$(INTDIR)\gmemdbg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gmemutil.cpp
"$(INTDIR)\gmemutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gmoubase.cpp
"$(INTDIR)\gmoubase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gmsgattr.cpp
"$(INTDIR)\gmsgattr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gprnutil.cpp
"$(INTDIR)\gprnutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gregex.cpp
"$(INTDIR)\gregex.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gsearch.cpp
"$(INTDIR)\gsearch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gsnd.cpp
"$(INTDIR)\gsnd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gsndwrap.cpp
"$(INTDIR)\gsndwrap.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gsrchmgr.cpp
"$(INTDIR)\gsrchmgr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gstrbags.cpp
"$(INTDIR)\gstrbags.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gstrctyp.cpp
"$(INTDIR)\gstrctyp.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gstrmail.cpp
"$(INTDIR)\gstrmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gstrname.cpp
"$(INTDIR)\gstrname.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gstrutil.cpp
"$(INTDIR)\gstrutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gtimjuld.cpp
"$(INTDIR)\gtimjuld.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gtimutil.cpp
"$(INTDIR)\gtimutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gtxtpara.cpp
"$(INTDIR)\gtxtpara.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrbase.cpp
"$(INTDIR)\gusrbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrezyc.cpp
"$(INTDIR)\gusrezyc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrgold.cpp
"$(INTDIR)\gusrgold.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrhuds.cpp
"$(INTDIR)\gusrhuds.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrmax.cpp
"$(INTDIR)\gusrmax.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrpcb.cpp
"$(INTDIR)\gusrpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrra2.cpp
"$(INTDIR)\gusrra2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gusrxbbs.cpp
"$(INTDIR)\gusrxbbs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlclip.cpp
"$(INTDIR)\gutlclip.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlcode.cpp
"$(INTDIR)\gutlcode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlgrp.cpp
"$(INTDIR)\gutlgrp.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlmisc.cpp
"$(INTDIR)\gutlmisc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlmtsk.cpp
"$(INTDIR)\gutlmtsk.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutltag.cpp
"$(INTDIR)\gutltag.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlvers.cpp
"$(INTDIR)\gutlvers.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlwin.cpp
"$(INTDIR)\gutlwin.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gutlwinm.cpp
"$(INTDIR)\gutlwinm.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gvidbase.cpp
"$(INTDIR)\gvidbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gvidinit.cpp
"$(INTDIR)\gvidinit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwildmat.cpp
"$(INTDIR)\gwildmat.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinbase.cpp
"$(INTDIR)\gwinbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwindow.cpp
"$(INTDIR)\gwindow.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinhlp1.cpp
"$(INTDIR)\gwinhlp1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinhlp2.cpp
"$(INTDIR)\gwinhlp2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwininit.cpp
"$(INTDIR)\gwininit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinline.cpp
"$(INTDIR)\gwinline.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinmenu.cpp
"$(INTDIR)\gwinmenu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinmnub.cpp
"$(INTDIR)\gwinmnub.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinpckf.cpp
"$(INTDIR)\gwinpckf.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinpcks.cpp
"$(INTDIR)\gwinpcks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinpick.cpp
"$(INTDIR)\gwinpick.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gall\gwinput2.cpp
"$(INTDIR)\gwinput2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gedacfg.cpp
"$(INTDIR)\gedacfg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxareas.cpp
"$(INTDIR)\gxareas.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxcrash.cpp
"$(INTDIR)\gxcrash.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxdb.cpp
"$(INTDIR)\gxdb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxdutch.cpp
"$(INTDIR)\gxdutch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxezy102.cpp
"$(INTDIR)\gxezy102.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxezy110.cpp
"$(INTDIR)\gxezy110.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfd.cpp
"$(INTDIR)\gxfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfecho4.cpp
"$(INTDIR)\gxfecho4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfecho5.cpp
"$(INTDIR)\gxfecho5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfecho6.cpp
"$(INTDIR)\gxfecho6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfidpcb.cpp
"$(INTDIR)\gxfidpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfm092.cpp
"$(INTDIR)\gxfm092.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfm100.cpp
"$(INTDIR)\gxfm100.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxfm116.cpp
"$(INTDIR)\gxfm116.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxgecho.cpp
"$(INTDIR)\gxgecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxhpt.cpp
"$(INTDIR)\gxhpt.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gximail4.cpp
"$(INTDIR)\gximail4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gximail5.cpp
"$(INTDIR)\gximail5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gximail6.cpp
"$(INTDIR)\gximail6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxinter.cpp
"$(INTDIR)\gxinter.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxlora.cpp
"$(INTDIR)\gxlora.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxmax3.cpp
"$(INTDIR)\gxmax3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxme2.cpp
"$(INTDIR)\gxme2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxopus.cpp
"$(INTDIR)\gxopus.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxpcb.cpp
"$(INTDIR)\gxpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxportal.cpp
"$(INTDIR)\gxportal.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxprobrd.cpp
"$(INTDIR)\gxprobrd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxqecho.cpp
"$(INTDIR)\gxqecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxqfront.cpp
"$(INTDIR)\gxqfront.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxquick.cpp
"$(INTDIR)\gxquick.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxra.cpp
"$(INTDIR)\gxra.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxraecho.cpp
"$(INTDIR)\gxraecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxspace.cpp
"$(INTDIR)\gxspace.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxsquish.cpp
"$(INTDIR)\gxsquish.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxsuper.cpp
"$(INTDIR)\gxsuper.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxsync.cpp
"$(INTDIR)\gxsync.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxtimed.cpp
"$(INTDIR)\gxtimed.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxtmail.cpp
"$(INTDIR)\gxtmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxts.cpp
"$(INTDIR)\gxts.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxwmail.cpp
"$(INTDIR)\gxwmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxwtr.cpp
"$(INTDIR)\gxwtr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxxbbs.cpp
"$(INTDIR)\gxxbbs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gcfg\gxxmail.cpp
"$(INTDIR)\gxxmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\glibc\regex.c
"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoarea.cpp
"$(INTDIR)\gmoarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoezyc1.cpp
"$(INTDIR)\gmoezyc1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoezyc2.cpp
"$(INTDIR)\gmoezyc2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoezyc3.cpp
"$(INTDIR)\gmoezyc3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoezyc4.cpp
"$(INTDIR)\gmoezyc4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoezyc5.cpp
"$(INTDIR)\gmoezyc5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmofido1.cpp
"$(INTDIR)\gmofido1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmofido2.cpp
"$(INTDIR)\gmofido2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmofido3.cpp
"$(INTDIR)\gmofido3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmofido4.cpp
"$(INTDIR)\gmofido4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmofido5.cpp
"$(INTDIR)\gmofido5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmohuds.cpp
"$(INTDIR)\gmohuds.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmojamm1.cpp
"$(INTDIR)\gmojamm1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmojamm2.cpp
"$(INTDIR)\gmojamm2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmojamm3.cpp
"$(INTDIR)\gmojamm3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmojamm4.cpp
"$(INTDIR)\gmojamm4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmojamm5.cpp
"$(INTDIR)\gmojamm5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmopcbd1.cpp
"$(INTDIR)\gmopcbd1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmopcbd2.cpp
"$(INTDIR)\gmopcbd2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmopcbd3.cpp
"$(INTDIR)\gmopcbd3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmopcbd4.cpp
"$(INTDIR)\gmopcbd4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmopcbd5.cpp
"$(INTDIR)\gmopcbd5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmosmb1.cpp
"$(INTDIR)\gmosmb1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmosmb2.cpp
"$(INTDIR)\gmosmb2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmosqsh1.cpp
"$(INTDIR)\gmosqsh1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmosqsh2.cpp
"$(INTDIR)\gmosqsh2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmosqsh3.cpp
"$(INTDIR)\gmosqsh3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmosqsh4.cpp
"$(INTDIR)\gmosqsh4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmosqsh5.cpp
"$(INTDIR)\gmosqsh5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmowcat1.cpp
"$(INTDIR)\gmowcat1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmowcat2.cpp
"$(INTDIR)\gmowcat2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmowcat3.cpp
"$(INTDIR)\gmowcat3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmowcat4.cpp
"$(INTDIR)\gmowcat4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmowcat5.cpp
"$(INTDIR)\gmowcat5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoxbbs1.cpp
"$(INTDIR)\gmoxbbs1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoxbbs2.cpp
"$(INTDIR)\gmoxbbs2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoxbbs3.cpp
"$(INTDIR)\gmoxbbs3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoxbbs4.cpp
"$(INTDIR)\gmoxbbs4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\gmb3\gmoxbbs5.cpp
"$(INTDIR)\gmoxbbs5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\msgidlib\fexist.c
"$(INTDIR)\fexist.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\msgidlib\ffind.c
"$(INTDIR)\ffind.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\msgidlib\genmsgid.c
"$(INTDIR)\genmsgid.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\msgidlib\patmat.c
"$(INTDIR)\patmat.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\smblib\lzh.c
"$(INTDIR)\lzh.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\smblib\smblib.c
"$(INTDIR)\smblib.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\fptools.c
"$(INTDIR)\fptools.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\uucheck.c
"$(INTDIR)\uucheck.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\uuencode.c
"$(INTDIR)\uuencode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\uulib.c
"$(INTDIR)\uulib.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\uunconc.c
"$(INTDIR)\uunconc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\uuscan.c
"$(INTDIR)\uuscan.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\uustring.c
"$(INTDIR)\uustring.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldlib\uulib\uuutil.c
"$(INTDIR)\uuutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\golded3\gedcyg.rc
"$(INTDIR)\gedcyg.res" : $(SOURCE) "$(INTDIR)"
    $(RSC) $(RSC_PROJ) $(SOURCE)

SOURCE=.\rddt\rddt.cpp
"$(INTDIR)\rddt.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\goldnode\goldnode.cpp
"$(INTDIR)\goldnode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ENDIF

