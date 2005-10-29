
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1998-1999 Odinn Sorensen
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
//  Configuration functions, part 0.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gccfgg.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

bool cfgingroup = false;
char* val;


//  ------------------------------------------------------------------

struct tglobalbool {
  word crc_token;
};

// it must be sorted by value
tglobalbool globalbool[gswitches_last] = {
  { CRC_INTERNETLOOKUP           }, // 0x0E22;
  { CRC_KLUDGECHRS               }, // 0x0EEF;
  { CRC_MOUSE                    }, // 0x1B1B;
  { CRC_DISPLOCALHIGH            }, // 0x1D8B;
  { CRC_AREALISTGROUPID          }, // 0x1F75;
  { CRC_ASKDELORIG               }, // 0x26DD;
  { CRC_LOOKUPLOCAL              }, // 0x34E7;
  { CRC_DISPSTATUSLINE           }, // 0x3FCF;
  { CRC_SCREENUSEBIOS            }, // 0x43DE;
  { CRC_HIGHLIGHTUNREAD          }, // 0x45DA;
  { CRC_AKAMATCHLOCAL            }, // 0x4AC4;
  { CRC_FIDONULLFIX              }, // 0x4F38;
  { CRC_DISPAUTONEXT             }, // 0x52D9;
  { CRC_LOOKUPUSERBASE           }, // 0x5852;
  { CRC_AREAKEEPLAST             }, // 0x5876;
  { CRC_FILELISTPAGEBAR          }, // 0x58AA;
  { CRC_PRINTFORMFEED            }, // 0x59FB;
  { CRC_AREALISTNOS              }, // 0x5FD7;
  { CRC_DISPPAGEBAR              }, // 0x60A4;
  { CRC_INTERNETREPLY            }, // 0x6253;
  { CRC_AREALISTPAGEBAR          }, // 0x6C37;
  { CRC_USEMSGID                 }, // 0x6FDB;
  { CRC_LOOKUPNET                }, // 0x7359;
  { CRC_JAMSMAPIHIGHWATER        }, // 0x74A4;
  { CRC_SQUISHDIRECT             }, // 0x7C35;
  { CRC_FIDOHWMARKS              }, // 0x7E07;
  { CRC_MSGLISTPAGEBAR           }, // 0x8041;
  { CRC_BEEPNOISES               }, // 0x84D5;
  { CRC_SCREENSHADOWS            }, // 0x8543;
  { CRC_DISPSOFTCR               }, // 0x85E9;
  { CRC_EMPTYTEARLINE            }, // 0x8D0A;
  { CRC_MENUDROPMSG              }, // 0x9645;
  { CRC_BEEPLOCALMSG             }, // 0x984C;
  { CRC_STATUSLINECLOCK          }, // 0x9C8A;
  { CRC_TWITTO                   }, // 0x9DFE;
  { CRC_KEYBDEFAULTS             }, // 0x9FAE;
  { CRC_DISPLISTWRAP             }, // 0xB36D;
  { CRC_QUOTESPACING             }, // 0xB403;
  { CRC_LOOKUPECHO               }, // 0xB787;
  { CRC_KEYBEXT                  }, // 0xC48A;
  { CRC_BEEPCOMMENT              }, // 0xC49E;
  { CRC_RCVDISABLESCFM           }, // 0xD3B2;
  { CRC_QUOTEBLANK               }, // 0xD3BB;
  { CRC_KEYBCLEAR                }, // 0xD407;
  { CRC_DOSPROMPT                }, // 0xD983;
  { CRC_AREACATCHUPREAD          }, // 0xDAFF;
  { CRC_FORMFEEDSEPARATOR        }, // 0xDBD6;
  { CRC_USEFLAGS                 }, // 0xE2B6;
  { CRC_JAMHARDDELETE            }, // 0xE2D5;
  { CRC_DISPREALMSGNO            }, // 0xE30F;
  { CRC_FRQWAZOO                 }, // 0xE49C;
  { CRC_MSGLISTVIEWSUBJ          }, // 0xED92;
  { CRC_AKAMATCHECHO             }, // 0xF0C1;
  { CRC_AREAFILEGROUPS           }, // 0xF0E7;
  { CRC_AREAAUTONEXT             }, // 0xF589;
  { CRC_TIMEOUTSAVEMSG           }, // 0xF644;
  { CRC_NODELISTWARN             }, // 0xF818;
  { CRC_AKAMATCHNET              }, // 0xFD6E;
};

void CfgJAMSMAPIHighwater(bool value)
{
  CFG->switches.handle(CRC_JAMSMAPIHIGHWATER, value ? "YES" : "NO");
}

bool gswitches::handle(word crc, const char* value) {

  uint left,right,middle;
  word cur;

  left = 0;
  right = gswitches_last - 1;

  if((crc < globalbool[left].crc_token) or (crc > globalbool[right].crc_token))
    return false;

  do {
    middle = (left + right)/2;

    cur = globalbool[middle].crc_token;

    if(crc < cur) {
      right = middle - 1;
    } else if(crc > cur) {
      left  = middle + 1;
    }
    else {
      cfg[middle] = make_bool(GetYesno(value));
      return true;
    }
  } while(left <= right);

  return false;
}

