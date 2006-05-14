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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

CPP=@cl.exe /IC:\PROGRA~1\MICROS~2\VC98\INCLUDE
RSC=rc.exe
LINK=@link.exe /LIBPATH:C:\PROGRA~1\MICROS~2\VC98\LIB

!IF  "$(CFG)" == "Release"
OBJ_DIR=Release
BIN_DIR=$(OBJ_DIR)\bin
!ELSEIF  "$(CFG)" == "Debug"
OBJ_DIR=Debug
BIN_DIR=$(OBJ_DIR)\bin
!ENDIF


LNK_OBJS= \
    $(OBJ_DIR)\gedcyg.res \
    $(OBJ_DIR)\gbmh.obj \
    $(OBJ_DIR)\gcharset.obj \
    $(OBJ_DIR)\gcrc16tb.obj \
    $(OBJ_DIR)\gcrc32tb.obj \
    $(OBJ_DIR)\gcrchash.obj \
    $(OBJ_DIR)\gcrckeyv.obj \
    $(OBJ_DIR)\gcrcm16.obj \
    $(OBJ_DIR)\gcrcm32.obj \
    $(OBJ_DIR)\gcrcs16.obj \
    $(OBJ_DIR)\gcrcs32.obj \
    $(OBJ_DIR)\gdbgerr.obj \
    $(OBJ_DIR)\gdbgtrk.obj \
    $(OBJ_DIR)\gdirposx.obj \
    $(OBJ_DIR)\geval.obj \
    $(OBJ_DIR)\gevalhum.obj \
    $(OBJ_DIR)\gevalrpn.obj \
    $(OBJ_DIR)\gespell.obj \
    $(OBJ_DIR)\gfile.obj \
    $(OBJ_DIR)\gfilport.obj \
    $(OBJ_DIR)\gfilutl1.obj \
    $(OBJ_DIR)\gfilutl2.obj \
    $(OBJ_DIR)\gftnaddr.obj \
    $(OBJ_DIR)\gftnnl.obj \
    $(OBJ_DIR)\gftnnlfd.obj \
    $(OBJ_DIR)\gftnnlfu.obj \
    $(OBJ_DIR)\gftnnlge.obj \
    $(OBJ_DIR)\gftnnlv7.obj \
    $(OBJ_DIR)\gfuzzy.obj \
    $(OBJ_DIR)\ghdrmime.obj \
    $(OBJ_DIR)\gkbdbase.obj \
    $(OBJ_DIR)\gkbdgetm.obj \
    $(OBJ_DIR)\gkbdwait.obj \
    $(OBJ_DIR)\glog.obj \
    $(OBJ_DIR)\gmemdbg.obj \
    $(OBJ_DIR)\gmemutil.obj \
    $(OBJ_DIR)\gmoubase.obj \
    $(OBJ_DIR)\gmsgattr.obj \
    $(OBJ_DIR)\gprnutil.obj \
    $(OBJ_DIR)\gregex.obj \
    $(OBJ_DIR)\gsearch.obj \
    $(OBJ_DIR)\gsnd.obj \
    $(OBJ_DIR)\gsndwrap.obj \
    $(OBJ_DIR)\gsrchmgr.obj \
    $(OBJ_DIR)\gstrbags.obj \
    $(OBJ_DIR)\gstrctyp.obj \
    $(OBJ_DIR)\gstrmail.obj \
    $(OBJ_DIR)\gstrname.obj \
    $(OBJ_DIR)\gstrutil.obj \
    $(OBJ_DIR)\gtimjuld.obj \
    $(OBJ_DIR)\gtimutil.obj \
    $(OBJ_DIR)\gtxtpara.obj \
    $(OBJ_DIR)\gusrbase.obj \
    $(OBJ_DIR)\gusrezyc.obj \
    $(OBJ_DIR)\gusrgold.obj \
    $(OBJ_DIR)\gusrhuds.obj \
    $(OBJ_DIR)\gusrmax.obj \
    $(OBJ_DIR)\gusrpcb.obj \
    $(OBJ_DIR)\gusrra2.obj \
    $(OBJ_DIR)\gusrxbbs.obj \
    $(OBJ_DIR)\gutlclip.obj \
    $(OBJ_DIR)\gutlcode.obj \
    $(OBJ_DIR)\gutlgrp.obj \
    $(OBJ_DIR)\gutlmisc.obj \
    $(OBJ_DIR)\gutlmtsk.obj \
    $(OBJ_DIR)\gutltag.obj \
    $(OBJ_DIR)\gutlvers.obj \
    $(OBJ_DIR)\gutlwin.obj \
    $(OBJ_DIR)\gutlwinm.obj \
    $(OBJ_DIR)\gvidbase.obj \
    $(OBJ_DIR)\gvidinit.obj \
    $(OBJ_DIR)\gwildmat.obj \
    $(OBJ_DIR)\gwinbase.obj \
    $(OBJ_DIR)\gwindow.obj \
    $(OBJ_DIR)\gwinhlp1.obj \
    $(OBJ_DIR)\gwinhlp2.obj \
    $(OBJ_DIR)\gwininit.obj \
    $(OBJ_DIR)\gwinline.obj \
    $(OBJ_DIR)\gwinmenu.obj \
    $(OBJ_DIR)\gwinmnub.obj \
    $(OBJ_DIR)\gwinpckf.obj \
    $(OBJ_DIR)\gwinpcks.obj \
    $(OBJ_DIR)\gwinpick.obj \
    $(OBJ_DIR)\gwinput2.obj \
    $(OBJ_DIR)\gedacfg.obj \
    $(OBJ_DIR)\gxareas.obj \
    $(OBJ_DIR)\gxcrash.obj \
    $(OBJ_DIR)\gxdb.obj \
    $(OBJ_DIR)\gxdutch.obj \
    $(OBJ_DIR)\gxezy102.obj \
    $(OBJ_DIR)\gxezy110.obj \
    $(OBJ_DIR)\gxfd.obj \
    $(OBJ_DIR)\gxfecho4.obj \
    $(OBJ_DIR)\gxfecho5.obj \
    $(OBJ_DIR)\gxfecho6.obj \
    $(OBJ_DIR)\gxfidpcb.obj \
    $(OBJ_DIR)\gxfm092.obj \
    $(OBJ_DIR)\gxfm100.obj \
    $(OBJ_DIR)\gxfm116.obj \
    $(OBJ_DIR)\gxgecho.obj \
    $(OBJ_DIR)\gxhpt.obj \
    $(OBJ_DIR)\gximail4.obj \
    $(OBJ_DIR)\gximail5.obj \
    $(OBJ_DIR)\gximail6.obj \
    $(OBJ_DIR)\gxinter.obj \
    $(OBJ_DIR)\gxlora.obj \
    $(OBJ_DIR)\gxmax3.obj \
    $(OBJ_DIR)\gxme2.obj \
    $(OBJ_DIR)\gxopus.obj \
    $(OBJ_DIR)\gxpcb.obj \
    $(OBJ_DIR)\gxportal.obj \
    $(OBJ_DIR)\gxprobrd.obj \
    $(OBJ_DIR)\gxqecho.obj \
    $(OBJ_DIR)\gxqfront.obj \
    $(OBJ_DIR)\gxquick.obj \
    $(OBJ_DIR)\gxra.obj \
    $(OBJ_DIR)\gxraecho.obj \
    $(OBJ_DIR)\gxspace.obj \
    $(OBJ_DIR)\gxsquish.obj \
    $(OBJ_DIR)\gxsuper.obj \
    $(OBJ_DIR)\gxsync.obj \
    $(OBJ_DIR)\gxtimed.obj \
    $(OBJ_DIR)\gxtmail.obj \
    $(OBJ_DIR)\gxts.obj \
    $(OBJ_DIR)\gxwmail.obj \
    $(OBJ_DIR)\gxwtr.obj \
    $(OBJ_DIR)\gxxbbs.obj \
    $(OBJ_DIR)\gxxmail.obj \
    $(OBJ_DIR)\regex.obj \
    $(OBJ_DIR)\gmoarea.obj \
    $(OBJ_DIR)\gmoezyc1.obj \
    $(OBJ_DIR)\gmoezyc2.obj \
    $(OBJ_DIR)\gmoezyc3.obj \
    $(OBJ_DIR)\gmoezyc4.obj \
    $(OBJ_DIR)\gmoezyc5.obj \
    $(OBJ_DIR)\gmofido1.obj \
    $(OBJ_DIR)\gmofido2.obj \
    $(OBJ_DIR)\gmofido3.obj \
    $(OBJ_DIR)\gmofido4.obj \
    $(OBJ_DIR)\gmofido5.obj \
    $(OBJ_DIR)\gmohuds.obj \
    $(OBJ_DIR)\gmojamm1.obj \
    $(OBJ_DIR)\gmojamm2.obj \
    $(OBJ_DIR)\gmojamm3.obj \
    $(OBJ_DIR)\gmojamm4.obj \
    $(OBJ_DIR)\gmojamm5.obj \
    $(OBJ_DIR)\gmopcbd1.obj \
    $(OBJ_DIR)\gmopcbd2.obj \
    $(OBJ_DIR)\gmopcbd3.obj \
    $(OBJ_DIR)\gmopcbd4.obj \
    $(OBJ_DIR)\gmopcbd5.obj \
    $(OBJ_DIR)\gmosmb1.obj \
    $(OBJ_DIR)\gmosmb2.obj \
    $(OBJ_DIR)\gmosqsh1.obj \
    $(OBJ_DIR)\gmosqsh2.obj \
    $(OBJ_DIR)\gmosqsh3.obj \
    $(OBJ_DIR)\gmosqsh4.obj \
    $(OBJ_DIR)\gmosqsh5.obj \
    $(OBJ_DIR)\gmowcat1.obj \
    $(OBJ_DIR)\gmowcat2.obj \
    $(OBJ_DIR)\gmowcat3.obj \
    $(OBJ_DIR)\gmowcat4.obj \
    $(OBJ_DIR)\gmowcat5.obj \
    $(OBJ_DIR)\gmoxbbs1.obj \
    $(OBJ_DIR)\gmoxbbs2.obj \
    $(OBJ_DIR)\gmoxbbs3.obj \
    $(OBJ_DIR)\gmoxbbs4.obj \
    $(OBJ_DIR)\gmoxbbs5.obj \
    $(OBJ_DIR)\lzh.obj \
    $(OBJ_DIR)\smblib.obj \
    $(OBJ_DIR)\fptools.obj \
    $(OBJ_DIR)\uucheck.obj \
    $(OBJ_DIR)\uuencode.obj \
    $(OBJ_DIR)\uulib.obj \
    $(OBJ_DIR)\uunconc.obj \
    $(OBJ_DIR)\uuscan.obj \
    $(OBJ_DIR)\uustring.obj \
    $(OBJ_DIR)\uuutil.obj \
    $(OBJ_DIR)\gcalst.obj \
    $(OBJ_DIR)\gcarea.obj \
    $(OBJ_DIR)\gccfgg.obj \
    $(OBJ_DIR)\gccfgg0.obj \
    $(OBJ_DIR)\gccfgg1.obj \
    $(OBJ_DIR)\gccfgg2.obj \
    $(OBJ_DIR)\gccfgg3.obj \
    $(OBJ_DIR)\gccfgg4.obj \
    $(OBJ_DIR)\gccfgg5.obj \
    $(OBJ_DIR)\gccfgg6.obj \
    $(OBJ_DIR)\gccfgg7.obj \
    $(OBJ_DIR)\gccfgg8.obj \
    $(OBJ_DIR)\gckeys.obj \
    $(OBJ_DIR)\gclang.obj \
    $(OBJ_DIR)\gcmisc.obj \
    $(OBJ_DIR)\gealst.obj \
    $(OBJ_DIR)\gearea.obj \
    $(OBJ_DIR)\gecarb.obj \
    $(OBJ_DIR)\gecmfd.obj \
    $(OBJ_DIR)\gectnr.obj \
    $(OBJ_DIR)\gectrl.obj \
    $(OBJ_DIR)\gedoit.obj \
    $(OBJ_DIR)\gedoss.obj \
    $(OBJ_DIR)\geedit.obj \
    $(OBJ_DIR)\geedit2.obj \
    $(OBJ_DIR)\geedit3.obj \
    $(OBJ_DIR)\gefile.obj \
    $(OBJ_DIR)\gefind.obj \
    $(OBJ_DIR)\geglob.obj \
    $(OBJ_DIR)\gehdre.obj \
    $(OBJ_DIR)\gehtml.obj \
    $(OBJ_DIR)\geinit.obj \
    $(OBJ_DIR)\geline.obj \
    $(OBJ_DIR)\gelmsg.obj \
    $(OBJ_DIR)\gemenu.obj \
    $(OBJ_DIR)\gemlst.obj \
    $(OBJ_DIR)\gemnus.obj \
    $(OBJ_DIR)\gemrks.obj \
    $(OBJ_DIR)\gemsgs.obj \
    $(OBJ_DIR)\genode.obj \
    $(OBJ_DIR)\geplay.obj \
    $(OBJ_DIR)\gepost.obj \
    $(OBJ_DIR)\geqwks.obj \
    $(OBJ_DIR)\gerand.obj \
    $(OBJ_DIR)\geread.obj \
    $(OBJ_DIR)\geread2.obj \
    $(OBJ_DIR)\gescan.obj \
    $(OBJ_DIR)\gesoup.obj \
    $(OBJ_DIR)\gesrch.obj \
    $(OBJ_DIR)\getpls.obj \
    $(OBJ_DIR)\geusrbse.obj \
    $(OBJ_DIR)\geutil.obj \
    $(OBJ_DIR)\geutil2.obj \
    $(OBJ_DIR)\geview.obj \
    $(OBJ_DIR)\gmarea.obj \
    $(OBJ_DIR)\golded3.obj \
    $(OBJ_DIR)\affentry.obj \
    $(OBJ_DIR)\affixmgr.obj \
    $(OBJ_DIR)\csutil.obj \
    $(OBJ_DIR)\dictmgr.obj \
    $(OBJ_DIR)\hashmgr.obj \
    $(OBJ_DIR)\hunspell.obj \
    $(OBJ_DIR)\suggmgr.obj

