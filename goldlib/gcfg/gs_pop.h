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
//  Portal of Power configuration structures.
//  ------------------------------------------------------------------

#ifndef __GS_POP_H
#define __GS_POP_H
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  Pascal types
typedef char     Char;
typedef uint8_t  Byte;
typedef uint8_t  Boolean;
typedef  int16_t Integer;
typedef uint16_t Word;
typedef  int32_t LongInt;
//  ------------------------------------------------------------------
//  Misc. types
typedef LongInt PopDate;
typedef LongInt PopTime;
typedef Byte    ColorSet[62];
typedef Char    PathStr[80];
// All string types, very handy!
typedef char S1[2], S2[3], S3[4], S4[5], S5[6], S6[7], S7[8], S8[9], S9[10], S10[11],
    S11[12], S12[13], S13[14], S14[15], S15[16], S16[17], S17[18], S18[19], S19[20],
    S20[21],
    S21[22], S22[23], S23[24], S24[25], S25[26], S26[27], S27[28], S28[29], S29[30],
    S30[31],
    S31[32], S32[33], S33[34], S34[35], S35[36], S36[37], S37[38], S38[39], S39[40],
    S40[41],
    S41[42], S42[43], S43[44], S44[45], S45[46], S46[47], S47[48], S48[49], S49[50],
    S50[51],
    S51[52], S52[53], S53[54], S54[55], S55[56], S56[57], S57[58], S58[59], S59[60],
    S60[61],
    S61[62], S62[63], S63[64], S64[65], S65[66], S66[67], S67[68], S68[69], S69[70],
    S70[71],
    S71[72], S72[73], S73[74], S74[75], S75[76], S76[77], S77[78], S78[79], S79[80],
    S80[81],
    S81[82], S82[83], S83[84], S84[85], S85[86], S86[87], S87[88], S88[89], S89[90],
    S90[91],
    S91[92], S92[93], S93[94], S94[95], S95[96], S96[97], S97[98], S98[99], S99[100],
    S100[101], S101[102], S102[103], S103[104], S104[105], S105[106], S106[107],
    S107[108],
    S108[109], S109[110], S110[111], S111[112], S112[113], S113[114], S114[115],
    S115[116],
    S116[117], S117[118], S118[119], S119[120], S120[121], S121[122], S122[123],
    S123[124],
    S124[125], S125[126], S126[127], S127[128], S128[129], S129[130], S130[131],
    S131[132],
    S132[133], S133[134], S134[135], S135[136], S136[137], S137[138], S138[139],
    S139[140],
    S140[141], S141[142], S142[143], S143[144], S144[145], S145[146], S146[147],
    S147[148],
    S148[149], S149[150], S150[151], S151[152], S152[153], S153[154], S154[155],
    S155[156],
    S156[157], S157[158], S158[159], S159[160], S160[161], S161[162], S162[163],
    S163[164],
    S164[165], S165[166], S166[167], S167[168], S168[169], S169[170], S170[171],
    S171[172],
    S172[173], S173[174], S174[175], S175[176], S176[177], S177[178], S178[179],
    S179[180],
    S180[181], S181[182], S182[183], S183[184], S184[185], S185[186], S186[187],
    S187[188],
    S188[189], S189[190], S190[191], S191[192], S192[193], S193[194], S194[195],
    S195[196],
    S196[197], S197[198], S198[199], S199[200], S200[201], S201[202], S202[203],
    S203[204],
    S204[205], S205[206], S206[207], S207[208], S208[209], S209[210], S210[211],
    S211[212],
    S212[213], S213[214], S214[215], S215[216], S216[217], S217[218], S218[219],
    S219[220],
    S220[221], S221[222], S222[223], S223[224], S224[225], S225[226], S226[227],
    S227[228],
    S228[229], S229[230], S230[231], S231[232], S232[233], S233[234], S234[235],
    S235[236],
    S236[237], S237[238], S238[239], S239[240], S240[241], S241[242], S242[243],
    S243[244],
    S244[245], S245[246], S246[247], S247[248], S248[249], S249[250], S250[251],
    S251[252],
    S252[253], S253[254], S254[255], S255[256];
