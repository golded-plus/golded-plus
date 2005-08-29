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

#CPP=@cl.exe /I "C:\Program Files\Microsoft Visual Studio\VC98\Include"
CPP=@cl.exe

#LINK32=link.exe /LIBPATH:"C:\Program Files\Microsoft Visual Studio\VC98\Lib"
LINK32=link.exe


!IF  "$(CFG)" == "Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\gedwin.exe" "$(OUTDIR)\rddtwin.exe" "$(OUTDIR)\gnwin.exe"

CLEAN :
    -@erase "$(INTDIR)\*.obj"
    -@erase "$(INTDIR)\vc60.idb"

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
#BSC32=bscmake.exe
#BSC32_FLAGS=/nologo /o"$(OUTDIR)\golded.bsc"
#BSC32_SBRS= \

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

ALL : "$(OUTDIR)\gedwind.exe" "$(OUTDIR)\gedwind.bsc" "$(OUTDIR)\rddtwind.exe" "$(OUTDIR)\rddtwind.bsc" "$(OUTDIR)\gnwind.exe" "$(OUTDIR)\gnwind.bsc"

CLEAN :
    -@erase "$(INTDIR)\*.obj"
    -@erase "$(INTDIR)\*.sbr"
    -@erase "$(INTDIR)\*.idb"
    -@erase "$(INTDIR)\*.pdb"
    -@erase "$(OUTDIR)\*.bsc"
    -@erase "$(OUTDIR)\*.ilk"

distclean:
    -@erase "$(OUTDIR)\gedwind.exe"
    -@erase "$(OUTDIR)\gedwind.bsc"
    -@erase "$(OUTDIR)\gnwind.exe"
    -@erase "$(OUTDIR)\gnwind.bsc"
    -@erase "$(OUTDIR)\rddtwind.exe"
    -@erase "$(OUTDIR)\rddtwind.bsc"


"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

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
    "$(INTDIR)\gmarea.sbr" \
    "$(INTDIR)\golded3.sbr"

GOLDED_SBRS= \
    "$(INTDIR)\gemain.sbr"

GOLDNODE_SBRS= \
    "$(INTDIR)\goldnode.sbr"

RDDT_SBRS= \
    "$(INTDIR)\rddt.sbr"

"$(OUTDIR)\gedwind.bsc" : "$(OUTDIR)" $(BSC32_SBRS) $(GOLDED_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS)  /o"$(OUTDIR)\gedwind.bsc" $(BSC32_SBRS) $(GOLDED_SBRS)
<<

"$(OUTDIR)\gnwind.bsc" : "$(OUTDIR)" $(BSC32_SBRS) $(GOLDNODE_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) /o"$(OUTDIR)\gnwind.bsc" $(BSC32_SBRS) $(GOLDNODE_SBRS)
<<

