
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
//  Language compiler
//  ------------------------------------------------------------------

#include <golded.h>
#include <gclang.h>
#include <gregex.h>


//  ------------------------------------------------------------------
//  Structure of language crc list

struct LangCrc {
  word   crc;
  char** langptr;
  char*  langdef;
};


//  ------------------------------------------------------------------
//  Language CRC16/Pointer/Default list

static LangCrc LangCrcs[] = {

  { CRC_MS_SWSUN              , NULL  , (char *)"Sun" },
  { CRC_MS_SWMON              , NULL  , (char *)"Mon" },
  { CRC_MS_SWTUE              , NULL  , (char *)"Tue" },
  { CRC_MS_SWWED              , NULL  , (char *)"Wed" },
  { CRC_MS_SWTHU              , NULL  , (char *)"Thu" },
  { CRC_MS_SWFRI              , NULL  , (char *)"Fri" },
  { CRC_MS_SWSAT              , NULL  , (char *)"Sat" },

  { CRC_MS_LWSUNDAY           , NULL  , (char *)"Sunday" },
  { CRC_MS_LWMONDAY           , NULL  , (char *)"Monday" },
  { CRC_MS_LWTUESDAY          , NULL  , (char *)"Tuesday" },
  { CRC_MS_LWWEDNESDAY        , NULL  , (char *)"Wednesday" },
  { CRC_MS_LWTHURSDAY         , NULL  , (char *)"Thursday" },
  { CRC_MS_LWFRIDAY           , NULL  , (char *)"Friday" },
  { CRC_MS_LWSATURDAY         , NULL  , (char *)"Saturday" },

  { CRC_MS_SMJAN              , NULL  , (char *)"Jan" },
  { CRC_MS_SMFEB              , NULL  , (char *)"Feb" },
  { CRC_MS_SMMAR              , NULL  , (char *)"Mar" },
  { CRC_MS_SMAPR              , NULL  , (char *)"Apr" },
  { CRC_MS_SMMAY              , NULL  , (char *)"May" },
  { CRC_MS_SMJUN              , NULL  , (char *)"Jun" },
  { CRC_MS_SMJUL              , NULL  , (char *)"Jul" },
  { CRC_MS_SMAUG              , NULL  , (char *)"Aug" },
  { CRC_MS_SMSEP              , NULL  , (char *)"Sep" },
  { CRC_MS_SMOCT              , NULL  , (char *)"Oct" },
  { CRC_MS_SMNOV              , NULL  , (char *)"Nov" },
  { CRC_MS_SMDEC              , NULL  , (char *)"Dec" },

  { CRC_MS_LMJANUARY          , NULL  , (char *)"January" },
  { CRC_MS_LMFEBRUARY         , NULL  , (char *)"February" },
  { CRC_MS_LMMARCH            , NULL  , (char *)"March" },
  { CRC_MS_LMAPRIL            , NULL  , (char *)"April" },
  { CRC_MS_LMMAY              , NULL  , (char *)"May" },
  { CRC_MS_LMJUNE             , NULL  , (char *)"June" },
  { CRC_MS_LMJULY             , NULL  , (char *)"July" },
  { CRC_MS_LMAUGUST           , NULL  , (char *)"August" },
  { CRC_MS_LMSEPTEMBER        , NULL  , (char *)"September" },
  { CRC_MS_LMOCTOBER          , NULL  , (char *)"October" },
  { CRC_MS_LMNOVEMBER         , NULL  , (char *)"November" },
  { CRC_MS_LMDECEMBER         , NULL  , (char *)"December" },

  { CRC_MS_UPTIME             , NULL  , (char *)"%days day(s) %hours:%minutes:%seconds.%useconds" },

  { CRC_ST_IMPORTFILE         , NULL  , (char *)"Import File" },
  { CRC_WT_IMPORTWHICHFILE    , NULL  , (char *)" Import which file? " },
  { CRC_WT_IMPORTPICK         , NULL  , (char *)" Import " },
  { CRC_WT_IMPORTTXT          , NULL  , (char *)" Import " },
  { CRC_MI_IMPORTTXTTEXT      , NULL  , (char *)"T File as Text   " },
  { CRC_MI_IMPORTTXTQUOTE     , NULL  , (char *)"Q File as Quote  " },
  { CRC_MI_IMPORTTXTUUE       , NULL  , (char *)"U Binary as UUE  " },
  { CRC_MI_IMPORTTXTMIME      , NULL  , (char *)"M Binary as MIME " },
  { CRC_MI_IMPORTTXTCLIP      , NULL  , (char *)"C From Clipboard " },
  { CRC_MI_IMPORTTXTXLAT      , NULL  , (char *)"s Use Charset..  " },
  { CRC_MI_IMPORTTXTQUIT      , NULL  , (char *)"N Nothing / Quit " },
  { CRC_ST_IMPORTSTATUS       , NULL  , (char *)"Importing from %s" },
  { CRC_ST_EXPORTFILE         , NULL  , (char *)"Export File" },
  { CRC_WT_EXPORTWHATFILE     , NULL  , (char *)" Export to what file? " },
  { CRC_ST_EXPORTSTATUS       , NULL  , (char *)"Exporting to %s" },
  { CRC_ST_EDITSTATUS         , NULL  , (char *)"Edit %i,%i (%02X). %s" },
  { CRC_ST_SELECTFILES        , NULL  , (char *)"Select Files" },
  { CRC_WL_SELECTEDFILES      , NULL  , (char *)"Selected Files      " },
  { CRC_WL_SELECTEDBYTES      , NULL  , (char *)"         Bytes      " },
  { CRC_WL_TOTALFILES         , NULL  , (char *)"Total    Files      " },
  { CRC_WL_TOTALBYTES         , NULL  , (char *)"         Bytes      " },
  { CRC_WL_SCANNINGDIRECTORY  , NULL  , (char *)"             Scanning Directory             " },
  { CRC_WL_NOFILESFOUND       , NULL  , (char *)"             * NO FILES FOUND *             " },
  { CRC_ST_FILESPRESSKEY      , NULL  , (char *)"Press any key to continue" },
  { CRC_WT_AREA               , NULL  , (char *)"Area" },
  { CRC_WT_DESCRIPTION        , NULL  , (char *)"Description" },
  { CRC_WT_MSGS               , NULL  , (char *)"Msgs" },
  { CRC_WT_LAST               , NULL  , (char *)"Last" },
  { CRC_WT_ECHOID             , NULL  , (char *)"EchoID" },
  { CRC_ST_UNREAD             , NULL  , (char *)"unread" },
  { CRC_WT_SCANAREAS          , NULL  , (char *)" Scan Areas " },
  { CRC_MI_SCANALL            , NULL  , (char *)"A Scan All       " },
  { CRC_MI_SCANMARKED         , NULL  , (char *)"M Scan Marked    " },
  { CRC_MI_SCANCURRENT        , NULL  , (char *)"C Scan Current   " },
  { CRC_MI_SCANMATCHING       , NULL  , (char *)"t Scan Matching  " },
  { CRC_MI_SCANUNSCANNED      , NULL  , (char *)"U Scan Unscanned " },
  { CRC_MI_NOSCAN             , NULL  , (char *)"N No Scan  / ESC " },
  { CRC_WT_HEATAREAS          , NULL  , (char *)" Heat Areas " },
  { CRC_MI_HEATALL            , NULL  , (char *)"A Heat All      " },
  { CRC_MI_HEATMARKED         , NULL  , (char *)"M Heat Marked   " },
  { CRC_MI_HEATCURRENT        , NULL  , (char *)"C Heat Current  " },
  { CRC_MI_NOHEAT             , NULL  , (char *)"N No Heat / ESC " },
  { CRC_WT_ZAPAREAS           , NULL  , (char *)" Zap Areas " },
  { CRC_MI_ZAPALL             , NULL  , (char *)"A Zap All      " },
  { CRC_MI_ZAPMARKED          , NULL  , (char *)"M Zap Marked   " },
  { CRC_MI_ZAPCURRENT         , NULL  , (char *)"C Zap Current  " },
  { CRC_MI_NOZAP              , NULL  , (char *)"N No Zap / ESC " },
  { CRC_MS_DOS_SHELL          , NULL  , (char *)"GoldED Shell. Type EXIT To Return." },
  { CRC_IL_SCANNINGAREA       , NULL  , (char *)" Scanning Area:" },
  { CRC_IL_SEARCHINGFOR       , NULL  , (char *)" Searching for" },
  { CRC_ST_READINGMSG         , NULL  , (char *)"Reading Msg %u of %u" },
  { CRC_ER_OUTOFMEM           , NULL  , (char *)"Out of memory!" },
  { CRC_MS_HEXDUMPHEAD        , NULL  , (char *)"Hexdump of message header:" },
  { CRC_MS_HEXDUMPTEXT        , NULL  , (char *)"Hexdump of message text:" },
  { CRC_ST_RENUMBERING        , NULL  , (char *)"Renumbering" },
  { CRC_ST_LOCKED             , NULL  , (char *)" (locked)" },
  { CRC_ST_RENUMBERED         , NULL  , (char *)"Messages Renumbered - Press Key" },
  { CRC_WL_BLANKMSG           , NULL  , (char *)"" },
  { CRC_WL_WAIT               , NULL  , (char *)" Wait " },
  { CRC_IL_GENHEXDUMP         , NULL  , (char *)" Generating Hexdump - Please Wait " },
  { CRC_ST_PROCESSCC          , NULL  , (char *)"Processing Carbon Copies" },
  { CRC_ST_STATUSCC           , NULL  , (char *)"CC: %s of %s" },
  { CRC_ST_DESTINATIONCC      , NULL  , (char *)"Select Carbon Copy Destination" },
  { CRC_MS_LISTCC             , NULL  , (char *)"%s  %s" },
  { CRC_WT_DELORIG            , NULL  , (char *)" Delete Original? " },
  { CRC_MI_DELORIGYES         , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_DELORIGNO          , NULL  , (char *)"N           No!          " },
  { CRC_WT_DROPMSG            , NULL  , (char *)" Drop This Msg? " },
  { CRC_MI_DROPMSGYES         , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_DROPMSGNO          , NULL  , (char *)"N           No!          " },
  { CRC_WT_ZONEGATE           , NULL  , (char *)" Send via ZoneGate? " },
  { CRC_MI_ZONEGATEYES        , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_ZONEGATENO         , NULL  , (char *)"N           No!          " },
  { CRC_ST_QUOTEPCT           , NULL  , (char *)"Your Msg Contains %i%% Quotes%s" },
  { CRC_WT_SAVEMSG            , NULL  , (char *)" Save these %i lines? " },
  { CRC_MI_YESGREAT           , NULL  , (char *)"Y Yes, it's great stuff " },
  { CRC_MI_KICKIT             , NULL  , (char *)"N No, drop it..   / ESC " },
  { CRC_MI_CONTINUE           , NULL  , (char *)"C Continue writing      " },
  { CRC_MI_ROT13              , NULL  , (char *)"R ROT13 Crypting        " },
  { CRC_MI_ATTRS              , NULL  , (char *)"A Change Attributes     " },
  { CRC_MI_ORIGS              , NULL  , (char *)"O Change Origin         " },
  { CRC_MI_VIEW               , NULL  , (char *)"V View the message      " },
  { CRC_ST_SELECTDESTNODE     , NULL  , (char *)"Select Destination Node" },
  { CRC_WT_ATTACHFILES        , NULL  , (char *)" Attach Files " },
  { CRC_WT_UPDREQFILES        , NULL  , (char *)" Update Request Files " },
  { CRC_ST_EDITHEADER         , NULL  , (char *)"Edit Header Data" },
  { CRC_WT_EDITING            , NULL  , (char *)" Editing " },
  { CRC_MI_INTERNALED         , NULL  , (char *)"I Internal Editor " },
  { CRC_MI_EXTERNALED         , NULL  , (char *)"E External Editor " },
  { CRC_MI_SAVEMESSAGE        , NULL  , (char *)"S Save Message    " },
  { CRC_MI_ATTRO              , NULL  , (char *)"A Attributes      " },
  { CRC_MI_TEMPLATE           , NULL  , (char *)"T Templates       " },
  { CRC_MI_ORIGIN             , NULL  , (char *)"O Origins         " },
  { CRC_MI_QUITMESSAGE        , NULL  , (char *)"Q Quit      / ESC " },
  { CRC_ST_TEMPLATES          , NULL  , (char *)" Templates " },
  { CRC_WT_CHANGETEMPLATES    , NULL  , (char *)"Change Default Template" },
  { CRC_WT_CARBONCOPY         , NULL  , (char *)" Carbon Copy " },
  { CRC_MI_CCPROCESS          , NULL  , (char *)"P Process CC's " },
  { CRC_MI_CCIGNORE           , NULL  , (char *)"I Ignore  CC's " },
  { CRC_MI_CCATTRIBS          , NULL  , (char *)"C Change Attrs " },
  { CRC_MI_CCLISTFMT          , NULL  , (char *)"L List Format  " },
  { CRC_WT_CCLIST             , NULL  , (char *)" CC List " },
  { CRC_MI_CCLISTKEEP         , NULL  , (char *)"K Keep       " },
  { CRC_MI_CCLISTNAMES        , NULL  , (char *)"N Names only " },
  { CRC_MI_CCLISTVISIBLE      , NULL  , (char *)"V Visible    " },
  { CRC_MI_CCLISTHIDDEN       , NULL  , (char *)"H Hidden     " },
  { CRC_MI_CCLISTREMOVE       , NULL  , (char *)"R Remove     " },
  { CRC_WT_ATTRTITLE          , NULL  , (char *)" Attributes " },
  { CRC_MI_ATTR01             , NULL  , (char *)" Private             <Alt-P>   Archive/Sent         <Alt-B> " },
  { CRC_MI_ATTR02             , NULL  , (char *)" Crash               <Alt-C>   Direct               <Alt-D> " },
  { CRC_MI_ATTR03             , NULL  , (char *)" Received            <Alt-R>   Zonegate             <Alt-G> " },
  { CRC_MI_ATTR04             , NULL  , (char *)" Sent                <Alt-S>   Hub/Host-Route       <Alt-V> " },
  { CRC_MI_ATTR05             , NULL  , (char *)" File Attach         <Alt-A>   Immediate            <Alt-I> " },
  { CRC_MI_ATTR06             , NULL  , (char *)" Transit             <Alt-J>   Xmail                <Alt-X> " },
  { CRC_MI_ATTR07             , NULL  , (char *)" Orphan              <Alt-O>   Erase File/Sent      <Alt-E> " },
  { CRC_MI_ATTR08             , NULL  , (char *)" Kill/Sent           <Alt-K>   Trunc File/Sent      <Alt-T> " },
  { CRC_MI_ATTR09             , NULL  , (char *)" Local               <Alt-W>   Locked               <Alt-L> " },
  { CRC_MI_ATTR10             , NULL  , (char *)" Hold                <Alt-H>   Confirm Rcpt Request <Alt-Y> " },
  { CRC_MI_ATTR11             , NULL  , (char *)" File Request        <Alt-F>   FTS1 Reserved        <Alt-1> " },
  { CRC_MI_ATTR12             , NULL  , (char *)" Return Rcpt Request <Alt-M>   QBBS Reserved, Net   <Alt-2> " },
  { CRC_MI_ATTR13             , NULL  , (char *)" Return Rcpt         <Alt-N>   QBBS Reserved, Echo  <Alt-3> " },
  { CRC_MI_ATTR14             , NULL  , (char *)" Audit Request       <Alt-Q>   Squish Scanned       <Alt-4> " },
  { CRC_MI_ATTR15             , NULL  , (char *)" File Update Request <Alt-U>   Zap all attribs      <Alt-Z> " },
  { CRC_HD_FROM               , NULL  , (char *)" From : " },
  { CRC_HD_TO                 , NULL  , (char *)" To   : " },
  { CRC_HD_SUBJ               , NULL  , (char *)" Subj : " },
  { CRC_HD_FILE               , NULL  , (char *)" File : " },
  { CRC_MS_EMPTYMSG           , NULL  , (char *)"\r" },
  { CRC_MS_AUTOATTACHMSG      , NULL  , (char *)"\r" },
  { CRC_MS_AUTOREQUESTMSG     , NULL  , (char *)"\r" },
  { CRC_MS_AUTOUPDREQMSG      , NULL  , (char *)"\r" },
  { CRC_WT_FILEATTACH         , NULL  , (char *)" File Attach " },
  { CRC_WT_FILEREQUEST        , NULL  , (char *)" File Request " },
  { CRC_WT_FILEUPDREQ         , NULL  , (char *)" Update Request " },
  { CRC_ST_FILEATTACHING      , NULL  , (char *)"File Attaching [%i/%i] to %u:%u/%u.%u" },
  { CRC_ST_FILEREQUESTING     , NULL  , (char *)"File Requesting [%i/%i] from %u:%u/%u.%u" },
  { CRC_ST_FILEUPDREQING      , NULL  , (char *)"Update Requesting [%i/%i] from %u:%u/%u.%u" },
  { CRC_ST_READMARKED         , NULL  , (char *)"Read Marked - Msg %u of %u (%u left)" },
  { CRC_ST_READALL            , NULL  , (char *)"Read All - Msg %u of %u (%u left)" },
  { CRC_ST_NOQRENUM           , NULL  , (char *)"This area cannot be renumbered" },
  { CRC_MS_HIDINGTWIT         , NULL  , (char *)"This is a Twit Message - Press <Home> to read."},
  { CRC_HD_VIA                , NULL  , (char *)"via" },
  { CRC_WT_CHANGEATTRS        , NULL  , (char *)"Change Message Attributes" },
  { CRC_WT_HEADERTEXT         , NULL  , (char *)" Enter Searchstring (Header+Text) " },
  { CRC_WT_HEADERONLY         , NULL  , (char *)" Enter Searchstring (Header Only) " },
  { CRC_WT_NEWAREA            , NULL  , (char *)">>Pick New Area: " },
  { CRC_WT_REPLYAREA          , NULL  , (char *)">>Answer In Area: " },
  { CRC_WT_COPYAREA           , NULL  , (char *)">>Copy To Area: " },
  { CRC_WT_MOVEAREA           , NULL  , (char *)">>Move To Area: " },
  { CRC_WT_FORWARDAREA        , NULL  , (char *)">>Forward To Area: " },
  { CRC_WT_FREQAREA           , NULL  , (char *)">>Filerequest In Area: " },
  { CRC_WT_FREQMENUTITLE      , NULL  , (char *)"Requestable files" },
  { CRC_ST_FREQSTAT           , NULL  , (char *)"Pick files to request" },
  { CRC_IL_FREQINFONOFILES    , NULL  , (char *)" No requestable files were found in this msg! " },
  { CRC_WT_COPY               , NULL  , (char *)" Copy " },
  { CRC_WT_MOVE               , NULL  , (char *)" Move " },
  { CRC_WT_COPYING            , NULL  , (char *)" Copying " },
  { CRC_WT_MOVING             , NULL  , (char *)" Moving " },
  { CRC_ST_COPYINGMSG         , NULL  , (char *)"Copying Msg %u of %u to %s" },
  { CRC_ST_MOVINGMSG          , NULL  , (char *)"Moving Msg %u of %u to %s" },
  { CRC_WT_DELETE             , NULL  , (char *)" Delete " },
  { CRC_WT_DELETING           , NULL  , (char *)" Deleting " },
  { CRC_ST_DELETINGMSG        , NULL  , (char *)"Deleting Msg %u of %u" },
  { CRC_WT_WRITE              , NULL  , (char *)" Write " },
  { CRC_WT_WRITEMSGS          , NULL  , (char *)" Write Msg(s) to File " },
  { CRC_WT_WRITING            , NULL  , (char *)" Writing " },
  { CRC_ST_WRITINGMSG         , NULL  , (char *)"Writing Msg %u of %u" },
  { CRC_WT_WRITINGFILE        , NULL  , (char *)" Writing Msg(s) to File %s " },
  { CRC_WT_WRITINGPRN         , NULL  , (char *)" Writing Msg(s) to Print Device " },
  { CRC_IL_READONLYWARN       , NULL  , (char *)" This area is READ ONLY! " },
  { CRC_WT_ISREADONLY         , NULL  , (char *)" Write msg anyway? " },
  { CRC_MI_READONLYYES        , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_READONLYNO         , NULL  , (char *)"N           No!          " },
  { CRC_IL_CHANGEWARN         , NULL  , (char *)" This msg is NOT from you! " },
  { CRC_WT_CHANGE             , NULL  , (char *)" Change this msg? " },
  { CRC_MI_CHANGEYES          , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_CHANGENO           , NULL  , (char *)"N           No!          " },
  { CRC_WT_DELETETHIS         , NULL  , (char *)" Delete this msg? " },
  { CRC_MI_DELETEYES          , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_DELETENO           , NULL  , (char *)"N           No!          " },
  { CRC_MI_DELETENOASK        , NULL  , (char *)"D       Don't ask..      " },
  { CRC_WT_GOTONEXT           , NULL  , (char *)" Goto Next Area? " },
  { CRC_MI_GOTONEXTYES        , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_GOTONEXTNO         , NULL  , (char *)"N           No!          " },
  { CRC_MI_GOTONEXTNEW        , NULL  , (char *)"U Yes, Next Unread Area. " },
  { CRC_WT_FORWARD            , NULL  , (char *)" Use FWD kludges? " },
  { CRC_MI_FORWARDYES         , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_FORWARDNO          , NULL  , (char *)"N           No!          " },
  { CRC_WT_MSG                , NULL  , (char *)"Msg" },
  { CRC_WT_MSGREAL            , NULL  , (char *)"Msg#" },
  { CRC_WT_FROML              , NULL  , (char *)"From" },
  { CRC_WT_TOL                , NULL  , (char *)"To" },
  { CRC_WT_SUBJL              , NULL  , (char *)"Subj" },
  { CRC_ST_MSGLISTER          , NULL  , (char *)"Lister - Msg %u of %u (%u left)" },
  { CRC_ST_COPYMOVEFORWARD    , NULL  , (char *)"Copy, Move or Forward this message" },
  { CRC_WT_SELECTACTION       , NULL  , (char *)" Action " },
  { CRC_MI_FORWARDMESSAGE     , NULL  , (char *)"F Forward Msg " },
  { CRC_MI_MOVEMESSAGE        , NULL  , (char *)"M Move    Msg " },
  { CRC_MI_COPYMESSAGE        , NULL  , (char *)"C Copy    Msg " },
  { CRC_MI_TOGGLESENT         , NULL  , (char *)"T Toggle sent " },
  { CRC_MI_QUITCMF            , NULL  , (char *)"Q Quit  / ESC " },
  { CRC_ST_ARE                , NULL  , (char *)"messages are" },
  { CRC_ST_IS                 , NULL  , (char *)"message is" },
  { CRC_ST_MARKED             , NULL  , (char *)"marked" },
  { CRC_MI_MARKEDMSG          , NULL  , (char *)"M Marked Msgs " },
  { CRC_MI_CURRENTMSG         , NULL  , (char *)"C Current Msg " },
  { CRC_MI_QUITMSGS           , NULL  , (char *)"Q Quit  / ESC " },
  { CRC_ST_WRITEMSGSTO        , NULL  , (char *)"Write Msg(s) to File or Printer" },
  { CRC_WT_WRITETO            , NULL  , (char *)" Write To: " },
  { CRC_MI_DISKFILE           , NULL  , (char *)"D Disk File    " },
  { CRC_MI_PRINTER            , NULL  , (char *)"P Print device " },
  { CRC_MI_QUITWRITE          , NULL  , (char *)"Q Quit   / ESC " },
  { CRC_ST_MARKINGOPTIONS     , NULL  , (char *)"Message Marking" },
  { CRC_WT_MARKWHAT           , NULL  , (char *)" Mark What? " },
  { CRC_MI_YOURMAIL           , NULL  , (char *)"Y Your personal mail     " },
  { CRC_MI_FROMTOSUBJ         , NULL  , (char *)"H Header (From/To/Subj)  " },
  { CRC_MI_TEXTHDR            , NULL  , (char *)"T Text and header        " },
  { CRC_MI_THREAD             , NULL  , (char *)"R Reply thread           " },
  { CRC_MI_ASREAD             , NULL  , (char *)"s Mark thread as read    " },
  { CRC_MI_NEWMSGS            , NULL  , (char *)"N New msgs      >current " },
  { CRC_MI_OLDMSGS            , NULL  , (char *)"O Old msgs      <current " },
  { CRC_MI_ALLMSGS            , NULL  , (char *)"A All msgs               " },
  { CRC_MI_UNMARK             , NULL  , (char *)"U Unmark all msgs        " },
  { CRC_MI_RANGE              , NULL  , (char *)"M Msg range (bookm-curr) " },
  { CRC_MI_MARKSTOGGLE        , NULL  , (char *)"g Toggle all marks       " },
  { CRC_MI_QUITMARKS          , NULL  , (char *)"Q Quit             / ESC " },
  { CRC_WT_ENTERMARKSTRING    , NULL  , (char *)" Enter Marking String " },
  { CRC_ST_SEARCHINGMSG       , NULL  , (char *)"Searching Msg %u of %u. Marked: %u" },
  { CRC_WT_USERLISTNAME       , NULL  , (char *)" Userlist filename? " },
  { CRC_IL_GENUSERLIST        , NULL  , (char *)" Generating Userlist " },
  { CRC_WT_FILEEXISTS         , NULL  , (char *)" File Exists! " },
  { CRC_MI_APPEND             , NULL  , (char *)"A Append to end of file   " },
  { CRC_MI_OVERWRITE          , NULL  , (char *)"O Overwrite the file      " },
  { CRC_MI_QUITEXIST          , NULL  , (char *)"R Re-enter filename / ESC " },
  { CRC_IL_WARNUNSENT         , NULL  , (char *)" WARNING! This message is NOT sent! " },
  { CRC_IL_WARNLOCKED         , NULL  , (char *)" WARNING! This message is LOcKed! "},
  { CRC_ST_CHANGEORIGIN       , NULL  , (char *)"Change Default Origin" },
  { CRC_WT_ORIGINS            , NULL  , (char *)" Origins " },
  { CRC_ST_CHANGEUSERNAME     , NULL  , (char *)"Change Default Username" },
  { CRC_WT_USERNAMES          , NULL  , (char *)" Usernames " },
  { CRC_ST_CHANGEAKA          , NULL  , (char *)"Change Default Address AKA" },
  { CRC_WT_AKAS               , NULL  , (char *)" AKA's " },
  { CRC_WT_LOOKUP             , NULL  , (char *)" Lookup: %s " },
  { CRC_WL_PHONE              , NULL  , (char *)"Phone" },
  { CRC_WT_QUITGOLDED         , NULL  , (char *)" Quit GoldED? " },
  { CRC_MI_QUITYES            , NULL  , (char *)"Y       Yes Please.      " },
  { CRC_MI_QUITNO             , NULL  , (char *)"N           No!          " },
  { CRC_MS_EDITCMD            , NULL  , (char *)"Editor: %s" },
  { CRC_IL_NOORIGDEFINED      , NULL  , (char *)" No origins defined. Press key " },
  { CRC_IL_NOUSERDEFINED      , NULL  , (char *)" No usernames defined. Press key " },
  { CRC_IL_NOAKADEFINED       , NULL  , (char *)" No AKA's defined. Press key " },
  { CRC_IL_NOTPLDEFINED       , NULL  , (char *)" No templates defined. Press key " },
  { CRC_IL_NOTHREADLIST       , NULL  , (char *)" Sorry, no thread available. Press key "},
  { CRC_MS_SKIPPINGTWIT       , NULL  , (char *)"Skipping Twit Message..." },
  { CRC_MS_SKIPPINGDELETED    , NULL  , (char *)"Skipping Deleted Message..." },
  { CRC_MS_KILLINGTWIT        , NULL  , (char *)"Killing Twit Message..." },
  { CRC_IL_WRITINGCFG         , NULL  , (char *)" Writing Configuration " },
  { CRC_IL_COULDNOTOPEN       , NULL  , (char *)" Couldn't open %s " },
  { CRC_MS_PROMPT             , NULL  , (char *)"Enter the command \"EXIT\" to return to GoldED.\r\n" },
  { CRC_IL_UNFINISHEDMSG      , NULL  , (char *)" Unfinished message found! " },
  { CRC_ST_LOADUNFINISHED     , NULL  , (char *)"Press any key to load it or <ESC> to ignore" },
  { CRC_IL_READINGECHOLIST    , NULL  , (char *)"Reading Echolist Files" },
  { CRC_IL_READINGADDRMACROS  , NULL  , (char *)"Reading Address Macros" },
  { CRC_IL_CHECKINGNODELISTS  , NULL  , (char *)"Checking Nodelists" },
  { CRC_ST_CROSSPOSTING       , NULL  , (char *)"Crossposting in %s" },
  { CRC_IL_TWITBLANKED        , NULL  , (char *)" Twit Msgs Blanked " },
  { CRC_IL_TWITSKIPPED        , NULL  , (char *)" Twit Msgs Skipped " },
  { CRC_IL_TWITIGNORESKIP     , NULL  , (char *)" Twit Msgs Ignored and Skipped " },
  { CRC_IL_TWITDISPLAYED      , NULL  , (char *)" Twit Msgs Displayed " },
  { CRC_IL_TWITKILLED         , NULL  , (char *)" Twit Msgs Killed " },
  { CRC_IL_STYLECODESNO       , NULL  , (char *)" Stylecodes disabled " },
  { CRC_IL_STYLECODESYES      , NULL  , (char *)" Stylecodes enabled " },
  { CRC_IL_STYLECODESHIDE     , NULL  , (char *)" Stylecodes enabled and stripped " },
  { CRC_ST_GENCFMRECEIPT      , NULL  , (char *)"Generating Confirmation Receipt" },
  { CRC_WT_NEW                , NULL  , (char *)"New" },
  { CRC_ST_LOOKUPINFO         , NULL  , (char *)"Nodelist Lookup Information" },
  { CRC_MS_DATETIMEFMT        , NULL  , (char *)"%d %b %y %H:%M:%S" },
  { CRC_MS_DATEFMT            , NULL  , (char *)"%d %b %y" },
  { CRC_MS_TIMEFMT            , NULL  , (char *)"%H:%M" },
  { CRC_MS_ORIGINALLYIN       , NULL  , (char *)"* Originally in %s" },
  { CRC_MS_CROSSPOSTEDIN      , NULL  , (char *)"* Crossposted in %s" },
  { CRC_MS_CCTO               , NULL  , (char *)"* Carbon copied to %s" },
  { CRC_ST_STATUSLINEHELP     , NULL  , (char *)"F1 Help" },
  { CRC_HD_OF                 , NULL  , (char *)"of" },
  { CRC_MS_ROBOTMSG           , NULL  , (char *)"\r--- @longpid @version\r" },
  { CRC_ST_STATUSLINETIMEFMT  , NULL  , (char *)"%H:%M:%S" },

  { CRC_WT_REPLIES            , NULL  , (char *)" Replies " },
  { CRC_ST_SELECTREPLY        , NULL  , (char *)"Select the reply you want to see" },
  { CRC_IL_WAITOREXIT         , NULL  , (char *)" At this point you must either wait or exit GoldED entirely - Sorry! " },
  { CRC_WT_REALLYEXIT         , NULL  , (char *)" Really exit? " },
  { CRC_MI_TAGLINES           , NULL  , (char *)"T Change Tagline" },
  { CRC_MI_HEADEREDIT         , NULL  , (char *)"H Edit Header" },
  { CRC_MI_SCANGROUP          , NULL  , (char *)"G Scan Group" },
  { CRC_WT_QWKPACKET          , NULL  , (char *)" QWK Packet " },
  { CRC_MI_SCANQWK            , NULL  , (char *)"Q QWK Packet" },
  { CRC_WT_SOUPPACKET         , NULL  , (char *)" SOUP Packet " },
  { CRC_MI_SCANSOUP           , NULL  , (char *)"S SOUP Packet" },
  { CRC_MI_SCANIMPORT         , NULL  , (char *)"I Import" },
  { CRC_MI_SCANEXPORT         , NULL  , (char *)"E Export" },
  { CRC_WT_SCANPM             , NULL  , (char *)" Scan PM " },
  { CRC_MI_TAGLINE            , NULL  , (char *)"g Taglines" },
  { CRC_WT_CONFIRM            , NULL  , (char *)" Honor receipt request? " },
  { CRC_MI_CONFIRMYES         , NULL  , (char *)"Y Yes, send a receipt msg " },
  { CRC_MI_CONFIRMNO          , NULL  , (char *)"N No, ignore the request  " },
  { CRC_IL_CONFIRMINFO        , NULL  , (char *)" The sender of this message requests confirmation of receipt (CFM) " },
  { CRC_ST_MSG                , NULL  , (char *)"msg" },
  { CRC_ST_MSGS               , NULL  , (char *)"msgs" },
  { CRC_ST_PERSONAL           , NULL  , (char *)"personal" },
  { CRC_WT_GRP                , NULL  , (char *)"Grp" },
  { CRC_IL_FOUNDPERSONAL      , NULL  , (char *)" Found %u personal mail%s in %u area%s " },
  { CRC_IL_NOPERSONAL         , NULL  , (char *)" No personal mail found " },
  { CRC_ST_ESCORCONTINUE      , NULL  , (char *)"ESC exits. Other keys: Tries to continue" },
  { CRC_MS_SPELLCHECKER       , NULL  , (char *)"SpellChecker: %s" },
  { CRC_WT_INS                , NULL  , (char *)" Ins " },
  { CRC_WT_DRAWSL             , NULL  , (char *)" DL1 " },
  { CRC_WT_DRAWDL             , NULL  , (char *)" DL2 " },
  { CRC_MI_FILELISTFROM       , NULL  , (char *)"Filelist from %s" },
  { CRC_ST_INITIALIZING       , NULL  , (char *)"Initializing ..." },
  { CRC_ST_CHECKING           , NULL  , (char *)"Checking" },
  { CRC_ST_LOCKSHARECAP       , NULL  , (char *)"%s Lock/Share Capability" },
  { CRC_WT_ATTRTURNOFF        , NULL  , (char *)" Press Alt-F1 to turn off this window " },
  { CRC_WT_TAGLINES           , NULL  , (char *)" Taglines " },
  { CRC_ST_CHANGETAGLINE      , NULL  , (char *)"Change Default Tagline" },
  { CRC_IL_NOTAGLINE          , NULL  , (char *)" No taglines defined. Press key " },
  { CRC_WT_CHARSETS           , NULL  , (char *)" Charsets " },
  { CRC_ST_CHANGEXLATIMP      , NULL  , (char *)"Change Default Import Charset" },
  { CRC_MI_CHARSETAUTO        , NULL  , (char *)" Auto " },
  { CRC_IL_NOXLATIMPORT       , NULL  , (char *)" No XLATIMPORT's defined. Press key " },
  { CRC_WT_N_A                , NULL  , (char *)"n/a" },
  { CRC_WT_WRITTEN            , NULL  , (char *)"Written" },
  { CRC_WT_ARRIVED            , NULL  , (char *)"Arrived" },
  { CRC_WT_RECEIVED           , NULL  , (char *)"Received" },
  { CRC_IL_NONODELIST         , NULL  , (char *)" Unable to access the nodelist or index files " },
  { CRC_IL_NODELISTMISSING    , NULL  , (char *)"Nodelist missing:" },
  { CRC_IL_NODELISTOUTDATED   , NULL  , (char *)"Nodelist out of date:" },
  { CRC_MS_REPLYLINKER        , NULL  , (char *)"Replylinker: %s" },
  { CRC_WT_ENTERMSGNO         , NULL  , (char *)" Enter msgno " },
  { CRC_IL_WAITUUDECODING     , NULL  , (char *)" Wait - uudecoding " },
  { CRC_IL_COMPLETEDUUDECODE  , NULL  , (char *)" Completed uudecode of %s " },
  { CRC_IL_NOTUUENCODED       , NULL  , (char *)" Could not find anything to uudecode " },
  { CRC_IL_UUEINVALIDPATH     , NULL  , (char *)" Invalid path for uudecoding - file not written " },
  { CRC_IL_TMPINVALIDPATH     , NULL  , (char *)" Invalid path for temporary files (TEMPPATH) - do nothing " },
  { CRC_IL_PATHREPORT         , NULL  , (char *)" Generating PATH report " },
  { CRC_IL_ERRORINSOUP        , NULL  , (char *)" Error in SOUP file %s " },
  { CRC_IL_WARNALREADYSENT    , NULL  , (char *)" WARNING: This message is already sent! " },
  { CRC_IL_WAITLOCKED         , NULL  , (char *)" Wait: %s is locked " },
  { CRC_ST_RETRYORESC         , NULL  , (char *)"%s retry (%lu). Press ESC to exit GoldED." },
  { CRC_ST_RETRYLOCK          , NULL  , (char *)"Lock" },
  { CRC_ST_RETRYOPEN          , NULL  , (char *)"Open" },
  { CRC_WT_TOUCHSEMAPHORE     , NULL  , (char *)" Enter name of semaphore file to touch " },
  { CRC_MI_WRITETOHDRNO       , NULL  , (char *)"H Use Header: NO   " },
  { CRC_MI_WRITETOHDRYES      , NULL  , (char *)"H Use Header: YES  "},
  { CRC_MI_WRITETOHDRONLY     , NULL  , (char *)"H Use Header: ONLY "},
  { CRC_MI_CLIPBOARD          , NULL  , (char *)"C Clipboard "},
  { CRC_WT_SELECTMARKS        , NULL  , (char *)" Select Mark " },
  { CRC_WT_SELECTMARKSEDIT    , NULL  , (char *)" Edit Mark Description " },
  { CRC_IL_DROPMARKSINFO      , NULL  , (char *)" %s Messages Are Marked " },
  { CRC_WT_DROPMARKS          , NULL  , (char *)" Drop Msg Marks " },
  { CRC_MI_DROPALL            , NULL  , (char *)"A Drop All       " },
  { CRC_MI_DROPMARKED         , NULL  , (char *)"M Drop Marked    " },
  { CRC_MI_DROPCURRENT        , NULL  , (char *)"C Drop Current   " },
  { CRC_MI_NODROP             , NULL  , (char *)"N No Drop / ESC  " },
  { CRC_WT_CATCHAREAS         , NULL  , (char *)" Catch-Up Areas " },
  { CRC_MI_CATCHALL           , NULL  , (char *)"A Catch All      " },
  { CRC_MI_CATCHMARKED        , NULL  , (char *)"M Catch Marked   " },
  { CRC_MI_CATCHCURRENT       , NULL  , (char *)"C Catch Current  " },
  { CRC_MI_NOCATCH            , NULL  , (char *)"N No Catch / ESC " },
  { CRC_WT_CROSSPOST          , NULL  , (char *)" Crosspost " },
  { CRC_MI_XCPROCESS          , NULL  , (char *)"P Process XC's " },
  { CRC_MI_XCIGNORE           , NULL  , (char *)"I Ignore  XC's " },
  { CRC_MI_XCLISTFMT          , NULL  , (char *)"L List Format  " },
  { CRC_WT_XCLIST             , NULL  , (char *)" XC List " },
  { CRC_MI_XCLISTRAW          , NULL  , (char *)"K Keep       " },
  { CRC_MI_XCLISTVERBOSE      , NULL  , (char *)"V Verbose    " },
  { CRC_MI_XCLISTLINE         , NULL  , (char *)"L Line       " },
  { CRC_MI_XCLISTREMOVE       , NULL  , (char *)"R Remove     " },

  { CRC_WT_ADDRESSBOOK        , NULL  , (char *)" Addressbook " },
  { CRC_WT_ADVANCEDSEARCH     , NULL  , (char *)" Advanced Search " },
  { CRC_IL_NOMOREMATCHES      , NULL  , (char *)" No more matches found " },
  { CRC_WT_HEADEREDITHELP_1   , NULL  , (char *)" Shift-F10: Nodelist " },
  { CRC_WT_HEADEREDITHELP_2   , NULL  , (char *)" F10: Addressbook " },
  { CRC_WT_THREADLISTTITLE    , NULL  , (char *)" Message Thread List " },
  { CRC_WT_ADVANCEDMARKING    , NULL  , (char *)" Advanced Marking " },
  { CRC_ST_USERSTATUSLINE     , NULL  , (char *)"GoldED's Address Book - %d of %d (%d left)" },
  { CRC_WT_USERHEADERNAME     , NULL  , (char *)"Name" },
  { CRC_WT_USERHEADERORG      , NULL  , (char *)"Organization" },
  { CRC_WT_USERHEADERAKA      , NULL  , (char *)"Address" },
  { CRC_IL_USERWAIT           , NULL  , (char *)" Wait - Browsing Address Book... " },

  { CRC_MI_ALSELECTIONS1      , NULL  , (char *)"Mark Areas with New Mail" },
  { CRC_MI_ALSELECTIONS2      , NULL  , (char *)"(unused)" },
  { CRC_IL_DECODE             , NULL  , (char *)"Decode" },
  { CRC_IL_PREPARING          , NULL  , (char *)"Preparing" },

  { CRC_WT_EXTERNUTIL         , NULL  , (char *)"External utilities" },
  { CRC_ST_EXTERNUTIL         , NULL  , (char *)"Select external utility to execute" },
  { CRC_WT_EXECCMDLINE        , NULL  , (char *)"Enter command line to execute" },
  { CRC_MI_ENTERCMDLINE       , NULL  , (char *)" Enter command line " },

  { CRC_WT_PEEKURL            , NULL  , (char *)"Available URLs" },
  { CRC_ST_PEEKURL            , NULL  , (char *)"Select URL" },
  { CRC_IL_PEEKINFONOURLS     , NULL  , (char *)" No valid URLs were found in this msg! " },
  { CRC_IL_PEEKINFONOHANDLER  , NULL  , (char *)" URL handler are not defined in config file! " },

  { CRC_MI_MMMAIN             , NULL  , (char *)"M Main " },
  { CRC_MI_MMSHELL            , NULL  , (char *)"S Shell to OS " },
  { CRC_MI_MMQUIT             , NULL  , (char *)"Q Quit GoldED " },
  { CRC_MI_MMEDIT             , NULL  , (char *)"E Edit " },
  { CRC_MI_MMNEWMSG           , NULL  , (char *)"E Enter new msg                        " },
  { CRC_MI_MMQUOTEREPLY       , NULL  , (char *)"Q Quote-reply                          " },
  { CRC_MI_MMDIRQUOTEREPLY    , NULL  , (char *)"D Direct quote-reply (ignore Reply-To) " },
  { CRC_MI_MMCMTREPLY         , NULL  , (char *)"C Comment-reply                        " },
  { CRC_MI_MMOTHERAREA        , NULL  , (char *)"O Other area reply >                   " },
  { CRC_WT_MMOTHERAREA        , NULL  , (char *)" Other Area " },
  { CRC_MI_MMOQUOTEREPLY      , NULL  , (char *)"Q Quote-reply                          " },
  { CRC_MI_MMODIRQUOTEREPLY   , NULL  , (char *)"D Direct quote-reply (ignore Reply-To) " },
  { CRC_MI_MMOCMTREPLY        , NULL  , (char *)"C Comment-reply                        " },
  { CRC_MI_MMCHANGE           , NULL  , (char *)"C Change " },
  { CRC_MI_MMORIGIN           , NULL  , (char *)"O Origin    " },
  { CRC_MI_MMUSERNAME         , NULL  , (char *)"U Username  " },
  { CRC_MI_MMAKA              , NULL  , (char *)"A Aka       " },
  { CRC_MI_MMATTRIBUTES       , NULL  , (char *)"M Msg attrs " },
  { CRC_MI_MMTEMPLATE         , NULL  , (char *)"T Template  " },
  { CRC_MI_MMUTIL             , NULL  , (char *)"U Util " },
  { CRC_MI_MMCOPYMOVEFORWARD  , NULL  , (char *)"C Copy/Move/Forward     " },
  { CRC_MI_MMLIST             , NULL  , (char *)"L List messages         " },
  { CRC_MI_MMWRITE            , NULL  , (char *)"W Write to disk/printer " },
  { CRC_MI_MMFREQ             , NULL  , (char *)"F File request          " },

  { CRC_LAST_CRC              , NULL  , NULL }
};


