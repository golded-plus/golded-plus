
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
//  Keyboard configuration compile.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gckeys.h>

extern std::string keybuf;


//  ------------------------------------------------------------------

CmdKey DefaultKeyset[] = {

  { Key_A_A     , KK_AreaAbort            , KT_A },
  { Key_A_X     , KK_AreaAskExit          , KT_A },
  { Key_Esc     , KK_AreaAskExit          , KT_A },
  { Key_A_F10   , KK_AreaAskExit          , KT_A },
  { Key_A_B     , KK_AreaBoardnos         , KT_A },
  { Key_A_C     , KK_AreaCatchUp          , KT_A },
  { Key_A_O     , KK_AreaDosShell         , KT_A },
  { Key_C_M     , KK_AreaDropMsgMarks     , KT_A },
  { Key_A_H     , KK_AreaHeat             , KT_A },
  { Key_A_J     , KK_AreaJump             , KT_A },
  { Key_C_Lft   , KK_AreaJump             , KT_A },
  { Key_C_Rgt   , KK_AreaJump             , KT_A },
  { Key_Tab     , KK_AreaJumpNextMatch    , KT_A },
  { Key_C_Ent   , KK_AreaJumpNextMatch    , KT_A },
  { Key_C_Q     , KK_AreaQuitNow          , KT_A },
  { Key_A_S     , KK_AreaScan             , KT_A },
  { Key_A_P     , KK_AreaScanPM           , KT_A },
  { Key_Ent     , KK_AreaSelect           , KT_A },
  { Key_Rgt     , KK_AreaSelect           , KT_A },
  { Key_A_R     , KK_AreaSelectMarks      , KT_A },
  { Key_A_T     , KK_AreaToggle           , KT_A },
  { Key_Ins     , KK_AreaToggle           , KT_A },
  { Key_C_Z     , KK_AreaTouchNetscan     , KT_A },
  { Key_A_W     , KK_AreaWriteGoldlast    , KT_A },
  { Key_A_Z     , KK_AreaZap              , KT_A },

  { Key_Esc     , KK_EditAbort            , KT_E },
  { Key_A_A     , KK_EditAnchor           , KT_E },
  { Key_A_X     , KK_EditAskExit          , KT_E },
  { Key_A_F4    , KK_EditAskExit          , KT_E },
  { Key_A_F5    , KK_EditClearDeleteBuf   , KT_E },
  { Key_A_F6    , KK_EditClearPasteBuf    , KT_E },
  { Key_A_C     , KK_EditCopy             , KT_E },
  { Key_C_Min   , KK_EditCopyAboveChar    , KT_E },
  { Key_A_M     , KK_EditCut              , KT_E },
  { Key_Del     , KK_EditDelChar          , KT_E },
  { Key_A_K     , KK_EditDeleteEOL        , KT_E },
  { Key_A_Y     , KK_EditDeleteEOL        , KT_E },
  { Key_BS      , KK_EditDelLeft          , KT_E },
  { Key_A_D     , KK_EditDelLine          , KT_E },
  { Key_C_Y     , KK_EditDelLine          , KT_E },
  { Key_C_BS    , KK_EditDelLtWord        , KT_E },
  { Key_C_F5    , KK_EditDelLtWord        , KT_E },
  { Key_C_T     , KK_EditDelRtWord        , KT_E },
  { Key_C_F6    , KK_EditDelRtWord        , KT_E },
  { Key_A_O     , KK_EditDosShell         , KT_E },
  { Key_F4      , KK_EditDupLine          , KT_E },
  { Key_A_W     , KK_EditExportText       , KT_E },
  { Key_Home    , KK_EditGoBegLine        , KT_E },
  { Key_C_PgDn  , KK_EditGoBotLine        , KT_E },
  { Key_C_End   , KK_EditGoBotMsg         , KT_E },
  { Key_Dwn     , KK_EditGoDown           , KT_E },
  { Key_A_Dwn   , KK_EditGoDown           , KT_E },
  { Key_C_Dwn   , KK_EditGoDown           , KT_E },
  { Key_End     , KK_EditGoEOL            , KT_E },
  { Key_Lft     , KK_EditGoLeft           , KT_E },
  { Key_A_Lft   , KK_EditGoLeft           , KT_E },
  { Key_PgDn    , KK_EditGoPgDn           , KT_E },
  { Key_PgUp    , KK_EditGoPgUp           , KT_E },
  { Key_Rgt     , KK_EditGoRight          , KT_E },
  { Key_A_Rgt   , KK_EditGoRight          , KT_E },
  { Key_C_PgUp  , KK_EditGoTopLine        , KT_E },
  { Key_C_Home  , KK_EditGoTopMsg         , KT_E },
  { Key_Up      , KK_EditGoUp             , KT_E },
  { Key_A_Up    , KK_EditGoUp             , KT_E },
  { Key_C_Up    , KK_EditGoUp             , KT_E },
  { Key_C_Lft   , KK_EditGoWordLeft       , KT_E },
  { Key_C_Rgt   , KK_EditGoWordRight      , KT_E },
  { Key_A_H     , KK_EditHeader           , KT_E },
  { Key_A_Q     , KK_EditImportQuotebuf   , KT_E },
  { Key_A_I     , KK_EditImportText       , KT_E },
  { Key_A_R     , KK_EditImportText       , KT_E },
  { Key_F3      , KK_EditImportText       , KT_E },
  { Key_F8      , KK_EditLoadFile         , KT_E },
  { Key_A_L     , KK_EditLookupCursor     , KT_E },
  { Key_S_F10   , KK_EditLookupDest       , KT_E },
  { Key_F10     , KK_EditLookupOrig       , KT_E },
  { Key_Ent     , KK_EditNewline          , KT_E },
  { Key_A_P     , KK_EditPaste            , KT_E },
  { Key_C_Q     , KK_EditQuitNow          , KT_E },
  { Key_A_B     , KK_EditReflow           , KT_E },
  { Key_F7      , KK_EditSaveFile         , KT_E },
  { Key_A_S     , KK_EditSaveMsg          , KT_E },
  { Key_C_Z     , KK_EditSaveMsg          , KT_E },
  { Key_F2      , KK_EditSaveMsg          , KT_E },
  { Key_F9      , KK_EditSpellCheck       , KT_E },
  { Key_Tab     , KK_EditTab              , KT_E },
  { Key_S_Tab   , KK_EditTabReverse       , KT_E },
  { Key_A_3     , KK_EditToggleCase       , KT_E },
  { Key_Ins     , KK_EditToggleInsert     , KT_E },
  { Key_A_2     , KK_EditToLower          , KT_E },
  { Key_A_1     , KK_EditToUpper          , KT_E },
  { Key_C_U     , KK_EditUnDelete         , KT_E },
  { Key_A_BS    , KK_EditUndo             , KT_E },
  { Key_A_Z     , KK_EditZapQuoteBelow    , KT_E },
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { Key_C_Ins   , KK_EditCopy             , KT_E },
  { Key_S_Del   , KK_EditCut              , KT_E },
  { Key_C_Del   , KK_EditDelete           , KT_E },
  { Key_S_Ins   , KK_EditPaste            , KT_E },
  { Key_S_PgUp  , KK_EditBlockPgUp        , KT_E },
  { Key_S_PgDn  , KK_EditBlockPgDn        , KT_E },
  { Key_S_Home  , KK_EditBlockHome        , KT_E },
  { Key_S_End   , KK_EditBlockEnd         , KT_E },
  { Key_S_Up    , KK_EditBlockUp          , KT_E },
  { Key_S_Dwn   , KK_EditBlockDown        , KT_E },
  { Key_S_Lft   , KK_EditBlockLeft        , KT_E },
  { Key_S_Rgt   , KK_EditBlockRight       , KT_E },
#endif
  { Key_C_N     , KK_EditSCodeNormal      , KT_E },
  { Key_C_B     , KK_EditSCodeBold        , KT_E },
  { Key_C_I     , KK_EditSCodeItalic      , KT_E },
  { Key_C_L     , KK_EditSCodeUnderline   , KT_E },
  { Key_C_R     , KK_EditSCodeReverse     , KT_E },
  { Key_C_D     , KK_EditDrawLines        , KT_E },

  { Key_Esc     , KK_FileAbort            , KT_F },
  { Key_A_X     , KK_FileAskExit          , KT_F },
  { Key_A_F4    , KK_FileAskExit          , KT_F },
  { Key_A_O     , KK_FileDosShell         , KT_F },
  { Key_Ins     , KK_FileMark             , KT_F },
  { Key_Plus    , KK_FileMark             , KT_F },
  { Key_C_Ent   , KK_FileMarkAll          , KT_F },
  { Key_C_Home  , KK_FileMarkAll          , KT_F },
  { Key_C_Q     , KK_FileQuitNow          , KT_F },
  { Key_Ent     , KK_FileSelect           , KT_F },
  { Key_Space   , KK_FileToggleMark       , KT_F },
  { Key_Multi   , KK_FileToggleMarkAll    , KT_F },
  { Key_Del     , KK_FileUnMark           , KT_F },
  { Key_Minus   , KK_FileUnMark           , KT_F },
  { Key_C_End   , KK_FileUnMarkAll        , KT_F },

  { Key_Esc     , KK_ListAbort            , KT_M },
  { Key_A_X     , KK_ListAskExit          , KT_M },
  { Key_A_F4    , KK_ListAskExit          , KT_M },
  { Key_A_O     , KK_ListDosShell         , KT_M },
  { Key_S_Tab   , KK_ListGotoBookMark     , KT_M },
  { Key_BS      , KK_ListGotoBookMark     , KT_M },
  { Key_S       , KK_ListMarkingOptions   , KT_M },
  { Key_A_S     , KK_ListMarkingOptions   , KT_M },
  { Key_A_F9    , KK_ListMarkingOptions   , KT_M },
  { Key_C_Q     , KK_ListQuitNow          , KT_M },
  { Key_Ent     , KK_ListSelect           , KT_M },
  { Key_Tab     , KK_ListToggleBookMark   , KT_M },
  { Key_C_D     , KK_ListToggleDate       , KT_M },
  { Key_Space   , KK_ListToggleMark       , KT_M },
  { Key_C_B     , KK_ListToggleWideSubj   , KT_M },
  { Key_C_PgUp  , KK_ListGotoPrev         , KT_M },
  { Key_C_PgDn  , KK_ListGotoNext         , KT_M },

  { Key_Esc     , KK_NodeAbort            , KT_N },
  { Key_A_X     , KK_NodeAskExit          , KT_N },
  { Key_A_F4    , KK_NodeAskExit          , KT_N },
  { Key_A_O     , KK_NodeDosShell         , KT_N },
  { Key_C_Q     , KK_NodeQuitNow          , KT_N },
  { Key_Ent     , KK_NodeSelect           , KT_N },

  { Key_A_F8    , KK_ReadShowDel          , KT_R },
  { Key_C_B     , KK_ReadAddressbookAdd   , KT_R },
  { Key_X       , KK_ReadAskExit          , KT_R },
  { Key_A_X     , KK_ReadAskExit          , KT_R },
  { Key_A_F4    , KK_ReadAskExit          , KT_R },
  { Key_C_A     , KK_ReadChangeAka        , KT_R },
  { Key_C_S     , KK_ReadChangeAttrs      , KT_R },
  { Key_C       , KK_ReadChangeMsg        , KT_R },
  { Key_A_C     , KK_ReadChangeMsg        , KT_R },
  { Key_F2      , KK_ReadChangeMsg        , KT_R },
  { Key_C_O     , KK_ReadChangeOrigin     , KT_R },
  { Key_C_I     , KK_ReadChangeTagline    , KT_R },
  { Key_C_T     , KK_ReadChangeTemplate   , KT_R },
  { Key_C_U     , KK_ReadChangeUsername   , KT_R },
  { Key_C_J     , KK_ReadChangeXlatImport , KT_R },
  { Key_G       , KK_ReadCommentMsg       , KT_R },
  { Key_A_G     , KK_ReadCommentMsg       , KT_R },
  { Key_M       , KK_ReadCopyMoveForward  , KT_R },
  { Key_A_M     , KK_ReadCopyMoveForward  , KT_R },
  { Key_S_F8    , KK_ReadCopyMoveForward  , KT_R },
  { Key_D       , KK_ReadDeleteMsg        , KT_R },
  { Key_A_D     , KK_ReadDeleteMsg        , KT_R },
  { Key_Del     , KK_ReadDeleteMsg        , KT_R },
  { Key_O       , KK_ReadDosShell         , KT_R },
  { Key_A_O     , KK_ReadDosShell         , KT_R },
  { Key_C_F10   , KK_ReadDosShell         , KT_R },
  { Key_C_R     , KK_ReadFidoRenumber     , KT_R },
  { Key_S_F4    , KK_ReadFidoRenumber     , KT_R },
  { Key_C_F     , KK_ReadFileRequest      , KT_R },
  { Key_F       , KK_ReadFindAll          , KT_R },
  { Key_A_F     , KK_ReadFindAll          , KT_R },
  { Key_S_F6    , KK_ReadFindAll          , KT_R },
  { Key_Z       , KK_ReadFindHeader       , KT_R },
  { Key_A_Z     , KK_ReadFindHeader       , KT_R },
  { Key_F6      , KK_ReadFindHeader       , KT_R },
  { Key_A_F6    , KK_ReadFindNext         , KT_R },
  { Key_S_Tab   , KK_ReadGotoBookMark     , KT_R },
  { Key_BS      , KK_ReadGotoBookMark     , KT_R },
  { Key_Lsr     , KK_ReadGotoFirstMsg     , KT_R },
  { Key_C_Home  , KK_ReadGotoFirstMsg     , KT_R },
  { Key_Gtr     , KK_ReadGotoLastMsg      , KT_R },
  { Key_C_End   , KK_ReadGotoLastMsg      , KT_R },
  { Key_C_G     , KK_ReadGotoMsgno        , KT_R },
  { Key_C_N     , KK_ReadGotoNextArea     , KT_R },
  { Key_Rgt     , KK_ReadGotoNextMsg      , KT_R },
  { Key_A_Rgt   , KK_ReadGotoNextUnread   , KT_R },
  { Key_A_U     , KK_ReadGotoNextUnread   , KT_R },
  { Key_C_P     , KK_ReadGotoPrevArea     , KT_R },
  { Key_Lft     , KK_ReadGotoPrevMsg      , KT_R },
  { Key_A_Lft   , KK_ReadGotoPrevUnread   , KT_R },
  { Key_Plus    , KK_ReadGotoReplies      , KT_R },
  { Key_C_Rgt   , KK_ReadGotoReplies      , KT_R },
  { Key_Multi   , KK_ReadGotoReplyNext    , KT_R },
  { Key_Minus   , KK_ReadGotoReplyPrev    , KT_R },
  { Key_C_Lft   , KK_ReadGotoReplyPrev    , KT_R },
  { Key_A_Dwn   , KK_ReadGotoThNextUnread , KT_R },
  { Key_S_F10   , KK_ReadLookupDest       , KT_R },
  { Key_F10     , KK_ReadLookupOrig       , KT_R },
  { Key_C_L     , KK_ReadMakeUserlist     , KT_R },
  { Key_C_K     , KK_ReadMakePathreport   , KT_R },
  { Key_S       , KK_ReadMarkingOptions   , KT_R },
  { Key_A_S     , KK_ReadMarkingOptions   , KT_R },
  { Key_A_F9    , KK_ReadMarkingOptions   , KT_R },
  { Key_A_L     , KK_ReadMessageList      , KT_R },
  { Key_F9      , KK_ReadMessageList      , KT_R },
  { Key_L       , KK_ReadMessageList      , KT_R },
  { Key_B       , KK_ReadMoveCommentMsg   , KT_R },
  { Key_A_B     , KK_ReadMoveCommentMsg   , KT_R },
  { Key_N       , KK_ReadMoveQuoteMsg     , KT_R },
  { Key_A_N     , KK_ReadMoveQuoteMsg     , KT_R },
  { Key_F5      , KK_ReadMoveQuoteMsg     , KT_R },
  { Key_Ent     , KK_ReadMsgContinue      , KT_R },
  { Key_End     , KK_ReadMsgEnd           , KT_R },
  { Key_Home    , KK_ReadMsgHome          , KT_R },
  { Key_Dwn     , KK_ReadMsgLineDown      , KT_R },
  { Key_C_Dwn   , KK_ReadMsgLineDown      , KT_R },
  { Key_Up      , KK_ReadMsgLineUp        , KT_R },
  { Key_C_Up    , KK_ReadMsgLineUp        , KT_R },
  { Key_PgDn    , KK_ReadMsgPgDn          , KT_R },
  { Key_PgUp    , KK_ReadMsgPgUp          , KT_R },
  { Key_A       , KK_ReadNewArea          , KT_R },
  { Key_A_A     , KK_ReadNewArea          , KT_R },
  { Key_Esc     , KK_ReadNewArea          , KT_R },
  { Key_C_F1    , KK_ReadNewArea          , KT_R },
  { Key_E       , KK_ReadNewMsg           , KT_R },
  { Key_A_E     , KK_ReadNewMsg           , KT_R },
  { Key_Ins     , KK_ReadNewMsg           , KT_R },
  { Key_C_Q     , KK_ReadQuitNow          , KT_R },
  { Key_C_W     , KK_ReadQuoteBuf         , KT_R },
  { Key_Q       , KK_ReadQuoteMsg         , KT_R },
  { Key_A_Q     , KK_ReadQuoteMsg         , KT_R },
  { Key_F4      , KK_ReadQuoteMsg         , KT_R },
  { Key_R       , KK_ReadReplyMsg         , KT_R },
  { Key_A_R     , KK_ReadReplyMsg         , KT_R },
  { Key_F3      , KK_ReadReplyMsg         , KT_R },
  { Key_S_3     , KK_ReadThreadtree       , KT_R },
  { Key_Tab     , KK_ReadToggleBookMark   , KT_R },
  { Key_I       , KK_ReadToggleHexdump    , KT_R },
  { Key_A_I     , KK_ReadToggleHexdump    , KT_R },
  { Key_C_F9    , KK_ReadToggleHexdump    , KT_R },
  { Key_H       , KK_ReadToggleHidden     , KT_R },
  { Key_A_H     , KK_ReadToggleHidden     , KT_R },
  { Key_C_F7    , KK_ReadToggleHidden     , KT_R },
  { Key_V       , KK_ReadToggleHiddKlud   , KT_R },
  { Key_A_V     , KK_ReadToggleHiddKlud   , KT_R },
  { Key_C_F5    , KK_ReadToggleHiddKlud   , KT_R },
  { Key_K       , KK_ReadToggleKludge     , KT_R },
  { Key_A_K     , KK_ReadToggleKludge     , KT_R },
  { Key_C_F6    , KK_ReadToggleKludge     , KT_R },
  { Key_Space   , KK_ReadToggleMark       , KT_R },
  { Key_J       , KK_ReadToggleMarkRead   , KT_R },
  { Key_A_J     , KK_ReadToggleMarkRead   , KT_R },
  { Key_P       , KK_ReadTogglePageBar    , KT_R },
  { Key_A_P     , KK_ReadTogglePageBar    , KT_R },
  { Key_C_F8    , KK_ReadTogglePageBar    , KT_R },
  { Key_C_V     , KK_ReadToggleQuote      , KT_R },
  { Key_Y       , KK_ReadToggleRealMsgno  , KT_R },
  { Key_A_Y     , KK_ReadToggleRealMsgno  , KT_R },
  { Key_C_F4    , KK_ReadToggleRealMsgno  , KT_R },
  { Key_U       , KK_ReadToggleROT13      , KT_R },
  { Key_C_F3    , KK_ReadToggleROT13      , KT_R },
  { Key_C_H     , KK_ReadToggleStyles     , KT_R },
  { Key_T       , KK_ReadToggleTwits      , KT_R },
  { Key_A_T     , KK_ReadToggleTwits      , KT_R },
  { Key_C_Z     , KK_ReadTouchNetscan     , KT_R },
  { Key_A_F10   , KK_ReadAddressbook      , KT_R },
  { Key_C_X     , KK_ReadUUDecode         , KT_R },
  { Key_W       , KK_ReadWriteMsg         , KT_R },
  { Key_A_W     , KK_ReadWriteMsg         , KT_R },
  { Key_F7      , KK_ReadWriteMsg         , KT_R },

  { Key_Esc     , KK_AddressbookQuit      , KT_B },
  { Key_Ins     , KK_AddressbookAdd       , KT_B },
  { Key_Ent     , KK_AddressbookSelect    , KT_B },
  { Key_Del     , KK_AddressbookDelete    , KT_B },
  { Key_A_P     , KK_AddressbookPack      , KT_B },

  { Key_A_4     , KK_HeaderToggleScanned  , KT_H },
  { Key_A_2     , KK_HeaderToggleGroupmsg , KT_H },
  { Key_A_G     , KK_HeaderToggleZonegate , KT_H },
  { Key_A_V     , KK_HeaderToggleHubhost  , KT_H },
  { Key_A_M     , KK_HeaderToggleRetrecreq, KT_H },
  { Key_A_C     , KK_HeaderToggleCrash    , KT_H },
  { Key_A_L     , KK_HeaderToggleLocked   , KT_H },
  { Key_A_R     , KK_HeaderToggleReceived , KT_H },
  { Key_A_W     , KK_HeaderToggleLocal    , KT_H },
  { Key_A_N     , KK_HeaderToggleRetrec   , KT_H },
  { Key_A_F     , KK_HeaderToggleFreq     , KT_H },
  { Key_S_F10   , KK_HeaderLookup         , KT_H },
  { Key_A_I     , KK_HeaderToggleImm      , KT_H },
  { Key_A_F1    , KK_HeaderToggleAttrWin  , KT_H },
  { Key_F10     , KK_HeaderAddressbook    , KT_H },
  { Key_A_B     , KK_HeaderToggleArcsent  , KT_H },
  { Key_A_H     , KK_HeaderToggleHold     , KT_H },
  { Key_A_Q     , KK_HeaderToggleAudit    , KT_H },
  { Key_A_X     , KK_HeaderToggleXmail    , KT_H },
  { Key_A_T     , KK_HeaderToggleTrunc    , KT_H },
  { Key_A_U     , KK_HeaderToggleUpdreq   , KT_H },
  { Key_A_Z     , KK_HeaderClearAttrib    , KT_H },
  { Key_A_K     , KK_HeaderToggleKill     , KT_H },
  { Key_A_J     , KK_HeaderToggleTransit  , KT_H },
  { Key_A_Y     , KK_HeaderToggleCfmrecreq, KT_H },
  { Key_A_O     , KK_HeaderToggleOrphan   , KT_H },
  { Key_A_A     , KK_HeaderToggleFile     , KT_H },
  { Key_A_E     , KK_HeaderToggleDelsent  , KT_H },
  { Key_A_D     , KK_HeaderToggleDirect   , KT_H },
  { Key_A_1     , KK_HeaderToggleReserved , KT_H },
  { Key_A_P     , KK_HeaderTogglePvt      , KT_H },
  { Key_A_S     , KK_HeaderToggleSent     , KT_H },

  { 0xFFFF      , 0xFFFF                  , 0xFF }
};


