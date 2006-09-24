# Microsoft Developer Studio Generated NMAKE File, Based on BuildAll.dsp
!IF "$(CFG)" == ""
CFG=Release
!MESSAGE No configuration specified. Defaulting to Release.
!ENDIF

!IF "$(CFG)" != "Release" && "$(CFG)" != "Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "BuildAll.mak" CFG="Debug"
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

!IF  "$(CFG)" == "Release"

OUTDIR=.\bin\release\win32
INTDIR=.\obj\release\win32
# Begin Custom Macros
OutDir=.\bin\release\win32
# End Custom Macros

!IF "$(RECURSE)" == "0"

ALL : "$(OUTDIR)\BuildAll.exe"

!ELSE

ALL : "goldlib - Win32 Release" "rddt - Win32 Release" "goldnode - Win32 Release" "golded - Win32 Release" "$(OUTDIR)\BuildAll.exe"

!ENDIF

!IF "$(RECURSE)" == "1"
CLEAN :"golded - Win32 ReleaseCLEAN" "goldnode - Win32 ReleaseCLEAN" "rddt - Win32 ReleaseCLEAN" "goldlib - Win32 ReleaseCLEAN"
!ELSE
CLEAN :
!ENDIF
	-@erase "$(OUTDIR)\BuildAll.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /Zi /O1 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BuildAll.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:console /pdb:none /machine:I386 /out:"$(OUTDIR)\BuildAll.exe"
LINK32_OBJS= \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\BuildAll.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Debug"

OUTDIR=.\bin\debug\win32
INTDIR=.\obj\debug\win32
# Begin Custom Macros
OutDir=.\bin\debug\win32
# End Custom Macros

!IF "$(RECURSE)" == "0"

ALL : "$(OUTDIR)\BuildAll.exe"

!ELSE

ALL : "goldlib - Win32 Debug" "rddt - Win32 Debug" "goldnode - Win32 Debug" "golded - Win32 Debug" "$(OUTDIR)\BuildAll.exe"

!ENDIF

!IF "$(RECURSE)" == "1"
CLEAN :"golded - Win32 DebugCLEAN" "goldnode - Win32 DebugCLEAN" "rddt - Win32 DebugCLEAN" "goldlib - Win32 DebugCLEAN"
!ELSE
CLEAN :
!ENDIF
	-@erase "$(OUTDIR)\BuildAll.exe"
	-@erase "$(OUTDIR)\BuildAll.ilk"
	-@erase "$(OUTDIR)\BuildAll.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BuildAll.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\BuildAll.pdb" /debug /machine:I386 /out:"$(OUTDIR)\BuildAll.exe" /pdbtype:sept
LINK32_OBJS= \
	"$(OUTDIR)\goldlib.lib"

"$(OUTDIR)\BuildAll.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("BuildAll.dep")
!INCLUDE "BuildAll.dep"
!ELSE
!MESSAGE Warning: cannot find "BuildAll.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "Release" || "$(CFG)" == "Debug"

!IF  "$(CFG)" == "Release"

"golded - Win32 Release" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Release"
   cd "."

"golded - Win32 ReleaseCLEAN" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Release" RECURSE=1 CLEAN
   cd "."

!ELSEIF  "$(CFG)" == "Debug"

"golded - Win32 Debug" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Debug"
   cd "."

"golded - Win32 DebugCLEAN" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\golded.mak CFG="golded - Win32 Debug" RECURSE=1 CLEAN
   cd "."

!ENDIF

!IF  "$(CFG)" == "Release"

"goldnode - Win32 Release" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldnode.mak CFG="goldnode - Win32 Release"
   cd "."

"goldnode - Win32 ReleaseCLEAN" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldnode.mak CFG="goldnode - Win32 Release" RECURSE=1 CLEAN
   cd "."

!ELSEIF  "$(CFG)" == "Debug"

"goldnode - Win32 Debug" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldnode.mak CFG="goldnode - Win32 Debug"
   cd "."

"goldnode - Win32 DebugCLEAN" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldnode.mak CFG="goldnode - Win32 Debug" RECURSE=1 CLEAN
   cd "."

!ENDIF

!IF  "$(CFG)" == "Release"

"rddt - Win32 Release" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\rddt.mak CFG="rddt - Win32 Release"
   cd "."

"rddt - Win32 ReleaseCLEAN" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\rddt.mak CFG="rddt - Win32 Release" RECURSE=1 CLEAN
   cd "."

!ELSEIF  "$(CFG)" == "Debug"

"rddt - Win32 Debug" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\rddt.mak CFG="rddt - Win32 Debug"
   cd "."

"rddt - Win32 DebugCLEAN" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\rddt.mak CFG="rddt - Win32 Debug" RECURSE=1 CLEAN
   cd "."

!ENDIF

!IF  "$(CFG)" == "Release"

"goldlib - Win32 Release" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release"
   cd "."

"goldlib - Win32 ReleaseCLEAN" :
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\goldlib.mak CFG="goldlib - Win32 Release" RECURSE=1 CLEAN
   cd "."

!ELSEIF  "$(CFG)" == "Debug"

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