"$(OUTDIR)\rddtwind.bsc" : "$(OUTDIR)" $(BSC32_SBRS) $(RDDT_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) /o"$(OUTDIR)\rddtwind.bsc" $(BSC32_SBRS) $(RDDT_SBRS)
<<

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

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcalst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcalst.obj"  "$(INTDIR)\gcalst.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gcarea.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcarea.obj"  "$(INTDIR)\gcarea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg.obj"  "$(INTDIR)\gccfgg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg0.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg0.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg0.obj" "$(INTDIR)\gccfgg0.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg1.obj" "$(INTDIR)\gccfgg1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg2.obj" "$(INTDIR)\gccfgg2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg3.obj" "$(INTDIR)\gccfgg3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg4.obj" "$(INTDIR)\gccfgg4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg5.obj" "$(INTDIR)\gccfgg5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg6.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg6.obj" "$(INTDIR)\gccfgg6.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg7.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg7.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg7.obj" "$(INTDIR)\gccfgg7.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gccfgg8.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gccfgg8.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gccfgg8.obj" "$(INTDIR)\gccfgg8.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gckeys.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gckeys.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gckeys.obj"  "$(INTDIR)\gckeys.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gclang.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gclang.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gclang.obj"  "$(INTDIR)\gclang.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gcmisc.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcmisc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcmisc.obj"  "$(INTDIR)\gcmisc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gealst.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gealst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gealst.obj"  "$(INTDIR)\gealst.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gearea.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gearea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gearea.obj"  "$(INTDIR)\gearea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gecarb.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gecarb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gecarb.obj"  "$(INTDIR)\gecarb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gecmfd.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gecmfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gecmfd.obj"  "$(INTDIR)\gecmfd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gectnr.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gectnr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gectnr.obj"  "$(INTDIR)\gectnr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gectrl.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gectrl.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gectrl.obj"  "$(INTDIR)\gectrl.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gedoit.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gedoit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gedoit.obj"  "$(INTDIR)\gedoit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gedoss.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gedoss.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gedoss.obj"  "$(INTDIR)\gedoss.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geedit.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geedit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geedit.obj"  "$(INTDIR)\geedit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geedit2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geedit2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geedit2.obj" "$(INTDIR)\geedit2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geedit3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geedit3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geedit3.obj" "$(INTDIR)\geedit3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gefile.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gefile.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gefile.obj"  "$(INTDIR)\gefile.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gefind.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gefind.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gefind.obj"  "$(INTDIR)\gefind.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geglob.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geglob.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geglob.obj"  "$(INTDIR)\geglob.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gehdre.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gehdre.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gehdre.obj"  "$(INTDIR)\gehdre.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gehdrs.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gehdrs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gehdrs.obj"  "$(INTDIR)\gehdrs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gehtml.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gehtml.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gehtml.obj"  "$(INTDIR)\gehtml.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geinit.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geinit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geinit.obj"  "$(INTDIR)\geinit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geline.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geline.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geline.obj"  "$(INTDIR)\geline.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gelmsg.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gelmsg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gelmsg.obj"  "$(INTDIR)\gelmsg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemain.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gemain.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gemain.obj"  "$(INTDIR)\gemain.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemenu.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gemenu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gemenu.obj"  "$(INTDIR)\gemenu.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemlst.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gemlst.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gemlst.obj"  "$(INTDIR)\gemlst.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemnus.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gemnus.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gemnus.obj"  "$(INTDIR)\gemnus.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemrks.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gemrks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gemrks.obj"  "$(INTDIR)\gemrks.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemsgid.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gemsgid.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gemsgid.obj" "$(INTDIR)\gemsgid.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gemsgs.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gemsgs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gemsgs.obj"  "$(INTDIR)\gemsgs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\genode.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\genode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\genode.obj"  "$(INTDIR)\genode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geplay.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geplay.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geplay.obj"  "$(INTDIR)\geplay.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gepost.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gepost.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gepost.obj"  "$(INTDIR)\gepost.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geqwks.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geqwks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geqwks.obj"  "$(INTDIR)\geqwks.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gerand.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gerand.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gerand.obj"  "$(INTDIR)\gerand.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geread.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geread.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geread.obj"  "$(INTDIR)\geread.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geread2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geread2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geread2.obj" "$(INTDIR)\geread2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gescan.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gescan.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gescan.obj"  "$(INTDIR)\gescan.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gesoup.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gesoup.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gesoup.obj"  "$(INTDIR)\gesoup.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gesrch.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gesrch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gesrch.obj"  "$(INTDIR)\gesrch.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\getpls.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\getpls.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\getpls.obj"  "$(INTDIR)\getpls.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geusrbse.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geusrbse.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geusrbse.obj"    "$(INTDIR)\geusrbse.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geutil.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geutil.obj"  "$(INTDIR)\geutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geutil2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geutil2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geutil2.obj" "$(INTDIR)\geutil2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\geview.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geview.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geview.obj"  "$(INTDIR)\geview.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gmarea.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmarea.obj"  "$(INTDIR)\gmarea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\golded3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\golded3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\golded3.obj" "$(INTDIR)\golded3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\be_lock.c
SOURCE=.\goldlib\gall\gbmh.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gbmh.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gbmh.obj"    "$(INTDIR)\gbmh.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcharset.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcharset.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcharset.obj"    "$(INTDIR)\gcharset.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrc16tb.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrc16tb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrc16tb.obj"    "$(INTDIR)\gcrc16tb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrc32tb.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrc32tb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrc32tb.obj"    "$(INTDIR)\gcrc32tb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrchash.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrchash.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrchash.obj"    "$(INTDIR)\gcrchash.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrckeyv.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrckeyv.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrckeyv.obj"    "$(INTDIR)\gcrckeyv.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcm16.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrcm16.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrcm16.obj" "$(INTDIR)\gcrcm16.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcm32.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrcm32.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrcm32.obj" "$(INTDIR)\gcrcm32.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcs16.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrcs16.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrcs16.obj" "$(INTDIR)\gcrcs16.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gcrcs32.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gcrcs32.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gcrcs32.obj" "$(INTDIR)\gcrcs32.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gdbgerr.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gdbgerr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gdbgerr.obj" "$(INTDIR)\gdbgerr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gdbgexit.cpp
SOURCE=.\goldlib\gall\gdbgtrk.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gdbgtrk.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gdbgtrk.obj" "$(INTDIR)\gdbgtrk.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gdirposx.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gdirposx.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gdirposx.obj"    "$(INTDIR)\gdirposx.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\geval.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\geval.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\geval.obj"   "$(INTDIR)\geval.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gevalhum.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gevalhum.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gevalhum.obj"    "$(INTDIR)\gevalhum.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gevalrpn.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gevalrpn.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gevalrpn.obj"    "$(INTDIR)\gevalrpn.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfile.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gfile.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gfile.obj"   "$(INTDIR)\gfile.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfilport.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gfilport.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gfilport.obj"    "$(INTDIR)\gfilport.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfilutl1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gfilutl1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gfilutl1.obj"    "$(INTDIR)\gfilutl1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfilutl2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gfilutl2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gfilutl2.obj"    "$(INTDIR)\gfilutl2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnaddr.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gftnaddr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gftnaddr.obj"    "$(INTDIR)\gftnaddr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnl.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gftnnl.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gftnnl.obj"  "$(INTDIR)\gftnnl.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlfd.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gftnnlfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gftnnlfd.obj"    "$(INTDIR)\gftnnlfd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlfu.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gftnnlfu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gftnnlfu.obj"    "$(INTDIR)\gftnnlfu.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlge.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gftnnlge.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gftnnlge.obj"    "$(INTDIR)\gftnnlge.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gftnnlv7.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gftnnlv7.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gftnnlv7.obj"    "$(INTDIR)\gftnnlv7.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gfuzzy.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gfuzzy.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gfuzzy.obj"  "$(INTDIR)\gfuzzy.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\ghdrmime.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\ghdrmime.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\ghdrmime.obj"    "$(INTDIR)\ghdrmime.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gkbdbase.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gkbdbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gkbdbase.obj"    "$(INTDIR)\gkbdbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gkbdgetm.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gkbdgetm.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gkbdgetm.obj"    "$(INTDIR)\gkbdgetm.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gkbdunix.cpp
SOURCE=.\goldlib\gall\gkbdwait.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gkbdwait.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gkbdwait.obj"    "$(INTDIR)\gkbdwait.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\glog.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\glog.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\glog.obj"    "$(INTDIR)\glog.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmemdbg.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmemdbg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmemdbg.obj" "$(INTDIR)\gmemdbg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmemutil.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmemutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmemutil.obj"    "$(INTDIR)\gmemutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmoubase.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoubase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoubase.obj"    "$(INTDIR)\gmoubase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gmsgattr.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmsgattr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmsgattr.obj"    "$(INTDIR)\gmsgattr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gprnutil.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gprnutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gprnutil.obj"    "$(INTDIR)\gprnutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gregex.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gregex.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gregex.obj"  "$(INTDIR)\gregex.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsearch.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gsearch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gsearch.obj" "$(INTDIR)\gsearch.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsigunix.cpp
SOURCE=.\goldlib\gall\gsnd.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gsnd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gsnd.obj"    "$(INTDIR)\gsnd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsndwrap.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gsndwrap.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gsndwrap.obj"    "$(INTDIR)\gsndwrap.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gsrchmgr.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gsrchmgr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gsrchmgr.obj"    "$(INTDIR)\gsrchmgr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrbags.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gstrbags.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gstrbags.obj"    "$(INTDIR)\gstrbags.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrctyp.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gstrctyp.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gstrctyp.obj"    "$(INTDIR)\gstrctyp.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrmail.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gstrmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gstrmail.obj"    "$(INTDIR)\gstrmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrname.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gstrname.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gstrname.obj"    "$(INTDIR)\gstrname.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gstrutil.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gstrutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gstrutil.obj"    "$(INTDIR)\gstrutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gtimjuld.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gtimjuld.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gtimjuld.obj"    "$(INTDIR)\gtimjuld.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gtimutil.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gtimutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gtimutil.obj"    "$(INTDIR)\gtimutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gtxtpara.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gtxtpara.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gtxtpara.obj"    "$(INTDIR)\gtxtpara.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrbase.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrbase.obj"    "$(INTDIR)\gusrbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrezyc.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrezyc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrezyc.obj"    "$(INTDIR)\gusrezyc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrgold.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrgold.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrgold.obj"    "$(INTDIR)\gusrgold.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrhuds.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrhuds.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrhuds.obj"    "$(INTDIR)\gusrhuds.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrmax.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrmax.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrmax.obj" "$(INTDIR)\gusrmax.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrpcb.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrpcb.obj" "$(INTDIR)\gusrpcb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrra2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrra2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrra2.obj" "$(INTDIR)\gusrra2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gusrxbbs.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gusrxbbs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gusrxbbs.obj"    "$(INTDIR)\gusrxbbs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlbeos.cpp
SOURCE=.\goldlib\gall\gutlclip.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlclip.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlclip.obj"    "$(INTDIR)\gutlclip.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlcode.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlcode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlcode.obj"    "$(INTDIR)\gutlcode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutldos.cpp
SOURCE=.\goldlib\gall\gutlgrp.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlgrp.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlgrp.obj" "$(INTDIR)\gutlgrp.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlmisc.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlmisc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlmisc.obj"    "$(INTDIR)\gutlmisc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlmtsk.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlmtsk.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlmtsk.obj"    "$(INTDIR)\gutlmtsk.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlos2.cpp
SOURCE=.\goldlib\gall\gutlos2m.cpp
SOURCE=.\goldlib\gall\gutltag.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutltag.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutltag.obj" "$(INTDIR)\gutltag.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlunix.cpp
SOURCE=.\goldlib\gall\gutlvers.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlvers.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlvers.obj"    "$(INTDIR)\gutlvers.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlwin.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlwin.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlwin.obj" "$(INTDIR)\gutlwin.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gutlwinm.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gutlwinm.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gutlwinm.obj"    "$(INTDIR)\gutlwinm.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gvidbase.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gvidbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gvidbase.obj"    "$(INTDIR)\gvidbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gvidinit.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gvidinit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gvidinit.obj"    "$(INTDIR)\gvidinit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwildmat.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwildmat.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwildmat.obj"    "$(INTDIR)\gwildmat.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinbase.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinbase.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinbase.obj"    "$(INTDIR)\gwinbase.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwindow.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwindow.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwindow.obj" "$(INTDIR)\gwindow.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinhlp1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinhlp1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinhlp1.obj"    "$(INTDIR)\gwinhlp1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinhlp2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinhlp2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinhlp2.obj"    "$(INTDIR)\gwinhlp2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwininit.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwininit.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwininit.obj"    "$(INTDIR)\gwininit.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinline.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinline.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinline.obj"    "$(INTDIR)\gwinline.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinmenu.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinmenu.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinmenu.obj"    "$(INTDIR)\gwinmenu.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinmnub.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinmnub.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinmnub.obj"    "$(INTDIR)\gwinmnub.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinpckf.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinpckf.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinpckf.obj"    "$(INTDIR)\gwinpckf.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinpcks.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinpcks.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinpcks.obj"    "$(INTDIR)\gwinpcks.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinpick.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinpick.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinpick.obj"    "$(INTDIR)\gwinpick.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gall\gwinput2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gwinput2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gwinput2.obj"    "$(INTDIR)\gwinput2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gedacfg.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gedacfg.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gedacfg.obj" "$(INTDIR)\gedacfg.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxareas.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxareas.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxareas.obj" "$(INTDIR)\gxareas.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxcrash.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxcrash.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxcrash.obj" "$(INTDIR)\gxcrash.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxdb.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxdb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxdb.obj"    "$(INTDIR)\gxdb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxdutch.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxdutch.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxdutch.obj" "$(INTDIR)\gxdutch.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxezy102.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxezy102.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxezy102.obj"    "$(INTDIR)\gxezy102.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxezy110.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxezy110.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxezy110.obj"    "$(INTDIR)\gxezy110.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfd.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfd.obj"    "$(INTDIR)\gxfd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfecho4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfecho4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfecho4.obj"    "$(INTDIR)\gxfecho4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfecho5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfecho5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfecho5.obj"    "$(INTDIR)\gxfecho5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfecho6.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfecho6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfecho6.obj"    "$(INTDIR)\gxfecho6.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfidpcb.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfidpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfidpcb.obj"    "$(INTDIR)\gxfidpcb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfm092.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfm092.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfm092.obj" "$(INTDIR)\gxfm092.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfm100.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfm100.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfm100.obj" "$(INTDIR)\gxfm100.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxfm116.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxfm116.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxfm116.obj" "$(INTDIR)\gxfm116.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxgecho.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxgecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxgecho.obj" "$(INTDIR)\gxgecho.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxhpt.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxhpt.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxhpt.obj"   "$(INTDIR)\gxhpt.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gximail4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gximail4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gximail4.obj"    "$(INTDIR)\gximail4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gximail5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gximail5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gximail5.obj"    "$(INTDIR)\gximail5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gximail6.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gximail6.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gximail6.obj"    "$(INTDIR)\gximail6.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxinter.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxinter.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxinter.obj" "$(INTDIR)\gxinter.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxlora.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxlora.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxlora.obj"  "$(INTDIR)\gxlora.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxmax3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxmax3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxmax3.obj"  "$(INTDIR)\gxmax3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxme2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxme2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxme2.obj"   "$(INTDIR)\gxme2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxopus.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxopus.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxopus.obj"  "$(INTDIR)\gxopus.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxpcb.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxpcb.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxpcb.obj"   "$(INTDIR)\gxpcb.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxportal.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxportal.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxportal.obj"    "$(INTDIR)\gxportal.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxprobrd.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxprobrd.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxprobrd.obj"    "$(INTDIR)\gxprobrd.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxqecho.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxqecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxqecho.obj" "$(INTDIR)\gxqecho.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxqfront.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxqfront.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxqfront.obj"    "$(INTDIR)\gxqfront.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxquick.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxquick.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxquick.obj" "$(INTDIR)\gxquick.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxra.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxra.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxra.obj"    "$(INTDIR)\gxra.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxraecho.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxraecho.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxraecho.obj"    "$(INTDIR)\gxraecho.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxspace.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxspace.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxspace.obj" "$(INTDIR)\gxspace.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxsquish.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxsquish.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxsquish.obj"    "$(INTDIR)\gxsquish.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxsuper.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxsuper.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxsuper.obj" "$(INTDIR)\gxsuper.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxsync.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxsync.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxsync.obj"  "$(INTDIR)\gxsync.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxtimed.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxtimed.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxtimed.obj" "$(INTDIR)\gxtimed.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxtmail.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxtmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxtmail.obj" "$(INTDIR)\gxtmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxts.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxts.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxts.obj"    "$(INTDIR)\gxts.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxwmail.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxwmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxwmail.obj" "$(INTDIR)\gxwmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxwtr.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxwtr.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxwtr.obj"   "$(INTDIR)\gxwtr.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxxbbs.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxxbbs.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxxbbs.obj"  "$(INTDIR)\gxxbbs.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gcfg\gxxmail.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gxxmail.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gxxmail.obj" "$(INTDIR)\gxxmail.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\glibc\dummy.c
SOURCE=.\goldlib\glibc\fnmatch.c
SOURCE=.\goldlib\glibc\glob.c
SOURCE=.\goldlib\glibc\regex.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\regex.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\regex.obj"   "$(INTDIR)\regex.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoarea.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoarea.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoarea.obj" "$(INTDIR)\gmoarea.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoezyc1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoezyc1.obj"    "$(INTDIR)\gmoezyc1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoezyc2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoezyc2.obj"    "$(INTDIR)\gmoezyc2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoezyc3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoezyc3.obj"    "$(INTDIR)\gmoezyc3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoezyc4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoezyc4.obj"    "$(INTDIR)\gmoezyc4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoezyc5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoezyc5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoezyc5.obj"    "$(INTDIR)\gmoezyc5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmofido1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmofido1.obj"    "$(INTDIR)\gmofido1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmofido2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmofido2.obj"    "$(INTDIR)\gmofido2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmofido3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmofido3.obj"    "$(INTDIR)\gmofido3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmofido4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmofido4.obj"    "$(INTDIR)\gmofido4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmofido5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmofido5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmofido5.obj"    "$(INTDIR)\gmofido5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmohuds.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmohuds.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmohuds.obj" "$(INTDIR)\gmohuds.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmohuds1.cpp
SOURCE=.\goldlib\gmb3\gmohuds2.cpp
SOURCE=.\goldlib\gmb3\gmohuds3.cpp
SOURCE=.\goldlib\gmb3\gmohuds4.cpp
SOURCE=.\goldlib\gmb3\gmohuds5.cpp
SOURCE=.\goldlib\gmb3\gmojamm1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmojamm1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmojamm1.obj"    "$(INTDIR)\gmojamm1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmojamm2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmojamm2.obj"    "$(INTDIR)\gmojamm2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmojamm3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmojamm3.obj"    "$(INTDIR)\gmojamm3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmojamm4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmojamm4.obj"    "$(INTDIR)\gmojamm4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmojamm5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmojamm5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmojamm5.obj"    "$(INTDIR)\gmojamm5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmopcbd1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmopcbd1.obj"    "$(INTDIR)\gmopcbd1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmopcbd2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmopcbd2.obj"    "$(INTDIR)\gmopcbd2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmopcbd3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmopcbd3.obj"    "$(INTDIR)\gmopcbd3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmopcbd4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmopcbd4.obj"    "$(INTDIR)\gmopcbd4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmopcbd5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmopcbd5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmopcbd5.obj"    "$(INTDIR)\gmopcbd5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosmb1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmosmb1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmosmb1.obj" "$(INTDIR)\gmosmb1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosmb2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmosmb2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmosmb2.obj" "$(INTDIR)\gmosmb2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmosqsh1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmosqsh1.obj"    "$(INTDIR)\gmosqsh1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmosqsh2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmosqsh2.obj"    "$(INTDIR)\gmosqsh2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmosqsh3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmosqsh3.obj"    "$(INTDIR)\gmosqsh3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmosqsh4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmosqsh4.obj"    "$(INTDIR)\gmosqsh4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmosqsh5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmosqsh5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmosqsh5.obj"    "$(INTDIR)\gmosqsh5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmowcat1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmowcat1.obj"    "$(INTDIR)\gmowcat1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmowcat2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmowcat2.obj"    "$(INTDIR)\gmowcat2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmowcat3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmowcat3.obj"    "$(INTDIR)\gmowcat3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmowcat4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmowcat4.obj"    "$(INTDIR)\gmowcat4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmowcat5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmowcat5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmowcat5.obj"    "$(INTDIR)\gmowcat5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs1.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoxbbs1.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoxbbs1.obj"    "$(INTDIR)\gmoxbbs1.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs2.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoxbbs2.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoxbbs2.obj"    "$(INTDIR)\gmoxbbs2.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs3.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoxbbs3.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoxbbs3.obj"    "$(INTDIR)\gmoxbbs3.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs4.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoxbbs4.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoxbbs4.obj"    "$(INTDIR)\gmoxbbs4.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\gmb3\gmoxbbs5.cpp

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gmoxbbs5.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gmoxbbs5.obj"    "$(INTDIR)\gmoxbbs5.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\fexist.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\fexist.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\fexist.obj"  "$(INTDIR)\fexist.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\ffind.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\ffind.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\ffind.obj"   "$(INTDIR)\ffind.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\genmsgid.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\genmsgid.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\genmsgid.obj"    "$(INTDIR)\genmsgid.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\msgidlib\patmat.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\patmat.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\patmat.obj"  "$(INTDIR)\patmat.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\smblib\lzh.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\lzh.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\lzh.obj" "$(INTDIR)\lzh.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\smblib\smblib.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\smblib.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\smblib.obj"  "$(INTDIR)\smblib.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\fptools.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\fptools.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\fptools.obj" "$(INTDIR)\fptools.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uucheck.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\uucheck.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\uucheck.obj" "$(INTDIR)\uucheck.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uuencode.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\uuencode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\uuencode.obj"    "$(INTDIR)\uuencode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uulib.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\uulib.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\uulib.obj"   "$(INTDIR)\uulib.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uunconc.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\uunconc.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\uunconc.obj" "$(INTDIR)\uunconc.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uuscan.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\uuscan.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\uuscan.obj"  "$(INTDIR)\uuscan.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uustring.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\uustring.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\uustring.obj"    "$(INTDIR)\uustring.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\goldlib\uulib\uuutil.c