//  ------------------------------------------------------------------

// must be sorted by crc_token
tglobalkey globalkeys[] = {
  { CRC_READLOOKUPDEST         , KK_ReadLookupDest       , KT_R }, // 0x0023
  { CRC_FILETOGGLEMARKALL      , KK_FileToggleMarkAll    , KT_F }, // 0x0032
  { CRC_EDITDRAWLINES          , KK_EditDrawLines        , KT_E }, // 0x00E8
  { CRC_READADDRESSBOOKADD     , KK_ReadAddressbookAdd   , KT_R }, // 0x00FA
  { CRC_LISTDOSSHELL           , KK_ListDosShell         , KT_M }, // 0x0130
  { CRC_READREADALL            , KK_ReadReadAll          , KT_R }, // 0x0146
  { CRC_READREADMARKED         , KK_ReadReadMarked       , KT_R }, // 0x01AE
  { CRC_READTWITSSHOW          , KK_ReadTwitsShow        , KT_R }, // 0x03C5
  { CRC_EDITGOLEFT             , KK_EditGoLeft           , KT_E }, // 0x043B
  { CRC_HEADERTOGGLESCANNED    , KK_HeaderToggleScanned  , KT_H }, // 0x0459
  { CRC_READTOGGLEHIDDEN       , KK_ReadToggleHidden     , KT_R }, // 0x04CE
  { CRC_EDITIMPORTTEXT         , KK_EditImportText       , KT_E }, // 0x0586
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SEND                 , Key_S_End               ,    0 }, // 0x0590
#endif
  { CRC_READGOTOPREVMSG        , KK_ReadGotoPrevMsg      , KT_R }, // 0x06D9
  { CRC_READPEEKURLS           , KK_ReadPeekURLs         , KT_R }, // 0x07BA
  { CRC_READREALMSGNO          , KK_ReadRealMsgno        , KT_R }, // 0x07F8
  { CRC_K_STAB                 , Key_S_Tab               ,    0 }, // 0x0886
  { CRC_EDITLOADFILE           , KK_EditLoadFile         , KT_A }, // 0x0931
  { CRC_READFILEREQUEST        , KK_ReadFileRequest      , KT_R }, // 0x0A07
  { CRC_AREASELECTMARKS        , KK_AreaSelectMarks      , KT_A }, // 0x0AC3
  { CRC_K_ALEFT                , Key_A_Lft               ,    0 }, // 0x0B39
  { CRC_HEADERTOGGLEGROUPMSG   , KK_HeaderToggleGroupmsg , KT_H }, // 0x0C51
  { CRC_K_AF4                  , Key_A_F4                ,    0 }, // 0x0EF0
  { CRC_K_AF5                  , Key_A_F5                ,    0 }, // 0x0EF1
  { CRC_K_AF6                  , Key_A_F6                ,    0 }, // 0x0EF2
  { CRC_K_AF7                  , Key_A_F7                ,    0 }, // 0x0EF3
  { CRC_K_AF1                  , Key_A_F1                ,    0 }, // 0x0EF5
  { CRC_K_AF2                  , Key_A_F2                ,    0 }, // 0x0EF6
  { CRC_K_AF3                  , Key_A_F3                ,    0 }, // 0x0EF7
  { CRC_K_AF8                  , Key_A_F8                ,    0 }, // 0x0EFC
  { CRC_K_AF9                  , Key_A_F9                ,    0 }, // 0x0EFD
  { CRC_K_BSPACE               , Key_BS                  ,    0 }, // 0x0F07
  { CRC_LISTGOTOBOOKMARK       , KK_ListGotoBookMark     , KT_M }, // 0x0F08
  { CRC_EDITDELETEEOL          , KK_EditDeleteEOL        , KT_E }, // 0x1143
  { CRC_FILEUNMARKALL          , KK_FileUnMarkAll        , KT_F }, // 0x121A
  { CRC_K_SPACE                , Key_Space               ,    0 }, // 0x1281
  { CRC_EDITREFLOW             , KK_EditReflow           , KT_E }, // 0x13F9
  { CRC_HEADERTOGGLEZONEGATE   , KK_HeaderToggleZonegate , KT_H }, // 0x140B
  { CRC_K_AF12                 , Key_A_F12               ,    0 }, // 0x14FC
  { CRC_K_AF10                 , Key_A_F10               ,    0 }, // 0x14FE
  { CRC_K_AF11                 , Key_A_F11               ,    0 }, // 0x14FF
  { CRC_LISTGOTONEXT           , KK_ListGotoNext         , KT_M }, // 0x1641
  { CRC_K_LEFT                 , Key_Lft                 ,    0 }, // 0x1694
  { CRC_READREPLYMSG           , KK_ReadReplyMsg         , KT_R }, // 0x172F
  { CRC_AREAGOTOLAST           , KK_AreaGotoLast         , KT_A }, // 0x177A
  { CRC_READINCREASEMARGIN     , KK_ReadIncreaseMargin   , KT_R }, // 0x1797
  { CRC_AREAUNMARK             , KK_AreaUnmark           , KT_A }, // 0x17B3
  { CRC_EDITEXITMSG            , KK_EditExitMsg          , KT_E }, // 0x1813
  { CRC_READTOGGLEPAGEBAR      , KK_ReadTogglePageBar    , KT_R }, // 0x18E4
  { CRC_K_F12                  , Key_F12                 ,    0 }, // 0x1930
  { CRC_K_F10                  , Key_F10                 ,    0 }, // 0x1932
  { CRC_K_F11                  , Key_F11                 ,    0 }, // 0x1933
  { CRC_READHIDEKLUDGE         , KK_ReadHideKludge       , KT_R }, // 0x19EC
  { CRC_READGOTOREPLIES        , KK_ReadGotoReplies      , KT_R }, // 0x1A6B
  { CRC_READMOVEQUOTEMSG       , KK_ReadMoveQuoteMsg     , KT_R }, // 0x1B41
  { CRC_EDITGOEOL              , KK_EditGoEOL            , KT_E }, // 0x1CC6
  { CRC_K_AUP                  , Key_A_Up                ,    0 }, // 0x1D94
  { CRC_AREAABORT              , KK_AreaAbort            , KT_A }, // 0x1E02
  { CRC_READTOGGLEMARK         , KK_ReadToggleMark       , KT_R }, // 0x1E64
  { CRC_READFIDORENUMBER       , KK_ReadFidoRenumber     , KT_R }, // 0x1E7C
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SDEL                 , Key_S_Del               ,    0 }, // 0x1EB9
#endif
  { CRC_EDITCOPY               , KK_EditCopy             , KT_E }, // 0x1F77
  { CRC_READHIDEQUOTE          , KK_ReadHideQuote        , KT_R }, // 0x1FA8
  { CRC_READEXTERNUTILMENU     , KK_ReadExternUtilMenu   , KT_R }, // 0x2000
  { CRC_READCHANGEAKA          , KK_ReadChangeAka        , KT_R }, // 0x2015
  { CRC_EDITGOWORDRIGHT        , KK_EditGoWordRight      , KT_E }, // 0x20D0
  { CRC_K_CRIGHT               , Key_C_Rgt               ,    0 }, // 0x20EC
  { CRC_EDITBLOCKRIGHT         , KK_EditBlockRight       , KT_E }, // 0x20F9
  { CRC_AREAJUMP               , KK_AreaJump             , KT_A }, // 0x2342
  { CRC_HEADERTOGGLEHUBHOST    , KK_HeaderToggleHubhost  , KT_H }, // 0x2505
  { CRC_K_CPGUP                , Key_C_PgUp              ,    0 }, // 0x2603
  { CRC_READGOTOPREVUNREAD     , KK_ReadGotoPrevUnread   , KT_R }, // 0x27EA
  { CRC_READTOGGLEREALMSGNO    , KK_ReadToggleRealMsgno  , KT_R }, // 0x2A3B
  { CRC_ADDRESSBOOKPACK        , KK_AddressbookPack      , KT_B }, // 0x2C5C
  { CRC_HEADERTOGGLERETRECREQ  , KK_HeaderToggleRetrecreq, KT_H }, // 0x2CF1
  { CRC_HEADERTOGGLECRASH      , KK_HeaderToggleCrash    , KT_H }, // 0x2CF9
  { CRC_ADDRESSBOOKSELECT      , KK_AddressbookSelect    , KT_B }, // 0x2D4A
  { CRC_AREADOSSHELL           , KK_AreaDosShell         , KT_A }, // 0x2DED
  { CRC_K_DOWN                 , Key_Dwn                 ,    0 }, // 0x2F6D
  { CRC_READNEWMSG             , KK_ReadNewMsg           , KT_R }, // 0x30CA
  { CRC_HEADERTOGGLELOCKED     , KK_HeaderToggleLocked   , KT_H }, // 0x312D
  { CRC_NODEGOTOFIRST          , KK_NodeGotoFirst        , KT_N }, // 0x321B
  { CRC_READUNDEFINE           , KK_ReadUndefine         , KT_R }, // 0x329D
  { CRC_K_ADOWN                , Key_A_Dwn               ,    0 }, // 0x32C0
  { CRC_K_CF12                 , Key_C_F12               ,    0 }, // 0x3480
  { CRC_K_CF10                 , Key_C_F10               ,    0 }, // 0x3482
  { CRC_K_CF11                 , Key_C_F11               ,    0 }, // 0x3483
  { CRC_READTWITSSKIP          , KK_ReadTwitsSkip        , KT_R }, // 0x35A1
  { CRC_K_CHOME                , Key_C_Home              ,    0 }, // 0x35C4
  { CRC_HEADERTOGGLERECEIVED   , KK_HeaderToggleReceived , KT_H }, // 0x36B9
  { CRC_K_CPGDN                , Key_C_PgDn              ,    0 }, // 0x371D
  { CRC_EDITCOPYABOVECHAR      , KK_EditCopyAboveChar    , KT_E }, // 0x380C
  { CRC_LISTSELECT             , KK_ListSelect           , KT_M }, // 0x3829
  { CRC_READGOTOFIRSTMSG       , KK_ReadGotoFirstMsg     , KT_R }, // 0x392F
  { CRC_READSTYLESSTRIP        , KK_ReadStylesStrip      , KT_R }, // 0x39E8
  { CRC_AREAGOTONEXT           , KK_AreaGotoNext         , KT_A }, // 0x3A9C
  { CRC_EDITDELLTWORD          , KK_EditDelLtWord        , KT_E }, // 0x3B67
  { CRC_LISTGOTOLAST           , KK_ListGotoLast         , KT_M }, // 0x3BA7
  { CRC_READSHOWQUOTE          , KK_ReadShowQuote        , KT_R }, // 0x3C6C
  { CRC_LISTTOGGLEWIDESUBJ     , KK_ListToggleWideSubj   , KT_M }, // 0x3C76
  { CRC_EDITGODOWN             , KK_EditGoDown           , KT_E }, // 0x3DC2
  { CRC_K_A0                   , Key_A_0                 ,    0 }, // 0x4030
  { CRC_K_A1                   , Key_A_1                 ,    0 }, // 0x4031
  { CRC_K_A2                   , Key_A_2                 ,    0 }, // 0x4032
  { CRC_K_A3                   , Key_A_3                 ,    0 }, // 0x4033
  { CRC_K_A4                   , Key_A_4                 ,    0 }, // 0x4034
  { CRC_K_A5                   , Key_A_5                 ,    0 }, // 0x4035
  { CRC_K_A6                   , Key_A_6                 ,    0 }, // 0x4036
  { CRC_K_A7                   , Key_A_7                 ,    0 }, // 0x4037
  { CRC_K_A8                   , Key_A_8                 ,    0 }, // 0x4038
  { CRC_K_A9                   , Key_A_9                 ,    0 }, // 0x4039
  { CRC_K_AA                   , Key_A_A                 ,    0 }, // 0x4041
  { CRC_K_AB                   , Key_A_B                 ,    0 }, // 0x4042
  { CRC_K_AC                   , Key_A_C                 ,    0 }, // 0x4043
  { CRC_K_AD                   , Key_A_D                 ,    0 }, // 0x4044
  { CRC_K_AE                   , Key_A_E                 ,    0 }, // 0x4045
  { CRC_K_AF                   , Key_A_F                 ,    0 }, // 0x4046
  { CRC_K_AG                   , Key_A_G                 ,    0 }, // 0x4047
  { CRC_K_AH                   , Key_A_H                 ,    0 }, // 0x4048
  { CRC_K_AI                   , Key_A_I                 ,    0 }, // 0x4049
  { CRC_K_AJ                   , Key_A_J                 ,    0 }, // 0x404A
  { CRC_K_AK                   , Key_A_K                 ,    0 }, // 0x404B
  { CRC_K_AL                   , Key_A_L                 ,    0 }, // 0x404C
  { CRC_K_AM                   , Key_A_M                 ,    0 }, // 0x404D
  { CRC_K_AN                   , Key_A_N                 ,    0 }, // 0x404E
  { CRC_K_AO                   , Key_A_O                 ,    0 }, // 0x404F
  { CRC_K_AP                   , Key_A_P                 ,    0 }, // 0x4050
  { CRC_K_AQ                   , Key_A_Q                 ,    0 }, // 0x4051
  { CRC_K_AR                   , Key_A_R                 ,    0 }, // 0x4052
  { CRC_K_AS                   , Key_A_S                 ,    0 }, // 0x4053
  { CRC_K_AT                   , Key_A_T                 ,    0 }, // 0x4054
  { CRC_K_AU                   , Key_A_U                 ,    0 }, // 0x4055
  { CRC_K_AV                   , Key_A_V                 ,    0 }, // 0x4056
  { CRC_K_AW                   , Key_A_W                 ,    0 }, // 0x4057
  { CRC_K_AX                   , Key_A_X                 ,    0 }, // 0x4058
  { CRC_K_AY                   , Key_A_Y                 ,    0 }, // 0x4059
  { CRC_K_AZ                   , Key_A_Z                 ,    0 }, // 0x405A
  { CRC_K_ADEL                 , Key_A_Del               ,    0 }, // 0x40C0
  { CRC_NODEMACRO              , KK_NodeMacro            , KT_N }, // 0x4129
  { CRC_READGOTOTHNEXTUNREAD   , KK_ReadGotoThNextUnread , KT_R }, // 0x413E
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SUP                  , Key_S_Up                ,    0 }, // 0x4151
#endif
  { CRC_AREAGOTOFIRST          , KK_AreaGotoFirst        , KT_A }, // 0x41CB
  { CRC_LISTABORT              , KK_ListAbort            , KT_M }, // 0x4313
  { CRC_READGOTOBOOKMARK       , KK_ReadGotoBookMark     , KT_R }, // 0x4383
  { CRC_READMSGCONTINUE        , KK_ReadMsgContinue      , KT_R }, // 0x4511
  { CRC_K_F1                   , Key_F1                  ,    0 }, // 0x4631
  { CRC_K_F2                   , Key_F2                  ,    0 }, // 0x4632
  { CRC_K_F3                   , Key_F3                  ,    0 }, // 0x4633
  { CRC_K_F4                   , Key_F4                  ,    0 }, // 0x4634
  { CRC_K_F5                   , Key_F5                  ,    0 }, // 0x4635
  { CRC_K_F6                   , Key_F6                  ,    0 }, // 0x4636
  { CRC_K_F7                   , Key_F7                  ,    0 }, // 0x4637
  { CRC_K_F8                   , Key_F8                  ,    0 }, // 0x4638
  { CRC_K_F9                   , Key_F9                  ,    0 }, // 0x4639
  { CRC_READTOGGLEKLUDGE       , KK_ReadToggleKludge     , KT_R }, // 0x46AB
  { CRC_HEADERTOGGLELOCAL      , KK_HeaderToggleLocal    , KT_H }, // 0x47A1
  { CRC_ADDRESSBOOKQUIT        , KK_AddressbookQuit      , KT_B }, // 0x47C7
  { CRC_READCHANGEATTRS        , KK_ReadChangeAttrs      , KT_R }, // 0x47F8
  { CRC_EDITSCODEITALIC        , KK_EditSCodeItalic      , KT_E }, // 0x487F
  { CRC_READSEARCH             , KK_ReadSearch           , KT_R }, // 0x48EF
  { CRC_READMSGPGDN            , KK_ReadMsgPgDn          , KT_R }, // 0x4908
  { CRC_READSTRIPHTML          , KK_ReadStripHTML        , KT_R }, // 0x499A
  { CRC_LISTGOTOPREV           , KK_ListGotoPrev         , KT_M }, // 0x49E9
  { CRC_READQUOTEBUF           , KK_ReadQuoteBuf         , KT_R }, // 0x49EE
  { CRC_EDITDELETE             , KK_EditDelete           , KT_E }, // 0x4A37
  { CRC_K_SF12                 , Key_S_F12               ,    0 }, // 0x4A85
  { CRC_K_SF11                 , Key_S_F11               ,    0 }, // 0x4A86
  { CRC_K_SF10                 , Key_S_F10               ,    0 }, // 0x4A87
  { CRC_READMAKEPATHREPORT     , KK_ReadMakePathreport   , KT_R }, // 0x4AA6
  { CRC_K_ESC                  , Key_Esc                 ,    0 }, // 0x4B22
  { CRC_READMSGHOME            , KK_ReadMsgHome          , KT_R }, // 0x4BD1
  { CRC_KK_MACRO               , Key_Macro               ,    0 }, // 0x4BE9
  { CRC_READTOUCHNETSCAN       , KK_ReadTouchNetscan     , KT_R }, // 0x4BF3
  { CRC_EDITUNDELETE           , KK_EditUnDelete         , KT_E }, // 0x4C34
  { CRC_K_DEL                  , Key_Del                 ,    0 }, // 0x4D0C
  { CRC_READUUDECODE           , KK_ReadUUDecode         , KT_R }, // 0x4F0B
  { CRC_FILEUNDEFINE           , KK_FileUndefine         , KT_F }, // 0x502B
  { CRC_K_CENTER               , Key_C_Ent               ,    0 }, // 0x50DC
  { CRC_AREAZAP                , KK_AreaZap              , KT_A }, // 0x5125
  { CRC_K_SF1                  , Key_S_F1                ,    0 }, // 0x5230
  { CRC_K_SF3                  , Key_S_F3                ,    0 }, // 0x5232
  { CRC_K_SF2                  , Key_S_F2                ,    0 }, // 0x5233
  { CRC_K_SF5                  , Key_S_F5                ,    0 }, // 0x5234
  { CRC_K_SF4                  , Key_S_F4                ,    0 }, // 0x5235
  { CRC_K_SF7                  , Key_S_F7                ,    0 }, // 0x5236
  { CRC_K_SF6                  , Key_S_F6                ,    0 }, // 0x5237
  { CRC_K_SF9                  , Key_S_F9                ,    0 }, // 0x5238
  { CRC_K_SF8                  , Key_S_F8                ,    0 }, // 0x5239
  { CRC_K_CLEFT                , Key_C_Lft               ,    0 }, // 0x535B
  { CRC_K_UP                   , Key_Up                  ,    0 }, // 0x5550
  { CRC_K_END                  , Key_End                 ,    0 }, // 0x5625
  { CRC_HEADERTOGGLERETREC     , KK_HeaderToggleRetrec   , KT_H }, // 0x56EA
  { CRC_K_ATAB                 , Key_A_Tab               ,    0 }, // 0x56FF
  { CRC_READMSGPGUP            , KK_ReadMsgPgUp          , KT_R }, // 0x5816
  { CRC_AREATOUCHNETSCAN       , KK_AreaTouchNetscan     , KT_A }, // 0x58C1
  { CRC_EDITNEWLINE            , KK_EditNewline          , KT_E }, // 0x59F3
  { CRC_K_CBSPACE              , Key_C_BS                ,    0 }, // 0x5A72
  { CRC_EDITEXPORTTEXT         , KK_EditExportText       , KT_E }, // 0x5AED
  { CRC_EDITLOOKUPORIG         , KK_EditLookupOrig       , KT_E }, // 0x5AFB
  { CRC_AREAHEAT               , KK_AreaHeat             , KT_A }, // 0x5B15
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SRIGHT               , Key_S_Rgt               ,    0 }, // 0x5B1F
#endif
  { CRC_READGOTONEXTAREA       , KK_ReadGotoNextArea     , KT_R }, // 0x5B23
  { CRC_K_TAB                  , Key_Tab                 ,    0 }, // 0x5B33
  { CRC_READHIDEHIDDEN         , KK_ReadHideHidden       , KT_R }, // 0x5B89
  { CRC_K_AEND                 , Key_A_End               ,    0 }, // 0x5BE9
  { CRC_FILEMACRO              , KK_FileMacro            , KT_F }, // 0x5D48
  { CRC_READMSGLINEDOWN        , KK_ReadMsgLineDown      , KT_R }, // 0x5DB2
  { CRC_K_CMINUS               , Key_C_Min               ,    0 }, // 0x5E2D
  { CRC_NODEUNDEFINE           , KK_NodeUndefine         , KT_N }, // 0x5E32
  { CRC_K_CA                   , Key_C_A                 ,    0 }, // 0x5E41
  { CRC_K_CB                   , Key_C_B                 ,    0 }, // 0x5E42
  { CRC_K_CC                   , Key_C_C                 ,    0 }, // 0x5E43
  { CRC_K_CD                   , Key_C_D                 ,    0 }, // 0x5E44
  { CRC_K_CE                   , Key_C_E                 ,    0 }, // 0x5E45
  { CRC_K_CF                   , Key_C_F                 ,    0 }, // 0x5E46
  { CRC_K_CG                   , Key_C_G                 ,    0 }, // 0x5E47
  { CRC_K_CH                   , Key_C_H                 ,    0 }, // 0x5E48
  { CRC_K_CI                   , Key_C_I                 ,    0 }, // 0x5E49
  { CRC_K_CJ                   , Key_C_J                 ,    0 }, // 0x5E4A
  { CRC_K_CK                   , Key_C_K                 ,    0 }, // 0x5E4B
  { CRC_K_CL                   , Key_C_L                 ,    0 }, // 0x5E4C
  { CRC_K_CM                   , Key_C_M                 ,    0 }, // 0x5E4D
  { CRC_K_CN                   , Key_C_N                 ,    0 }, // 0x5E4E
  { CRC_K_CO                   , Key_C_O                 ,    0 }, // 0x5E4F
  { CRC_K_CP                   , Key_C_P                 ,    0 }, // 0x5E50
  { CRC_K_CQ                   , Key_C_Q                 ,    0 }, // 0x5E51
  { CRC_K_CR                   , Key_C_R                 ,    0 }, // 0x5E52
  { CRC_K_CS                   , Key_C_S                 ,    0 }, // 0x5E53
  { CRC_K_CT                   , Key_C_T                 ,    0 }, // 0x5E54
  { CRC_K_CU                   , Key_C_U                 ,    0 }, // 0x5E55
  { CRC_K_CV                   , Key_C_V                 ,    0 }, // 0x5E56
  { CRC_K_CW                   , Key_C_W                 ,    0 }, // 0x5E57
  { CRC_K_CX                   , Key_C_X                 ,    0 }, // 0x5E58
  { CRC_K_CY                   , Key_C_Y                 ,    0 }, // 0x5E59
  { CRC_K_CZ                   , Key_C_Z                 ,    0 }, // 0x5E5A
  { CRC_READCOMMENTMSG         , KK_ReadCommentMsg       , KT_R }, // 0x5E89
  { CRC_READTOGGLETWITS        , KK_ReadToggleTwits      , KT_R }, // 0x5FD1
  { CRC_LISTWIDESUBJ           , KK_ListWideSubj         , KT_M }, // 0x6026
  { CRC_EDITGOPGDN             , KK_EditGoPgDn           , KT_E }, // 0x607D
  { CRC_K_CDEL                 , Key_C_Del               ,    0 }, // 0x60BC
  { CRC_KK_AUTO                , Key_Auto                ,    0 }, // 0x60E2
  { CRC_READMARK               , KK_ReadMark             , KT_R }, // 0x6190
  { CRC_READFINDHEADER         , KK_ReadFindHeader       , KT_R }, // 0x6343
  { CRC_EDITDELETESOL          , KK_EditDeleteSOL        , KT_E }, // 0x63B4
  { CRC_K_PGUP                 , Key_PgUp                ,    0 }, // 0x63CC
  { CRC_HEADERTOGGLEFREQ       , KK_HeaderToggleFreq     , KT_H }, // 0x63D9
  { CRC_EDITABORT              , KK_EditAbort            , KT_E }, // 0x6468
  { CRC_EDITSAVEFILE           , KK_EditSaveFile         , KT_E }, // 0x64A4
  { CRC_HEADERLOOKUP           , KK_HeaderLookup         , KT_H }, // 0x64F2
  { CRC_AREAGOTOPREV           , KK_AreaGotoPrev         , KT_A }, // 0x6534
  { CRC_EDITDELLEFT            , KK_EditDelLeft          , KT_E }, // 0x6552
  { CRC_EDITSCODEBOLD          , KK_EditSCodeBold        , KT_E }, // 0x657D
  { CRC_EDITSAVEMSG            , KK_EditSaveMsg          , KT_E }, // 0x660C
  { CRC_K_CGMUL                , Key_C_StrG              ,    0 }, // 0x66CA
  { CRC_NODESELECT             , KK_NodeSelect           , KT_N }, // 0x6A64
  { CRC_K_CDOWN                , Key_C_Dwn               ,    0 }, // 0x6AA2
  { CRC_EDITGORIGHT            , KK_EditGoRight          , KT_E }, // 0x6AFE
  { CRC_READTOUCHSEMAPHORE     , KK_ReadTouchSemaphore   , KT_R }, // 0x6B1D
  { CRC_EDITGOTOPMSG           , KK_EditGoTopMsg         , KT_E }, // 0x6BD3
  { CRC_READCHANGETEMPLATE     , KK_ReadChangeTemplate   , KT_R }, // 0x6C86
  { CRC_FILEUNMARK             , KK_FileUnMark           , KT_F }, // 0x6D1B
  { CRC_EDITTOLOWER            , KK_EditToLower          , KT_E }, // 0x6D85
  { CRC_K_AHOME                , Key_A_Home              ,    0 }, // 0x6DA6
  { CRC_HEADERTOGGLEIMM        , KK_HeaderToggleImm      , KT_H }, // 0x6E15
  { CRC_LISTTOGGLEDATE         , KK_ListToggleDate       , KT_M }, // 0x6F1F
  { CRC_K_APGDN                , Key_A_PgDn              ,    0 }, // 0x6F7F
  { CRC_HEADERTOGGLEATTRWIN    , KK_HeaderToggleAttrWin  , KT_H }, // 0x6F95
  { CRC_K_HOME                 , Key_Home                ,    0 }, // 0x700B
  { CRC_ADDRESSBOOKDELETE      , KK_AddressbookDelete    , KT_B }, // 0x70D1
  { CRC_EDITGOPGUP             , KK_EditGoPgUp           , KT_E }, // 0x7163
  { CRC_READTWITSIGNORE        , KK_ReadTwitsIgnore      , KT_R }, // 0x71CA
  { CRC_READMSGLINEUP          , KK_ReadMsgLineUp        , KT_R }, // 0x7238
  { CRC_EDITHEADER             , KK_EditHeader           , KT_E }, // 0x726F
  { CRC_K_PGDN                 , Key_PgDn                ,    0 }, // 0x72D2
  { CRC_READWRITEMSG           , KK_ReadWriteMsg         , KT_R }, // 0x73AC
  { CRC_READGOTOMSGNO          , KK_ReadGotoMsgno        , KT_R }, // 0x73F7
  { CRC_READCHANGEUSERNAME     , KK_ReadChangeUsername   , KT_R }, // 0x7438
  { CRC_HEADERADDRESSBOOK      , KK_HeaderAddressbook    , KT_H }, // 0x76FC
  { CRC_EDITUNDEFINE           , KK_EditUndefine         , KT_E }, // 0x7873
  { CRC_LISTGOTOFIRST          , KK_ListGotoFirst        , KT_M }, // 0x7925
  { CRC_READSTYLESNONE         , KK_ReadStylesNone       , KT_R }, // 0x79D7
  { CRC_K_CEND                 , Key_C_End               ,    0 }, // 0x7B95
  { CRC_READSTYLESSHOW         , KK_ReadStylesShow       , KT_R }, // 0x7D0D
  { CRC_K_APGUP                , Key_A_PgUp              ,    0 }, // 0x7E61
  { CRC_READFINDNEXT           , KK_ReadFindNext         , KT_R }, // 0x8082
  { CRC_EDITQUITNOW            , KK_EditQuitNow          , KT_E }, // 0x80BA
  { CRC_LISTMARKINGOPTIONS     , KK_ListMarkingOptions   , KT_M }, // 0x8138
  { CRC_EDITZAPQUOTEBELOW      , KK_EditZapQuoteBelow    , KT_E }, // 0x814F
  { CRC_EDITDUPLINE            , KK_EditDupLine          , KT_E }, // 0x816A
  { CRC_READSHOWHIDDKLUD       , KK_ReadShowHiddKlud     , KT_R }, // 0x81EB
  { CRC_EDITDELRTWORD          , KK_EditDelRtWord        , KT_E }, // 0x829A
  { CRC_AREAUNDEFINE           , KK_AreaUndefine         , KT_A }, // 0x835B
  { CRC_READGOTOREPLYNEXT      , KK_ReadGotoReplyNext    , KT_R }, // 0x844F
  { CRC_READHIDEHIDDKLUD       , KK_ReadHideHiddKlud     , KT_R }, // 0x84ED
  { CRC_EDITMACRO              , KK_EditMacro            , KT_E }, // 0x85CD
  { CRC_READSHOWDEL            , KK_ReadShowDel          , KT_R }, // 0x8615
  { CRC_K_ENTER                , Key_Ent                 ,    0 }, // 0x87BD
  { CRC_EDITGOTOPLINE          , KK_EditGoTopLine        , KT_E }, // 0x87C2
