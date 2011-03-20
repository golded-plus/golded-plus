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
# PROP Output_Dir "bin\release\win32"
# PROP Intermediate_Dir "obj\release\win32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /Gi- /GX /Zi /O1 /Ob1 /I "..\golded3" /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /J /FD /c
# SUBTRACT CPP /Gy /Fr
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib user32.lib advapi32.lib /nologo /subsystem:console /debug /machine:I386 /out:"bin\release\win32\gedwin.exe"

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\debug\win32"
# PROP Intermediate_Dir "obj\debug\win32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".\golded3" /I ".\goldlib" /I ".\goldlib\gall" /I ".\goldlib\gcui" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /I "..\golded3" /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /J /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib user32.lib advapi32.lib /nologo /subsystem:console /debug /machine:I386 /out:"bin\debug\win32\gedwin.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "golded - Win32 Release"
# Name "golded - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "golded3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\golded3\gcalst.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gcarea.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg0.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg1.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg2.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg3.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg4.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg5.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg6.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg7.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gccfgg8.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gckeys.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gclang.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gcmisc.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gealst.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gearea.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gecarb.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gecmfd.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gectnr.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gectrl.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gedoit.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gedoss.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geedit.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geedit2.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geedit3.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gefile.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gefind.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geglob.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gehdre.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gehtml.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geinit.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geline.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gelmsg.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gemain.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gemenu.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gemlst.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gemnus.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gemrks.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gemsgs.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\genode.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geplay.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gepost.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geqwks.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gerand.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geread.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geread2.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gescan.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gesoup.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gesrch.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\getpls.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geusrbse.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geutil.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geutil2.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\geview.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\gmarea.cpp
# End Source File
# Begin Source File

SOURCE=..\golded3\golded3.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "golded3.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\golded3\gccfgg.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gckeys.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gclang.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gcprot.h
# End Source File
# Begin Source File

SOURCE=..\golded3\geall.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gecfgg.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gectnr.h
# End Source File
# Begin Source File

SOURCE=..\golded3\geedit.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gefn.h
# End Source File
# Begin Source File

SOURCE=..\golded3\geglob.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gehelp.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gekeys.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gelang.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gemnus.h
# End Source File
# Begin Source File

SOURCE=..\golded3\geprot.h
# End Source File
# Begin Source File

SOURCE=..\golded3\geqwks.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gesrch.h
# End Source File
# Begin Source File

SOURCE=..\golded3\geusrbse.h
# End Source File
# Begin Source File

SOURCE=..\golded3\geview.h
# End Source File
# Begin Source File

SOURCE=..\golded3\gmarea.h
# End Source File
# Begin Source File

SOURCE=..\golded3\golded.h
# End Source File
# Begin Source File

SOURCE=..\golded3\golded3.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\golded3\mygolded.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\golded3\gedcyg.rc
# End Source File
# Begin Source File

SOURCE=..\golded3\gedw.ico
# End Source File
# End Group
# End Target
# End Project
