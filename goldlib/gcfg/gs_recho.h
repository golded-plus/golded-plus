//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  RA-ECHO areafile structure
//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack(1)
#endif

//  ------------------------------------------------------------------
//  Original Pascal unit:
//  ------------------------------------------------------------------
//
//  UNIT EchoDefs;
//
//  (*  Definitions for RA-ECHO v.1.0 *)
//
//
//    INTERFACE
//
//    CONST
//
//      EchoConfigName = 'RA-ECHO.CFG';
//      AreaInfoName   = 'AREAS';
//      NodeFileName   = 'NODE';
//      DupeFileName   = 'DUPES';
//      LinkFileName   = 'LINKS';
//      ExportMarkName = 'LASTEXP';
//      ImportMarkName = 'LASTIMP';
//      ConfTextName   = 'CONFNAME';
//      EchoExt        = '.RAE';
//
//    TYPE
//
//      LastReadRec   = RECORD
//                        LastMsg  : word;
//                        LastDate : LONGINT;
//                      END;  (* of RECORD *)
//
//
//      AddressRec    = RECORD
//                        Zone,
//                        Net,
//                        Node,
//                        Point  : word;
//                        Domain : STRING [20];
//                      END;  (* of RECORD *)
//
//
//      PacketFlags   = (NoFlags,Hold,Crash,Direct,KillSend,KillFileSend,TruncatFileSend);
//
//
//      MailTypeDef   = (Echomail, Netmail, Localmail, Newmail);
//
//
//      ArcProgramDef = (NoArc,ZIPArc,LZHArc,PAKArc,ARCArc,ZOOArc,LHAArc,ARJArc
//                      ,HyperArc,UnknownArc,
//                      UserArc1,UserArc2,UserArc3,UserArc4,UserArc5,UserArc6);
//
//      NodeDatenRec  = RECORD
//                        PointNr,                         (* Point-Number                              *)
//                        NodeNr,                          (* Node-Number                               *)
//                        NetNr,                           (* Net-Number                                *)
//                        ZoneNr,                          (* Zone-Number                               *)
//                        SendZone,                        (* Zone of  Sender                           *)
//                        SendNet,                         (* Net of  Sender                            *)
//                        SendNode,                        (* Node of  Sender                           *)
//                        SendPoint     : word;            (* Point of  Sender                          *)
//                        Conferences   : ARRAY [1..20] OF booleanEAN; (* Echomail Conferences             *)
//                        Access        : word;            (* Access   Level                            *)
//                        NodeName      : STRING [30];     (* Name of Node                              *)
//                        PassWord      : STRING [20];     (* Password for Areafix/Filefix              *)
//                        ArcType       : ArcProgramDef;   (* Definition of the arcer                   *)
//                        ArcPassword   : STRING [20];     (* Password in Arcmail header                *)
//                        ReAddress     : booleanEAN;         (* true, if address has to be ramapped       *)
//                                                         (* to 3-d Pointnet address                   *)
//                        ArcMailStatus : PacketFlags;     (* Status of  ArcMail-Packets for the Node   *)
//                                                         (* Node o. Hold o. Crash o. Direct           *)
//                        NameExport    : booleanEAN;         (* true, if Node wants to get mail addresed  *)
//                                                         (* to his name no matter if he is linked to that echo *)
//                        AddPacket     : booleanEAN;         (* Append mail to exeistimg packets          *)
//                        TotalImport,                     (* Totale Imports from this node (# of mails *)
//                        TotalExport   : word;            (* Totale Exports to this node               *)
//                        NotifyFlag    : booleanEAN;         (* Does this node get notify messages        *)
//                        AcceptNewEchos: booleanEAN;         (* New echos received from this node are     *)
//                                                         (* created as passthru echos                 *)
//                        nodeispassive : booleanEAN;         (* if false mail will not be exported to     *)
//                                                         (* this node                                 *)
//                        LastDate      : STRING [8];      (* The last date the user requested via Filefix *)
//                        AddNewAreas   : booleanEAN;
//                        packnetmail   : ArcProgramDef;
//                        NetmailStatus : Packetflags;
//                        Dummy         : ARRAY [1..6] OF byte; (* - unused - *)
//                      END;  (* of RECORD *)
//
//      AreaDatenRec  = ARRAY [1..500] OF booleanEAN;
//
//      NodeFileRec   = RECORD
//                        NodeDaten  : NodeDatenRec;
//                        AreaFeld   : AreaDatenRec;
//                      END;  (* of RECORD *)
//
//
//  (* Das File AREAS.RAE enth„lt alle 500 Echo-Areas, Nr. 1-200 sind die
//     internen Areas von RA, 201-500 die Passthru Areas                   *)
//
//
//      EchoAreaRec   = RECORD
//                        Name           : STRING [40]; (* Area Name                    *)
//                        Descr          : STRING [60]; (* Description                  *)
//                        MailType       : MailTypeDef;
//                        ImportFlag     : booleanEAN;     (* true, if the message has to be    *)
//                                                      (* imported to a directory as *.MSG  *)
//                        MsgDirectory   : STRING [60];
//                        ReadSecurity   : Word;        (* Read access level            *)
//                        WriteSecurity  : Word;        (* Write access level           *)
//                        OriginLine     : String [60];
//                        AreaAddress    : AddressRec;  (* Adress of sender for this echo      *)
//                        ConferenceNr   : Byte;        (* The conference this echo belongs    *)
//                                                      (* to. If 0 the echo has NO conference *)
//                      END;  (* of RECORD *)
//
//      ConfNameRec   = STRING [255];
//
//
//  IMPLEMENTATION
//
//  END.
//
//  ------------------------------------------------------------------


//  ------------------------------------------------------------------

enum
{
    RAE_ECHO,
    RAE_NET,
    RAE_LOCAL
};


//  ------------------------------------------------------------------
//  Structure for AREAS.RAE Version 1.00

typedef struct
{
    char  echoid[41];
    char  desc[61];
    byte  type;
    byte  isopus;
    char  path[61];
    word  readsec;
    word  writesec;
    char  origin[61];
    ftn_addr addr;
    char  domain[21];
    byte  conference;
} TRaEchoArea100, *PRaEchoArea100;


//  ------------------------------------------------------------------
//  Structure for AREAS.RAE Version 1.01 (I hope!)

typedef struct
{
    char  echoid[41];
    char  desc[61];
    byte  type;
    byte  isopus;
    char  path[61];
    word  readsec;
    word  writesec;
    char  origin[61];
    ftn_addr addr;
    char  domain[21];
    byte  conference;
    word  maxdays;
    word  maxrecv;
    word  maxcount;
    word  totalimp;
    word  totalexp;
    char  dummy[130];
} TRaEchoArea101, *PRaEchoArea101;


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
    #pragma pack()
#endif

//  ------------------------------------------------------------------
