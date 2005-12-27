
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
//  Prototypes for the configuration keyword functions.
//  ------------------------------------------------------------------


#ifndef __GCPROT_H
#define __GCPROT_H


//  ------------------------------------------------------------------

void CfgAddress          ();
void CfgAddressbookadd   ();
void CfgAddresslookupfirst();
void CfgAddressmacro     ();
void CfgAdeptxbbspath    ();
void CfgAdeptxbbsuserno  ();
void CfgAkamatch         ();
void CfgAkamatchecho     ();
void CfgAkamatchfromto   ();
void CfgAkamatching      ();
void CfgAkamatchlocal    ();
void CfgAkamatchmanually ();
void CfgAkamatchnet      ();
void CfgArea             ();
void CfgAreaautoid       ();
void CfgAreaautonext     ();
void CfgAreacfmreplyto   ();
void CfgAreacopyaddid    ();
void CfgAreacopydirect   ();
void CfgAreacopyto       ();
void CfgAreadef          ();
void CfgAreadesc         ();
void CfgAreaexcl         ();
void CfgAreafilegroups   ();
void CfgAreaforwarddirect();
void CfgAreafreqdirect   ();
void CfgAreafreqto       ();
void CfgAreaincl         ();
void CfgAreaisemail      ();
void CfgAreaisnews       ();
void CfgAreakeeplast     ();
void CfgArealistechomax  ();
void CfgArealistformat   ();
void CfgArealistgroupid  ();
void CfgArealistgrouporder();
void CfgArealistnos      ();
void CfgArealistpagebar  ();
void CfgArealistscan     ();
void CfgArealistsort     ();
void CfgArealisttype     ();
void CfgAreapath         ();
void CfgAreapmscan       ();
void CfgAreapmscanexcl   ();
void CfgAreapmscanincl   ();
void CfgAreareadonly     ();
void CfgArearecyclebin   ();
void CfgArearename       ();
void CfgAreareplydirect  ();
void CfgAreareplyto      ();
void CfgAreascan         ();
void CfgAreascanexcl     ();
void CfgAreascanincl     ();
void CfgAreascansort     ();
void CfgAreasep          ();
void CfgAreastart        ();
void CfgAreatypeorder    ();
void CfgAreayouwroteto   ();
void CfgAskdelorig       ();
void CfgAttachpath       ();
void CfgAttribsattach    ();
void CfgAttribscc        ();
void CfgAttribscfm       ();
void CfgAttribsecho      ();
void CfgAttribsemail     ();
void CfgAttribsfrq       ();
void CfgAttribslocal     ();
void CfgAttribsnet       ();
void CfgAttribsnews      ();
void CfgAttributes       ();
void CfgBeepcomment      ();
void CfgBeeplocalmsg     ();
void CfgBeepnoises       ();
void CfgBeepfactor       ();
void CfgBeepyourmail     ();
void CfgCarboncopylist   ();
void CfgColor            ();
void CfgColorname        ();
void CfgColorset         ();
void CfgConfirmfile      ();
void CfgConfirmresponse  ();
void CfgCookiepath       ();
void CfgCrosspost        ();
void CfgCrosspostlist    ();
void CfgCtrlinfo         ();
void CfgCtrlinfoecho     ();
void CfgCtrlinfoemail    ();
void CfgCtrlinfolocal    ();
void CfgCtrlinfonet      ();
void CfgCtrlinfonews     ();
void CfgDispareano       ();
void CfgDispattachsize   ();
void CfgDispautonext     ();
void CfgDisphdrdateset   ();
void CfgDisphdrlocation  ();
void CfgDisphdrnameset   ();
void CfgDisphdrnodeset   ();
void CfgDisplistcursor   ();
void CfgDisplistwrap     ();
void CfgDisplocalhigh    ();
void CfgDispmargin       ();
void CfgDispmsgsize      ();
void CfgDisppagebar      ();
void CfgDisppmfirst      ();
void CfgDisprealmsgno    ();
void CfgDispsoftcr       ();
void CfgDispstatusline   ();
void CfgDisptabsize      ();
void CfgDosprompt        ();
void CfgEditautoattach   ();
void CfgEditautosave     ();
void CfgEditchangedate   ();
void CfgEditcharpara     ();
void CfgEditcharspace    ();
void CfgEditcomment      ();
void CfgEditcompletion   ();
void CfgEditcrlfterm     ();
void CfgEditfieldclear   ();
void CfgEdithardline     ();
void CfgEdithardlines    ();
void CfgEdithardterm     ();
void CfgEdithdrnameset   ();
void CfgEdithdrnodeset   ();
void CfgEditheaderattrs  ();
void CfgEditheaderfirst  ();
void CfgEditinternal     ();
void CfgEditmenu         ();
void CfgEditmixcase      ();
void CfgEditor           ();
void CfgEditorfile       ();
void CfgEditquotemargin  ();
void CfgEditreplyre      ();
void CfgEditsavemenu     ();
void CfgEditsaveutil     ();
void CfgEditsoftcrxlat   ();
void CfgEditspellcheck   ();
void CfgEditundelete     ();
void CfgEmptytearline    ();
void CfgEncodeemailheaders();
void CfgEndgroup         ();
void CfgEvent            ();
void CfgExcludenodes     ();
void CfgExternoptions    ();
void CfgExternutil       ();
void CfgEzycommsgbase    ();
void CfgEzycomuserbase   ();
void CfgEzycomuserno     ();
void CfgFidohwmarks      ();
void CfgFidolastread     ();
void CfgFidomsgtype      ();
void CfgFidonullfix      ();
void CfgFidouserlist     ();
void CfgFidouserno       ();
void CfgFilealias        ();
void CfgFilelistpagebar  ();
void CfgFollowupto       ();
void CfgForcetemplate    ();
void CfgFrqext           ();
void CfgFrqnodemap       ();
void CfgFrqoptions       ();
void CfgFrqwazoo         ();
void CfgGedhandshake     ();
void CfgGermankeyboard   ();
void CfgGoldbasepath     ();
void CfgGoldbasesyspath  ();
void CfgGoldbaseuserno   ();
void CfgGoldpath         ();
void CfgGroup            ();
void CfgHighlightunread  ();
void CfgHighlighturls    ();
void CfgHudsonpath       ();
void CfgHudsonsizewarn   ();
void CfgHudsonsyspath    ();
void CfgHudsonuserno     ();
void CfgIgnorecharset    ();
void CfgImportbegin      ();
void CfgImportend        ();
void CfgInboundpath      ();
void CfgIncludenodes     ();
void CfgInputfile        ();
void CfgIntensecolors    ();
void CfgInternetaddress  ();
void CfgInternetdomain   ();
void CfgInternetgate     ();
void CfgInternetgateexp  ();
void CfgInternetlookup   ();
void CfgInternetmsgid    ();
void CfgInternetreply    ();
void CfgInternetrfcbody  ();
void CfgInternetviagate  ();
void CfgInvalidate       ();
void CfgJampath          ();
void CfgKeybclear        ();
void CfgKeybdefaults     ();
void CfgKeybext          ();
void CfgKeybmode         ();
void CfgKeybstack        ();
void CfgKludge           ();
void CfgKludgechrs       ();
void CfgLatintolocal     ();
void CfgLoadlanguage     ();
void CfgLocationalias    ();
void CfgLocationdelimiter();
void CfgLogfile          ();
void CfgLogformat        ();
void CfgLookupecho       ();
void CfgLookuplocal      ();
void CfgLookupnet        ();
void CfgLookupuserbase   ();
void CfgMailinglist      ();
void CfgMailtoss         ();
void CfgMappath          ();
void CfgMember           ();
void CfgMenudropmsg      ();
void CfgMenumarked       ();
void CfgMenunextarea     ();
void CfgMouse            ();
void CfgMsglistdate      ();
void CfgMsglistfast      ();
void CfgMsglistfirst     ();
void CfgMsglistheader    ();
void CfgMsglistpagebar   ();
void CfgMsglistviewsubj  ();
void CfgMsglistwidesubj  ();
void CfgNamesfile        ();
void CfgNewsgroups       ();
void CfgNickname         ();
void CfgNetname          ();
void CfgNodelist         ();
void CfgNodelistwarn     ();
void CfgNodepath         ();
void CfgNodepathfd       ();
void CfgNodepathfd       ();
void CfgNodepathv7       ();
void CfgNodev7flags      ();
void CfgNodev7modem      ();
void CfgOrganization     ();
void CfgOrigin           ();
void CfgOutboundpath     ();
void CfgOutputfile       ();
void CfgPathreportfile   ();
void CfgPcboardpath      ();
void CfgPcboarduserno    ();
void CfgPeekurloptions   ();
void CfgPersonalmail     ();
void CfgPlay             ();
void CfgPrintdevice      ();
void CfgPrintinit        ();
void CfgPrintlength      ();
void CfgPrintmargin      ();
void CfgPrintreset       ();
void CfgQuoteblank       ();
void CfgQuotectrl        ();
void CfgQuotebuffile     ();
void CfgQuotebufmode     ();
void CfgQuotechars       ();
void CfgQuotemargin      ();
void CfgQuotespacing     ();
void CfgQuotestring      ();
void CfgQuotestops       ();
void CfgQuotewraphard    ();
void CfgQuoteusenewai    ();
void CfgQwkbadmsgs       ();
void CfgQwkconfmap       ();
void CfgQwkexportpath    ();
void CfgQwkimportpath    ();
void CfgQwkoptions       ();
void CfgQwkreplylinker   ();
void CfgQwktosslog       ();
void CfgRa2usersbbs      ();
void CfgReplylink        ();
void CfgReplylinkfloat   ();
void CfgReplylinklist    ();
void CfgReplylinkshowalways();
void CfgReplyto          ();
void CfgRobotname        ();
#if defined(__GOLD_SPELL__)
void CfgScheckerdeflang  ();
void CfgScheckerenabled  ();
void CfgScheckeruserdic  ();
#endif
void CfgScreenblanker    ();
void CfgScreenmaxcol     ();
void CfgScreenmaxrow     ();
void CfgScreenpalette    ();
void CfgScreenshadows    ();
void CfgScreensize       ();
void CfgScreenusebios    ();
void CfgSearchfor        ();
void CfgSemaphore        ();
void CfgSeqDir           ();
void CfgSeqMsgId         ();
void CfgSeqOutRun        ();
void CfgSharemode        ();
void CfgShowdeleted      ();
void CfgSounddevice      ();
void CfgSoundpath        ();
void CfgSoupbadmsgs      ();
void CfgSoupemail        ();
void CfgSoupexportmargin ();
void CfgSoupexportpath   ();
void CfgSoupimportpath   ();
void CfgSoupnewsrcfile   ();
void CfgSoupreplylinker  ();
void CfgSoupreplyto      ();
void CfgSouptosslog      ();
void CfgSquishdirect     ();
void CfgSquishscan       ();
void CfgSquishuserno     ();
void CfgSquishuserpath   ();
void CfgStatuslineclock  ();
void CfgStatuslinehelp   ();
void CfgStripHTML        ();
void CfgStylecodes       ();
void CfgStylecodepunct   ();
void CfgStylecodestops   ();
void CfgTagline          ();
void CfgTaglinechar      ();
void CfgTaglinesupport   ();
void CfgTasktitle        ();
void CfgTearline         ();
void CfgTemplate         ();
void CfgTemplatematch    ();
void CfgTemplatepath     ();
void CfgTemppath         ();
void CfgTimeout          ();
void CfgTimeoutsavemsg   ();
void CfgTitlestatus      ();
void CfgTwitmode         ();
void CfgTwitname         ();
void CfgTwitsubj         ();
void CfgTwitto           ();
void CfgUnpacker         ();
void CfgUrlhandler       ();
void CfgUsearea          ();
void CfgUsecharset       ();
void CfgUseflags         ();
void CfgUsefwd           ();
void CfgUseintl          ();
void CfgUsemsgid         ();
void CfgUsepid           ();
void CfgUserlist         ();
void CfgUserlistfile     ();
void CfgUsername         ();
void CfgUsesoftcrxlat    ();
void CfgUsetzutc         ();
void CfgUudecodepath     ();
void CfgViewhidden       ();
void CfgViewkludge       ();
void CfgViewquote        ();
void CfgWhoto            ();
void CfgWildcatuserno    ();
void CfgWriteheader      ();
void CfgWritetemplate    ();
void CfgXlatcharset      ();
void CfgXlatcharsetalias ();
void CfgXlatescset       ();
void CfgXlatexport       ();
void CfgXlatimport       ();
void CfgXlatlocalset     ();
void CfgXlatpath         ();
void CfgZonegating       ();


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
