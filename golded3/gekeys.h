
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Keyboard system.
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------
//
//  The keyboard system in GoldED
//
//  Keycodes in the range 0x0000-0xFDFF are normal keys.
//  Keycodes in the range 0xFE00-0xFE7F are macro numbers.
//  Keycodes in the range 0xFE80-0xFFFE are command numbers.
//
//  The global variable cmdkeys holds a list of all commands and
//  the keys or macro numbers, sorted by key/macro.
//
//  The global variable macros holds all the macros, sorted by key.
//  The global variable totmacros contains the total number of macros.
//
//  The macro list is searched first, then the command list, thus
//  allowing macros that override commands.
//
//  ------------------------------------------------------------------

const gkey KK_Auto                 = 0xFD00;
const gkey KK_Macro                = 0xFE00;
const gkey KK_AreaMacro            = 0xFE01;
const gkey KK_EditMacro            = 0xFE02;
const gkey KK_FileMacro            = 0xFE03;
const gkey KK_ListMacro            = 0xFE04;
const gkey KK_NodeMacro            = 0xFE05;
const gkey KK_ReadMacro            = 0xFE06;
                                // = 0xFE07-0xFE7F

const gkey KK_Commands             = 0xFE80;
const gkey KK_ExternUtil01         = 0xFE81;
const gkey KK_ExternUtil02         = 0xFE82;
const gkey KK_ExternUtil03         = 0xFE83;
const gkey KK_ExternUtil04         = 0xFE84;
const gkey KK_ExternUtil05         = 0xFE85;
const gkey KK_ExternUtil06         = 0xFE86;
const gkey KK_ExternUtil07         = 0xFE87;
const gkey KK_ExternUtil08         = 0xFE88;
const gkey KK_ExternUtil09         = 0xFE89;
const gkey KK_ExternUtil10         = 0xFE8A;
const gkey KK_ExternUtil11         = 0xFE8B;
const gkey KK_ExternUtil12         = 0xFE8C;
const gkey KK_ExternUtil13         = 0xFE8D;
const gkey KK_ExternUtil14         = 0xFE8E;
const gkey KK_ExternUtil15         = 0xFE8F;
const gkey KK_ExternUtil16         = 0xFE90;
const gkey KK_ExternUtil17         = 0xFE91;
const gkey KK_ExternUtil18         = 0xFE92;
const gkey KK_ExternUtil19         = 0xFE93;
const gkey KK_ExternUtil20         = 0xFE94;
const gkey KK_ExternUtil21         = 0xFE95;
const gkey KK_ExternUtil22         = 0xFE96;
const gkey KK_ExternUtil23         = 0xFE97;
const gkey KK_ExternUtil24         = 0xFE98;
const gkey KK_ExternUtil25         = 0xFE99;
                                // = 0xFE9A;
                                // = 0xFE9B;
                                // = 0xFE9C;
                                // = 0xFE9D;
                                // = 0xFE9E;
                                // = 0xFE9F;

const gkey KK_AreaAbort            = 0xFEA0;
const gkey KK_AreaAskExit          = 0xFEA1;
const gkey KK_AreaBoardnos         = 0xFEA2;
const gkey KK_AreaCatchUp          = 0xFEA3;
const gkey KK_AreaDosShell         = 0xFEA4;
const gkey KK_AreaDropMsgMarks     = 0xFEA5;
const gkey KK_AreaGotoFirst        = 0xFEA6;
const gkey KK_AreaGotoLast         = 0xFEA7;
const gkey KK_AreaGotoNext         = 0xFEA8;
const gkey KK_AreaGotoPrev         = 0xFEA9;
const gkey KK_AreaHeat             = 0xFEAA;
const gkey KK_AreaJump             = 0xFEAC;
const gkey KK_AreaJumpNextMatch    = 0xFEAD;
const gkey KK_AreaMainMenu         = 0xFEAE;
                                // = 0xFEAF;
