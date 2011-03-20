# Microsoft Developer Studio Generated NMAKE File, Based on goldnode.dsp
!IF "$(CFG)" == ""
CFG=goldnode - Win32 Debug
!MESSAGE No configuration specified. Defaulting to goldnode - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "goldnode - Win32 Release" && "$(CFG)" != "goldnode - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "goldnode.mak" CFG="goldnode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "goldnode - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "goldnode - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "goldnode - Win32 Release"

OUTDIR=.\bin\release\win32
INTDIR=.\obj\release\win32
# Begin Custom Macros
OutDir=.\bin\release\win32
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\gnwin.exe"

!ELSE 

ALL : "goldlib - Win32 Release" "golded - Win32 Release" "$(OUTDIR)\gnwin.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"golded - Win32 ReleaseCLEAN" "goldlib - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\goldnode.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\gnwin.exe"
	-@erase "$(OUTDIR)\gnwin.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\goldnode.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib obj\release\win32\golded3.obj /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\gnwin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\gnwin.exe" 
LINK32_OBJS= \
	"$(INTDIR)\goldnode.obj" \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\gnwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "goldnode - Win32 Debug"

OUTDIR=.\bin\debug\win32
INTDIR=.\obj\debug\win32
# Begin Custom Macros
OutDir=.\bin\debug\win32
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\gnwin.exe"

!ELSE 

ALL : "goldlib - Win32 Debug" "golded - Win32 Debug" "$(OUTDIR)\gnwin.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"golded - Win32 DebugCLEAN" "goldlib - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\goldnode.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\gnwin.exe"
	-@erase "$(OUTDIR)\gnwin.ilk"
	-@erase "$(OUTDIR)\gnwin.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\goldnode.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib user32.lib obj\debug\win32\golded3.obj /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\gnwin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\gnwin.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\goldnode.obj" \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\gnwin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("goldnode.dep")
!INCLUDE "goldnode.dep"
!ELSE 
!MESSAGE Warning: cannot find "goldnode.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "goldnode - Win32 Release" || "$(CFG)" == "goldnode - Win32 Debug"

!IF  "$(CFG)" == "goldnode - Win32 Release"

"golded - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Release" 
   cd "."

"golded - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "goldnode - Win32 Debug"

"golded - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Debug" 
   cd "."

"golded - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "goldnode - Win32 Release"

"goldlib - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release" 
   cd "."

"goldlib - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "goldnode - Win32 Debug"

"goldlib - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Debug" 
   cd "."

"goldlib - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

SOURCE=..\goldnode\goldnode.cpp

"$(INTDIR)\goldnode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

