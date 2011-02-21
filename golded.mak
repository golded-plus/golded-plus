# $Id$
# Microsoft Visual Studio 6 makefile.
# (c) Ianos Gnatiuc 2:5030/830.17
#
# Usage:
#
# NMAKE /f "golded.mak" CFG="Release"
#
# to build standard binary or
#
# NMAKE /f "golded.mak" CFG="Debug"
#
# to build debug binary


# Uncomment to disable MS Office spellchecker support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NO_MSSPELL
# Uncomment to disable HunSpell spellchecker support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NO_MYSPELL


# Comment to disable keyword AreaRecycleBin
CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /D__INCLUDE_NEW_KEYWORDS__


# Uncomment to disable Ezycom messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOEZY
# Uncomment to disable Goldbase messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOGOLD
# Uncomment to disable Hudson messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOHUDS
# Uncomment to disable JAM messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOJAM
# Uncomment to disable PCBoard messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOPCB
# Uncomment to disable Squish messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOSQSH
# Uncomment to disable SMB messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOSMB
# Uncomment to disable WildCat! messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOWCAT
# Uncomment to disable AdeptXBBS messagebase format
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGMB_NOXBBS


# Uncomment to disable AdeptXBBS support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOXBBS
# Uncomment to disable CrashMail support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOCMAIL
# Uncomment to disable CrashEcho support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOCECHO
# Uncomment to disable D'Bridge support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NODB
# Uncomment to disable Dutchie support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NODUTCHIE
# Uncomment to disable Ezycom support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOEZY
# Uncomment to disable FastEcho support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOFE
# Uncomment to disable Fidoconfig support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOFIDOCONF
# Uncomment to disable FidoPCB support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOFIDOPCB
# Uncomment to disable FMail support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOFMAIL
# Uncomment to disable FrontDoor support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOFD
# Uncomment to disable GEcho support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOGECHO
# Uncomment to disable IMail support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOIMAIL
# Uncomment to disable InterMail support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOINTERMAIL
# Uncomment to disable LoraBBS support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOLORA
# Uncomment to disable Maximus support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOMAXIMUS
# Uncomment to disable ME2 support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOME2
# Uncomment to disable Opus support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOOPUS
# Uncomment to disable Parma tosser support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOPARTOSS
# Uncomment to disable PCBoard support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOPCB
# Uncomment to disable Portal of Power support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOPORTAL
# Uncomment to disable ProBoard support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOPROBOARD
# Uncomment to disable QEcho support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOQECHO
# Uncomment to disable QFront support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOQFRONT
# Uncomment to disable QuickBBS support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOQBBS
# Uncomment to disable RA-ECHO support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NORAECHO
# Uncomment to disable RemoteAccess support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NORA
# Uncomment to disable SpaceToss support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOSPCT
# Uncomment to disable Squish support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOSQSH
# Uncomment to disable SuperBBS support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOSBBS
# Uncomment to disable TerMail support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOTERMAIL
# Uncomment to disable timEd support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOTIMED
# Uncomment to disable TosScan support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOTOSSCAN
# Uncomment to disable WaterGate support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOWATERGATE
# Uncomment to disable Wmail support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOWMAIL
# Uncomment to disable xMail support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOXMAIL
# Uncomment to disable Synchronet BBS support
# CPP_EXTRA_FLAGS=$(CPP_EXTRA_FLAGS) /DGCFG_NOSYNCHRONET


!IF "$(CFG)" == ""
CFG=Release
!MESSAGE No configuration specified. Defaulting to "Release".
!ENDIF

all:
   cd "windows"
!IF "$(CFG)" == "Debug"
#   $(MAKE) /$(MAKEFLAGS) /F goldlib.mak  CFG="goldlib - Win32 Debug"
   $(MAKE) /$(MAKEFLAGS) /F golded.mak   CFG="golded - Win32 Debug"
   $(MAKE) /$(MAKEFLAGS) /F goldnode.mak CFG="goldnode - Win32 Debug"
   $(MAKE) /$(MAKEFLAGS) /F rddt.mak     CFG="rddt - Win32 Debug"
!ELSE
#   $(MAKE) /$(MAKEFLAGS) /F goldlib.mak  CFG="goldlib - Win32 Release"
   $(MAKE) /$(MAKEFLAGS) /F golded.mak   CFG="golded - Win32 Release"
   $(MAKE) /$(MAKEFLAGS) /F goldnode.mak CFG="goldnode - Win32 Release"
   $(MAKE) /$(MAKEFLAGS) /F rddt.mak     CFG="rddt - Win32 Release"
!ENDIF
   cd ".."