#if defined(__GOLD_SPELL__)
  { CRC_EDITSCHECKERMENU       , KK_EditSCheckerMenu     , KT_E }, // 0x881F
#endif
  { CRC_HEADERTOGGLEARCSENT    , KK_HeaderToggleArcsent  , KT_H }, // 0x88A7
  { CRC_EDITBLOCKPGDN          , KK_EditBlockPgDn        , KT_E }, // 0x895B
  { CRC_EDITASKEXIT            , KK_EditAskExit          , KT_E }, // 0x89AF
  { CRC_HEADERTOGGLEHOLD       , KK_HeaderToggleHold     , KT_H }, // 0x8A5F
  { CRC_EDITBLOCKHOME          , KK_EditBlockHome        , KT_E }, // 0x8B82
  { CRC_READSHOWHIDDEN         , KK_ReadShowHidden       , KT_R }, // 0x8B88
  { CRC_READDELETEMSG          , KK_ReadDeleteMsg        , KT_R }, // 0x8BCE
  { CRC_EDITTAB                , KK_EditTab              , KT_E }, // 0x8C26
  { CRC_LISTUNMARK             , KK_ListUnmark           , KT_M }, // 0x8DEB
  { CRC_K_KEY5                 , Key_5Num                ,    0 }, // 0x9062
  { CRC_READSEQUENTMSGNO       , KK_ReadSequentMsgno     , KT_R }, // 0x9085
  { CRC_EDITPASTE              , KK_EditPaste            , KT_E }, // 0x90C4
  { CRC_HEADERTOGGLEAUDIT      , KK_HeaderToggleAudit    , KT_H }, // 0x91B7
  { CRC_EDITGOBOTLINE          , KK_EditGoBotLine        , KT_E }, // 0x92DD
  { CRC_EXTERNUTIL18           , KK_ExternUtil18         , KT_R }, // 0x94B0
  { CRC_EXTERNUTIL19           , KK_ExternUtil19         , KT_R }, // 0x94B1
  { CRC_READMAINMENU           , KK_ReadMainMenu         , KT_R }, // 0x94B3
  { CRC_EXTERNUTIL10           , KK_ExternUtil10         , KT_R }, // 0x94B8
  { CRC_EXTERNUTIL11           , KK_ExternUtil11         , KT_R }, // 0x94B9
  { CRC_EXTERNUTIL12           , KK_ExternUtil12         , KT_R }, // 0x94BA
  { CRC_EXTERNUTIL13           , KK_ExternUtil13         , KT_R }, // 0x94BB
  { CRC_EXTERNUTIL14           , KK_ExternUtil14         , KT_R }, // 0x94BC
  { CRC_EXTERNUTIL15           , KK_ExternUtil15         , KT_R }, // 0x94BD
  { CRC_EXTERNUTIL16           , KK_ExternUtil16         , KT_R }, // 0x94BE
  { CRC_EXTERNUTIL17           , KK_ExternUtil17         , KT_R }, // 0x94BF
  { CRC_EXTERNUTIL08           , KK_ExternUtil08         , KT_R }, // 0x95B0
  { CRC_EXTERNUTIL09           , KK_ExternUtil09         , KT_R }, // 0x95B1
  { CRC_EXTERNUTIL01           , KK_ExternUtil01         , KT_R }, // 0x95B9
  { CRC_EXTERNUTIL02           , KK_ExternUtil02         , KT_R }, // 0x95BA
  { CRC_EXTERNUTIL03           , KK_ExternUtil03         , KT_R }, // 0x95BB
  { CRC_EXTERNUTIL04           , KK_ExternUtil04         , KT_R }, // 0x95BC
  { CRC_EXTERNUTIL05           , KK_ExternUtil05         , KT_R }, // 0x95BD
  { CRC_EXTERNUTIL06           , KK_ExternUtil06         , KT_R }, // 0x95BE
  { CRC_EXTERNUTIL07           , KK_ExternUtil07         , KT_R }, // 0x95BF
  { CRC_HEADERTOGGLEXMAIL      , KK_HeaderToggleXmail    , KT_H }, // 0x9622
  { CRC_AREABOARDNOS           , KK_AreaBoardnos         , KT_A }, // 0x96EA
  { CRC_EXTERNUTIL20           , KK_ExternUtil20         , KT_R }, // 0x97B8
  { CRC_EXTERNUTIL21           , KK_ExternUtil21         , KT_R }, // 0x97B9
  { CRC_EXTERNUTIL22           , KK_ExternUtil22         , KT_R }, // 0x97BA
  { CRC_EXTERNUTIL23           , KK_ExternUtil23         , KT_R }, // 0x97BB
  { CRC_EXTERNUTIL24           , KK_ExternUtil24         , KT_R }, // 0x97BC
  { CRC_K_INS                  , Key_Ins                 ,    0 }, // 0x97BE
  { CRC_READTHREADTREE         , KK_ReadThreadtree       , KT_R }, // 0x9827
  { CRC_EDITBLOCKPGUP          , KK_EditBlockPgUp        , KT_E }, // 0x9842
  { CRC_K_ARIGHT               , Key_A_Rgt               ,    0 }, // 0x9911
  { CRC_READTWITSKILL          , KK_ReadTwitsKill        , KT_R }, // 0x9A25
  { CRC_K_AINS                 , Key_A_Ins               ,    0 }, // 0x9A72
  { CRC_EDITGOBOTMSG           , KK_EditGoBotMsg         , KT_E }, // 0x9B29
  { CRC_READDOSSHELL           , KK_ReadDosShell         , KT_R }, // 0x9C2B
  { CRC_READGOTOLASTMSG        , KK_ReadGotoLastMsg      , KT_R }, // 0x9E72
  { CRC_NODEABORT              , KK_NodeAbort            , KT_N }, // 0xA08C
  { CRC_READTOGGLEHIDDKLUD     , KK_ReadToggleHiddKlud   , KT_R }, // 0xA24F
  { CRC_AREASELECT             , KK_AreaSelect           , KT_A }, // 0xA271
  { CRC_READSHOWHTML           , KK_ReadShowHTML         , KT_R }, // 0xA27F
  { CRC_LISTMACRO              , KK_ListMacro            , KT_M }, // 0xA2B6
  { CRC_EDITDELCHAR            , KK_EditDelChar          , KT_E }, // 0xA3C7
  { CRC_AREAASKEXIT            , KK_AreaAskExit          , KT_A }, // 0xA3D1
  { CRC_READLOOKUPORIG         , KK_ReadLookupOrig       , KT_R }, // 0xA41C
  { CRC_READASKEXIT            , KK_ReadAskExit          , KT_R }, // 0xA500
  { CRC_AREAJUMPNEXTMATCH      , KK_AreaJumpNextMatch    , KT_A }, // 0xA556
  { CRC_HEADERTOGGLETRUNC      , KK_HeaderToggleTrunc    , KT_H }, // 0xA5B4
  { CRC_READTOGGLEBOOKMARK     , KK_ReadToggleBookMark   , KT_R }, // 0xA7E2
  { CRC_EDITSOUNDKILL          , KK_EditSoundkill        , KT_E }, // 0xA9A7
  { CRC_AREAQUITNOW            , KK_AreaQuitNow          , KT_A }, // 0xAAC4
  { CRC_READNEWAREA            , KK_ReadNewArea          , KT_R }, // 0xAB5E
  { CRC_READQUITNOW            , KK_ReadQuitNow          , KT_R }, // 0xAC15
  { CRC_HEADERTOGGLEUPDREQ     , KK_HeaderToggleUpdreq   , KT_H }, // 0xAC35
  { CRC_EDITDELLINE            , KK_EditDelLine          , KT_E }, // 0xACCF
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SPGDN                , Key_S_PgDn              ,    0 }, // 0xAD44
#endif
  { CRC_READSOUNDKILL          , KK_ReadSoundkill        , KT_R }, // 0xAE29
  { CRC_LISTUNDEFINE           , KK_ListUndefine         , KT_M }, // 0xAF86
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SHOME                , Key_S_Home              ,    0 }, // 0xAF9D
#endif
  { CRC_EDITSCODEUNDERLINE     , KK_EditSCodeUnderline   , KT_E }, // 0xB1D9
  { CRC_READTOGGLEQUOTE        , KK_ReadToggleQuote      , KT_R }, // 0xB293
  { CRC_EDITUNDO               , KK_EditUndo             , KT_E }, // 0xB295
  { CRC_READDIRQUOTEMSG        , KK_ReadDirQuoteMsg      , KT_R }, // 0xB351
  { CRC_FILETOGGLEMARK         , KK_FileToggleMark       , KT_F }, // 0xB431
  { CRC_READTWITSBLANK         , KK_ReadTwitsBlank       , KT_R }, // 0xB571
  { CRC_FILEGOTOPREV           , KK_FileGotoPrev         , KT_F }, // 0xB644
  { CRC_EDITLOOKUPCURSOR       , KK_EditLookupCursor     , KT_E }, // 0xB7CC
  { CRC_NODEGOTOPREV           , KK_NodeGotoPrev         , KT_N }, // 0xB85D
  { CRC_AREADROPMSGMARKS       , KK_AreaDropMsgMarks     , KT_A }, // 0xB90C
  { CRC_K_CINS                 , Key_C_Ins               ,    0 }, // 0xBA0E
  { CRC_READGOTONEXTUNREAD     , KK_ReadGotoNextUnread   , KT_R }, // 0xBA34
  { CRC_READADDRESSBOOK        , KK_ReadAddressbook      , KT_R }, // 0xBAC1
  { CRC_READTOGGLEHTML         , KK_ReadToggleHTML       , KT_R }, // 0xBC02
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SPGUP                , Key_S_PgUp              ,    0 }, // 0xBC5A
#endif
  { CRC_FILEABORT              , KK_FileAbort            , KT_F }, // 0xBCED
  { CRC_READCHANGETAGLINE      , KK_ReadChangeTagline    , KT_R }, // 0xBDD2
  { CRC_READQUOTEMSG           , KK_ReadQuoteMsg         , KT_R }, // 0xBE00
  { CRC_HEADERCLEARATTRIB      , KK_HeaderClearAttrib    , KT_H }, // 0xBED5
  { CRC_READCOPYMOVEFORWARD    , KK_ReadCopyMoveForward  , KT_R }, // 0xBFAF
  { CRC_HEADERTOGGLEKILL       , KK_HeaderToggleKill     , KT_H }, // 0xBFC2
  { CRC_HEADERTOGGLETRANSIT    , KK_HeaderToggleTransit  , KT_H }, // 0xC112
  { CRC_HEADERTOGGLECFMRECREQ  , KK_HeaderToggleCfmrecreq, KT_H }, // 0xC12D
  { CRC_READTOGGLEROT13        , KK_ReadToggleROT13      , KT_R }, // 0xC157
  { CRC_EDITTOGGLECASE         , KK_EditToggleCase       , KT_E }, // 0xC282
  { CRC_FILEGOTOLAST           , KK_FileGotoLast         , KT_F }, // 0xC40A
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SINS                 , Key_S_Ins               ,    0 }, // 0xC40B
#endif
  { CRC_AREASCANPM             , KK_AreaScanPM           , KT_A }, // 0xC4FF
  { CRC_LISTMARK               , KK_ListMark             , KT_M }, // 0xC530
  { CRC_READMSGEND             , KK_ReadMsgEnd           , KT_R }, // 0xC5AD
  { CRC_AREATOGGLE             , KK_AreaToggle           , KT_A }, // 0xC734
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SLEFT                , Key_S_Lft               ,    0 }, // 0xC902
#endif
  { CRC_HEADERTOGGLEORPHAN     , KK_HeaderToggleOrphan   , KT_H }, // 0xC98A
  { CRC_HEADERTOGGLEFILE       , KK_HeaderToggleFile     , KT_H }, // 0xC997
  { CRC_READSHOWKLUDGE         , KK_ReadShowKludge       , KT_R }, // 0xC9ED
  { CRC_NODEGOTOLAST           , KK_NodeGotoLast         , KT_N }, // 0xCA13
  { CRC_FILEGOTOFIRST          , KK_FileGotoFirst        , KT_F }, // 0xCAD5
  { CRC_HEADERTOGGLEDELSENT    , KK_HeaderToggleDelsent  , KT_H }, // 0xCD4B
  { CRC_LISTTOGGLEBOOKMARK     , KK_ListToggleBookMark   , KT_M }, // 0xCFA0
  { CRC_READTOGGLEMARKREAD     , KK_ReadToggleMarkRead   , KT_R }, // 0xCFAE
  { CRC_LISTQUITNOW            , KK_ListQuitNow          , KT_M }, // 0xD037
  { CRC_FILEMARKALL            , KK_FileMarkAll          , KT_F }, // 0xD115
  { CRC_HEADERTOGGLEDIRECT     , KK_HeaderToggleDirect   , KT_H }, // 0xD1D2
  { CRC_AREAWRITEGOLDLAST      , KK_AreaWriteGoldlast    , KT_A }, // 0xD353
  { CRC_LISTTOGGLEMARK         , KK_ListToggleMark       , KT_M }, // 0xD389
  { CRC_EDITTOUPPER            , KK_EditToUpper          , KT_E }, // 0xD3DD
  { CRC_FILEASKEXIT            , KK_FileAskExit          , KT_F }, // 0xD40C
  { CRC_EDITBLOCKDOWN          , KK_EditBlockDown        , KT_E }, // 0xD4E4
  { CRC_READMESSAGELIST        , KK_ReadMessageList      , KT_R }, // 0xD5D8
  { CRC_EDITGOUP               , KK_EditGoUp             , KT_E }, // 0xD6BA
  { CRC_EDITDOSSHELL           , KK_EditDosShell         , KT_E }, // 0xD6C5
  { CRC_EDITTOGGLEINSERT       , KK_EditToggleInsert     , KT_E }, // 0xD732
  { CRC_FILESELECT             , KK_FileSelect           , KT_F }, // 0xD8D9
  { CRC_READDECREASEMARGIN     , KK_ReadDecreaseMargin   , KT_R }, // 0xD8EA
  { CRC_LISTASKEXIT            , KK_ListAskExit          , KT_M }, // 0xD922
  { CRC_AREACATCHUP            , KK_AreaCatchUp          , KT_A }, // 0xDA5C
  { CRC_READMOVEDIRQUOTEMSG    , KK_ReadMoveDirQuoteMsg  , KT_R }, // 0xDBC0
  { CRC_READGOTOREPLYPREV      , KK_ReadGotoReplyPrev    , KT_R }, // 0xDBE7
  { CRC_EDITANCHOR             , KK_EditAnchor           , KT_E }, // 0xDC07
  { CRC_FILEQUITNOW            , KK_FileQuitNow          , KT_F }, // 0xDD19
  { CRC_AREASOUNDKILL          , KK_AreaSoundkill        , KT_A }, // 0xDFD3
  { CRC_EDITBLOCKUP            , KK_EditBlockUp          , KT_E }, // 0xE004
  { CRC_EDITGOBEGLINE          , KK_EditGoBegLine        , KT_E }, // 0xE045
  { CRC_READGOTONEXTMSG        , KK_ReadGotoNextMsg      , KT_R }, // 0xE0F1
  { CRC_EDITSPELLCHECK         , KK_EditSpellCheck       , KT_E }, // 0xE167
  { CRC_AREASCAN               , KK_AreaScan             , KT_A }, // 0xE440
  { CRC_EDITGOWORDLEFT         , KK_EditGoWordLeft       , KT_E }, // 0xE48C
  { CRC_EDITSCODENORMAL        , KK_EditSCodeNormal      , KT_E }, // 0xE4CE
  { CRC_READMOVECOMMENTMSG     , KK_ReadMoveCommentMsg   , KT_R }, // 0xE4E7
  { CRC_READGOTOREPLY1ST       , KK_ReadGotoReply1st     , KT_R }, // 0xE54F
  { CRC_READTOGGLEHEXDUMP      , KK_ReadToggleHexdump    , KT_R }, // 0xE6D5
  { CRC_LISTNARROWSUBJ         , KK_ListNarrowSubj       , KT_M }, // 0xE756
  { CRC_NODEQUITNOW            , KK_NodeQuitNow          , KT_N }, // 0xE780
  { CRC_NODEGOTONEXT           , KK_NodeGotoNext         , KT_N }, // 0xE7F5
  { CRC_READMAKEUSERLIST       , KK_ReadMakeUserlist     , KT_R }, // 0xE8C6
  { CRC_EDITCLEARPASTEBUF      , KK_EditClearPasteBuf    , KT_E }, // 0xE8E4
  { CRC_K_AENTER               , Key_A_Ent               ,    0 }, // 0xE921
  { CRC_READMARKINGOPTIONS     , KK_ReadMarkingOptions   , KT_R }, // 0xE97A
  { CRC_READUNMARK             , KK_ReadUnmark           , KT_R }, // 0xE9BB
  { CRC_FILEGOTONEXT           , KK_FileGotoNext         , KT_F }, // 0xE9EC
  { CRC_EDITBLOCKLEFT          , KK_EditBlockLeft        , KT_E }, // 0xED1D
  { CRC_K_CUP                  , Key_C_Up                ,    0 }, // 0xEE6B
  { CRC_NODEASKEXIT            , KK_NodeAskExit          , KT_N }, // 0xEE95
  { CRC_READGOTOPREVAREA       , KK_ReadGotoPrevArea     , KT_R }, // 0xEECB
  { CRC_READFINDALL            , KK_ReadFindAll          , KT_R }, // 0xEF19
  { CRC_NODEDOSSHELL           , KK_NodeDosShell         , KT_N }, // 0xF084