GOLDED_OBJS=$(OBJ_DIR)\gemain.obj
RDDT_OBJS=$(OBJ_DIR)\rddt.obj
GOLDNODE_OBJS=$(OBJ_DIR)\goldnode.obj

ALL_OBJS=$(LNK_OBJS) $(GOLDED_OBJS) $(RDDT_OBJS) $(GOLDNODE_OBJS)

CPP_FLAGS=/nologo /c /J /W3 /Gi /EHac /FD /Igolded3 /Igoldlib\gall /Igoldlib\gcfg /Igoldlib\glibc /Igoldlib\gmb3 /Igoldlib\smblib /Igoldlib\uulib /Igoldlib\hunspell /DWIN32 /D_CONSOLE /DHAVE_CONFIG_H /Fo$(OBJ_DIR)\\ /Fd$(OBJ_DIR)\\
RSC_FLAGS=/igolded3 /fo$(OBJ_DIR)\gedcyg.res
LNK_FLAGS=advapi32.lib winmm.lib user32.lib /nologo /subsystem:console /machine:I386

!IF  "$(CFG)" == "Release"
CPP_PROJ=$(CPP_FLAGS) /Gy /MD /O1 /DNDEBUG $(CPP_EXTRA_FLAGS)
RSC_PROJ=$(RSC_FLAGS) /dNDEBUG
LNK_PROJ=$(LNK_FLAGS) /incremental:no /OPT:REF,ICF,NOWIN98
!ELSEIF  "$(CFG)" == "Debug"
CPP_PROJ=$(CPP_FLAGS) /MDd /Gm /ZI /GZ /Od /D_DEBUG $(CPP_EXTRA_FLAGS)
RSC_PROJ=$(RSC_FLAGS) /d_DEBUG
LNK_PROJ=$(LNK_FLAGS) /incremental:yes /debug /pdbtype:sept
!ENDIF