// Nodelist Types supported
typedef Byte NodeListType;
enum NodeListTypes
{
    NewNodelistType, QBBSNodelistType, RANodelistType, Version7, SBBSNodeListType
};

// A 4D FidoNet address
struct AddressType
{
    Integer Zone, Net, Node, Point;
};

typedef S50 SendToType[2];
struct FunctionKeyType
{
    Byte ErrorLevel;
    S70  Description;
};

#define MaxAddresses 16
//  ------------------------------------------------------------------
//  Contents : Configuration information.
//  FileName : PORTAL.CFG
//  Size     : 10240 bytes
//  Comments :
enum NodeStatsT {nsUnknown, nsKnown, nsPassword};

enum ReqLimitsT {rlPrCall, rlPrDay};

#define NodeStatT 3
#define ReqLimitT 2

typedef struct ConfigT * ConfigP;
struct ConfigT
{
    Byte    Version;
    Byte    TaskNumber;
    PathStr SwapDir, Inbound, Outbound, Banner, DefaultMacro, Editor,
            LogfileName;
    Byte            LogLevel;
    Boolean         UseYearInLog;
    PathStr         NodeList;
    NodeListType    NodelistTyp;
    Byte            MainAdrNum;
    AddressType     Adresses[MaxAddresses];
    Integer         PointNet;
    Boolean         UseFakeAddress;
    S20             Sysop;
    S60             System;
    Boolean         SwapOnExec;
    PathStr         EnterBBSText, DoingMailText, PressESCText;
    Byte            MaxRinging;
    PopTime         ZMHStart, ZMHEnd, CallTime, CallWidth;
    Boolean         ConnectFastest;
    PopTime         OutReReadDelay;
    S20             FuncReqPwd;
    Boolean         UseJanus;
    Boolean         UseEMSI;
    Boolean         FastCalls;
    Boolean         KeepEmptyDirs;
    Byte            Filler[272];
    FunctionKeyType FKey[2][10];
    ColorSet        Color[4];
    struct
    {
        Byte    BBSType;
        PathStr BBSPath;
        PathStr UserFile;
        Word    MinBaud;
        PathStr MinBaudFile;
        Byte    UserErrorLevel;
        struct
        {
            Char Key;
            S8   BatName;
        } Multi[5];

        Byte Filler[64];
    } BBS;

    struct
    {
        Byte    CommPort;
        Word    BaudRate;
        S40     PreInit, Init, NoAnswer, Dial, Busy, HangUp, Answer;
        PopTime ReInit;
        S10     LockedBaud;
        struct
        {
            S10  ConStr;
            Byte ErrLvl;
        } ExitStr[5];

        struct
        {
            Byte    Bit;
            S40     Dial;
            Boolean DialOut;
            Word    Baud;
        } ModemType[8];

        Byte Filler[64];
    } Modem;

    struct
    {
        PopTime BlankTime;
        Boolean ChkSnow, KeepOffScrMode, ScrMode, HardwareBlank, ExplodingWin;
        Byte    Filler[32];
    } Screen;

    struct
    {
        PathStr FileList, NewsList, Header, Footer, TopFile, StatFile;
        Boolean Adopt;
        S40     AdoptComment;
        Boolean DupeCheck;
        Boolean Stat;
        Boolean IncludeTop;
        Byte    Top;
        Boolean DkDate;
        Byte    NewsDays;
        Byte    Threshold;
        Boolean RemMissing, OkPortal, ZapZero, Touch;
        S12     CrapFiles[10], ExcludeFiles[10], NoDupeCheck[10];
        S12     NonAdoptFiles[10];
        S3      NonAdoptAreas[10];
        S3      PrivateAreas[11];
        PathStr DoBefore;
        PathStr DoAfter;
        PathStr DoPack;
        PathStr DupeReport;
        S55     TXTFreq[4];
        PathStr OkBimodempath;
        Byte    Filler[32];
    } ListFiles;

    struct
    {
        struct
        {
            Word    MaxFiles;
            PopTime MaxTime;
            LongInt MaxBytes;
        } Limit[NodeStatT][ReqLimitT];