const gkey KK_AreaQuitNow          = 0xFEB0;
const gkey KK_AreaScan             = 0xFEB1;
const gkey KK_AreaScanPM           = 0xFEB2;
const gkey KK_AreaSelect           = 0xFEB3;
const gkey KK_AreaSelectMarks      = 0xFEB4;
const gkey KK_AreaSoundkill        = 0xFEB5;
const gkey KK_AreaToggle           = 0xFEB6;
const gkey KK_AreaTouchNetscan     = 0xFEB7;
const gkey KK_AreaWriteGoldlast    = 0xFEB8;
const gkey KK_AreaZap              = 0xFEB9;
const gkey KK_AreaMark             = 0xFEBA;
const gkey KK_AreaUnmark           = 0xFEBB;
                                // = 0xFEBC;
                                // = 0xFEBD;
                                // = 0xFEBE;
                                // = 0xFEBF;

const gkey KK_EditAbort            = 0xFEC0;
const gkey KK_EditAnchor           = 0xFEC1;
const gkey KK_EditAskExit          = 0xFEC2;
const gkey KK_EditClearDeleteBuf   = 0xFEC3;
const gkey KK_EditClearPasteBuf    = 0xFEC4;
const gkey KK_EditCopy             = 0xFEC5;
const gkey KK_EditCopyAbove        = 0xFEC6;
const gkey KK_EditCopyAboveChar    = 0xFEC7;
const gkey KK_EditCut              = 0xFEC8;
const gkey KK_EditDelChar          = 0xFEC9;
const gkey KK_EditDeleteEOL        = 0xFECA;
const gkey KK_EditDelLeft          = 0xFECB;
const gkey KK_EditDelLine          = 0xFECC;
const gkey KK_EditDelLtWord        = 0xFECD;
const gkey KK_EditDelRtWord        = 0xFECE;
const gkey KK_EditDosShell         = 0xFECF;
const gkey KK_EditDupLine          = 0xFED0;
const gkey KK_EditExitMsg          = 0xFED1;
const gkey KK_EditExportText       = 0xFED2;
const gkey KK_EditGoBegLine        = 0xFED3;
const gkey KK_EditGoBotLine        = 0xFED4;
const gkey KK_EditGoBotMsg         = 0xFED5;
const gkey KK_EditGoDown           = 0xFED6;
const gkey KK_EditGoEOL            = 0xFED7;
const gkey KK_EditGoLeft           = 0xFED8;
const gkey KK_EditGoPgDn           = 0xFED9;
const gkey KK_EditGoPgUp           = 0xFEDA;
const gkey KK_EditGoRight          = 0xFEDB;
const gkey KK_EditGoTopLine        = 0xFEDC;
const gkey KK_EditGoTopMsg         = 0xFEDD;
const gkey KK_EditGoUp             = 0xFEDE;
const gkey KK_EditGoWordLeft       = 0xFEDF;
const gkey KK_EditGoWordRight      = 0xFEE0;
const gkey KK_EditHeader           = 0xFEE1;
const gkey KK_EditImportQuotebuf   = 0xFEE2;
const gkey KK_EditImportText       = 0xFEE3;
                                // = 0xFEE4;
const gkey KK_EditLoadFile         = 0xFEE5;
const gkey KK_EditLookupCursor     = 0xFEE6;
const gkey KK_EditLookupDest       = 0xFEE7;
const gkey KK_EditLookupOrig       = 0xFEE8;
                                // = 0xFEE9;
                                // = 0xFEEA;