#if !defined(__UNIX__) || defined(__USE_NCURSES__)
  { CRC_K_SDOWN                , Key_S_Dwn               ,    0 }, // 0xF0FB
#endif
  { CRC_READCHANGEXLATIMPORT   , KK_ReadChangeXlatImport , KT_R }, // 0xF169
  { CRC_HEADERTOGGLERESERVED   , KK_HeaderToggleReserved , KT_H }, // 0xF297
  { CRC_READTOGGLESTYLES       , KK_ReadToggleStyles     , KT_R }, // 0xF47E
  { CRC_EDITBLOCKEND           , KK_EditBlockEnd         , KT_E }, // 0xF55B
  { CRC_EDITTABREVERSE         , KK_EditTabReverse       , KT_E }, // 0xF5B6
  { CRC_HEADERTOGGLEPVT        , KK_HeaderTogglePvt      , KT_H }, // 0xF614
  { CRC_HEADERTOGGLESENT       , KK_HeaderToggleSent     , KT_H }, // 0xF68C
  { CRC_AREAMARK               , KK_AreaMark             , KT_A }, // 0xF77B
  { CRC_K_RIGHT                , Key_Rgt                 ,    0 }, // 0xF78D
  { CRC_EDITIMPORTQUOTEBUF     , KK_EditImportQuotebuf   , KT_E }, // 0xF797
  { CRC_READMACRO              , KK_ReadMacro            , KT_R }, // 0xF7D8
  { CRC_FILEMARK               , KK_FileMark             , KT_F }, // 0xF828
  { CRC_EDITCLEARDELETEBUF     , KK_EditClearDeleteBuf   , KT_E }, // 0xF8DE
  { CRC_READCHANGEMSG          , KK_ReadChangeMsg        , KT_R }, // 0xF99F
  { CRC_K_ABSPACE              , Key_A_BS                ,    0 }, // 0xF9D7
  { CRC_EDITCUT                , KK_EditCut              , KT_E }, // 0xFAE6
  { CRC_EDITSCODEREVERSE       , KK_EditSCodeReverse     , KT_E }, // 0xFC0E
  { CRC_K_CF9                  , Key_C_F9                ,    0 }, // 0xFD02
  { CRC_K_CF8                  , Key_C_F8                ,    0 }, // 0xFD03
  { CRC_K_CF3                  , Key_C_F3                ,    0 }, // 0xFD08
  { CRC_K_CF2                  , Key_C_F2                ,    0 }, // 0xFD09
  { CRC_K_CF1                  , Key_C_F1                ,    0 }, // 0xFD0A
  { CRC_K_CF7                  , Key_C_F7                ,    0 }, // 0xFD0C
  { CRC_K_CF6                  , Key_C_F6                ,    0 }, // 0xFD0D
  { CRC_K_CF5                  , Key_C_F5                ,    0 }, // 0xFD0E
  { CRC_K_CF4                  , Key_C_F4                ,    0 }, // 0xFD0F
  { CRC_READCHANGEORIGIN       , KK_ReadChangeOrigin     , KT_R }, // 0xFD2C
  { CRC_FILEDOSSHELL           , KK_FileDosShell         , KT_F }, // 0xFE9D
  { CRC_EDITLOOKUPDEST         , KK_EditLookupDest       , KT_E }, // 0xFEC4
  { CRC_ADDRESSBOOKADD         , KK_AddressbookAdd       , KT_B }, // 0xFF55
  { CRC_AREAMACRO              , KK_AreaMacro            , KT_A }, // 0xFFA7
};