//  ------------------------------------------------------------------
//  Compare two crcs

static int CmpLangCrc(LangCrc* a, LangCrc* b) {

  return((a->crc > b->crc) ? 1 : ((a->crc < b->crc) ? -1 : 0));
}


//  ------------------------------------------------------------------
//  Call the function matching the keyword

static bool SwitchLanguage(word crc, char* str)
{
  LangCrc* lptr;
  LangCrc lkey;
  lkey.crc = crc;

  switch (crc)
  {
  case CRC_ST_EDITSTATUS:
    gregex reg;
    reg.compile("^.*%[0-9-]*[dioux].*%[0-9-]*[dioux].*%[0-9-]*[dioux].*%[-0-9\\.]*s", gregex::icase);
    if (!reg.match(str))
    {
      STD_PRINTNL("ST_EDITSTATUS has wrong format. Read NOTEWORK.TXT for details or announce author if this error is false positive (first try to set true locale). Your format string is \"" << str << "\", should be 3 numbers and one string.");
      SayBibi();
      waitkeyt(10000);
    }
    break;
  }

  lptr = (LangCrc*)bsearch(&lkey, LangCrcs, sizeof(LangCrcs)/sizeof(LangCrc), sizeof(LangCrc), (StdCmpCP)CmpLangCrc);
  if(lptr != NULL) {
    if(lptr->langptr != NULL) {
      throw_xfree(*(lptr->langptr));
      *(lptr->langptr) = throw_strdup(StripQuotes(str));
    }
    return 0;
  } else
    return bool(-1);
}


