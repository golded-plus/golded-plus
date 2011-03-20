# Microsoft Developer Studio Project File - Name="goldlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=goldlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "goldlib.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "goldlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\release\win32"
# PROP Intermediate_Dir "obj\release\win32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /O1 /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "HAVE_STDARG_H" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /J /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "goldlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "goldlib___Win32_Debug"
# PROP BASE Intermediate_Dir "goldlib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\debug\win32"
# PROP Intermediate_Dir "obj\debug\win32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "HAVE_STDARG_H" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /J /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "goldlib - Win32 Release"
# Name "goldlib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "gall"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gall\be_lock.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gbmh.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcharset.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrc16tb.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrc32tb.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrchash.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrckeyv.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrcm16.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrcm32.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrcs16.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrcs32.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdbgerr.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdbgexit.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdbgtrk.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdirposx.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gespell.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\geval.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gevalhum.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gevalrpn.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfile.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfilport.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfilutl1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfilutl2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnaddr.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnl.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlfd.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlfu.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlge.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlv7.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfuzzy.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\ghdrmime.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gkbdunix.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\glog.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gmemdbg.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gmemutil.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gmsgattr.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gprnutil.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gregex.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsearch.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsigunix.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsnd.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsndwrap.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrbags.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrctyp.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrmail.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrname.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrutil.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gtimjuld.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gtimutil.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gtxtpara.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrbase.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrezyc.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrgold.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrhuds.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrmax.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrpcb.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrra2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrxbbs.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlbeos.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlclip.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlcode.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutldos.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlgrp.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlmisc.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlmtsk.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlos2.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlos2m.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutltag.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlunix.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlvers.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlwin.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlwinm.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gwildmat.cpp
# End Source File
# End Group
# Begin Group "gcui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gcui\gkbdbase.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gkbdgetm.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gkbdwait.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gmoubase.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gsrchmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gvidbase.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gvidinit.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinbase.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinhlp1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinhlp2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwininit.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinline.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinmenu.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinmnub.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinpckf.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinpcks.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinpick.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinput2.cpp
# End Source File
# End Group
# Begin Group "gcfg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gcfg\gedacfg.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxareas.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxcrash.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxdb.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxdutch.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxezy102.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxezy110.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfd.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfecho4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfecho5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfecho6.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfidpcb.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfm092.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfm100.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxfm116.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxgecho.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxhpt.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gximail4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gximail5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gximail6.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxinter.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxlora.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxmax3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxme2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxopus.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxpcb.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxportal.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxprobrd.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxqecho.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxqfront.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxquick.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxra.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxraecho.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxspace.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxsquish.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxsuper.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxsync.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxtimed.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxtmail.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxts.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxwmail.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxwtr.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxxbbs.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gxxmail.cpp
# End Source File
# End Group
# Begin Group "glibc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\glibc\dummy.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\glibc\fnmatch.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\glibc\glob.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\glibc\regex.cpp
# End Source File
# End Group
# Begin Group "gmb3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoarea.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoezyc1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoezyc2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoezyc3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoezyc4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoezyc5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmofido1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmofido2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmofido3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmofido4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmofido5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmohuds.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmohuds1.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmohuds2.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmohuds3.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmohuds4.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmohuds5.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmojamm1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmojamm2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmojamm3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmojamm4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmojamm5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmopcbd1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmopcbd2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmopcbd3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmopcbd4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmopcbd5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosmb1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosmb2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosqsh1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosqsh2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosqsh3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosqsh4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosqsh5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmowcat1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmowcat2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmowcat3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmowcat4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmowcat5.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoxbbs1.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoxbbs2.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoxbbs3.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoxbbs4.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoxbbs5.cpp
# End Source File
# End Group
# Begin Group "smblib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\smblib\lzh.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\smblib\smblib.cpp
# End Source File
# End Group
# Begin Group "uulib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\uulib\fptools.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uucheck.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uuencode.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uulib.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uunconc.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uuscan.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uustring.cpp
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uuutil.cpp
# End Source File
# End Group
# Begin Group "hunspell"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\hunspell\affentry.cxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\affixmgr.cxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\csutil.cxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\dictmgr.cxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\hashmgr.cxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\hunspell.cxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\suggmgr.cxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\utf_info.cxx
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "gall.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gall\be_lock.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gasmamis.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gbmh.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcharset.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcmpall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gcrcall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gctype.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdbgerr.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdbgtrk.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdefs.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gdirposx.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gespell.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\geval.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfile.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfilutil.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnl.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlfd.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlfu.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlge.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gftnnlv7.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gfuzzy.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\ghdrmime.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gkbdunix.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\glog.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gmemall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gmemdbg.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gmemi86.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gmsgattr.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gprnall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gregex.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsearch.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gshare.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsigunix.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsndall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gsndsapi.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrarr.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrbags.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gstrmail.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gtimall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gtxtpara.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrbase.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrezyc.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrgold.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrhuds.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrmax.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrpcb.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrra2.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrwcat.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gusrxbbs.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlclip.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlcode.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlgrp.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlmisc.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlmtsk.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlos.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutltag.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gutlunix.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gall\gwildmat.h
# End Source File
# End Group
# Begin Group "gcui.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gcui\gcurses.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gkbdbase.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gkbdcode.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gmnubase.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gmoubase.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gsrchmgr.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gvidall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinall.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwindow.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinhelp.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinpick.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcui\gwinput.h
# End Source File
# End Group
# Begin Group "gcfg.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gcfg\gedacfg.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_db.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_ez102.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_ez110.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_fd.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_fech4.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_fech5.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_fech6.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_fm092.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_fm10g.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_fm116.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_ge120.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_im160.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_im175.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_inter.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_lo240.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_max3.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_opus.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_pb200.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_pcb.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_pop.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_qbbs.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_qfrnt.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_qwk.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_ra.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_ra2.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_recho.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_sbbs.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_sync.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_ts.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_wmail.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_wtr.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_xbbs.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gcfg\gs_xmail.h
# End Source File
# End Group
# Begin Group "glibc.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\glibc\config.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\glibc\fnmatch.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\glibc\glob.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\glibc\regex.h
# End Source File
# End Group
# Begin Group "gmb3.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\gmb3\gmo_msg.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoarea.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoezyc.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmofido.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmohuds.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmojamm.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmopcbd.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoprot.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosmb.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmosqsh.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmowcat.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\gmb3\gmoxbbs.h
# End Source File
# End Group
# Begin Group "smblib.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\smblib\filewrap.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\smblib\genwrap.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\smblib\lzh.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\smblib\smbdefs.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\smblib\smblib.h
# End Source File
# End Group
# Begin Group "uulib.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\uulib\config.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\crc32.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\fptools.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uudeview.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uuint.h
# End Source File
# Begin Source File

SOURCE=..\goldlib\uulib\uustring.h
# End Source File
# End Group
# Begin Group "hunspell.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\goldlib\hunspell\affentry.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\affixmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\atypes.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\baseaffi.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\csutil.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\dictmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\hashmgr.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\htypes.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\hunspell.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\langnum.hxx
# End Source File
# Begin Source File

SOURCE=..\goldlib\hunspell\suggmgr.hxx
# End Source File
# End Group
# End Group
# End Target
# End Project
