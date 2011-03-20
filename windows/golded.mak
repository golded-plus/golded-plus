# Microsoft Developer Studio Generated NMAKE File, Based on golded.dsp
!IF "$(CFG)" == ""
CFG=golded - Win32 Debug
!MESSAGE No configuration specified. Defaulting to golded - Win32 Debug.
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

OUTDIR=.\bin\release\win32
INTDIR=.\obj\release\win32
# Begin Custom Macros
OutDir=.\bin\release\win32
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\gedwin.exe"

!ELSE 

ALL : "goldlib - Win32 Release" "$(OUTDIR)\gedwin.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"goldlib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
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
	-@erase "$(INTDIR)\gckeys.obj"
	-@erase "$(INTDIR)\gclang.obj"
	-@erase "$(INTDIR)\gcmisc.obj"
	-@erase "$(INTDIR)\gealst.obj"
	-@erase "$(INTDIR)\gearea.obj"
	-@erase "$(INTDIR)\gecarb.obj"
	-@erase "$(INTDIR)\gecmfd.obj"
	-@erase "$(INTDIR)\gectnr.obj"
	-@erase "$(INTDIR)\gectrl.obj"
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
	-@erase "$(INTDIR)\gehtml.obj"
	-@erase "$(INTDIR)\geinit.obj"
	-@erase "$(INTDIR)\geline.obj"
	-@erase "$(INTDIR)\gelmsg.obj"
	-@erase "$(INTDIR)\gemain.obj"
	-@erase "$(INTDIR)\gemenu.obj"
	-@erase "$(INTDIR)\gemlst.obj"
	-@erase "$(INTDIR)\gemnus.obj"
	-@erase "$(INTDIR)\gemrks.obj"
	-@erase "$(INTDIR)\gemsgs.obj"
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
	-@erase "$(INTDIR)\geview.obj"
	-@erase "$(INTDIR)\gmarea.obj"
	-@erase "$(INTDIR)\golded3.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\gedwin.exe"
	-@erase "$(OUTDIR)\gedwin.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /Zi /O1 /I "..\golded3" /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /I "..\goldlib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "HAVE_STDARG_H" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /c

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\gedcyg.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\golded.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib advapi32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\gedwin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\gedwin.exe" 
LINK32_OBJS= \
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
	"$(INTDIR)\gehtml.obj" \
	"$(INTDIR)\geinit.obj" \
	"$(INTDIR)\geline.obj" \
	"$(INTDIR)\gelmsg.obj" \
	"$(INTDIR)\gemain.obj" \
	"$(INTDIR)\gemenu.obj" \
	"$(INTDIR)\gemlst.obj" \
	"$(INTDIR)\gemnus.obj" \
	"$(INTDIR)\gemrks.obj" \
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
	"$(INTDIR)\gedcyg.res" \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\gedwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

OUTDIR=.\bin\debug\win32
INTDIR=.\obj\debug\win32
# Begin Custom Macros
OutDir=.\bin\debug\win32
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\gedwin.exe"

!ELSE 

ALL : "goldlib - Win32 Debug" "$(OUTDIR)\gedwin.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"goldlib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
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
	-@erase "$(INTDIR)\gckeys.obj"
	-@erase "$(INTDIR)\gclang.obj"
	-@erase "$(INTDIR)\gcmisc.obj"
	-@erase "$(INTDIR)\gealst.obj"
	-@erase "$(INTDIR)\gearea.obj"
	-@erase "$(INTDIR)\gecarb.obj"
	-@erase "$(INTDIR)\gecmfd.obj"
	-@erase "$(INTDIR)\gectnr.obj"
	-@erase "$(INTDIR)\gectrl.obj"
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
	-@erase "$(INTDIR)\gehtml.obj"
	-@erase "$(INTDIR)\geinit.obj"
	-@erase "$(INTDIR)\geline.obj"
	-@erase "$(INTDIR)\gelmsg.obj"
	-@erase "$(INTDIR)\gemain.obj"
	-@erase "$(INTDIR)\gemenu.obj"
	-@erase "$(INTDIR)\gemlst.obj"
	-@erase "$(INTDIR)\gemnus.obj"
	-@erase "$(INTDIR)\gemrks.obj"
	-@erase "$(INTDIR)\gemsgs.obj"
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
	-@erase "$(INTDIR)\geview.obj"
	-@erase "$(INTDIR)\gmarea.obj"
	-@erase "$(INTDIR)\golded3.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\gedwin.exe"
	-@erase "$(OUTDIR)\gedwin.ilk"
	-@erase "$(OUTDIR)\gedwin.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".\golded3" /I ".\goldlib" /I ".\goldlib\gall" /I ".\goldlib\gcui" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /I "..\golded3" /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcui" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /I "..\goldlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "HAVE_STDARG_H" /D "HAVE_CONFIG_H" /D "__INCLUDE_NEW_KEYWORDS__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /GZ /c

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\gedcyg.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\golded.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib advapi32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\gedwin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\gedwin.exe" /pdbtype:sept 
LINK32_OBJS= \
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
	"$(INTDIR)\gehtml.obj" \
	"$(INTDIR)\geinit.obj" \
	"$(INTDIR)\geline.obj" \
	"$(INTDIR)\gelmsg.obj" \
	"$(INTDIR)\gemain.obj" \
	"$(INTDIR)\gemenu.obj" \
	"$(INTDIR)\gemlst.obj" \
	"$(INTDIR)\gemnus.obj" \
	"$(INTDIR)\gemrks.obj" \
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
	"$(INTDIR)\gedcyg.res" \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\gedwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("golded.dep")
