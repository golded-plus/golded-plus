
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
//  Configuration functions, part 7.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

extern char* val;


//  ------------------------------------------------------------------

void CfgQuotestring() {

  char buf[10];
  strxcpy(buf, StripQuotes(val), sizeof(buf));
  if(*buf == NUL)
    strcpy(buf, " > ");
  else {
    if(buf[strlen(buf)-1] != ' ')
      strcat(buf, " ");
  }
  if(cfgingroup)
    CFG->grp.AddItm(GRP_QUOTESTRING, buf, strlen(buf)+1);
  else
    strcpy(CFG->quotestring, buf);
}

//  ------------------------------------------------------------------

void CfgQuotestops() {

  char buf[10];
  strxcpy(buf, StripQuotes(val), sizeof(buf));
  if(*buf == NUL)
    strcpy(buf, " > ");
  else {
    if(buf[strlen(buf)-1] != ' ')
      strcat(buf, " ");
  }
  if(cfgingroup)
    CFG->grp.AddItm(GRP_QUOTESTOPS, buf, strlen(buf)+1);
  else
    strcpy(CFG->quotestops, buf);
}

//  ------------------------------------------------------------------

void CfgQuotewraphard() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_QUOTEWRAPHARD, flag);
  else
    CFG->quotewraphard = flag;
}

//  ------------------------------------------------------------------