//  ------------------------------------------------------------------

static gkey SwitchKeyDefs(word crc, int* type) {

  uint left,right,middle;
  word cur;

  *type = 0;
  left = 0;
  right = (sizeof(globalkeys)/sizeof(tglobalkey))-1;

  if((crc < globalkeys[left].crc_token) or (crc > globalkeys[right].crc_token))
    return 0;

  do {
    middle = (left + right)/2;

    cur = globalkeys[middle].crc_token;

    if(crc < cur)
      right = middle - 1;
    else if(crc > cur)
      left  = middle + 1;
    else {
      *type = globalkeys[middle].keytype;
      return globalkeys[middle].keyval;
    }

  } while(left <= right);

  return 0;
}


//  ------------------------------------------------------------------

void KeyCmdAdd(gkey keycmd, gkey keyval, int keytype) {

  if(keyval <= 0xFF)
    keyval |= (gkey)(scancode(keyval) << 8);

  std::list<CmdKey>::iterator tmp = CFG->cmdkey.begin();
  if(CFG->switches.get(keybdefaults)) {
    while(tmp != CFG->cmdkey.end()) {
      if(tmp->type == keytype) {
        if(tmp->key == keyval) {
          tmp->cmd = keycmd;
          return;
        }
      }
      tmp++;
    }
  }

  CmdKey tmp2;
  tmp2.key  = keyval;
  tmp2.cmd  = keycmd;
  tmp2.type = keytype;
  CFG->cmdkey.push_back(tmp2);
}


