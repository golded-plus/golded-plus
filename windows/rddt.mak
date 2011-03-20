# Microsoft Developer Studio Generated NMAKE File, Based on rddt.dsp
!IF "$(CFG)" == ""
CFG=rddt - Win32 Debug
!MESSAGE No configuration specified. Defaulting to rddt - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "rddt - Win32 Release" && "$(CFG)" != "rddt - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rddt.mak" CFG="rddt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rddt - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "rddt - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "rddt - Win32 Release"

OUTDIR=.\bin\release\win32
INTDIR=.\obj\release\win32
# Begin Custom Macros
OutDir=.\bin\release\win32
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\rddtwin.exe"

!ELSE 

ALL : "goldlib - Win32 Release" "golded - Win32 Release" "$(OUTDIR)\rddtwin.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"golded - Win32 ReleaseCLEAN" "goldlib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\rddt.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\rddtwin.exe"
	-@erase "$(OUTDIR)\rddtwin.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /Zi /O1 /I ".\golded3" /I ".\goldlib" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /I "..\golded3" /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /I "..\goldlib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "HAVE_STDARG_H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /c

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rddt.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib obj\release\win32\golded3.obj /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\rddtwin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\rddtwin.exe" 
LINK32_OBJS= \
	"$(INTDIR)\rddt.obj" \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\rddtwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "rddt - Win32 Debug"

OUTDIR=.\bin\debug\win32
INTDIR=.\obj\debug\win32
# Begin Custom Macros
OutDir=.\bin\debug\win32
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\rddtwin.exe"

!ELSE 

ALL : "goldlib - Win32 Debug" "golded - Win32 Debug" "$(OUTDIR)\rddtwin.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"golded - Win32 DebugCLEAN" "goldlib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\rddt.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\rddtwin.exe"
	-@erase "$(OUTDIR)\rddtwin.ilk"
	-@erase "$(OUTDIR)\rddtwin.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".\golded3" /I ".\goldlib" /I ".\goldlib\gall" /I ".\goldlib\gcfg" /I ".\goldlib\glibc" /I ".\goldlib\gmb3" /I ".\goldlib\msgidlib" /I ".\goldlib\smblib" /I ".\goldlib\uulib" /I "..\golded3" /I "..\goldlib" /I "..\goldlib\gall" /I "..\goldlib\gcfg" /I "..\goldlib\glibc" /I "..\goldlib\gmb3" /I "..\goldlib\smblib" /I "..\goldlib\uulib" /I "..\goldlib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "HAVE_STDARG_H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /GZ /c

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rddt.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib obj\debug\win32\golded3.obj /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\rddtwin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\rddtwin.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\rddt.obj" \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\rddtwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("rddt.dep")
!INCLUDE "rddt.dep"
!ELSE 
!MESSAGE Warning: cannot find "rddt.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "rddt - Win32 Release" || "$(CFG)" == "rddt - Win32 Debug"

!IF  "$(CFG)" == "rddt - Win32 Release"

"golded - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Release" 
   cd "."

"golded - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "rddt - Win32 Debug"

"golded - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Debug" 
   cd "."

"golded - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "rddt - Win32 Release"

"goldlib - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release" 
   cd "."

"goldlib - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "rddt - Win32 Debug"

"goldlib - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Debug" 
   cd "."

"goldlib - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

SOURCE=..\rddt\rddt.cpp

"$(INTDIR)\rddt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