void CfgQuoteusenewai()
{
  CFG->quoteusenewai = make_bool(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgQwkbadmsgs() {

  QWK->BadMsgs(val);
}

//  ------------------------------------------------------------------

void CfgQwkconfmap() {

  char* bbsid;
  getkeyval(&bbsid, &val);

  char* confname;
  getkeyval(&confname, &val);
  strtrim(StripQuotes(confname));

  char* echoid;
  getkeyval(&echoid, &val);

  QWK->AddMap(bbsid, echoid, confname);
}

//  ------------------------------------------------------------------

void CfgQwkexportpath() {

  QWK->ExportPath(val);
}

//  ------------------------------------------------------------------

void CfgQwkimportpath() {

  QWK->ImportPath(val);
}

//  ------------------------------------------------------------------

void CfgQwkoptions() {

  char* bbsid;
  getkeyval(&bbsid, &val);
  QWK->FindAddBBS(bbsid);
  val = strtok(val, ", \t");
  while(val) {
    if(strieql(val, "KLUDGES"))
      QWK->KludgesAllowed(true);
    else if(strieql(val, "NOKLUDGES"))
      QWK->KludgesAllowed(false);
    else if(strieql(val, "MIXCASE"))
      QWK->MixCaseAllowed(true);
    else if(strieql(val, "NOMIXCASE"))
      QWK->MixCaseAllowed(false);
    else if(strieql(val, "RECEIPT"))
      QWK->ReceiptAllowed(true);
    else if(strieql(val, "NORECEIPT"))
      QWK->ReceiptAllowed(false);
    else if(strnieql(val, "MAXLINES=", 9))
      QWK->MaxLines(atoi(val+9));
    val = strtok(NULL, ", \t");
  }
}

//  ------------------------------------------------------------------

void CfgQwkreplylinker() {

  QWK->ReplyLinker(val);
}

//  ------------------------------------------------------------------

void CfgQwktosslog() {

  QWK->TossLog(val);
}

//  ------------------------------------------------------------------

void CfgRa2usersbbs() {

  CFG->ra2usersbbs = GetYesno(val) ? 2 : 1;
}

//  ------------------------------------------------------------------

void CfgReplylink() {

  if(strieql(val, "Chain"))
    CFG->replylink = REPLYLINK_CHAIN;
  else if(strieql(val, "Direct"))
    CFG->replylink = REPLYLINK_DIRECT;
}

//  ------------------------------------------------------------------

void CfgReplylinkfloat()
{
  CFG->replylinkfloat = make_bool(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgReplylinklist() {

  if(strieql(val, "Fast"))
    CFG->replylinklist = REPLYLINKLIST_FAST;
  else if(strieql(val, "Full"))
    CFG->replylinklist = REPLYLINKLIST_FULL;
}

//  ------------------------------------------------------------------

void CfgReplylinkshowalways()
{
  CFG->replylinkshowalways = make_bool(GetYesno(val));
}
//  ------------------------------------------------------------------

void CfgRobotname(const char* v)  {

  char* ptr = val = throw_strdup(v);
  CfgRobotname();
  throw_free(ptr);
}

void CfgRobotname() {

  tokenize(CFG->robotname, val);
}

//  ------------------------------------------------------------------

#if !defined(GCFG_NOSPELLDLL)
void CfgScheckerdeflang()
{
  CFG->scheckerdeflang = atoi(val);
  if (CFG->scheckerdeflang == 0)
    CFG->scheckerdeflang = 0xFFFF;
}

//  ------------------------------------------------------------------

void CfgScheckerenabled()
{
  CFG->scheckerenabled = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgScheckeruserdic()
{
  strcpy(CFG->scheckeruserdic, StripQuotes(val));
}
#endif

//  ------------------------------------------------------------------

void CfgScreenblanker() {

  GTok t;
  if(t.First(val)) {
    do {
      val = t.Token();
      if(isdigit(*val))
        CFG->screenblanker = atoi(val);
      else if(striinc("BLACK", val))
        CFG->screenblankertype = BLANK_ALLBLACK;
    } while(t.Next());
  }
}

//  ------------------------------------------------------------------

void CfgScreenmaxcol() {

  CFG->screenmaxcol = atoi(val);
}

//  ------------------------------------------------------------------

void CfgScreenmaxrow() {

  CFG->screenmaxrow = atoi(val);
}

//  ------------------------------------------------------------------

void CfgScreenpalette() {

  //  Syntax:
  //
  //    SCREENPALETTE <reg> <val>   or
  //    SCREENPALETTE <reg> (red green blue)

  char* key;
  getkeyval(&key, &val);

  int palreg = atoi(key);
  if(palreg >= 0 and palreg <= 15) {
    if(*val != '(')
      CFG->screenpalette[palreg] = atoi(val);
    else {
      val = strskip_wht(val+1);
      int redval = atoi(val);         // 0 .. 3
      val = strskip_wht(strskip_txt(val));
      int greenval = atoi(val);       // 0 .. 3
      val = strskip_wht(strskip_txt(val));
      int blueval = atoi(val);        // 0 .. 3

      // Construct the rgbRGB color val (nuts!)
      CFG->screenpalette[palreg] =
        ((redval   & 2) ? 0x04 : 0x00)|
        ((redval   & 1) ? 0x20 : 0x00)|
        ((greenval & 2) ? 0x02 : 0x00)|
        ((greenval & 1) ? 0x10 : 0x00)|
        ((blueval  & 2) ? 0x01 : 0x00)|
        ((blueval  & 1) ? 0x08 : 0x00);
    }
    CFG->screenpalette[16] = true;    // Indicate that changes were made
  }
}

//  ------------------------------------------------------------------

void CfgScreensize() {

  CFG->screensize = atoi(val);
  if(CFG->screensize == 4350) {
    CFG->screensize = 50;
  }
  else if(CFG->screensize == 0) {
    if(strnieql(val, "Mode", 4)) {
      sscanf(val+4, " %x", (uint *)&CFG->screensize);
      CFG->screensize <<= 8;
    }
  }
}


//  ------------------------------------------------------------------

void CfgSearchfor() {

  INam buf;
  strxcpy(buf, StripQuotes(val), sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_SEARCHFOR, buf, strlen(buf)+1);
  else
    strcpy(CFG->searchfor, buf);
}

//  ------------------------------------------------------------------

#define CRC_NETSCAN            0xEC12
#define CRC_ECHOSCAN           0x6E6D
#define CRC_EXPORTLIST         0x1B14
#define CRC_IMPORTLIST         0x447F
#define CRC_SCANALL            0x1E57
#define CRC_SCANTHIS           0x1DAE
#define CRC_SCANNETMAIL        0x74D6
#define CRC_PMSCANALL          0xD093
#define CRC_PMSCANTHIS         0xE12C
#define CRC_PMSCANNETMAIL      0x9F1F
#define CRC_QWKIMPORT          0x3063
#define CRC_QWKEXPORT          0xD7C2
#define CRC_SOUPIMPORT         0x5564
#define CRC_SOUPEXPORT         0xB2C5
#define CRC_EXITNOW            0x65DD
#define CRC_IDLETIME           0x3BBC

void CfgSemaphore() {

  char* key;
  word crc = getkeyvalcrc(&key, &val);

  switch(crc) {
    case CRC_NETSCAN:       strcpy(CFG->semaphore.netscan, val);       break;
    case CRC_ECHOSCAN:      strcpy(CFG->semaphore.echoscan, val);      break;
    case CRC_EXPORTLIST:    strcpy(CFG->semaphore.exportlist, val);    break;
    case CRC_IMPORTLIST:    strcpy(CFG->semaphore.importlist, val);    break;
    case CRC_SCANALL:       strcpy(CFG->semaphore.scanall, val);       break;
    case CRC_SCANTHIS:      strcpy(CFG->semaphore.scanthis, val);      break;
    case CRC_SCANNETMAIL:   strcpy(CFG->semaphore.scannetmail, val);   break;
    case CRC_PMSCANALL:     strcpy(CFG->semaphore.pmscanall, val);     break;
    case CRC_PMSCANTHIS:    strcpy(CFG->semaphore.pmscanthis, val);    break;
    case CRC_PMSCANNETMAIL: strcpy(CFG->semaphore.pmscannetmail, val); break;
    case CRC_QWKIMPORT:     strcpy(CFG->semaphore.qwkimport, val);     break;
    case CRC_QWKEXPORT:     strcpy(CFG->semaphore.qwkexport, val);     break;
    case CRC_SOUPIMPORT:    strcpy(CFG->semaphore.soupimport, val);    break;
    case CRC_SOUPEXPORT:    strcpy(CFG->semaphore.soupexport, val);    break;
    case CRC_EXITNOW:       strcpy(CFG->semaphore.exitnow, val);       break;
    case CRC_IDLETIME:      CFG->semaphore.idletime = atoi(val);       break;
    default:
      return;
  }
}

//  ------------------------------------------------------------------

void CfgSeqDir() {

  MapPath(PathCopy(CFG->seqdir, val));
}

//  ------------------------------------------------------------------

void CfgSeqMsgId() {

  CFG->seqmsgid = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgSeqOutRun_Error()
{
  STD_PRINTNL("* Warning: Bad SeqOutrun value \'" << val << "\', ignored.");
}

void CfgSeqOutRun() {

  char *p;
  uint32_t s = 0;

  if(not isdigit((int)(*val))) {
    CfgSeqOutRun_Error();
    return;
  }

  s = (uint32_t)atol(val);
  p = val;
  while(isdigit((int)(*p)))
    p++;
  if(*p == '\0')
  {
    CFG->seqoutrun = s;

    if (veryverbose)
      STD_PRINTNL("  SeqOutRun: \'" << val << "\' --> " << CFG->seqoutrun);

    return;
  }
  if(p[1]) {
    CfgSeqOutRun_Error();
    return;
  }
  switch(g_tolower(*p)) {
    case 'y':
      s *= 365;
    case 'd':
      s *= 24;
    case 'h':
      s *= 60*60;
      break;
    case 'w':
      s *= 7l*24*60*60;
      break;
    case 'm':
      s *= 31l*24*60*60;
      break;
    default:
      CfgSeqOutRun_Error();
      return;
  }
  CFG->seqoutrun = s;

  if (veryverbose)
    STD_PRINTNL("  SeqOutRun: \'" << val << "\' --> " << CFG->seqoutrun);
}

//  ------------------------------------------------------------------

void CfgSharemode() {

  if(atoi(val))
    CFG->sharemode = atoi(val);
  else
    CFG->sharemode = GetYesno(val) ? SH_DENYNO : SH_COMPAT;
}

//  ------------------------------------------------------------------

void CfgShowdeleted() {

  CFG->showdeleted = make_bool(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgSoundpath() {

  PathCopy(CFG->soundpath, val);
}

//  ------------------------------------------------------------------

void CfgSoupbadmsgs() {

  strupr(strxcpy(CFG->soupbadmsgs, val, sizeof(Echo)));
}

//  ------------------------------------------------------------------

void CfgSoupemail() {

  strupr(strxcpy(CFG->soupemail, val, sizeof(Echo)));
}

//  ------------------------------------------------------------------

void CfgSoupexportmargin() {

  CFG->soupexportmargin = atoi(val);
}

//  ------------------------------------------------------------------

void CfgSoupexportpath() {

  PathCopy(CFG->soupexportpath, val);
}

//  ------------------------------------------------------------------

void CfgSoupimportpath() {

  PathCopy(CFG->soupimportpath, val);
}

//  ------------------------------------------------------------------

void CfgSoupnewsrcfile() {

  strcpy(CFG->soupnewsrcfile, val);
}

//  ------------------------------------------------------------------

void CfgSoupreplylinker() {

  strcpy(CFG->soupreplylinker, val);
}

//  ------------------------------------------------------------------

void CfgSoupreplyto() {

  strxcpy(CFG->soupreplyto, val, sizeof(CFG->soupreplyto));
}

//  ------------------------------------------------------------------

void CfgSouptosslog() {

  strcpy(CFG->souptosslog, val);
}

//  ------------------------------------------------------------------

void CfgSquishscan() {

  const int SQS_API   = 1;
  const int SQS_QUICK = 2;

  if(strieql(val, "API"))
    CFG->squishscan = SQS_API;
  else if(strieql(val, "QUICK"))
    CFG->squishscan = SQS_QUICK;
}

//  ------------------------------------------------------------------