//  ------------------------------------------------------------------

void RunMacro(Macro* m) {

  gkey* mac = m->buf;
  while(*mac) {
    kbput(*mac);
    mac++;
  }
}


//  ------------------------------------------------------------------

static void PlayMacro() {

  RunMacro(&CFG->macro[0xFFFF-gkbd.curronkey->pass]);
}


//  ------------------------------------------------------------------

static void SetKeybDefaults() {

  // Setup default keyset
  CmdKey* tmp = DefaultKeyset;
  int keys = (sizeof(DefaultKeyset) / sizeof(CmdKey)) - 1;
  int k = 0;
  while(k < keys) {
    if(tmp->key <= 0xFF)
      tmp->key = (gkey)g_tolower(tmp->key);
    KeyCmdAdd(tmp->cmd, tmp->key, tmp->type);
    tmp++;
    k++;
  }
}


//  ------------------------------------------------------------------

//bool CmdKeyCmp(CmdKey a, CmdKey b) {
bool operator<(CmdKey &a, CmdKey &b) {

  int cmp = CmpV(a.type, b.type);
  if(cmp != 0)
    return (cmp < 0);
  return (CmpV(a.key, b.key) < 0);
}


//  ------------------------------------------------------------------

int ReadKeysCfg(int force) {

  byte ch;
  gkey* mac;
  FILE *ifp;
  char* ptr;
  char* ptr2;
  int keytype;
  int n;
  char buf[1024];
  gkey keyval=0, keycmd;
  uint line=0;

  const char* cfg = AddPath(CFG->goldpath, CFG->keyscfg);
  ifp = fsopen(cfg, "rt", CFG->sharemode);
  if (ifp)
  {
    const char* cfgname = strrchr(cfg, '\\');
    cfgname = cfgname ? cfgname+1 : cfg;

    if (not quiet)
      STD_PRINT("* Reading " << cfg << std::endl);

    CFG->macro.clear();
    CFG->cmdkey.clear();
    if(CFG->switches.get(keybdefaults))
      SetKeybDefaults();

    while(fgets(buf, sizeof(buf), ifp)) {
      line++;
      ptr = strskip_wht(buf);
      if(*ptr == ';' or strblank(ptr))
        continue;
      ptr2 = ptr;
      ptr = strskip_txt(ptr);
      if (*ptr == NUL)
      {
        STD_PRINT("* " << cfgname << ": Incomplete line " << line << "." << std::endl);
        SayBibi();
        cfgerrors++;
        continue;
      }
      *ptr++ = NUL;
      // If either straight Key or $Key do not make lookup by CRC
      if(*ptr2 == '$') {
        uint _keyval = 0;
        sscanf(ptr2+1, "%4x", &_keyval);
        keyval = (gkey)_keyval;
      }
      else if(strlen(ptr2) == 1)
        keyval = (gkey)g_tolower(*ptr2);  // Always convert to lowercase internally
      else
      {
        keycmd = SwitchKeyDefs(strCrc16(strupr(ptr2)), &keytype);
        if (not keycmd or keytype)
        {
          STD_PRINT("* " << cfgname << ": Invalid key \"" << ptr2 << "\" in line " << line << "." << std::endl);
          SayBibi();
          cfgerrors++;
          continue;
        }
        keyval = keycmd;
      }
      ptr = strskip_wht(ptr);
      ptr2 = ptr;
      ptr = strskip_txt(ptr);
      if(*ptr != NUL) {
        *ptr++ = NUL;
      }
      keycmd = SwitchKeyDefs(strCrc16(strupr(ptr2)), &keytype);
      if(keycmd) {
        if((keycmd >= KK_Macro) and (keycmd <= KK_ReadMacro)) {
          Macro tmp2;
          memset(&tmp2, 0, sizeof(Macro));
          n = 0;
          ptr = strskip_wht(ptr);
          while(*ptr) {
            if(*ptr == '\"') {  // Start of literal string
              ptr++;
              while((*ptr != '\"') and (n < (sizeof(tmp2.buf)/sizeof(gkey)))) {
                // allow '\"' and '\\' in config
                if((ptr[0] == '\\') and ((ptr[1] == '\"') or (ptr[1] == '\\')))
                  ptr++;
                ch = *ptr++;
                tmp2.buf[n++] = (gkey)(ch | (scancode(ch) << 8));
              }
              if(*ptr != NUL) {
                *ptr++ = NUL;
              }
            }
            else {
              ptr2 = ptr;
              ptr = strskip_txt(ptr);
              if(*ptr != NUL) {
                *ptr++ = NUL;
              }
              int tmpkt;
              keycmd = SwitchKeyDefs(strCrc16(strupr(ptr2)), &tmpkt);
              if(keycmd) {
                if(n < (sizeof(tmp2.buf)/sizeof(gkey)))
                  tmp2.buf[n++] = (gkey)keycmd;
              }
              else
              {
                STD_PRINT("* " << cfgname << ": Invalid key \"" << ptr2 << "\" in line " << line << "." << std::endl);
                SayBibi();
                cfgerrors++;
                continue;
              }
            }
            ptr = strskip_wht(ptr);
          }
          if(keyval <= 0xFF)
            keyval |= (gkey)(scancode(keyval) << 8);
          tmp2.key = (gkey)keyval;
          tmp2.type = keytype;
          CFG->macro.push_back(tmp2);

          // delete overlayed (non-macro) key definition
          std::list<CmdKey>::iterator tmp = CFG->cmdkey.begin();
          while(tmp != CFG->cmdkey.end()) {
            if(tmp->type == keytype) {
              if(tmp->key == keyval) {
                // delete it by setting type to an invalid value
                tmp->type = 0xFF;
                break;
              }
            }
            tmp++;
          }
        }
        else if(keytype)
          KeyCmdAdd(keycmd, keyval, keytype);
        else
        {
          STD_PRINT("* " << cfgname << ": Invalid key \"" << ptr2 << "\" in line " << line << "." << std::endl);
          SayBibi();
          cfgerrors++;
          continue;
        }
      }
      else
      {
        STD_PRINT("* " << cfgname << ": Invalid key \"" << ptr2 << "\" in line " << line << "." << std::endl);
        SayBibi();
        cfgerrors++;
        continue;
      }
    }
    fclose(ifp);
  }

  // Setup default keyset when no keys are defined
  if(CFG->cmdkey.empty())
    SetKeybDefaults();

  // Prepare keyboard and macro tables for binary searching
//  CFG->cmdkey.sort(CmdKeyCmp);
  CFG->cmdkey.sort();

  // Count keys
  std::list<CmdKey>::iterator ck = CFG->cmdkey.begin();
  while(ck != CFG->cmdkey.end()) {
    switch(ck->type) {
      case KT_A: AreaKeys++; if(AreaKeys == 1) AreaKey = ck; break;
      case KT_E: EditKeys++; if(EditKeys == 1) EditKey = ck; break;
      case KT_F: FileKeys++; if(FileKeys == 1) FileKey = ck; break;
      case KT_G:                                             break;
      case KT_M: ListKeys++; if(ListKeys == 1) ListKey = ck; break;
      case KT_N: NodeKeys++; if(NodeKeys == 1) NodeKey = ck; break;
      case KT_R: ReadKeys++; if(ReadKeys == 1) ReadKey = ck; break;
      case KT_B: AddressbookKeys++; if(AddressbookKeys == 1) AddressbookKey = ck; break;
      case KT_H: HeaderKeys++; if(HeaderKeys == 1) HeaderKey = ck; break;
    }
    ck++;
  }

  // Assign global macros
  std::vector<Macro>::iterator k;
  for (k = CFG->macro.begin(), n = 0; k != CFG->macro.end(); k++, n++)
  {
    if (k->type == 0)
    {
      if (k->key == KK_Auto)
      {
        if (keybuf.empty() && !*CFG->keybstack)
        {
          // Start automacro
          mac = k->buf;
          while(*mac)
          {
            kbput(*mac);
            mac++;
          }
        }
      }
      else
        setonkey(k->key, PlayMacro, (gkey)(0xFFFF-n));
    }
  }

  return true;
}


//  ------------------------------------------------------------------