all: $(BIN_DIR) $(DEF_FILE) $(LNK_OBJS) $(BIN_DIR)\gedwin.exe $(BIN_DIR)\rddtwin.exe $(BIN_DIR)\gnwin.exe

clean:
    -@erase $(OBJ_DIR)\*.obj
    -@erase $(OBJ_DIR)\*.res
    -@erase $(OBJ_DIR)\vc??.idb

distclean:
    -@erase $(BIN_DIR)\gedwin.exe
    -@erase $(BIN_DIR)\gedwin.pdb
    -@erase $(BIN_DIR)\rddtwin.exe
    -@erase $(BIN_DIR)\rddtwin.pdb
    -@erase $(BIN_DIR)\gnwin.exe
    -@erase $(BIN_DIR)\gnwin.pdb
    -@erase $(BIN_DIR)\File_ID.Diz

$(BIN_DIR): $(OBJ_DIR)
    @if not exist $@ mkdir $@

$(OBJ_DIR):
    @if not exist $@ mkdir $@

$(BIN_DIR)\gedwin.exe: $(GOLDED_OBJS)
    $(LINK) $(LNK_PROJ) /pdb:$(BIN_DIR)\gedwin.pdb /out:$(BIN_DIR)\gedwin.exe $(LNK_OBJS) $(GOLDED_OBJS)