const gkey KK_EditNewline          = 0xFEEB;
const gkey KK_EditPaste            = 0xFEEC;
const gkey KK_EditQuitNow          = 0xFEED;
const gkey KK_EditReflow           = 0xFEEE;
const gkey KK_EditSaveFile         = 0xFEEF;
const gkey KK_EditSaveMsg          = 0xFEF0;
const gkey KK_EditSoundkill        = 0xFEF1;
const gkey KK_EditSpellCheck       = 0xFEF2;
const gkey KK_EditTab              = 0xFEF3;
const gkey KK_EditTabReverse       = 0xFEF4;
const gkey KK_EditToggleCase       = 0xFEF5;
const gkey KK_EditToggleInsert     = 0xFEF6;
const gkey KK_EditToLower          = 0xFEF7;
const gkey KK_EditToUpper          = 0xFEF8;
const gkey KK_EditUnDelete         = 0xFEF9;
const gkey KK_EditZapQuoteBelow    = 0xFEFA;
const gkey KK_EditBlockRight       = 0xFEFB;
const gkey KK_EditBlockLeft        = 0xFEFC;
const gkey KK_EditBlockUp          = 0xFEFD;
const gkey KK_EditBlockDown        = 0xFEFE;
const gkey KK_EditBlockHome        = 0xFEFF;
const gkey KK_EditBlockEnd         = 0xFF00;
const gkey KK_EditBlockPgDn        = 0xFF01;
const gkey KK_EditBlockPgUp        = 0xFF02;
const gkey KK_EditDelete           = 0xFF03;
const gkey KK_EditUndo             = 0xFF04;
const gkey KK_EditDeleteSOL        = 0xFF05;
const gkey KK_EditBlockWordLeft    = 0xFF06;
const gkey KK_EditBlockWordRight   = 0xFF07;
const gkey KK_EditSCodeNormal      = 0xFF08;
const gkey KK_EditSCodeBold        = 0xFF09;
const gkey KK_EditSCodeItalic      = 0xFF0A;
const gkey KK_EditSCodeUnderline   = 0xFF0B;
const gkey KK_EditSCodeReverse     = 0xFF0C;
#if defined(GCFG_SPELL_INCLUDED)
const gkey KK_EditSCheckerMenu     = 0xFF0D;
#endif
const gkey KK_EditDrawLines        = 0xFF0E;
                                // = 0xFE0F;

const gkey KK_FileAbort            = 0xFF10;
const gkey KK_FileAskExit          = 0xFF11;
const gkey KK_FileDosShell         = 0xFF12;
const gkey KK_FileGotoFirst        = 0xFF13;
const gkey KK_FileGotoLast         = 0xFF14;
const gkey KK_FileGotoNext         = 0xFF15;
const gkey KK_FileGotoPrev         = 0xFF16;
const gkey KK_FileMark             = 0xFF17;
const gkey KK_FileMarkAll          = 0xFF18;
                                // = 0xFE19;
const gkey KK_FileQuitNow          = 0xFF1A;
const gkey KK_FileSelect           = 0xFF1B;
const gkey KK_FileToggleMark       = 0xFF1C;
const gkey KK_FileToggleMarkAll    = 0xFF1D;
const gkey KK_FileUnMark           = 0xFF1E;
const gkey KK_FileUnMarkAll        = 0xFF1F;

const gkey KK_ListAbort            = 0xFF20;
const gkey KK_ListAskExit          = 0xFF21;
const gkey KK_ListDosShell         = 0xFF22;
const gkey KK_ListGotoBookMark     = 0xFF23;
const gkey KK_ListGotoFirst        = 0xFF24;
const gkey KK_ListGotoLast         = 0xFF25;
const gkey KK_ListGotoNext         = 0xFF26;
const gkey KK_ListGotoPrev         = 0xFF27;
const gkey KK_ListMainMenu         = 0xFF28;
const gkey KK_ListMarkingOptions   = 0xFF29;
                                // = 0xFE2A;
const gkey KK_ListQuitNow          = 0xFF2B;
const gkey KK_ListSelect           = 0xFF2C;
const gkey KK_ListToggleBookMark   = 0xFF2D;
const gkey KK_ListToggleDate       = 0xFF2E;
const gkey KK_ListToggleMark       = 0xFF2F;
const gkey KK_ListToggleWideSubj   = 0xFF30;
const gkey KK_ListMark             = 0xFF31;
const gkey KK_ListUnmark           = 0xFF32;
const gkey KK_ListWideSubj         = 0xFF33;
const gkey KK_ListNarrowSubj       = 0xFF34;

                                // = 0xFE35;
                                // = 0xFE36;
                                // = 0xFE37;
                                // = 0xFE38;
                                // = 0xFE39;
                                // = 0xFE3A;
                                // = 0xFE3B;
                                // = 0xFE3C;
                                // = 0xFE3D;
                                // = 0xFE3E;
                                // = 0xFE3F;

