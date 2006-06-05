
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

  { CRC_MS_SWSUN              , NULL  , "Sun" },
  { CRC_MS_SWMON              , NULL  , "Mon" },
  { CRC_MS_SWTUE              , NULL  , "Tue" },
  { CRC_MS_SWWED              , NULL  , "Wed" },
  { CRC_MS_SWTHU              , NULL  , "Thu" },
  { CRC_MS_SWFRI              , NULL  , "Fri" },
  { CRC_MS_SWSAT              , NULL  , "Sat" },

  { CRC_MS_LWSUNDAY           , NULL  , "Sunday" },
  { CRC_MS_LWMONDAY           , NULL  , "Monday" },
  { CRC_MS_LWTUESDAY          , NULL  , "Tuesday" },
  { CRC_MS_LWWEDNESDAY        , NULL  , "Wednesday" },
  { CRC_MS_LWTHURSDAY         , NULL  , "Thursday" },
  { CRC_MS_LWFRIDAY           , NULL  , "Friday" },
  { CRC_MS_LWSATURDAY         , NULL  , "Saturday" },

  { CRC_MS_SMJAN              , NULL  , "Jan" },
  { CRC_MS_SMFEB              , NULL  , "Feb" },
  { CRC_MS_SMMAR              , NULL  , "Mar" },
  { CRC_MS_SMAPR              , NULL  , "Apr" },
  { CRC_MS_SMMAY              , NULL  , "May" },
  { CRC_MS_SMJUN              , NULL  , "Jun" },
  { CRC_MS_SMJUL              , NULL  , "Jul" },
  { CRC_MS_SMAUG              , NULL  , "Aug" },
  { CRC_MS_SMSEP              , NULL  , "Sep" },
  { CRC_MS_SMOCT              , NULL  , "Oct" },
  { CRC_MS_SMNOV              , NULL  , "Nov" },
  { CRC_MS_SMDEC              , NULL  , "Dec" },

  { CRC_MS_LMJANUARY          , NULL  , "January" },
  { CRC_MS_LMFEBRUARY         , NULL  , "February" },
  { CRC_MS_LMMARCH            , NULL  , "March" },
  { CRC_MS_LMAPRIL            , NULL  , "April" },
  { CRC_MS_LMMAY              , NULL  , "May" },
  { CRC_MS_LMJUNE             , NULL  , "June" },
  { CRC_MS_LMJULY             , NULL  , "July" },
  { CRC_MS_LMAUGUST           , NULL  , "August" },
  { CRC_MS_LMSEPTEMBER        , NULL  , "September" },
  { CRC_MS_LMOCTOBER          , NULL  , "October" },
  { CRC_MS_LMNOVEMBER         , NULL  , "November" },
  { CRC_MS_LMDECEMBER         , NULL  , "December" },

  { CRC_ST_IMPORTFILE         , NULL  , "Import File" },
  { CRC_WT_IMPORTWHICHFILE    , NULL  , " Import which file? " },
  { CRC_WT_IMPORTPICK         , NULL  , " Import " },
  { CRC_WT_IMPORTTXT          , NULL  , " Import " },
  { CRC_MI_IMPORTTXTTEXT      , NULL  , "T File as Text   " },
  { CRC_MI_IMPORTTXTQUOTE     , NULL  , "Q File as Quote  " },
  { CRC_MI_IMPORTTXTUUE       , NULL  , "U Binary as UUE  " },
  { CRC_MI_IMPORTTXTMIME      , NULL  , "M Binary as MIME " },
  { CRC_MI_IMPORTTXTCLIP      , NULL  , "C From Clipboard " },
  { CRC_MI_IMPORTTXTXLAT      , NULL  , "s Use Charset..  " },
  { CRC_MI_IMPORTTXTQUIT      , NULL  , "N Nothing / Quit " },
  { CRC_ST_IMPORTSTATUS       , NULL  , "Importing from %s" },
  { CRC_ST_EXPORTFILE         , NULL  , "Export File" },
  { CRC_WT_EXPORTWHATFILE     , NULL  , " Export to what file? " },
  { CRC_ST_EXPORTSTATUS       , NULL  , "Exporting to %s" },
  { CRC_ST_EDITSTATUS         , NULL  , "Edit %i,%i (%02X). %s" },
  { CRC_ST_SELECTFILES        , NULL  , "Select Files" },
  { CRC_WL_SELECTEDFILES      , NULL  , "Selected Files      " },
  { CRC_WL_SELECTEDBYTES      , NULL  , "         Bytes      " },
  { CRC_WL_TOTALFILES         , NULL  , "Total    Files      " },
  { CRC_WL_TOTALBYTES         , NULL  , "         Bytes      " },
  { CRC_WL_SCANNINGDIRECTORY  , NULL  , "             Scanning Directory             " },
  { CRC_WL_NOFILESFOUND       , NULL  , "             * NO FILES FOUND *             " },
  { CRC_ST_FILESPRESSKEY      , NULL  , "Press any key to continue" },
  { CRC_WT_AREA               , NULL  , "Area" },
  { CRC_WT_DESCRIPTION        , NULL  , "Description" },
  { CRC_WT_MSGS               , NULL  , "Msgs" },
  { CRC_WT_LAST               , NULL  , "Last" },
  { CRC_WT_ECHOID             , NULL  , "EchoID" },
  { CRC_ST_UNREAD             , NULL  , "unread" },
  { CRC_WT_SCANAREAS          , NULL  , " Scan Areas " },
  { CRC_MI_SCANALL            , NULL  , "A Scan All       " },
  { CRC_MI_SCANMARKED         , NULL  , "M Scan Marked    " },
  { CRC_MI_SCANCURRENT        , NULL  , "C Scan Current   " },
  { CRC_MI_SCANMATCHING       , NULL  , "t Scan Matching  " },
  { CRC_MI_SCANUNSCANNED      , NULL  , "U Scan Unscanned " },
  { CRC_MI_NOSCAN             , NULL  , "N No Scan  / ESC " },
  { CRC_WT_HEATAREAS          , NULL  , " Heat Areas " },
  { CRC_MI_HEATALL            , NULL  , "A Heat All      " },
  { CRC_MI_HEATMARKED         , NULL  , "M Heat Marked   " },
  { CRC_MI_HEATCURRENT        , NULL  , "C Heat Current  " },
  { CRC_MI_NOHEAT             , NULL  , "N No Heat / ESC " },
  { CRC_WT_ZAPAREAS           , NULL  , " Zap Areas " },
  { CRC_MI_ZAPALL             , NULL  , "A Zap All      " },
  { CRC_MI_ZAPMARKED          , NULL  , "M Zap Marked   " },
  { CRC_MI_ZAPCURRENT         , NULL  , "C Zap Current  " },
  { CRC_MI_NOZAP              , NULL  , "N No Zap / ESC " },
  { CRC_MS_DOS_SHELL          , NULL  , "GoldED Shell. Type EXIT To Return." },
  { CRC_IL_SCANNINGAREA       , NULL  , " Scanning Area:" },
  { CRC_IL_SEARCHINGFOR       , NULL  , " Searching for" },
  { CRC_ST_READINGMSG         , NULL  , "Reading Msg %u of %u" },
  { CRC_ER_OUTOFMEM           , NULL  , "Out of memory!" },
  { CRC_MS_HEXDUMPHEAD        , NULL  , "Hexdump of message header:" },
  { CRC_MS_HEXDUMPTEXT        , NULL  , "Hexdump of message text:" },
  { CRC_ST_RENUMBERING        , NULL  , "Renumbering" },
  { CRC_ST_LOCKED             , NULL  , " (locked)" },
  { CRC_ST_RENUMBERED         , NULL  , "Messages Renumbered - Press Key" },
  { CRC_WL_BLANKMSG           , NULL  , "" },
  { CRC_WL_WAIT               , NULL  , " Wait " },
  { CRC_IL_GENHEXDUMP         , NULL  , " Generating Hexdump - Please Wait " },
  { CRC_ST_PROCESSCC          , NULL  , "Processing Carbon Copies" },
  { CRC_ST_STATUSCC           , NULL  , "CC: %s of %s" },
  { CRC_ST_DESTINATIONCC      , NULL  , "Select Carbon Copy Destination" },
  { CRC_MS_LISTCC             , NULL  , "%s  %s" },
  { CRC_WT_DELORIG            , NULL  , " Delete Original? " },
  { CRC_MI_DELORIGYES         , NULL  , "Y       Yes Please.      " },
  { CRC_MI_DELORIGNO          , NULL  , "N           No!          " },
  { CRC_WT_DROPMSG            , NULL  , " Drop This Msg? " },
  { CRC_MI_DROPMSGYES         , NULL  , "Y       Yes Please.      " },
  { CRC_MI_DROPMSGNO          , NULL  , "N           No!          " },
  { CRC_WT_ZONEGATE           , NULL  , " Send via ZoneGate? " },
  { CRC_MI_ZONEGATEYES        , NULL  , "Y       Yes Please.      " },
  { CRC_MI_ZONEGATENO         , NULL  , "N           No!          " },
  { CRC_ST_QUOTEPCT           , NULL  , "Your Msg Contains %i%% Quotes%s" },
  { CRC_WT_SAVEMSG            , NULL  , " Save these %i lines? " },
  { CRC_MI_YESGREAT           , NULL  , "Y Yes, it's great stuff " },
  { CRC_MI_KICKIT             , NULL  , "N No, drop it..   / ESC " },
  { CRC_MI_CONTINUE           , NULL  , "C Continue writing      " },
  { CRC_MI_ROT13              , NULL  , "R ROT13 Crypting        " },
  { CRC_MI_ATTRS              , NULL  , "A Change Attributes     " },
  { CRC_MI_ORIGS              , NULL  , "O Change Origin         " },
  { CRC_MI_VIEW               , NULL  , "V View the message      " },
  { CRC_ST_SELECTDESTNODE     , NULL  , "Select Destination Node" },
  { CRC_WT_ATTACHFILES        , NULL  , " Attach Files " },
  { CRC_WT_UPDREQFILES        , NULL  , " Update Request Files " },
  { CRC_ST_EDITHEADER         , NULL  , "Edit Header Data" },
  { CRC_WT_EDITING            , NULL  , " Editing " },
  { CRC_MI_INTERNALED         , NULL  , "I Internal Editor " },
  { CRC_MI_EXTERNALED         , NULL  , "E External Editor " },
  { CRC_MI_SAVEMESSAGE        , NULL  , "S Save Message    " },
  { CRC_MI_ATTRO              , NULL  , "A Attributes      " },
  { CRC_MI_TEMPLATE           , NULL  , "T Templates       " },
  { CRC_MI_ORIGIN             , NULL  , "O Origins         " },
  { CRC_MI_QUITMESSAGE        , NULL  , "Q Quit      / ESC " },
  { CRC_ST_TEMPLATES          , NULL  , " Templates " },
  { CRC_WT_CHANGETEMPLATES    , NULL  , "Change Default Template" },
  { CRC_WT_CARBONCOPY         , NULL  , " Carbon Copy " },
  { CRC_MI_CCPROCESS          , NULL  , "P Process CC's " },
  { CRC_MI_CCIGNORE           , NULL  , "I Ignore  CC's " },
  { CRC_MI_CCATTRIBS          , NULL  , "C Change Attrs " },
  { CRC_MI_CCLISTFMT          , NULL  , "L List Format  " },
  { CRC_WT_CCLIST             , NULL  , " CC List " },
  { CRC_MI_CCLISTKEEP         , NULL  , "K Keep       " },
  { CRC_MI_CCLISTNAMES        , NULL  , "N Names only " },
  { CRC_MI_CCLISTVISIBLE      , NULL  , "V Visible    " },
  { CRC_MI_CCLISTHIDDEN       , NULL  , "H Hidden     " },
  { CRC_MI_CCLISTREMOVE       , NULL  , "R Remove     " },
  { CRC_WT_ATTRTITLE          , NULL  , " Attributes " },
  { CRC_MI_ATTR01             , NULL  , " Private             <Alt-P>   Archive/Sent         <Alt-B> " },
  { CRC_MI_ATTR02             , NULL  , " Crash               <Alt-C>   Direct               <Alt-D> " },
  { CRC_MI_ATTR03             , NULL  , " Received            <Alt-R>   Zonegate             <Alt-G> " },
  { CRC_MI_ATTR04             , NULL  , " Sent                <Alt-S>   Hub/Host-Route       <Alt-V> " },
  { CRC_MI_ATTR05             , NULL  , " File Attach         <Alt-A>   Immediate            <Alt-I> " },
  { CRC_MI_ATTR06             , NULL  , " Transit             <Alt-J>   Xmail                <Alt-X> " },
  { CRC_MI_ATTR07             , NULL  , " Orphan              <Alt-O>   Erase File/Sent      <Alt-E> " },
  { CRC_MI_ATTR08             , NULL  , " Kill/Sent           <Alt-K>   Trunc File/Sent      <Alt-T> " },
  { CRC_MI_ATTR09             , NULL  , " Local               <Alt-W>   Locked               <Alt-L> " },
  { CRC_MI_ATTR10             , NULL  , " Hold                <Alt-H>   Confirm Rcpt Request <Alt-Y> " },
  { CRC_MI_ATTR11             , NULL  , " File Request        <Alt-F>   FTS1 Reserved        <Alt-1> " },
  { CRC_MI_ATTR12             , NULL  , " Return Rcpt Request <Alt-M>   QBBS Reserved, Net   <Alt-2> " },
  { CRC_MI_ATTR13             , NULL  , " Return Rcpt         <Alt-N>   QBBS Reserved, Echo  <Alt-3> " },
  { CRC_MI_ATTR14             , NULL  , " Audit Request       <Alt-Q>   Squish Scanned       <Alt-4> " },
  { CRC_MI_ATTR15             , NULL  , " File Update Request <Alt-U>   Zap all attribs      <Alt-Z> " },
  { CRC_HD_FROM               , NULL  , " From : " },
  { CRC_HD_TO                 , NULL  , " To   : " },
  { CRC_HD_SUBJ               , NULL  , " Subj : " },
  { CRC_HD_FILE               , NULL  , " File : " },
  { CRC_MS_EMPTYMSG           , NULL  , "\r" },
  { CRC_MS_AUTOATTACHMSG      , NULL  , "\r" },
  { CRC_MS_AUTOREQUESTMSG     , NULL  , "\r" },
  { CRC_MS_AUTOUPDREQMSG      , NULL  , "\r" },
  { CRC_WT_FILEATTACH         , NULL  , " File Attach " },
  { CRC_WT_FILEREQUEST        , NULL  , " File Request " },
  { CRC_WT_FILEUPDREQ         , NULL  , " Update Request " },
  { CRC_ST_FILEATTACHING      , NULL  , "File Attaching [%i/%i] to %u:%u/%u.%u" },
  { CRC_ST_FILEREQUESTING     , NULL  , "File Requesting [%i/%i] from %u:%u/%u.%u" },
  { CRC_ST_FILEUPDREQING      , NULL  , "Update Requesting [%i/%i] from %u:%u/%u.%u" },
  { CRC_ST_READMARKED         , NULL  , "Read Marked - Msg %u of %u (%u left)" },
  { CRC_ST_READALL            , NULL  , "Read All - Msg %u of %u (%u left)" },
  { CRC_ST_NOQRENUM           , NULL  , "This area cannot be renumbered" },
  { CRC_MS_HIDINGTWIT         , NULL  , "This is a Twit Message - Press <Home> to read."},
  { CRC_HD_VIA                , NULL  , "via" },
  { CRC_WT_CHANGEATTRS        , NULL  , "Change Message Attributes" },
  { CRC_WT_HEADERTEXT         , NULL  , " Enter Searchstring (Header+Text) " },
  { CRC_WT_HEADERONLY         , NULL  , " Enter Searchstring (Header Only) " },
  { CRC_WT_NEWAREA            , NULL  , ">>Pick New Area: " },
  { CRC_WT_REPLYAREA          , NULL  , ">>Answer In Area: " },
  { CRC_WT_COPYAREA           , NULL  , ">>Copy To Area: " },
  { CRC_WT_MOVEAREA           , NULL  , ">>Move To Area: " },
  { CRC_WT_FORWARDAREA        , NULL  , ">>Forward To Area: " },
  { CRC_WT_FREQAREA           , NULL  , ">>Filerequest In Area: " },
  { CRC_WT_FREQMENUTITLE      , NULL  , "Requestable files" },
  { CRC_ST_FREQSTAT           , NULL  , "Pick files to request" },
  { CRC_IL_FREQINFONOFILES    , NULL  , " No requestable files were found in this msg! " },
  { CRC_WT_COPY               , NULL  , " Copy " },
  { CRC_WT_MOVE               , NULL  , " Move " },
  { CRC_WT_COPYING            , NULL  , " Copying " },
  { CRC_WT_MOVING             , NULL  , " Moving " },
  { CRC_ST_COPYINGMSG         , NULL  , "Copying Msg %u of %u to %s" },
  { CRC_ST_MOVINGMSG          , NULL  , "Moving Msg %u of %u to %s" },
  { CRC_WT_DELETE             , NULL  , " Delete " },
  { CRC_WT_DELETING           , NULL  , " Deleting " },
  { CRC_ST_DELETINGMSG        , NULL  , "Deleting Msg %u of %u" },
  { CRC_WT_WRITE              , NULL  , " Write " },
  { CRC_WT_WRITEMSGS          , NULL  , " Write Msg(s) to File " },
  { CRC_WT_WRITING            , NULL  , " Writing " },
  { CRC_ST_WRITINGMSG         , NULL  , "Writing Msg %u of %u" },
  { CRC_WT_WRITINGFILE        , NULL  , " Writing Msg(s) to File %s " },
  { CRC_WT_WRITINGPRN         , NULL  , " Writing Msg(s) to Print Device " },
  { CRC_IL_READONLYWARN       , NULL  , " This area is READ ONLY! " },
  { CRC_WT_ISREADONLY         , NULL  , " Write msg anyway? " },
  { CRC_MI_READONLYYES        , NULL  , "Y       Yes Please.      " },
  { CRC_MI_READONLYNO         , NULL  , "N           No!          " },
  { CRC_IL_CHANGEWARN         , NULL  , " This msg is NOT from you! " },
  { CRC_WT_CHANGE             , NULL  , " Change this msg? " },
  { CRC_MI_CHANGEYES          , NULL  , "Y       Yes Please.      " },
  { CRC_MI_CHANGENO           , NULL  , "N           No!          " },
  { CRC_WT_DELETETHIS         , NULL  , " Delete this msg? " },
  { CRC_MI_DELETEYES          , NULL  , "Y       Yes Please.      " },
  { CRC_MI_DELETENO           , NULL  , "N           No!          " },
  { CRC_MI_DELETENOASK        , NULL  , "D       Don't ask..      " },
  { CRC_WT_GOTONEXT           , NULL  , " Goto Next Area? " },
  { CRC_MI_GOTONEXTYES        , NULL  , "Y       Yes Please.      " },
  { CRC_MI_GOTONEXTNO         , NULL  , "N           No!          " },
  { CRC_MI_GOTONEXTNEW        , NULL  , "U Yes, Next Unread Area. " },
  { CRC_WT_FORWARD            , NULL  , " Use FWD kludges? " },
  { CRC_MI_FORWARDYES         , NULL  , "Y       Yes Please.      " },
  { CRC_MI_FORWARDNO          , NULL  , "N           No!          " },
  { CRC_WT_MSG                , NULL  , "Msg" },
  { CRC_WT_MSGREAL            , NULL  , "Msg#" },
  { CRC_WT_FROML              , NULL  , "From" },
  { CRC_WT_TOL                , NULL  , "To" },
  { CRC_WT_SUBJL              , NULL  , "Subj" },
  { CRC_ST_MSGLISTER          , NULL  , "Lister - Msg %u of %u (%u left)" },
  { CRC_ST_COPYMOVEFORWARD    , NULL  , "Copy, Move or Forward this message" },
  { CRC_WT_SELECTACTION       , NULL  , " Action " },
  { CRC_MI_FORWARDMESSAGE     , NULL  , "F Forward Msg " },
  { CRC_MI_MOVEMESSAGE        , NULL  , "M Move    Msg " },
  { CRC_MI_COPYMESSAGE        , NULL  , "C Copy    Msg " },
  { CRC_MI_TOGGLESENT         , NULL  , "T Toggle sent " },
  { CRC_MI_QUITCMF            , NULL  , "Q Quit  / ESC " },
  { CRC_ST_ARE                , NULL  , "messages are" },
  { CRC_ST_IS                 , NULL  , "message is" },
  { CRC_ST_MARKED             , NULL  , "marked" },
  { CRC_MI_MARKEDMSG          , NULL  , "M Marked Msgs " },
  { CRC_MI_CURRENTMSG         , NULL  , "C Current Msg " },
  { CRC_MI_QUITMSGS           , NULL  , "Q Quit  / ESC " },
  { CRC_ST_WRITEMSGSTO        , NULL  , "Write Msg(s) to File or Printer" },
  { CRC_WT_WRITETO            , NULL  , " Write To: " },
  { CRC_MI_DISKFILE           , NULL  , "D Disk File    " },
  { CRC_MI_PRINTER            , NULL  , "P Print device " },
  { CRC_MI_QUITWRITE          , NULL  , "Q Quit   / ESC " },
  { CRC_ST_MARKINGOPTIONS     , NULL  , "Message Marking" },
  { CRC_WT_MARKWHAT           , NULL  , " Mark What? " },
  { CRC_MI_YOURMAIL           , NULL  , "Y Your personal mail     " },
  { CRC_MI_FROMTOSUBJ         , NULL  , "H Header (From/To/Subj)  " },
  { CRC_MI_TEXTHDR            , NULL  , "T Text and header        " },
  { CRC_MI_THREAD             , NULL  , "R Reply thread           " },
  { CRC_MI_ASREAD             , NULL  , "s Mark thread as read    " },
  { CRC_MI_NEWMSGS            , NULL  , "N New msgs      >current " },
  { CRC_MI_OLDMSGS            , NULL  , "O Old msgs      <current " },
  { CRC_MI_ALLMSGS            , NULL  , "A All msgs               " },
  { CRC_MI_UNMARK             , NULL  , "U Unmark all msgs        " },
  { CRC_MI_RANGE              , NULL  , "M Msg range (bookm-curr) " },
  { CRC_MI_MARKSTOGGLE        , NULL  , "g Toggle all marks       " },
  { CRC_MI_QUITMARKS          , NULL  , "Q Quit             / ESC " },
  { CRC_WT_ENTERMARKSTRING    , NULL  , " Enter Marking String " },
  { CRC_ST_SEARCHINGMSG       , NULL  , "Searching Msg %u of %u. Marked: %u" },
  { CRC_WT_USERLISTNAME       , NULL  , " Userlist filename? " },
  { CRC_IL_GENUSERLIST        , NULL  , " Generating Userlist " },
  { CRC_WT_FILEEXISTS         , NULL  , " File Exists! " },
  { CRC_MI_APPEND             , NULL  , "A Append to end of file   " },
  { CRC_MI_OVERWRITE          , NULL  , "O Overwrite the file      " },
  { CRC_MI_QUITEXIST          , NULL  , "R Re-enter filename / ESC " },
  { CRC_IL_WARNUNSENT         , NULL  , " WARNING! This message is NOT sent! " },
  { CRC_IL_WARNLOCKED         , NULL  , " WARNING! This message is LOcKed! "},
  { CRC_ST_CHANGEORIGIN       , NULL  , "Change Default Origin" },
  { CRC_WT_ORIGINS            , NULL  , " Origins " },
  { CRC_ST_CHANGEUSERNAME     , NULL  , "Change Default Username" },
  { CRC_WT_USERNAMES          , NULL  , " Usernames " },
  { CRC_ST_CHANGEAKA          , NULL  , "Change Default Address AKA" },
  { CRC_WT_AKAS               , NULL  , " AKA's " },
  { CRC_WT_LOOKUP             , NULL  , " Lookup: %s " },
  { CRC_WL_PHONE              , NULL  , "Phone" },
  { CRC_WT_QUITGOLDED         , NULL  , " Quit GoldED? " },
  { CRC_MI_QUITYES            , NULL  , "Y       Yes Please.      " },
  { CRC_MI_QUITNO             , NULL  , "N           No!          " },
  { CRC_MS_EDITCMD            , NULL  , "Editor: %s" },
  { CRC_IL_NOORIGDEFINED      , NULL  , " No origins defined. Press key " },
  { CRC_IL_NOUSERDEFINED      , NULL  , " No usernames defined. Press key " },
  { CRC_IL_NOAKADEFINED       , NULL  , " No AKA's defined. Press key " },
  { CRC_IL_NOTPLDEFINED       , NULL  , " No templates defined. Press key " },
  { CRC_IL_NOTHREADLIST       , NULL  , " Sorry, no thread available. Press key "},
  { CRC_MS_SKIPPINGTWIT       , NULL  , "Skipping Twit Message..." },
  { CRC_MS_SKIPPINGDELETED    , NULL  , "Skipping Deleted Message..." },
  { CRC_MS_KILLINGTWIT        , NULL  , "Killing Twit Message..." },
  { CRC_IL_WRITINGCFG         , NULL  , " Writing Configuration " },
  { CRC_IL_COULDNOTOPEN       , NULL  , " Couldn't open %s " },
  { CRC_MS_PROMPT             , NULL  , "Enter the command \"EXIT\" to return to GoldED.\r\n" },
  { CRC_IL_UNFINISHEDMSG      , NULL  , " Unfinished message found! " },
  { CRC_ST_LOADUNFINISHED     , NULL  , "Press any key to load it or <ESC> to ignore" },
  { CRC_IL_READINGECHOLIST    , NULL  , "Reading Echolist Files" },
  { CRC_IL_READINGADDRMACROS  , NULL  , "Reading Address Macros" },
  { CRC_IL_CHECKINGNODELISTS  , NULL  , "Checking Nodelists" },
  { CRC_ST_CROSSPOSTING       , NULL  , "Crossposting in %s" },
  { CRC_IL_TWITBLANKED        , NULL  , " Twit Msgs Blanked " },
  { CRC_IL_TWITSKIPPED        , NULL  , " Twit Msgs Skipped " },
  { CRC_IL_TWITIGNORESKIP     , NULL  , " Twit Msgs Ignored and Skipped " },
  { CRC_IL_TWITDISPLAYED      , NULL  , " Twit Msgs Displayed " },
  { CRC_IL_TWITKILLED         , NULL  , " Twit Msgs Killed " },
  { CRC_IL_STYLECODESNO       , NULL  , " Stylecodes disabled " },
  { CRC_IL_STYLECODESYES      , NULL  , " Stylecodes enabled " },
  { CRC_IL_STYLECODESHIDE     , NULL  , " Stylecodes enabled and stripped " },
  { CRC_ST_GENCFMRECEIPT      , NULL  , "Generating Confirmation Receipt" },
  { CRC_WT_NEW                , NULL  , "New" },
  { CRC_ST_LOOKUPINFO         , NULL  , "Nodelist Lookup Information" },
  { CRC_MS_DATETIMEFMT        , NULL  , "%d %b %y %H:%M:%S" },
  { CRC_MS_DATEFMT            , NULL  , "%d %b %y" },
  { CRC_MS_TIMEFMT            , NULL  , "%H:%M" },
  { CRC_MS_ORIGINALLYIN       , NULL  , "* Originally in %s" },
  { CRC_MS_CROSSPOSTEDIN      , NULL  , "* Crossposted in %s" },
  { CRC_MS_CCTO               , NULL  , "* Carbon copied to %s" },
  { CRC_ST_STATUSLINEHELP     , NULL  , "F1 Help" },
  { CRC_HD_OF                 , NULL  , "of" },
  { CRC_MS_ROBOTMSG           , NULL  , "\r--- @longpid @version\r" },
  { CRC_ST_STATUSLINETIMEFMT  , NULL  , "%H:%M:%S" },

  { CRC_WT_REPLIES            , NULL  , " Replies " },
  { CRC_ST_SELECTREPLY        , NULL  , "Select the reply you want to see" },
  { CRC_IL_WAITOREXIT         , NULL  , " At this point you must either wait or exit GoldED entirely - Sorry! " },
  { CRC_WT_REALLYEXIT         , NULL  , " Really exit? " },
  { CRC_MI_TAGLINES           , NULL  , "T Change Tagline" },
  { CRC_MI_HEADEREDIT         , NULL  , "H Edit Header" },
  { CRC_MI_SCANGROUP          , NULL  , "G Scan Group" },
  { CRC_WT_QWKPACKET          , NULL  , " QWK Packet " },
  { CRC_MI_SCANQWK            , NULL  , "Q QWK Packet" },
  { CRC_WT_SOUPPACKET         , NULL  , " SOUP Packet " },
  { CRC_MI_SCANSOUP           , NULL  , "S SOUP Packet" },
  { CRC_MI_SCANIMPORT         , NULL  , "I Import" },
  { CRC_MI_SCANEXPORT         , NULL  , "E Export" },
  { CRC_WT_SCANPM             , NULL  , " Scan PM " },
  { CRC_MI_TAGLINE            , NULL  , "g Taglines" },
  { CRC_WT_CONFIRM            , NULL  , " Honor receipt request? " },
  { CRC_MI_CONFIRMYES         , NULL  , "Y Yes, send a receipt msg " },
  { CRC_MI_CONFIRMNO          , NULL  , "N No, ignore the request  " },
  { CRC_IL_CONFIRMINFO        , NULL  , " The sender of this message requests confirmation of receipt (CFM) " },
  { CRC_ST_MSG                , NULL  , "msg" },
  { CRC_ST_MSGS               , NULL  , "msgs" },
  { CRC_ST_PERSONAL           , NULL  , "personal" },
  { CRC_WT_GRP                , NULL  , "Grp" },
  { CRC_IL_FOUNDPERSONAL      , NULL  , " Found %u personal mail%s in %u area%s " },
  { CRC_IL_NOPERSONAL         , NULL  , " No personal mail found " },
  { CRC_ST_ESCORCONTINUE      , NULL  , "ESC exits. Other keys: Tries to continue" },
  { CRC_MS_SPELLCHECKER       , NULL  , "SpellChecker: %s" },
  { CRC_WT_INS                , NULL  , " Ins " },
  { CRC_WT_DRAWSL             , NULL  , " DL1 " },
  { CRC_WT_DRAWDL             , NULL  , " DL2 " },
  { CRC_MI_FILELISTFROM       , NULL  , "Filelist from %s" },
  { CRC_ST_INITIALIZING       , NULL  , "Initializing ..." },
  { CRC_ST_CHECKING           , NULL  , "Checking" },
  { CRC_ST_LOCKSHARECAP       , NULL  , "%s Lock/Share Capability" },
  { CRC_WT_ATTRTURNOFF        , NULL  , " Press Alt-F1 to turn off this window " },
  { CRC_WT_TAGLINES           , NULL  , " Taglines " },
  { CRC_ST_CHANGETAGLINE      , NULL  , "Change Default Tagline" },
  { CRC_IL_NOTAGLINE          , NULL  , " No taglines defined. Press key " },
  { CRC_WT_CHARSETS           , NULL  , " Charsets " },
  { CRC_ST_CHANGEXLATIMP      , NULL  , "Change Default Import Charset" },
  { CRC_MI_CHARSETAUTO        , NULL  , " Auto " },
  { CRC_IL_NOXLATIMPORT       , NULL  , " No XLATIMPORT's defined. Press key " },
  { CRC_WT_N_A                , NULL  , "n/a" },
  { CRC_WT_WRITTEN            , NULL  , "Written" },
  { CRC_WT_ARRIVED            , NULL  , "Arrived" },
  { CRC_WT_RECEIVED           , NULL  , "Received" },
  { CRC_IL_NONODELIST         , NULL  , " Unable to access the nodelist or index files " },
  { CRC_IL_NODELISTMISSING    , NULL  , "Nodelist missing:" },
  { CRC_IL_NODELISTOUTDATED   , NULL  , "Nodelist out of date:" },
  { CRC_MS_REPLYLINKER        , NULL  , "Replylinker: %s" },
  { CRC_WT_ENTERMSGNO         , NULL  , " Enter msgno " },
  { CRC_IL_WAITUUDECODING     , NULL  , " Wait - uudecoding " },
  { CRC_IL_COMPLETEDUUDECODE  , NULL  , " Completed uudecode of %s " },
  { CRC_IL_NOTUUENCODED       , NULL  , " Could not find anything to uudecode " },
  { CRC_IL_UUEINVALIDPATH     , NULL  , " Invalid path for uudecoding - file not written " },
  { CRC_IL_PATHREPORT         , NULL  , " Generating PATH report " },
  { CRC_IL_ERRORINSOUP        , NULL  , " Error in SOUP file %s " },
  { CRC_IL_WARNALREADYSENT    , NULL  , " WARNING: This message is already sent! " },
  { CRC_IL_WAITLOCKED         , NULL  , " Wait: %s is locked " },
  { CRC_ST_RETRYORESC         , NULL  , "%s retry (%lu). Press ESC to exit GoldED." },
  { CRC_ST_RETRYLOCK          , NULL  , "Lock" },
  { CRC_ST_RETRYOPEN          , NULL  , "Open" },
  { CRC_WT_TOUCHSEMAPHORE     , NULL  , " Enter name of semaphore file to touch " },
  { CRC_MI_WRITETOHDRNO       , NULL  , "H Use Header: NO   " },
  { CRC_MI_WRITETOHDRYES      , NULL  , "H Use Header: YES  "},
  { CRC_MI_WRITETOHDRONLY     , NULL  , "H Use Header: ONLY "},
  { CRC_MI_CLIPBOARD          , NULL  , "C Clipboard "},
  { CRC_WT_SELECTMARKS        , NULL  , " Select Mark " },
  { CRC_WT_SELECTMARKSEDIT    , NULL  , " Edit Mark Description " },
  { CRC_IL_DROPMARKSINFO      , NULL  , " %s Messages Are Marked " },
  { CRC_WT_DROPMARKS          , NULL  , " Drop Msg Marks " },
  { CRC_MI_DROPALL            , NULL  , "A Drop All       " },
  { CRC_MI_DROPMARKED         , NULL  , "M Drop Marked    " },
  { CRC_MI_DROPCURRENT        , NULL  , "C Drop Current   " },
  { CRC_MI_NODROP             , NULL  , "N No Drop / ESC  " },
  { CRC_WT_CATCHAREAS         , NULL  , " Catch-Up Areas " },
  { CRC_MI_CATCHALL           , NULL  , "A Catch All      " },
  { CRC_MI_CATCHMARKED        , NULL  , "M Catch Marked   " },
  { CRC_MI_CATCHCURRENT       , NULL  , "C Catch Current  " },
  { CRC_MI_NOCATCH            , NULL  , "N No Catch / ESC " },
  { CRC_WT_CROSSPOST          , NULL  , " Crosspost " },
  { CRC_MI_XCPROCESS          , NULL  , "P Process XC's " },
  { CRC_MI_XCIGNORE           , NULL  , "I Ignore  XC's " },
  { CRC_MI_XCLISTFMT          , NULL  , "L List Format  " },
  { CRC_WT_XCLIST             , NULL  , " XC List " },
  { CRC_MI_XCLISTRAW          , NULL  , "K Keep       " },
  { CRC_MI_XCLISTVERBOSE      , NULL  , "V Verbose    " },
  { CRC_MI_XCLISTLINE         , NULL  , "L Line       " },
  { CRC_MI_XCLISTREMOVE       , NULL  , "R Remove     " },

  { CRC_WT_ADDRESSBOOK        , NULL  , " Addressbook " },
  { CRC_WT_ADVANCEDSEARCH     , NULL  , " Advanced Search " },
  { CRC_IL_NOMOREMATCHES      , NULL  , " No more matches found " },
  { CRC_WT_HEADEREDITHELP_1   , NULL  , " Shift-F10: Nodelist " },
  { CRC_WT_HEADEREDITHELP_2   , NULL  , " F10: Addressbook " },
  { CRC_WT_THREADLISTTITLE    , NULL  , " Message Thread List " },
  { CRC_WT_ADVANCEDMARKING    , NULL  , " Advanced Marking " },
  { CRC_ST_USERSTATUSLINE     , NULL  , "GoldED's Address Book - %d of %d (%d left)" },
  { CRC_WT_USERHEADERNAME     , NULL  , "Name" },
  { CRC_WT_USERHEADERORG      , NULL  , "Organization" },
  { CRC_WT_USERHEADERAKA      , NULL  , "Address" },
  { CRC_IL_USERWAIT           , NULL  , " Wait - Browsing Address Book... " },

  { CRC_MI_ALSELECTIONS1      , NULL  , "Mark Areas with New Mail" },
  { CRC_MI_ALSELECTIONS2      , NULL  , "(unused)" },
  { CRC_IL_DECODE             , NULL  , "Decode" },
  { CRC_IL_PREPARING          , NULL  , "Preparing" },

  { CRC_WT_EXTERNUTIL         , NULL  , "External utilities" },
  { CRC_ST_EXTERNUTIL         , NULL  , "Select external utility to execute" },
  { CRC_WT_EXECCMDLINE        , NULL  , "Enter command line to execute" },
  { CRC_MI_ENTERCMDLINE       , NULL  , " Enter command line " },

  { CRC_WT_PEEKURL            , NULL  , "Available URLs" },
  { CRC_ST_PEEKURL            , NULL  , "Select URL" },
  { CRC_IL_PEEKINFONOURLS     , NULL  , " No valid URLs were found in this msg! " },

  { CRC_MI_MMMAIN             , NULL  , "M Main " },
  { CRC_MI_MMSHELL            , NULL  , "S Shell to OS " },
  { CRC_MI_MMQUIT             , NULL  , "Q Quit GoldED " },
  { CRC_MI_MMEDIT             , NULL  , "E Edit " },
  { CRC_MI_MMNEWMSG           , NULL  , "E Enter new msg                        " },
  { CRC_MI_MMQUOTEREPLY       , NULL  , "Q Quote-reply                          " },
  { CRC_MI_MMDIRQUOTEREPLY    , NULL  , "D Direct quote-reply (ignore Reply-To) " },
  { CRC_MI_MMCMTREPLY         , NULL  , "C Comment-reply                        " },
  { CRC_MI_MMOTHERAREA        , NULL  , "O Other area reply >                   " },
  { CRC_WT_MMOTHERAREA        , NULL  , " Other Area " },
  { CRC_MI_MMOQUOTEREPLY      , NULL  , "Q Quote-reply                          " },
  { CRC_MI_MMODIRQUOTEREPLY   , NULL  , "D Direct quote-reply (ignore Reply-To) " },
  { CRC_MI_MMOCMTREPLY        , NULL  , "C Comment-reply                        " },
  { CRC_MI_MMCHANGE           , NULL  , "C Change " },
  { CRC_MI_MMORIGIN           , NULL  , "O Origin    " },
  { CRC_MI_MMUSERNAME         , NULL  , "U Username  " },
  { CRC_MI_MMAKA              , NULL  , "A Aka       " },
  { CRC_MI_MMATTRIBUTES       , NULL  , "M Msg attrs " },
  { CRC_MI_MMTEMPLATE         , NULL  , "T Template  " },
  { CRC_MI_MMUTIL             , NULL  , "U Util " },
  { CRC_MI_MMCOPYMOVEFORWARD  , NULL  , "C Copy/Move/Forward     " },
  { CRC_MI_MMLIST             , NULL  , "L List messages         " },
  { CRC_MI_MMWRITE            , NULL  , "W Write to disk/printer " },
  { CRC_MI_MMFREQ             , NULL  , "F File request          " },

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
    reg.compile("^.*%[0-9]*[dioux].*%[0-9]*[dioux].*%[0-9]*[dioux].*%[0-9]*s", gregex::icase);
    if (!reg.match(str))
    {
      STD_PRINTNL("ST_EDITSTATUS have wrong format. Read NOTEWORK.TXT for details or announce author if this error is false positive.");
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