$(BIN_DIR)\rddtwin.exe: $(RDDT_OBJS)
    $(LINK) $(LNK_PROJ) /pdb:$(BIN_DIR)\rddtwin.pdb /out:$(BIN_DIR)\rddtwin.exe $(LNK_OBJS) $(RDDT_OBJS)

$(BIN_DIR)\gnwin.exe: $(GOLDNODE_OBJS)
    $(LINK) $(LNK_PROJ) /pdb:$(BIN_DIR)\gnwin.pdb /out:$(BIN_DIR)\gnwin.exe $(LNK_OBJS) $(GOLDNODE_OBJS)


$(ALL_OBJS):


#
# RS rules
#

{golded3}.rc{$(OBJ_DIR)}.res:
    $(RSC) $(RSC_PROJ) $<

#
# CPP rules
#

{golded3}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{rddt}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldnode}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\gall}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\gcfg}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\glibc}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\gmb3}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\smblib}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\uulib}.cpp{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\hunspell}.cxx{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

#
# C rules
#

{golded3}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{rddt}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldnode}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\gall}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\gcfg}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\glibc}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\gmb3}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\smblib}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<

{goldlib\uulib}.c{$(OBJ_DIR)}.obj:
    $(CPP) @<<
    $(CPP_PROJ) $<
<<
