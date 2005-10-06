# Microsoft Developer Studio Project File - Name="golded" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=golded - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "golded.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "golded.mak" CFG="golded - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "golded - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "golded - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "golded - Win32 DRelease" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gi /GX /O1 /Ob2 /I ".\golded3" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H" /J /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib user32.lib /nologo /subsystem:console /machine:I386 /out:"Release/gedwin.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".\golded3" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H" /Fr /J /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "golded___Win32_DRelease"
# PROP BASE Intermediate_Dir "golded___Win32_DRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DRelease"
# PROP Intermediate_Dir "DRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O1 /I ".\golded3" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H" /J /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /Gm /Gi /GX /Zi /O1 /I ".\golded3" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H" /Fr /J /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Release/gedwin.exe" /pdbtype:sept
# ADD LINK32 winmm.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /out:"DRelease/gedwin.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "golded - Win32 Release"
# Name "golded - Win32 Debug"
# Name "golded - Win32 DRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "golded3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\golded3\gcalst.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gcarea.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg0.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg1.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg2.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg3.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg4.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg5.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg6.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg7.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gccfgg8.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gckeys.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gclang.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gcmisc.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gealst.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gearea.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gecarb.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gecmfd.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gectnr.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gedoit.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gedoss.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geedit.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geedit2.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geedit3.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gefile.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gefind.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geglob.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gehdre.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gehdrs.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gehtml.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geinit.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geline.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gelmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gemain.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gemenu.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gemlst.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gemnus.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gemrks.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gemsgid.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gemsgs.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\genode.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geplay.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gepost.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geqwks.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gerand.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geread.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geread2.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gescan.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gesoup.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gesrch.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\getpls.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geusrbse.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geutil.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geutil2.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\geview.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\gmarea.cpp
# End Source File
# Begin Source File

SOURCE=.\golded3\golded3.cpp
# End Source File
# End Group
# Begin Group "goldlib"

# PROP Default_Filter ""
# Begin Group "gall"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\gall\be_lock.c

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gbmh.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcharset.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrc16tb.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrc32tb.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrchash.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrckeyv.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrcm16.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrcm32.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrcs16.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrcs32.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdbgerr.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdbgexit.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdbgtrk.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdirposx.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\geval.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gevalhum.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gevalrpn.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfile.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfilport.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfilutl1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfilutl2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnaddr.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnl.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlfd.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlfu.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlge.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlv7.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfuzzy.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\ghdrmime.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gkbdbase.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gkbdgetm.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gkbdunix.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gkbdwait.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\glog.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmemdbg.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmemutil.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmoubase.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmsgattr.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gprnutil.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gregex.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsearch.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsigunix.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsnd.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsndwrap.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsrchmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrbags.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrctyp.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrmail.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrname.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrutil.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gtimjuld.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gtimutil.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gtxtpara.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrbase.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrezyc.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrgold.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrhuds.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrmax.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrpcb.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrra2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrxbbs.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlbeos.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlclip.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlcode.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutldos.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlgrp.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlmisc.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlmtsk.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlos2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlos2m.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutltag.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlunix.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlvers.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlwin.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlwinm.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gvidbase.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gvidinit.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwildmat.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinbase.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinhlp1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinhlp2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwininit.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinline.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinmnub.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinpckf.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinpcks.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinpick.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinput2.cpp
# End Source File
# End Group
# Begin Group "gcfg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\gcfg\gedacfg.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxareas.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxcrash.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxdb.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxdutch.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxezy102.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxezy110.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfd.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfecho4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfecho5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfecho6.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfidpcb.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfm092.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfm100.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxfm116.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxgecho.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxhpt.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gximail4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gximail5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gximail6.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxinter.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxlora.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxmax3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxme2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxopus.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxpcb.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxportal.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxprobrd.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxqecho.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxqfront.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxquick.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxra.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxraecho.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxspace.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxsquish.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxsuper.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxsync.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxtimed.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxtmail.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxts.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxwmail.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxwtr.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxxbbs.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gxxmail.cpp
# End Source File
# End Group
# Begin Group "glibc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\glibc\dummy.c

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\glibc\fnmatch.c

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\glibc\glob.c

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\glibc\regex.c
# End Source File
# End Group
# Begin Group "gmb3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoarea.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoezyc1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoezyc2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoezyc3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoezyc4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoezyc5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmofido1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmofido2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmofido3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmofido4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmofido5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmohuds.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmohuds1.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmohuds2.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmohuds3.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmohuds4.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmohuds5.cpp