//  ------------------------------------------------------------------
//  Unallocate all language strings

static void LangReset(void) {

  LangCrc* lngptr = LangCrcs;

  for(; lngptr->crc < CRC_LAST_CRC; lngptr++)
    if(lngptr->langptr != NULL)
      throw_xfree(*(lngptr->langptr));

  throw_release(LNG);

  int n;
  for(n=0; n<7; n++)
    gsweekday[n] = NULL;
  for(n=0; n<7; n++)
    glweekday[n] = NULL;
  for(n=0; n<12; n++)
    gsmonth[n] = NULL;
  for(n=0; n<12; n++)
    glmonth[n] = NULL;
}


//  ------------------------------------------------------------------
//  Initialize language

void LangInit() {

  LNG = (LangGed*)throw_calloc(1, sizeof(LangGed));
  LNG->Outofmem = throw_strdup("Out of memory!");

  // Init LangCrc pointers to LNG
  char** lptr = (char**)LNG;
  for(LangCrc* lngptr = LangCrcs; lngptr->crc != CRC_LAST_CRC; lngptr++, lptr++)
    lngptr->langptr = lptr;

  // Sort the crc list
  qsort(LangCrcs, sizeof(LangCrcs)/sizeof(LangCrc), sizeof(LangCrc), (StdCmpCP)CmpLangCrc);

  atexit(LangReset);
}