!IF  "$(CFG)" == "Release"


"$(INTDIR)\uuutil.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\uuutil.obj"  "$(INTDIR)\uuutil.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF

SOURCE=.\golded3\gedcyg.rc

!IF  "$(CFG)" == "Release"


"$(INTDIR)\gedcyg.res" : $(SOURCE) "$(INTDIR)"
    $(RSC) /fo"$(INTDIR)\gedcyg.res" /i "golded3" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "Debug"


"$(INTDIR)\gedcyg.res" : $(SOURCE) "$(INTDIR)"
    $(RSC) /fo"$(INTDIR)\gedcyg.res" /i "golded3" /d "_DEBUG" $(SOURCE)


!ENDIF


SOURCE=.\rddt\rddt.cpp

!IF  "$(CFG)" == "Release"

"$(INTDIR)\rddt.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ELSEIF  "$(CFG)" == "Debug"

"$(INTDIR)\rddt.obj"  "$(INTDIR)\rddt.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ENDIF

SOURCE=.\goldnode\goldnode.cpp

!IF  "$(CFG)" == "Release"

"$(INTDIR)\goldnode.obj" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ELSEIF  "$(CFG)" == "Debug"

"$(INTDIR)\goldnode.obj"  "$(INTDIR)\goldnode.sbr" : $(SOURCE) "$(INTDIR)"
    $(CPP) $(CPP_PROJ) $(SOURCE)

!ENDIF

!ENDIF