//  ------------------------------------------------------------------

int SwitchCfg(word crc, char k, char* v) {

  val = v;
  bool found = true;

  if(CFG->switches.handle(crc, v))
    return true;

  // Switch the first letter of the keyword
  switch(toupper(k)) {
    case 'A': goto SwitchA;
    case 'B': goto SwitchB;
    case 'C': goto SwitchC;
    case 'D': goto SwitchD;
    case 'E': goto SwitchE;
    case 'F': goto SwitchF;
    case 'G': goto SwitchG;
    case 'H': goto SwitchH;
    case 'I': goto SwitchI;
    case 'J': goto SwitchJ;
    case 'K': goto SwitchK;
    case 'L': goto SwitchL;
    case 'M': goto SwitchM;
    case 'N': goto SwitchN;
    case 'O': goto SwitchO;
    case 'P': goto SwitchP;
    case 'Q': goto SwitchQ;
    case 'R': goto SwitchR;
    case 'S': goto SwitchS;
    case 'T': goto SwitchT;
    case 'U': goto SwitchU;
    case 'V': goto SwitchV;
    case 'W': goto SwitchW;
    case 'X': goto SwitchX;
    case 'Y': goto SwitchY;
    case 'Z': goto SwitchZ;
    default:  found = false;
  }
  goto End;

SwitchA:
  switch(crc) {
    case CRC_ADDRESS          :  CfgAddress          ();  break;
    case CRC_ADDRESSBOOKADD   :  CfgAddressbookadd   ();  break;
    case CRC_ADDRESSLOOKUPFIRST: CfgAddresslookupfirst(); break;
    case CRC_ADDRESSMACRO     :  CfgAddressmacro     ();  break;
    case CRC_ADEPTXBBSPATH    :  CfgAdeptxbbspath    ();  break;
    case CRC_ADEPTXBBSUSERNO  :  CfgAdeptxbbsuserno  ();  break;
    case CRC_AKA              :  CfgAddress          ();  break;
    case CRC_AKAMATCH         :  CfgAkamatch         ();  break;
    case CRC_AKAMATCHFROMTO   :  CfgAkamatchfromto   ();  break;
    case CRC_AKAMATCHING      :  CfgAkamatching      ();  break;
    case CRC_AKAMATCHMANUALLY :  CfgAkamatchmanually ();  break;
    case CRC_AREAAUTOID       :  CfgAreaautoid       ();  break;
    case CRC_AREACFMREPLYTO   :  CfgAreacfmreplyto   ();  break;
    case CRC_AREACOPYADDID    :  CfgAreacopyaddid    ();  break;
    case CRC_AREACOPYDIRECT   :  CfgAreacopydirect   ();  break;
    case CRC_AREACOPYTO       :  CfgAreacopyto       ();  break;
    case CRC_AREADEF          :  CfgAreadef          ();  break;
    case CRC_AREADESC         :  CfgAreadesc         ();  break;
    case CRC_AREAEXCL         :  CfgAreaexcl         ();  break;
    case CRC_AREAFORWARDDIRECT:  CfgAreaforwarddirect();  break;
    case CRC_AREAFREQDIRECT   :  CfgAreafreqdirect   ();  break;
    case CRC_AREAFREQTO       :  CfgAreafreqto       ();  break;
    case CRC_AREAINCL         :  CfgAreaincl         ();  break;
    case CRC_AREAISEMAIL      :  CfgAreaisemail      ();  break;
    case CRC_AREAISNEWS       :  CfgAreaisnews       ();  break;
    case CRC_AREALISTECHOMAX  :  CfgArealistechomax  ();  break;
    case CRC_AREALISTFORMAT   :  CfgArealistformat   ();  break;
    case CRC_AREALISTGROUPORDER: CfgArealistgrouporder(); break;
    case CRC_AREALISTSCAN     :  CfgArealistscan     ();  break;
    case CRC_AREALISTSORT     :  CfgArealistsort     ();  break;
    case CRC_AREALISTTYPE     :  CfgArealisttype     ();  break;
    case CRC_AREAPATH         :  CfgAreapath         ();  break;
    case CRC_AREAPMSCAN       :  CfgAreapmscan       ();  break;
    case CRC_AREAPMSCANEXCL   :  CfgAreapmscanexcl   ();  break;
    case CRC_AREAPMSCANINCL   :  CfgAreapmscanincl   ();  break;
    case CRC_AREAREADONLY     :  CfgAreareadonly     ();  break;
    case CRC_AREARENAME       :  CfgArearename       ();  break;
    case CRC_AREAREPLYDIRECT  :  CfgAreareplydirect  ();  break;
    case CRC_AREAREPLYTO      :  CfgAreareplyto      ();  break;
    case CRC_AREASCAN         :  CfgAreascan         ();  break;
    case CRC_AREASCANEXCL     :  CfgAreascanexcl     ();  break;
    case CRC_AREASCANINCL     :  CfgAreascanincl     ();  break;
    case CRC_AREASCANSORT     :  CfgAreascansort     ();  break;
    case CRC_AREASEP          :  CfgAreasep          ();  break;
    case CRC_AREASTART        :  CfgAreastart        ();  break;
    case CRC_AREATYPEORDER    :  CfgAreatypeorder    ();  break;
    case CRC_AREAYOUWROTETO   :  CfgAreayouwroteto   ();  break;
    case CRC_ATTACHPATH       :  CfgAttachpath       ();  break;
    case CRC_ATTRIBSATTACH    :  CfgAttribsattach    ();  break;
    case CRC_ATTRIBSCC        :  CfgAttribscc        ();  break;
    case CRC_ATTRIBSCFM       :  CfgAttribscfm       ();  break;
    case CRC_ATTRIBSECHO      :  CfgAttribsecho      ();  break;
    case CRC_ATTRIBSEMAIL     :  CfgAttribsemail     ();  break;
    case CRC_ATTRIBSFRQ       :  CfgAttribsfrq       ();  break;
    case CRC_ATTRIBSLOCAL     :  CfgAttribslocal     ();  break;
    case CRC_ATTRIBSNET       :  CfgAttribsnet       ();  break;
    case CRC_ATTRIBSNEWS      :  CfgAttribsnews      ();  break;
    case CRC_ATTRIBUTES       :  CfgAttributes       ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchB:
  switch(crc) {
    case CRC_BEEPFACTOR       :  CfgBeepfactor       ();  break;
    case CRC_BEEPYOURMAIL     :  CfgBeepyourmail     ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchC:
  switch(crc) {
    case CRC_CARBONCOPYLIST   :  CfgCarboncopylist   ();  break;
    case CRC_COLOR            :  CfgColor            ();  break;
    case CRC_COLORNAME        :  CfgColorname        ();  break;
    case CRC_COLORSET         :  CfgColorset         ();  break;
    case CRC_CONFIRMFILE      :  CfgConfirmfile      ();  break;
    case CRC_CONFIRMRESPONSE  :  CfgConfirmresponse  ();  break;
    case CRC_COOKIEPATH       :  CfgCookiepath       ();  break;
    case CRC_CROSSPOST        :  CfgCrosspost        ();  break;
    case CRC_CROSSPOSTLIST    :  CfgCrosspostlist    ();  break;
    case CRC_CTRLINFO         :  CfgCtrlinfo         ();  break;
    case CRC_CTRLINFOECHO     :  CfgCtrlinfoecho     ();  break;
    case CRC_CTRLINFOEMAIL    :  CfgCtrlinfoemail    ();  break;
    case CRC_CTRLINFOLOCAL    :  CfgCtrlinfolocal    ();  break;
    case CRC_CTRLINFONET      :  CfgCtrlinfonet      ();  break;
    case CRC_CTRLINFONEWS     :  CfgCtrlinfonews     ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchD:
  switch(crc) {
    case CRC_DISPAREANO       :  CfgDispareano       ();  break;
    case CRC_DISPATTACHSIZE   :  CfgDispattachsize   ();  break;
    case CRC_DISPHDRDATESET   :  CfgDisphdrdateset   ();  break;
    case CRC_DISPHDRLOCATION  :  CfgDisphdrlocation  ();  break;
    case CRC_DISPHDRNAMESET   :  CfgDisphdrnameset   ();  break;
    case CRC_DISPHDRNODESET   :  CfgDisphdrnodeset   ();  break;
    case CRC_DISPLISTCURSOR   :  CfgDisplistcursor   ();  break;
    case CRC_DISPMARGIN       :  CfgDispmargin       ();  break;
    case CRC_DISPMSGSIZE      :  CfgDispmsgsize      ();  break;
    case CRC_DISPPMFIRST      :  CfgDisppmfirst      ();  break;
    case CRC_DISPTABSIZE      :  CfgDisptabsize      ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchE:
  switch(crc) {
    case CRC_EDITAUTOATTACH   :  CfgEditautoattach   ();  break;
    case CRC_EDITAUTOSAVE     :  CfgEditautosave     ();  break;
    case CRC_EDITCHANGEDATE   :  CfgEditchangedate   ();  break;
    case CRC_EDITCHARPARA     :  CfgEditcharpara     ();  break;
    case CRC_EDITCHARSPACE    :  CfgEditcharspace    ();  break;
    case CRC_EDITCOMMENT      :  CfgEditcomment      ();  break;
    case CRC_EDITCOMPLETION   :  CfgEditcompletion   ();  break;
    case CRC_EDITCRLFTERM     :  CfgEditcrlfterm     ();  break;
    case CRC_EDITFIELDCLEAR   :  CfgEditfieldclear   ();  break;
    case CRC_EDITHARDLINE     :  CfgEdithardline     ();  break;
    case CRC_EDITHARDLINES    :  CfgEdithardlines    ();  break;
    case CRC_EDITHARDTERM     :  CfgEdithardterm     ();  break;
    case CRC_EDITHDRNAMESET   :  CfgEdithdrnameset   ();  break;
    case CRC_EDITHDRNODESET   :  CfgEdithdrnodeset   ();  break;
    case CRC_EDITHEADERATTRS  :  CfgEditheaderattrs  ();  break;
    case CRC_EDITHEADERFIRST  :  CfgEditheaderfirst  ();  break;
    case CRC_EDITINTERNAL     :  CfgEditinternal     ();  break;
    case CRC_EDITMENU         :  CfgEditmenu         ();  break;
    case CRC_EDITMIXCASE      :  CfgEditmixcase      ();  break;
    case CRC_EDITOR           :  CfgEditor           ();  break;
    case CRC_EDITORFILE       :  CfgEditorfile       ();  break;
    case CRC_EDITQUOTEMARGIN  :  CfgEditquotemargin  ();  break;
    case CRC_EDITREPLYRE      :  CfgEditreplyre      ();  break;
    case CRC_EDITSAVEMENU     :  CfgEditsavemenu     ();  break;
    case CRC_EDITSAVEUTIL     :  CfgEditsaveutil     ();  break;
    case CRC_EDITSOFTCRXLAT   :  CfgEditsoftcrxlat   ();  break;
    case CRC_EDITSPELLCHECK   :  CfgEditspellcheck   ();  break;
    case CRC_EDITUNDELETE     :  CfgEditundelete     ();  break;
    case CRC_ENDGROUP         :  CfgEndgroup         ();  break;
    case CRC_ENCODEEMAILHEADERS: CfgEncodeemailheaders(); break;
    case CRC_EVENT            :  CfgEvent            ();  break;
    case CRC_EXCLUDENODES     :  CfgExcludenodes     ();  break;
    case CRC_EXTERNOPTIONS    :  CfgExternoptions    ();  break;
    case CRC_EXTERNUTIL       :  CfgExternutil       ();  break;
    case CRC_EZYCOMMSGBASE    :  CfgEzycommsgbase    ();  break;
    case CRC_EZYCOMUSERBASE   :  CfgEzycomuserbase   ();  break;
    case CRC_EZYCOMUSERNO     :  CfgEzycomuserno     ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchF:
  switch(crc) {
    case CRC_FIDOLASTREAD     :  CfgFidolastread     ();  break;
    case CRC_FIDOMSGTYPE      :  CfgFidomsgtype      ();  break;
    case CRC_FIDOUSERLIST     :  CfgFidouserlist     ();  break;
    case CRC_FIDOUSERNO       :  CfgFidouserno       ();  break;
    case CRC_FILEALIAS        :  CfgFilealias        ();  break;
    case CRC_FORCETEMPLATE    :  CfgForcetemplate    ();  break;
    case CRC_FRQEXT           :  CfgFrqext           ();  break;
    case CRC_FRQNODEMAP       :  CfgFrqnodemap       ();  break;
    case CRC_FRQOPTIONS       :  CfgFrqoptions       ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchG:
  switch(crc) {
    case CRC_GEDHANDSHAKE     :  CfgGedhandshake     ();  break;
    case CRC_GERMANKEYBOARD   :  CfgGermankeyboard   ();  break;
    case CRC_GOLDBASEPATH     :  CfgGoldbasepath     ();  break;
    case CRC_GOLDBASESYSPATH  :  CfgGoldbasesyspath  ();  break;
    case CRC_GOLDBASEUSERNO   :  CfgGoldbaseuserno   ();  break;
    case CRC_GOLDPATH         :  CfgGoldpath         ();  break;
    case CRC_GROUP            :  CfgGroup            ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchH:
  switch(crc) {
    case CRC_HUDSONPATH       :  CfgHudsonpath       ();  break;
    case CRC_HUDSONSIZEWARN   :  CfgHudsonsizewarn   ();  break;
    case CRC_HUDSONSYSPATH    :  CfgHudsonsyspath    ();  break;
    case CRC_HUDSONUSERNO     :  CfgHudsonuserno     ();  break;
    case CRC_HIGHLIGHTURLS    :  CfgHighlighturls    ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchI:
  switch(crc) {
    case CRC_IGNORECHARSET    :  CfgIgnorecharset    ();  break;
    case CRC_IMPORTBEGIN      :  CfgImportbegin      ();  break;
    case CRC_IMPORTEND        :  CfgImportend        ();  break;
    case CRC_INBOUNDPATH      :  CfgInboundpath      ();  break;
    case CRC_INCLUDENODES     :  CfgIncludenodes     ();  break;
    case CRC_INPUTFILE        :  CfgInputfile        ();  break;
    case CRC_INTENSECOLORS    :  CfgIntensecolors    ();  break;
    case CRC_INTERNETADDRESS  :  CfgInternetaddress  ();  break;
    case CRC_INTERNETDOMAIN   :  CfgInternetdomain   ();  break;
    case CRC_INTERNETGATE     :  CfgInternetgate     ();  break;
    case CRC_INTERNETGATEEXP  :  CfgInternetgateexp  ();  break;
    case CRC_INTERNETMSGID    :  CfgInternetmsgid    ();  break;
    case CRC_INTERNETRFCBODY  :  CfgInternetrfcbody  ();  break;
    case CRC_INTERNETVIAGATE  :  CfgInternetviagate  ();  break;
    case CRC_INVALIDATE       :  CfgInvalidate       ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchJ:
  switch(crc) {
    case CRC_JAMPATH          :  CfgJampath          ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchK:
  switch(crc) {
    case CRC_KEYBMODE         :  CfgKeybmode         ();  break;
    case CRC_KEYBSTACK        :  CfgKeybstack        ();  break;
    case CRC_KLUDGE           :  CfgKludge           ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchL:
  switch(crc) {
    case CRC_LOADLANGUAGE     :  CfgLoadlanguage     ();  break;
    case CRC_LOCATIONALIAS    :  CfgLocationalias    ();  break;
    case CRC_LOCATIONDELIMITER:  CfgLocationdelimiter();  break;
    case CRC_LOGFILE          :  CfgLogfile          ();  break;
    case CRC_LOGFORMAT        :  CfgLogformat        ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchM:
  switch(crc) {
    case CRC_MAILINGLIST      :  CfgMailinglist      ();  break;
    case CRC_MAILTOSS         :  CfgMailtoss         ();  break;
    case CRC_MAPPATH          :  CfgMappath          ();  break;
    case CRC_MEMBER           :  CfgMember           ();  break;
    case CRC_MENUMARKED       :  CfgMenumarked       ();  break;
    case CRC_MENUNEXTAREA     :  CfgMenunextarea     ();  break;
    case CRC_MSGLISTDATE      :  CfgMsglistdate      ();  break;
    case CRC_MSGLISTFAST      :  CfgMsglistfast      ();  break;
    case CRC_MSGLISTFIRST     :  CfgMsglistfirst     ();  break;
    case CRC_MSGLISTHEADER    :  CfgMsglistheader    ();  break;
    case CRC_MSGLISTWIDESUBJ  :  CfgMsglistwidesubj  ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchN:
  switch(crc) {
    case CRC_NAMESFILE        :  CfgNamesfile        ();  break;
    case CRC_NICKNAME         :  CfgNickname         ();  break;
    case CRC_NETNAME          :  CfgNetname          ();  break;
    case CRC_NODELIST         :  CfgNodelist         ();  break;
    case CRC_NODEPATH         :  CfgNodepath         ();  break;
    case CRC_NODEPATHFD       :  CfgNodepathfd       ();  break;
    case CRC_NODEPATHIM       :  CfgNodepathfd       ();  break;
    case CRC_NODEPATHV7       :  CfgNodepathv7       ();  break;
    case CRC_NODEV7FLAGS      :  CfgNodev7flags      ();  break;
    case CRC_NODEV7MODEM      :  CfgNodev7modem      ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchO:
  switch(crc) {
    case CRC_ORGANIZATION     :  CfgOrganization     ();  break;
    case CRC_ORIGIN           :  CfgOrigin           ();  break;
    case CRC_OUTBOUNDPATH     :  CfgOutboundpath     ();  break;
    case CRC_OUTPUTFILE       :  CfgOutputfile       ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchP:
  switch(crc) {
    case CRC_PCBOARDPATH      :  CfgPcboardpath      ();  break;
    case CRC_PCBOARDUSERNO    :  CfgPcboarduserno    ();  break;
    case CRC_PEEKURLOPTIONS   :  CfgPeekurloptions   ();  break;
    case CRC_PERSONALMAIL     :  CfgPersonalmail     ();  break;
    case CRC_PLAY             :  CfgPlay             ();  break;
    case CRC_PRINTDEVICE      :  CfgPrintdevice      ();  break;
    case CRC_PRINTINIT        :  CfgPrintinit        ();  break;
    case CRC_PRINTLENGTH      :  CfgPrintlength      ();  break;
    case CRC_PRINTMARGIN      :  CfgPrintmargin      ();  break;
    case CRC_PRINTRESET       :  CfgPrintreset       ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchQ:
  switch(crc) {
    case CRC_QUOTEBUFFILE     :  CfgQuotebuffile     ();  break;
    case CRC_QUOTEBUFMODE     :  CfgQuotebufmode     ();  break;
    case CRC_QUOTECHARS       :  CfgQuotechars       ();  break;
    case CRC_QUOTECTRL        :  CfgQuotectrl        ();  break;
    case CRC_QUOTEMARGIN      :  CfgQuotemargin      ();  break;
    case CRC_QUOTESTRING      :  CfgQuotestring      ();  break;
    case CRC_QUOTESTOPS       :  CfgQuotestops       ();  break;
    case CRC_QUOTEWRAPHARD    :  CfgQuotewraphard    ();  break;
    case CRC_QUOTEUSENEWAI    :  CfgQuoteusenewai    ();  break;
    case CRC_QWKBADMSGS       :  CfgQwkbadmsgs       ();  break;
    case CRC_QWKCONFMAP       :  CfgQwkconfmap       ();  break;
    case CRC_QWKEXPORTPATH    :  CfgQwkexportpath    ();  break;
    case CRC_QWKIMPORTPATH    :  CfgQwkimportpath    ();  break;
    case CRC_QWKOPTIONS       :  CfgQwkoptions       ();  break;
    case CRC_QWKREPLYLINKER   :  CfgQwkreplylinker   ();  break;
    case CRC_QWKTOSSLOG       :  CfgQwktosslog       ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchR:
  switch(crc) {
    case CRC_RA2USERSBBS      :  CfgRa2usersbbs      ();  break;
    case CRC_REPLYLINK        :  CfgReplylink        ();  break;
    case CRC_REPLYLINKFLOAT   :  CfgReplylinkfloat   ();  break;
    case CRC_REPLYLINKLIST    :  CfgReplylinklist    ();  break;
    case CRC_REPLYLINKSHOWALWAYS:CfgReplylinkshowalways();break;
    case CRC_ROBOTNAME        :  CfgRobotname        ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchS:
  switch(crc) {
    case CRC_SCREENBLANKER    :  CfgScreenblanker    ();  break;
    case CRC_SCREENMAXCOL     :  CfgScreenmaxcol     ();  break;
    case CRC_SCREENMAXROW     :  CfgScreenmaxrow     ();  break;
    case CRC_SCREENPALETTE    :  CfgScreenpalette    ();  break;
    case CRC_SCREENSIZE       :  CfgScreensize       ();  break;
    case CRC_SEARCHFOR        :  CfgSearchfor        ();  break;
    case CRC_SEMAPHORE        :  CfgSemaphore        ();  break;
    case CRC_SEQDIR           :  CfgSeqDir           ();  break;
    case CRC_SEQMSGID         :  CfgSeqMsgId         ();  break;
    case CRC_SEQOUTRUN        :  CfgSeqOutRun        ();  break;
    case CRC_SHAREMODE        :  CfgSharemode        ();  break;
    case CRC_SHOWDELETED      :  CfgShowdeleted      ();  break;
    case CRC_SOUNDPATH        :  CfgSoundpath        ();  break;
    case CRC_SOUPBADMSGS      :  CfgSoupbadmsgs      ();  break;
    case CRC_SOUPEMAIL        :  CfgSoupemail        ();  break;
    case CRC_SOUPEXPORTMARGIN :  CfgSoupexportmargin ();  break;
    case CRC_SOUPEXPORTPATH   :  CfgSoupexportpath   ();  break;
    case CRC_SOUPIMPORTPATH   :  CfgSoupimportpath   ();  break;
    case CRC_SOUPNEWSRCFILE   :  CfgSoupnewsrcfile   ();  break;
    case CRC_SOUPREPLYLINKER  :  CfgSoupreplylinker  ();  break;
    case CRC_SOUPREPLYTO      :  CfgSoupreplyto      ();  break;
    case CRC_SOUPTOSSLOG      :  CfgSouptosslog      ();  break;
    case CRC_SQUISHSCAN       :  CfgSquishscan       ();  break;
    case CRC_SQUISHUSERNO     :  CfgSquishuserno     ();  break;
    case CRC_SQUISHUSERPATH   :  CfgSquishuserpath   ();  break;
    case CRC_STATUSLINEHELP   :  CfgStatuslinehelp   ();  break;
    case CRC_STRIPHTML        :  CfgStripHTML        ();  break;
    case CRC_STYLECODEPUNCT   :  CfgStylecodepunct   ();  break;
    case CRC_STYLECODES       :  CfgStylecodes       ();  break;
    case CRC_STYLECODESTOPS   :  CfgStylecodestops   ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchT:
  switch(crc) {
    case CRC_TAGLINE          :  CfgTagline          ();  break;
    case CRC_TAGLINECHAR      :  CfgTaglinechar      ();  break;
    case CRC_TAGLINESUPPORT   :  CfgTaglinesupport   ();  break;
    case CRC_TASKTITLE        :  CfgTasktitle        ();  break;
    case CRC_TEARLINE         :  CfgTearline         ();  break;
    case CRC_TEMPLATE         :  CfgTemplate         ();  break;
    case CRC_TEMPLATEMATCH    :  CfgTemplatematch    ();  break;
    case CRC_TEMPLATEPATH     :  CfgTemplatepath     ();  break;
    case CRC_TEMPPATH         :  CfgTemppath         ();  break;
    case CRC_TIMEOUT          :  CfgTimeout          ();  break;
    case CRC_TITLESTATUS      :  CfgTitlestatus      ();  break;
    case CRC_TWITMODE         :  CfgTwitmode         ();  break;
    case CRC_TWITNAME         :  CfgTwitname         ();  break;
    case CRC_TWITSUBJ         :  CfgTwitsubj         ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchU:
  switch(crc) {
    case CRC_UNPACKER         :  CfgUnpacker         ();  break;
    case CRC_URLHANDLER       :  CfgUrlhandler       ();  break;
    case CRC_USEAREA          :  CfgUsearea          ();  break;
    case CRC_USECHARSET       :  CfgUsecharset       ();  break;
    case CRC_USEFWD           :  CfgUsefwd           ();  break;
    case CRC_USEINTL          :  CfgUseintl          ();  break;
    case CRC_USEPID           :  CfgUsepid           ();  break;
    case CRC_USERLIST         :  CfgUserlist         ();  break;
    case CRC_USERLISTFILE     :  CfgUserlistfile     ();  break;
    case CRC_USERNAME         :  CfgUsername         ();  break;
    case CRC_USESOFTCRXLAT    :  CfgUsesoftcrxlat    ();  break;
    case CRC_USETZUTC         :  CfgUsetzutc         ();  break;
    case CRC_UUDECODEPATH     :  CfgUudecodepath     ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchV:
  switch(crc) {
    case CRC_VIEWHIDDEN       :  CfgViewhidden       ();  break;
    case CRC_VIEWKLUDGE       :  CfgViewkludge       ();  break;
    case CRC_VIEWQUOTE        :  CfgViewquote        ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchW:
  switch(crc) {
    case CRC_WHOTO            :  CfgWhoto            ();  break;
    case CRC_WILDCATUSERNO    :  CfgWildcatuserno    ();  break;
    case CRC_WRITEHEADER      :  CfgWriteheader      ();  break;
    case CRC_WRITETEMPLATE    :  CfgWritetemplate    ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchX:
  switch(crc) {
    case CRC_XLATCHARSET      :  CfgXlatcharset      ();  break;
    case CRC_XLATESCSET       :  CfgXlatescset       ();  break;
    case CRC_XLATEXPORT       :  CfgXlatexport       ();  break;
    case CRC_XLATIMPORT       :  CfgXlatimport       ();  break;
    case CRC_XLATLOCALSET     :  CfgXlatlocalset     ();  break;
    case CRC_XLATPATH         :  CfgXlatpath         ();  break;
    default                   :  found = false;
  }
  goto End;

SwitchY:
  switch(crc) {
    default                   :  found = false;
  }
  goto End;

SwitchZ:
  switch(crc) {
    case CRC_ZONEGATING       :  CfgZonegating       ();  break;
    default                   :  found = false;
  }
  goto End;

End:
  if(found)
    return true;

  if(cmdlineoldkeyw == false)
    return false;

  switch(crc) {
    case CRC_AREA             :  CfgArea             ();  break;
    case CRC_REGISTERKEY      :                           break;
    case CRC_REGISTERNAME     :                           break;
    case CRC_SERIALNO         :                           break;
    default                   :  return false;
  }

  return true;
}


//  ------------------------------------------------------------------

static int do_if(char* val) {

  if(strieql(val, "OS/2") or strieql(val, "OS2")) {
    #ifdef __OS2__
    return true;
    #else
    return false;
    #endif
  }
  else if(strieql(val, "NT") or strieql(val, "W32") or strieql(val, "WIN32")) {
    #ifdef __WIN32__
    return true;
    #else
    return false;
    #endif
  }
  else if(strieql(val, "386") or strieql(val, "DOS") or strieql(val, "DPMI32")) {
    #ifdef __MSDOS__
    return true;
    #else
    return false;
    #endif
  }
  else if(strieql(val, "LINUX") or strieql(val, "UNIX")) {
    #ifdef __UNIX__
    return true;
    #else
    return false;
    #endif
  }
  else if(strieql(val, "FIREBIRD"))
    return true;
  else if(strieql(val, "ASA") or strieql(val, "PLUS"))
    return true;
  else if(strieql(val, "YES") or strieql(val, "TRUE") or strieql(val, "ON"))
    return true;
  return !!atoi(val);
}


//  ------------------------------------------------------------------
//  Compile a GoldED text config file

int ReadCfg(const char* cfgfile, int ignoreunknown) {

  const word CRC_APP      = 0x08B5;

  FILE* fp;
  Path cfg;
  char buf[1024];
  static int inuse = 0;
  static int in_if = false;
  static int in_else = false;
  static int if_status = false;
  char* val;
  char* key;
  char* cfgname;
  word crc;
  int cfgignore=NO, line=0;

  // Open the file
  strcpy(cfg, cfgfile);
  MakePathname(cfg, CFG->goldpath, cfg);

  fp = fsopen(cfg, "rt", CFG->sharemode);
  if(fp) {

    cfgname = strrchr(cfg, '\\');
    cfgname = cfgname ? cfgname+1 : cfg;
    inuse++;

    if(not quiet)
      std::cout << "* Reading " << cfg << std::endl;

    // Assign file buffer
    setvbuf(fp, NULL, _IOFBF, 8192);

    // Read each line
    while(fgets((val=buf), sizeof(buf), fp)) {

      line++;

      // Replace TABs with SPACEs
      strchg(val, '\t', ' ');

      val = strskip_wht(val);

      if(isalpha(*val)) {

        // Extract key and val
        crc = getkeyvalcrc(&key, &val);

        int _gotcond = true;
        switch(crc) {
          case CRC_IF:
            if(in_if) {
              std::cout << "* " << cfgname << ": Misplaced IF at line " << line << ". IF's cannot be nested." << std::endl;
              cfgerrors++;
            }
            in_if = true;
            if_status = do_if(val);
            cfgignore = not if_status;
            break;
          case CRC_ELIF:
          case CRC_ELSEIF:
            if(not in_if or in_else) {
              std::cout << "* " << cfgname << ": Misplaced ELIF/ELSEIF at line " << line << "." << std::endl;
              cfgerrors++;
            }
            if(if_status)
              cfgignore = true;
            else {
              if_status = do_if(val);
              cfgignore = not if_status;
            }
            break;
          case CRC_ELSE:
            if(not in_if or in_else) {
              std::cout << "* " << cfgname << ": Misplaced ELSE at line " << line << "." << std::endl;
              cfgerrors++;
            }
            in_else = true;
            if_status ^= true;
            cfgignore = not if_status;
            break;
          case CRC_ENDIF:
            if(not in_if) {
              std::cout << "* " << cfgname << ": Misplaced ENDIF at line " << line << "." << std::endl;
              cfgerrors++;
            }
            cfgignore = false;
            in_if = in_else = false;
            break;
          case CRC_IGNORE:
            cfgignore ^= true;
            break;
          default:
            _gotcond = false;
        }

        // Tell the world what we found
        if(veryverbose)
        {
#if defined(_MSC_VER)
            char buff[256];
            sprintf(buff, " %c %04d: %s %s\n", cfgignore ? '-' : '+', line, key, val);
            std::cout << buff;
#else
           std::cout << " " << (cfgignore ? '-' : '+') << std::setw(4) << std::setfill('0') << line << std::setfill(' ') << ": " << key << " " << val << std::endl;
#endif
        }

        // Call switch function to act on the key
        if(not cfgignore) {
          if(not _gotcond) {
            switch(crc) {
              case CRC_INCLUDE:
                strschg_environ(val);
                MapPath(val);
                if(not quiet)
                  std::cout << "* Including " << val << std::endl;
                ReadCfg(val);          // NOTE! This is a recursive call!
                if(not quiet)
                  std::cout << "* Resuming " << cfg << std::endl;
                break;
              case CRC_AREAFILE:
                strschg_environ(val);
                MapPath(val);
                if(not quiet)
                  std::cout << "* Handling " << key << " " << val << std::endl;
                AL.GetAreafile(val);
                if(not quiet)
                  std::cout << "* Resuming " << cfg << std::endl;
                break;
              case CRC_APP:
                // Ignore 3rd party application lines
                break;
              case CRC_REM:
                // Ignore remark lines
                break;
              default:
                if(not SwitchCfg(crc, *key, val)) {
                  if(not ignoreunknown) {
                    std::cout << "* " << cfgname << ": Unknown keyword \"" << key << "\" at line " << line << "." << std::endl;
                    SayBibi();
                    cfgerrors++;
                  }
                }
            }
          }
        }
      }
    }

    // Close and return all OK
    fclose(fp);
    inuse--;

    // When the final cfg is compiled
    if(inuse == 0) {

      // Mark all areas listed in the NEWSRC file as newsgroups
      gfile gfp;
      gfp.fopen(CFG->soupnewsrcfile, "rt");
      if(gfp.isopen()) {
        char buf2[512];
        while(gfp.fgets(buf2, sizeof(buf2))) {
          char* ptr = strpbrk(buf2, ":! ");
          if(ptr) {
            *ptr = NUL;
            Area* ap = AL.AreaEchoToPtr(buf2);
            if(ap)
              ap->set_type(ap->isnet() ? GMB_SOUP|GMB_EMAIL|GMB_NET : GMB_SOUP|GMB_NEWSGROUP|GMB_ECHO);
          }
        }
        gfp.fclose();
      }

      if(*CFG->soupemail) {
        Area* ap = AL.AreaEchoToPtr(buf);
        if(ap)
          ap->set_type(GMB_SOUP|GMB_EMAIL|GMB_NET);
      }

      std::vector<MailList>::iterator z;
      for(z = CFG->mailinglist.begin(); z != CFG->mailinglist.end(); z++) {
        Area* ap = AL.AreaEchoToPtr(z->echoid);
        if(ap)
          ap->set_type(GMB_SOUP|GMB_EMAIL|GMB_NET);
      }

      // Mark all QWK areas
      if(QWK->FirstBBS()) {
        do {
          if(QWK->FirstConf()) {
            do {
              Area* ap = AL.AreaEchoToPtr(buf);
              if(ap)
                ap->set_type(ap->type() | GMB_QWK);
            } while(QWK->NextConf());
          }
        } while(QWK->NextBBS());
      }
    }

    return 1;
  }
  else {

    LOG.ErrOpen();
    LOG.printf("! Unable to open configuration file.");
    LOG.printf(": %s", cfg);
    LOG.ErrOSInfo();
    OpenErrorExit();
  }

  // Error opening file
  return 0;
}


//  ------------------------------------------------------------------