//  ------------------------------------------------------------------
//  Assign weekday/month names

void AssignDateNames() {

  gsweekday[0] = LNG->SWSun;
  gsweekday[1] = LNG->SWMon;
  gsweekday[2] = LNG->SWTue;
  gsweekday[3] = LNG->SWWed;
  gsweekday[4] = LNG->SWThu;
  gsweekday[5] = LNG->SWFri;
  gsweekday[6] = LNG->SWSat;

  glweekday[0] = LNG->LWSunday;
  glweekday[1] = LNG->LWMonday;
  glweekday[2] = LNG->LWTuesday;
  glweekday[3] = LNG->LWWednesday;
  glweekday[4] = LNG->LWThursday;
  glweekday[5] = LNG->LWFriday;
  glweekday[6] = LNG->LWSaturday;

  gsmonth[0]  = LNG->SMJan;
  gsmonth[1]  = LNG->SMFeb;
  gsmonth[2]  = LNG->SMMar;
  gsmonth[3]  = LNG->SMApr;
  gsmonth[4]  = LNG->SMMay;
  gsmonth[5]  = LNG->SMJun;
  gsmonth[6]  = LNG->SMJul;
  gsmonth[7]  = LNG->SMAug;
  gsmonth[8]  = LNG->SMSep;
  gsmonth[9]  = LNG->SMOct;
  gsmonth[10] = LNG->SMNov;
  gsmonth[11] = LNG->SMDec;

  glmonth[0]  = LNG->LMJanuary;
  glmonth[1]  = LNG->LMFebruary;
  glmonth[2]  = LNG->LMMarch;
  glmonth[3]  = LNG->LMApril;
  glmonth[4]  = LNG->LMMay;
  glmonth[5]  = LNG->LMJune;
  glmonth[6]  = LNG->LMJuly;
  glmonth[7]  = LNG->LMAugust;
  glmonth[8]  = LNG->LMSeptember;
  glmonth[9]  = LNG->LMOctober;
  glmonth[10] = LNG->LMNovember;
  glmonth[11] = LNG->LMDecember;
}