const gkey KK_NodeAbort            = 0xFF40;
const gkey KK_NodeAskExit          = 0xFF41;
const gkey KK_NodeDosShell         = 0xFF42;
const gkey KK_NodeGotoFirst        = 0xFF43;
const gkey KK_NodeGotoLast         = 0xFF44;
const gkey KK_NodeGotoNext         = 0xFF45;
const gkey KK_NodeGotoPrev         = 0xFF46;
                                // = 0xFE47;
                                // = 0xFE48;
const gkey KK_NodeQuitNow          = 0xFF49;
const gkey KK_NodeSelect           = 0xFF4A;

const gkey KK_AddressbookQuit      = 0xFF4B;
const gkey KK_AddressbookAdd       = 0xFF4C;
const gkey KK_AddressbookSelect    = 0xFF4D;
const gkey KK_AddressbookDelete    = 0xFF4E;
const gkey KK_AddressbookPack      = 0xFF4F;

const gkey KK_ReadAddressbookAdd   = 0xFF50;
const gkey KK_ReadAskExit          = 0xFF51;
const gkey KK_ReadChangeAka        = 0xFF52;
const gkey KK_ReadChangeAttrs      = 0xFF53;
                                // = 0xFE54;
                                // = 0xFE55;
const gkey KK_ReadChangeMsg        = 0xFF56;
const gkey KK_ReadChangeOrigin     = 0xFF57;
const gkey KK_ReadChangeTagline    = 0xFF58;
const gkey KK_ReadChangeTemplate   = 0xFF59;
const gkey KK_ReadChangeUsername   = 0xFF5A;
const gkey KK_ReadChangeXlatImport = 0xFF5B;
const gkey KK_ReadCommentMsg       = 0xFF5C;
const gkey KK_ReadCopyMoveForward  = 0xFF5D;
const gkey KK_ReadDecreaseMargin   = 0xFF5E;
const gkey KK_ReadDeleteMsg        = 0xFF5F;
const gkey KK_ReadDosShell         = 0xFF60;
const gkey KK_ReadExternUtilMenu   = 0xFF61;
const gkey KK_ReadFidoRenumber     = 0xFF62;
const gkey KK_ReadFileRequest      = 0xFF63;
const gkey KK_ReadFindAll          = 0xFF64;
const gkey KK_ReadFindHeader       = 0xFF65;
const gkey KK_ReadGotoBookMark     = 0xFF66;
const gkey KK_ReadGotoFirstMsg     = 0xFF67;
const gkey KK_ReadGotoLastMsg      = 0xFF68;
const gkey KK_ReadGotoMsgno        = 0xFF69;
const gkey KK_ReadGotoNextArea     = 0xFF6A;
const gkey KK_ReadGotoNextMsg      = 0xFF6B;
const gkey KK_ReadGotoNextUnread   = 0xFF6C;
const gkey KK_ReadGotoPrevArea     = 0xFF6D;
const gkey KK_ReadGotoPrevMsg      = 0xFF6E;
const gkey KK_ReadGotoPrevUnread   = 0xFF6F;
const gkey KK_ReadGotoReplies      = 0xFF70;
const gkey KK_ReadGotoReply1st     = 0xFF71;
const gkey KK_ReadGotoReplyNext    = 0xFF72;
const gkey KK_ReadGotoReplyPrev    = 0xFF73;
const gkey KK_ReadGotoThNextUnread = 0xFE74;
const gkey KK_ReadIncreaseMargin   = 0xFF75;
const gkey KK_ReadLookupDest       = 0xFF76;
const gkey KK_ReadLookupOrig       = 0xFF77;
const gkey KK_ReadMainMenu         = 0xFF78;
const gkey KK_ReadMakeUserlist     = 0xFF79;
const gkey KK_ReadMarkingOptions   = 0xFF7A;
const gkey KK_ReadMessageList      = 0xFF7B;
const gkey KK_ReadMakePathreport   = 0xFF7C;
const gkey KK_ReadMoveCommentMsg   = 0xFF7D;
const gkey KK_ReadMoveQuoteMsg     = 0xFF7E;
const gkey KK_ReadMsgContinue      = 0xFF7F;
const gkey KK_ReadMsgEnd           = 0xFF80;
const gkey KK_ReadMsgHome          = 0xFF81;
const gkey KK_ReadMsgLineDown      = 0xFF82;
const gkey KK_ReadMsgLineUp        = 0xFF83;
const gkey KK_ReadMsgPgDn          = 0xFF84;
const gkey KK_ReadMsgPgUp          = 0xFF85;
const gkey KK_ReadNewArea          = 0xFF86;
const gkey KK_ReadNewMsg           = 0xFF87;
const gkey KK_ReadQuitNow          = 0xFF88;
const gkey KK_ReadQuoteBuf         = 0xFF89;
const gkey KK_ReadQuoteMsg         = 0xFF8A;
const gkey KK_ReadReplyMsg         = 0xFF8B;
const gkey KK_ReadSearch           = 0xFF8C;
const gkey KK_ReadSoundkill        = 0xFF8D;
                                // = 0xFE8E;
