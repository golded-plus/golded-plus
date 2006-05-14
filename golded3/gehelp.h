
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
//  Help Categories.
//  ------------------------------------------------------------------
#ifndef __GEHELP_H
#define __GEHELP_H

//  ------------------------------------------------------------------
//  Help Categories


//  ------------------------------------------------------------------
//  General

#define H_General           1000
#define H_AskDropMsg        1001
#define H_AskExit           1002
#define H_AskOverWrite      1003
#define H_Attributes        1004
#define H_ChangeAka         1005
#define H_ChangeOrigin      1006
#define H_ChangeTemplate    1007
#define H_ChangeUsername    1008
#define H_ChangeTagline     1009
#define H_ChangeXlatImport  1010


//  ------------------------------------------------------------------
//  Area

#define H_Area              2000
#define H_AreaMark          2001
#define H_ScanAreas         2002
#define H_AreaSelection     2003
#define H_HeatAreas         2004
#define H_ZapAreas          2005
#define H_SelectMarks       2006
#define H_AreaDropMarks     2007
#define H_AreaCatchUp       2008

//  ------------------------------------------------------------------
//  Reader Menus

#define H_Reader            3000
#define H_AskChange         3001
#define H_AskDelete         3002
#define H_AskDoMarks        3003
#define H_AskNextArea       3004
#define H_CopyMoveForward   3005
#define H_FindString        3006
#define H_InputMsgno        3007
#define H_MarkMessages      3008
#define H_MarkString        3009
#define H_MessageBrowser    3010
#define H_WriteMessage      3011
#define H_EditCryptKey      3012
#define H_FileRequest       3013
#define H_MenuConfirm       3014
#define H_GotoReplies       3015
#define H_ReplyThread       3016
#define H_ReadExternUtil    3017
#define H_PeekURL           3018
#define H_MainMenu          3019


//  ------------------------------------------------------------------
//  Header Menus

#define H_Header            4000
#define H_AskDelOrig        4001
#define H_AskReadOnly       4002
#define H_AskZoneGate       4003
#define H_CarbonCopy        4004
#define H_FileSelect        4005
#define H_Message           4006
#define H_NodelistBrowser   4007
#define H_SaveMsg           4008
#define H_AskForward        4009
#define H_Crosspost         4010


//  ------------------------------------------------------------------
//  Editor Menus

#define H_Editor            5000
#define H_ExportFile        5001
#define H_ImportFile        5002
#define H_ImportTxt         5003


//  ------------------------------------------------------------------
//  Userbase Menus

#define H_Userbase          6000
#define H_EditAdrEntry      6001


//  ------------------------------------------------------------------
//  Error messages

#define H_EOutOfMemory      9001
#define H_EWarnMsgtxt       9002
#define H_ENodelist         9003
#define H_EBetaVersion      9004
#define H_EOpenQbase        9005
#define H_ENoUser           9006
#define H_ECtrlBreak        9007
#define H_EQbaseRebuild     9008
#define H_EMemoryWarning    9009


//  ------------------------------------------------------------------

#endif