//  ------------------------------------------------------------------
//  Load a GoldED language file

void LoadLanguage(const char* file)
{
  char* ptr;
  char* str;
  int line = 0;
  char buf[256];

  if (file==NULL || *file==0)
  {
    return;
  }
  gfile fp(AddPath(CFG->goldpath, file), "rt", CFG->sharemode);
  if (fp.isopen())
  {
    fp.SetvBuf(NULL, _IOFBF, 8192);
    while (fp.Fgets((ptr=buf), sizeof(buf)))
    {
      line++;
      ptr = strskip_wht(ptr);
      if(g_isalpha(*ptr)) {
        str = ptr;
        strtrim(str);
        str = strskip_txt(str);
        if(*str) {
          *str++ = NUL;
          str = strskip_wht(str);
          strschg(str, "\\n", "\n");
          strschg(str, "\\r", "\r");
          strschg(str, "\\\"", "\"");
        }
        if(SwitchLanguage(strCrc16(strupr(ptr)), str)) {
          if(cmdlineoldkeyw == false) {
            w_infof(" %s line %u: \"%s\" is obsolete or unknown ", file, line, ptr);
            SayBibi();
            waitkeyt(10000);
            w_info(NULL);
            cfgerrors++;
          }
        }
      }
    }

    // Assign weekday/month names
    AssignDateNames();
  }
  else
    LOG.printf("? Can't load language file \"%s\", OS reports: %s!", file, strerror(errno));
}