!IF  "$(CFG)" == "golded - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "golded - Win32 DRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmojamm1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmojamm2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmojamm3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmojamm4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmojamm5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmopcbd1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmopcbd2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmopcbd3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmopcbd4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmopcbd5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosmb1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosmb2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosqsh1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosqsh2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosqsh3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosqsh4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosqsh5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmowcat1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmowcat2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmowcat3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmowcat4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmowcat5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoxbbs1.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoxbbs2.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoxbbs3.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoxbbs4.cpp
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoxbbs5.cpp
# End Source File
# End Group
# Begin Group "msgidlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\msgidlib\fexist.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\msgidlib\ffind.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\msgidlib\genmsgid.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\msgidlib\patmat.c
# End Source File
# End Group
# Begin Group "smblib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\smblib\lzh.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\smblib\smblib.c
# End Source File
# End Group
# Begin Group "uulib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\uulib\fptools.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uucheck.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uuencode.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uulib.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uunconc.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uuscan.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uustring.c
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uuutil.c
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "golded3.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\golded3\gccfgg.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gckeys.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gclang.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gcprot.h
# End Source File
# Begin Source File

SOURCE=.\golded3\geall.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gecfgg.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gectnr.h
# End Source File
# Begin Source File

SOURCE=.\golded3\geedit.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gefn.h
# End Source File
# Begin Source File

SOURCE=.\golded3\geglob.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gehelp.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gekeys.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gelang.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gemnus.h
# End Source File
# Begin Source File

SOURCE=.\golded3\geprot.h
# End Source File
# Begin Source File

SOURCE=.\golded3\geqwks.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gesrch.h
# End Source File
# Begin Source File

SOURCE=.\golded3\geusrbse.h
# End Source File
# Begin Source File

SOURCE=.\golded3\geview.h
# End Source File
# Begin Source File

SOURCE=.\golded3\gmarea.h
# End Source File
# Begin Source File

SOURCE=.\golded3\golded.h
# End Source File
# Begin Source File

SOURCE=.\golded3\golded3.h
# End Source File
# End Group
# Begin Group "goldlib.h"

# PROP Default_Filter ""
# Begin Group "gall.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\gall\be_lock.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gasmamis.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gbmh.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcharset.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcmpall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcrcall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gctype.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gcurses.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdbgerr.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdbgtrk.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdefs.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gdirposx.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\geval.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfile.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfilutil.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnl.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlfd.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlfu.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlge.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gftnnlv7.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gfuzzy.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\ghdrmime.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gkbdbase.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gkbdcode.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gkbdunix.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\glog.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmemall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmemdbg.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmemi86.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmnubase.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmoubase.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gmsgattr.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gprnall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gregex.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsearch.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gshare.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsigunix.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsndall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsndsapi.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gsrchmgr.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrarr.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrbags.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gstrmail.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gtimall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gtxtpara.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrbase.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrezyc.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrgold.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrhuds.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrmax.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrpcb.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrra2.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrwcat.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gusrxbbs.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlclip.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlcode.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlgrp.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlmisc.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlmtsk.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlos.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutltag.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gutlunix.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gvidall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwildmat.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinall.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwindow.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinhelp.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinpick.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gall\gwinput.h
# End Source File
# End Group
# Begin Group "gcfg.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\gcfg\gedacfg.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_db.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_ez102.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_ez110.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_fd.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_fech4.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_fech5.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_fech6.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_fm092.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_fm10g.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_fm116.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_ge120.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_im160.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_im175.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_inter.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_lo240.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_max3.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_opus.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_pb200.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_pcb.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_pop.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_qbbs.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_qfrnt.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_qwk.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_ra.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_ra2.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_recho.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_sbbs.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_sync.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_ts.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_wmail.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_wtr.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_xbbs.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gcfg\gs_xmail.h
# End Source File
# End Group
# Begin Group "glibc.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\glibc\config.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\glibc\fnmatch.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\glibc\glob.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\glibc\regex.h
# End Source File
# End Group
# Begin Group "gmb3.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\gmb3\gmo_msg.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoarea.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoezyc.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmofido.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmohuds.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmojamm.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmopcbd.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoprot.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosmb.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmosqsh.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmowcat.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\gmb3\gmoxbbs.h
# End Source File
# End Group
# Begin Group "msgidlib.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\msgidlib\ffind.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\msgidlib\genmsgid.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\msgidlib\hsksupp.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\msgidlib\typedefs.h
# End Source File
# End Group
# Begin Group "smblib.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\smblib\filewrap.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\smblib\genwrap.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\smblib\lzh.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\smblib\smbdefs.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\smblib\smblib.h
# End Source File
# End Group
# Begin Group "uulib.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goldlib\uulib\config.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\crc32.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\fptools.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uudeview.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uuint.h
# End Source File
# Begin Source File

SOURCE=.\goldlib\uulib\uustring.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\golded3\mygolded.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\golded3\gedcyg.rc
# End Source File
# Begin Source File

SOURCE=.\golded3\gedw.ico
# End Source File
# End Group
# End Target
# End Project