        Boolean RspAsPkt;
        Boolean SkipAfterFirst;
        Byte    Filler[63];
    } Request;

    struct
    {
        S50     AddToSeenBy;
        Boolean CheckZones;
        PathStr NetMailDir;
        PathStr Filler3;
        Boolean Secure;
        PathStr BadMsgs;
        PathStr SaveDupesDir;
        Word    MaxDupes;
        PathStr EchoTossLog;
        Boolean StripCrash;
        Boolean SetAKASent;
        Boolean PvtEchoMail;
        Boolean SaveFwdMail;
        PathStr RouteFile;
        Boolean ForwardMail;
        Boolean KillFwdFiles;
        PathStr Filler2;
        Boolean OldExt;
        Byte    DefaultPacker;
        Byte    NetMailBoard;
        Byte    Filler[128];
    } MailScanner;

    struct
    {
        PathStr PreCmd;
        PathStr SecureDir;
        Word    MsgAttribute;
        Boolean KillSent;
        Boolean MsgPrivate;
        S72     Subject;
        Byte    Filler[64];
    } FwdFile;

    struct
    {
        PathStr DupeDir;
        PathStr HoldDir;
        PathStr BeforeMoving;
        Boolean RequestMissing;
        Byte    Filler[64];
    } Tick;

    struct
    {
        S10     BiOverride[5];
        Boolean NoSendOverride;
        Word    MaxBaud;
        PathStr BiModemPath;
        Byte    Filler[16];
    } BiMail;

    struct
    {
        PathStr CostFileName;
        S10     IntPrefix;
        S10     OurPrefix;
        Boolean UseFidoUserLst;
        S5      MTypeStr[8];
        Byte    Filler[64];
    } NLCompiler;

    struct
    {
        Boolean CRSendsLF, DestructiveBS;
        S40     ModemInit;
        Byte    Filler[128];
    } DumbTerm;

    struct
    {
        Char    Key;
        S8      Name;
        Boolean AskDLName;
        S10     AutoStart;
        PathStr SendCmd;
        PathStr RecvCmd;
    } ExtProt[5];

    struct
    {
        Char    DLCntStart, DLCntStop;
        Boolean AdoptDefault, InsDLCnt, AddInbound;
        Byte    FirstArea, LastArea;
        Byte    DlCDigits;
        Boolean DlCZeroFill;
        S50     ViewCMD;
        Byte    Filler[11];
    } AreaMan;

    struct
    {
        S40 AddCmd, UnPackCmd, EraseCmd, TestCmd;
    } Packer[7];
};

//  ------------------------------------------------------------------
//  Contents : Message Areas
//  FileName : PORTAL.ARE
//  Size     : 1024
//  Comments : **NOT USED YET**
typedef struct AreasTypeT * AreasTypeP;
struct AreasTypeT
{
    PathStr    Directory;
    S32        EchoNames[3];
    S50        Origin;
    SendToType SendTo;
    SendToType SendOnly;
    S20        ScramblePwd;
    Boolean    StripSeenBy;
    Boolean    Pvt2EMail;
    Byte       Level, Keys;
    Word       MaxScan;
    PopDate    ScanDate;
    Word       Scanned;
    S40        Description;
    Byte       AreaType;
    Boolean    ImportSB;
    Byte       UsedAka;           // 0 = Address[Config.MainAdrNum], otherwise Address[UsedAka]
    Byte       Filler[513];
};

//  ------------------------------------------------------------------
//  Contents : Nodelist Segments - for Nodelist Compiler
//  FileName : PORTAL.NLS
//  Size     : 512 bytes
//  Comments : One record per used nodelist. Shared among all tasks
typedef struct NodelistSegT * NodelistSegP;
struct NodelistSegT
{
    S8         NodeListName;
    S8         DiffFileName;
    Boolean    CheckCRC;
    PathStr    NewNLPath;
    SendToType Include;
    SendToType Exclude;
    Byte       Filler[209];
};

//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------

#endif // ifndef __GS_POP_H
//  ------------------------------------------------------------------