//  ------------------------------------------------------------------
//  Read the main language definition file

bool ReadLangCfg(int force)
{
  char* ptr;
  char* str;
  long size;
  char buf[1024];
  LangCrc* lngptr;
  int line = 0;

  // Read the GOLDLANG.CFG if there is one
  const char* cfgname = AddPath(CFG->goldpath, CFG->langcfg);

  gfile fpi(cfgname, "rt", CFG->sharemode);
  if (fpi.isopen())
  {
    fpi.SetvBuf(NULL, _IOFBF, 8192);

    if (not quiet)
      STD_PRINTNL("* Reading " << cfgname);

    cfgname = CleanFilename(cfgname);

    while (fpi.Fgets((ptr=buf), sizeof(buf)))
    {
      line++;
      ptr = strskip_wht(ptr);
      if(g_isalpha(*ptr)) {
        str = ptr;
        strtrim(str);
        str = strskip_txt(str);
        if(*str) {
          *str++ = NUL;
          str = strskip_wht(str);
          strschg(str, "\\n", "\n");
          strschg(str, "\\r", "\r");
          strschg(str, "\\\"", "\"");
        }

        if (SwitchLanguage(strCrc16(strupr(ptr)), str))
        {
          if (cmdlineoldkeyw == false)
          {
            STD_PRINTNL("* " << cfgname << " line " << line << ": \"" << ptr << "\" is obsolete or unknown.");
            SayBibi();
            cfgerrors++;
          }
        }
      }
    }
  }

  // Fill in the defaults if there are "holes"
  size = 0;
  lngptr = LangCrcs;
  for(; lngptr->crc < CRC_LAST_CRC; lngptr++) {
    if(lngptr->langptr != NULL) {
      if(*(lngptr->langptr) == NULL)
        *(lngptr->langptr) = throw_strdup(lngptr->langdef);
      size += strlen(*(lngptr->langptr)) + 1;
    }
  }

  AssignDateNames();

  return true;
}


//  ------------------------------------------------------------------