const gkey KK_ReadThreadtree       = 0xFF8F;
const gkey KK_ReadToggleBookMark   = 0xFF90;
const gkey KK_ReadToggleHexdump    = 0xFF91;
const gkey KK_ReadToggleHidden     = 0xFF92;
const gkey KK_ReadToggleHiddKlud   = 0xFF93;
const gkey KK_ReadToggleKludge     = 0xFF94;
const gkey KK_ReadToggleMark       = 0xFF95;
const gkey KK_ReadToggleMarkRead   = 0xFF96;
const gkey KK_ReadTogglePageBar    = 0xFF97;
const gkey KK_ReadToggleQuote      = 0xFF98;
const gkey KK_ReadToggleRealMsgno  = 0xFF99;
const gkey KK_ReadToggleROT13      = 0xFF9A;
const gkey KK_ReadToggleStyles     = 0xFF9B;
const gkey KK_ReadToggleTwits      = 0xFF9C;
const gkey KK_ReadTouchNetscan     = 0xFF9D;
const gkey KK_ReadTouchSemaphore   = 0xFF9E;
const gkey KK_ReadAddressbook      = 0xFF9F;
const gkey KK_ReadUUDecode         = 0xFFA0;
const gkey KK_ReadWriteMsg         = 0xFFA1;
const gkey KK_ReadDirQuoteMsg      = 0xFFA2;
const gkey KK_ReadMoveDirQuoteMsg  = 0xFFA3;
const gkey KK_ReadPeekURLs         = 0xFFA4;
const gkey KK_ReadShowDel          = 0xFFA5;
const gkey KK_ReadShowKludge       = 0xFFA6;
const gkey KK_ReadHideKludge       = 0xFFA7;
const gkey KK_ReadShowHidden       = 0xFFA8;
const gkey KK_ReadHideHidden       = 0xFFA9;
const gkey KK_ReadShowHiddKlud     = 0xFFAA;
const gkey KK_ReadHideHiddKlud     = 0xFFAB;
const gkey KK_ReadMark             = 0xFFAC;
const gkey KK_ReadUnmark           = 0xFFAD;
const gkey KK_ReadReadMarked       = 0xFFAE;
const gkey KK_ReadReadAll          = 0xFFAF;
const gkey KK_ReadRealMsgno        = 0xFFB0;
const gkey KK_ReadSequentMsgno     = 0xFFB1;
const gkey KK_ReadStylesNone       = 0xFFB2;
const gkey KK_ReadStylesShow       = 0xFFB3;
const gkey KK_ReadStylesStrip      = 0xFFB4;
const gkey KK_ReadTwitsShow        = 0xFFB5;
const gkey KK_ReadTwitsBlank       = 0xFFB6;
const gkey KK_ReadTwitsSkip        = 0xFFB7;
const gkey KK_ReadTwitsIgnore      = 0xFFB8;
const gkey KK_ReadTwitsKill        = 0xFFB9;
const gkey KK_ReadStripHTML        = 0xFFBA;
const gkey KK_ReadShowHTML         = 0xFFBB;
const gkey KK_ReadToggleHTML       = 0xFFBC;
const gkey KK_ReadShowQuote        = 0xFFBD;
const gkey KK_ReadHideQuote        = 0xFFBE;
const gkey KK_ReadFindNext         = 0xFFBF;