!INCLUDE "golded.dep"
!ELSE 
!MESSAGE Warning: cannot find "golded.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "golded - Win32 Release" || "$(CFG)" == "golded - Win32 Debug"
SOURCE=..\golded3\gcalst.cpp

"$(INTDIR)\gcalst.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gcarea.cpp

"$(INTDIR)\gcarea.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg.cpp

"$(INTDIR)\gccfgg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg0.cpp

"$(INTDIR)\gccfgg0.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg1.cpp

"$(INTDIR)\gccfgg1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg2.cpp

"$(INTDIR)\gccfgg2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg3.cpp

"$(INTDIR)\gccfgg3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg4.cpp

"$(INTDIR)\gccfgg4.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg5.cpp

"$(INTDIR)\gccfgg5.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg6.cpp

"$(INTDIR)\gccfgg6.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg7.cpp

"$(INTDIR)\gccfgg7.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gccfgg8.cpp

"$(INTDIR)\gccfgg8.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gckeys.cpp

"$(INTDIR)\gckeys.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gclang.cpp

"$(INTDIR)\gclang.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gcmisc.cpp

"$(INTDIR)\gcmisc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gealst.cpp

"$(INTDIR)\gealst.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gearea.cpp

"$(INTDIR)\gearea.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gecarb.cpp

"$(INTDIR)\gecarb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gecmfd.cpp

"$(INTDIR)\gecmfd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gectnr.cpp

"$(INTDIR)\gectnr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gectrl.cpp

"$(INTDIR)\gectrl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gedoit.cpp

"$(INTDIR)\gedoit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gedoss.cpp

"$(INTDIR)\gedoss.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geedit.cpp

"$(INTDIR)\geedit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geedit2.cpp

"$(INTDIR)\geedit2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geedit3.cpp

"$(INTDIR)\geedit3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gefile.cpp

"$(INTDIR)\gefile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gefind.cpp

"$(INTDIR)\gefind.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geglob.cpp

"$(INTDIR)\geglob.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gehdre.cpp

"$(INTDIR)\gehdre.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gehtml.cpp

"$(INTDIR)\gehtml.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geinit.cpp

"$(INTDIR)\geinit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geline.cpp

"$(INTDIR)\geline.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gelmsg.cpp

"$(INTDIR)\gelmsg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gemain.cpp

"$(INTDIR)\gemain.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gemenu.cpp

"$(INTDIR)\gemenu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gemlst.cpp

"$(INTDIR)\gemlst.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gemnus.cpp

"$(INTDIR)\gemnus.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gemrks.cpp

"$(INTDIR)\gemrks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gemsgs.cpp

"$(INTDIR)\gemsgs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\genode.cpp

"$(INTDIR)\genode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geplay.cpp

"$(INTDIR)\geplay.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gepost.cpp

"$(INTDIR)\gepost.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geqwks.cpp

"$(INTDIR)\geqwks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gerand.cpp

"$(INTDIR)\gerand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geread.cpp

"$(INTDIR)\geread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geread2.cpp

"$(INTDIR)\geread2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gescan.cpp

"$(INTDIR)\gescan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gesoup.cpp

"$(INTDIR)\gesoup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gesrch.cpp

"$(INTDIR)\gesrch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\getpls.cpp

"$(INTDIR)\getpls.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geusrbse.cpp

"$(INTDIR)\geusrbse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geutil.cpp

"$(INTDIR)\geutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geutil2.cpp

"$(INTDIR)\geutil2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\geview.cpp

"$(INTDIR)\geview.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gmarea.cpp

"$(INTDIR)\gmarea.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\golded3.cpp

"$(INTDIR)\golded3.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\golded3\gedcyg.rc

!IF  "$(CFG)" == "golded - Win32 Release"


"$(INTDIR)\gedcyg.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\gedcyg.res" /i "\work\src\golded\golded3" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "golded - Win32 Debug"


"$(INTDIR)\gedcyg.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\gedcyg.res" /i "\work\src\golded\golded3" /d "_DEBUG" $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "golded - Win32 Release"

"goldlib - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release" 
   cd "."

"goldlib - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "golded - Win32 Debug"

"goldlib - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Debug" 
   cd "."

"goldlib - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