const gkey KK_HeaderAddressbook    = 0xFFC0;
const gkey KK_HeaderLookup         = 0xFFC1;
const gkey KK_HeaderToggleAttrWin  = 0xFFC2;
const gkey KK_HeaderClearAttrib    = 0xFFC3;
const gkey KK_HeaderToggleReserved = 0xFFC4;
const gkey KK_HeaderToggleGroupmsg = 0xFFC5;
const gkey KK_HeaderToggleScanned  = 0xFFC6;
const gkey KK_HeaderToggleFile     = 0xFFC7;
const gkey KK_HeaderToggleArcsent  = 0xFFC8;
const gkey KK_HeaderToggleCrash    = 0xFFC9;
const gkey KK_HeaderToggleDirect   = 0xFFCA;
const gkey KK_HeaderToggleDelsent  = 0xFFCB;
const gkey KK_HeaderToggleFreq     = 0xFFCC;
const gkey KK_HeaderToggleZonegate = 0xFFCD;
const gkey KK_HeaderToggleHold     = 0xFFCE;
const gkey KK_HeaderToggleImm      = 0xFFCF;
const gkey KK_HeaderToggleTransit  = 0xFFD0;
const gkey KK_HeaderToggleKill     = 0xFFD1;
const gkey KK_HeaderToggleLocked   = 0xFFD2;
const gkey KK_HeaderToggleRetrecreq= 0xFFD3;
const gkey KK_HeaderToggleRetrec   = 0xFFD4;
const gkey KK_HeaderToggleOrphan   = 0xFFD5;
const gkey KK_HeaderTogglePvt      = 0xFFD6;
const gkey KK_HeaderToggleAudit    = 0xFFD7;
const gkey KK_HeaderToggleReceived = 0xFFD8;
const gkey KK_HeaderToggleSent     = 0xFFD9;
const gkey KK_HeaderToggleTrunc    = 0xFFDA;
const gkey KK_HeaderToggleUpdreq   = 0xFFDB;
const gkey KK_HeaderToggleHubhost  = 0xFFDC;
const gkey KK_HeaderToggleLocal    = 0xFFDD;
const gkey KK_HeaderToggleXmail    = 0xFFDE;
const gkey KK_HeaderToggleCfmrecreq= 0xFFDF;

const gkey KK_AreaUndefine         = 0xFFE0;
const gkey KK_EditUndefine         = 0xFFE1;
const gkey KK_FileUndefine         = 0xFFE2;
const gkey KK_ListUndefine         = 0xFFE3;
const gkey KK_NodeUndefine         = 0xFFE4;
const gkey KK_ReadUndefine         = 0xFFE5;
const gkey KK_ZzzzLast             = 0xFFE6;


//  ------------------------------------------------------------------
//  Key groups (type of key)

const int KT_A =  1;   // Arealist
const int KT_E =  2;   // Editor
const int KT_G =  4;   // General (not used)
const int KT_M =  8;   // Msg lister
const int KT_N = 16;   // Node browser
const int KT_R = 32;   // Reader                  (+ external utils)
const int KT_F = 64;   // File browser
const int KT_B = 128;  // Addressbook browser
const int KT_H = 256;  // Header editor


//  ------------------------------------------------------------------
//  Key command

struct CmdKey {
  gkey key;        // Associated key or macro number
  gkey cmd;        // Command number
  int  type;       // Type of key
};


//  ------------------------------------------------------------------
//  Macro

struct Macro {
  int  type;       // Type of key
  gkey key;        // Associated key
  gkey buf[128];   // Actual keys/commands
};


//  ------------------------------------------------------------------

struct tglobalkey {
  word crc_token;
  gkey keyval;
  word keytype;
};

//  ------------------------------------------------------------------

