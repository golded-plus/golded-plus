
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
//  Configuration functions, part 5.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gccfgg.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

extern char* val;


//  ------------------------------------------------------------------

void CfgHappybirthday() {

  if(strieql(val, "FRIEND"))
    CFG->happybirthday = true;
}

//  ------------------------------------------------------------------

void CfgHudsonpath(const char *path, bool force) {

  if(force or strblank(CFG->hudsonpath))
    MapPath(PathCopy(CFG->hudsonpath, path));
}

void CfgHudsonpath() {

  CfgHudsonpath(val, true);
}

//  ------------------------------------------------------------------

void CfgHudsonsizewarn() {

  CFG->hudsonsizewarn = atol(val);
}

//  ------------------------------------------------------------------

void CfgHudsonsyspath() {

  PathCopy(CFG->hudsonsyspath, val);
}

//  ------------------------------------------------------------------

void CfgHudsonuserno() {

  CFG->hudsonuserno = atoi(val);
}

//  ------------------------------------------------------------------

void CfgIgnorecharset() {

  CFG->ignorecharset = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgImportbegin() {

  strxcpy(CFG->importbegin, StripQuotes(val), sizeof(CFG->importbegin));
}

//  ------------------------------------------------------------------

void CfgImportend() {

  strxcpy(CFG->importend, StripQuotes(val), sizeof(CFG->importend));
}

//  ------------------------------------------------------------------

void CfgInboundpath() {

  PathCopy(CFG->inboundpath, val);
}

//  ------------------------------------------------------------------

void CfgIncludenodes() {

  // Only used by GoldNODE
}

//  ------------------------------------------------------------------

void CfgInputfile() {

  Path buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_INPUTFILE, buf, strlen(buf)+1);
  else
    strcpy(CFG->inputfile, buf);
}

//  ------------------------------------------------------------------

void CfgIntensecolors() {

  CFG->intensecolors = GetYesno(val);
  if(CFG->intensecolors)
    memcpy(CFG->color, gold_color2, sizeof(CFG->color));
}

//  ------------------------------------------------------------------

void CfgInternetaddress() {

  IAdr buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_INTERNETADDRESS, buf, strlen(buf)+1);
  else
    strcpy(CFG->internetaddress, buf);
}

//  ------------------------------------------------------------------

void CfgInternetdomain() {

  strxcpy(CFG->internetdomain, val, sizeof(CFG->internetdomain));
}

//  ------------------------------------------------------------------

void CfgInternetgate() {

  Node tmp;
  char* ptr = strchr(val, ',');
  if(ptr) {
    *ptr++ = NUL;
    strxcpy(tmp.name, strtrim(val), sizeof(Name));
  }
  else {
    ptr = val;
    tmp.name[0] = NUL;
  }
  if(not CFG->aka.empty())
    tmp.addr = CFG->aka[0].addr;
  tmp.addr.set(strskip_wht(ptr));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_INTERNETGATE, &tmp, sizeof(Node));
  else
    CFG->internetgate = tmp;
}

//  ------------------------------------------------------------------

void CfgInternetgateexp() {

  uint exp = 0;
  char *key;
  do {
    getkeyval(&key, &val);
    if(strieql(key, "Name"))
      exp = (exp << 2) | RFCName;
    if(strieql(key, "Address"))
      exp = (exp << 2) | RFCAddress;
  }
  while(*val != NUL);
  CFG->internetgateexp = exp;
}

//  ------------------------------------------------------------------

void CfgInternetmsgid() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_INTERNETMSGID, flag);
  else
    CFG->internetmsgid = flag;
}

//  ------------------------------------------------------------------

void CfgInternetrfcbody() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_INTERNETRFCBODY, flag);
  else
    CFG->internetrfcbody = flag;
}

//  ------------------------------------------------------------------

void CfgInternetserver() {

  GTok t;
  if(t.First(val)) {
    int server = 0;
    if(strieql(t.Token(), "NNTP"))
      server = 1;
    else if(strieql(t.Token(), "SMTP"))
      server = 2;
    else if(strieql(t.Token(), "POP3"))
      server = 3;
    int parameter = 1;
    while(t.Next()) {
      if(server == 1)
        strxcpy(CFG->internetserver.nntp.server, t.Token(), sizeof(CFG->internetserver.nntp.server));
      else if(server == 2)
        strxcpy(CFG->internetserver.smtp.server, t.Token(), sizeof(CFG->internetserver.smtp.server));
      else if(server == 3) {
        if(parameter == 1)
          strxcpy(CFG->internetserver.pop3.server, t.Token(), sizeof(CFG->internetserver.pop3.server));
        else if(parameter == 2)
          strxcpy(CFG->internetserver.pop3.username, t.Token(), sizeof(CFG->internetserver.pop3.username));
        else if(parameter == 3)
          strxcpy(CFG->internetserver.pop3.password, t.Token(), sizeof(CFG->internetserver.pop3.password));
      }
      parameter++;
    }
  }
}

//  ------------------------------------------------------------------

void CfgInvalidate() {

  const word CRC_SEENBY = 0x01D0;
  const word CRC_CC     = 0x4343;
  const word CRC_XC     = 0x5843;
  const word CRC_XP     = 0x5850;

  char* key;

  switch(getkeyvalcrc(&key, &val)) {
    case CRC_TEARLINE:
      getkeyval(&key, &val);
      CFG->invalidate.tearline.first = StripQuotes(key);
      CFG->invalidate.tearline.second = StripQuotes(val);
      break;
    case CRC_ORIGIN:
      getkeyval(&key, &val);
      CFG->invalidate.origin.first = StripQuotes(key);
      CFG->invalidate.origin.second = StripQuotes(val);
      break;
    case CRC_SEENBY:
      getkeyval(&key, &val);
      CFG->invalidate.seenby.first = StripQuotes(key);
      CFG->invalidate.seenby.second = StripQuotes(val);
      break;
    case CRC_CC:
      getkeyval(&key, &val);
      CFG->invalidate.cc.first = StripQuotes(key);
      CFG->invalidate.cc.second = StripQuotes(val);
      break;
    case CRC_XC:
      getkeyval(&key, &val);
      CFG->invalidate.xc.first = StripQuotes(key);
      CFG->invalidate.xc.second = StripQuotes(val);
      break;
    case CRC_XP:
      getkeyval(&key, &val);
      CFG->invalidate.xp.first = StripQuotes(key);
      CFG->invalidate.xp.second = StripQuotes(val);
      break;
  }
}

//  ------------------------------------------------------------------

void CfgJampath(const char *path, bool force) {

  if(force or strblank(CFG->jampath))
    MapPath(PathCopy(CFG->jampath, path));
}

void CfgJampath() {

  CfgJampath(val, true);
}

//  ------------------------------------------------------------------

void CfgKeybmode() {

  if(strnieql(val, "Poll", 4))
    CFG->keybmode = KEYB_POLL;
  else if(strnieql(val, "Block", 5))
    CFG->keybmode = KEYB_BLOCK;
}

//  ------------------------------------------------------------------

void CfgKeybstack() {

  strcat(CFG->keybstack, val);
}

//  ------------------------------------------------------------------

void CfgKludge() {

  StripQuotes(val);
  if(*val == CTRL_A)
    val++;
  CFG->kludge.push_back(val);
}

//  ------------------------------------------------------------------

void CfgLoadlanguage() {

  Path buf;
  strxcpy(buf, val, sizeof(buf));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_LOADLANGUAGE, buf, strlen(buf)+1);
  else
    strcpy(CFG->loadlanguage, buf);
}

//  ------------------------------------------------------------------

void CfgLogfile() {

  strcpy(CFG->logfile, val);
}

//  ------------------------------------------------------------------

void CfgLogformat() {

  if(strieql(val, "FD"))
    CFG->logformat = GLOG_FD;
  else if(strieql(val, "MAX"))
    CFG->logformat = GLOG_MAX;
  else if(strieql(val, "BINK"))
    CFG->logformat = GLOG_BINK;
  else if(strieql(val, "QBBS"))
    CFG->logformat = GLOG_QBBS;
  else if(strieql(val, "DB"))
    CFG->logformat = GLOG_DB;
}

//  ------------------------------------------------------------------

void CfgMailinglist() {

  char* key;
  MailList mlst;
  mlst.sender_is_pattern = false;
  getkeyval(&key, &val);
  strxcpy(mlst.echoid, key, sizeof(Echo));
  getkeyval(&key, &val);
  strxcpy(mlst.sender, key, sizeof(IAdr));
  getkeyval(&key, &val);
  strxcpy(mlst.contribution, key, sizeof(IAdr));
  CFG->mailinglist.push_back(mlst);
}

//  ------------------------------------------------------------------

void CfgMailtoss() {

  char* key;
  MailList mlst;
  mlst.sender_is_pattern = true;
  getkeyval(&key, &val);
  strxcpy(mlst.echoid, key, sizeof(Echo));
  getkeyval(&key, &val);
  strxcpy(mlst.contribution, key, sizeof(IAdr));
  strxcpy(mlst.sender, val, sizeof(IAdr));
  CFG->mailinglist.push_back(mlst);
}

//  ------------------------------------------------------------------

void CfgMappath() {

  char* key;
  std::pair<std::string, std::string> mapentry;

  getkeyval(&key, &val);

  mapentry.first = key;
  mapentry.second = val;
  CFG->mappath.push_back(mapentry);
}

//  ------------------------------------------------------------------

void CfgMember() {

  if(cfgingroup) {
    val = strtok(val, ", \t");
    while(val) {
      CFG->grp.AddMbr(val);
      val = strtok(NULL, ", \t");
    }
  }
}

//  ------------------------------------------------------------------

void CfgMenumarked() {

  if(strieql(val, "Marked"))
    CFG->menumarked = MODE_MARKED;
  else if(strieql(val, "Current"))
    CFG->menumarked = MODE_CURRENT;
  else if(strieql(val, "Previous"))
    CFG->menumarked = MODE_PREVIOUS;
  else
    CFG->menumarked = MODE_DONT;
}

//  ------------------------------------------------------------------

void CfgMsglistdate() {

  int flag;
  if(strieql(val, "Written"))
    flag = MSGLISTDATE_WRITTEN;
  else if(strieql(val, "Arrived"))
    flag = MSGLISTDATE_ARRIVED;
  else if(strieql(val, "Received"))
    flag = MSGLISTDATE_RECEIVED;
  else
    flag = GetYesno(val) ? MSGLISTDATE_WRITTEN : MSGLISTDATE_NONE;
  if(cfgingroup)
    CFG->grp.AddItm(GRP_MSGLISTDATE, flag);
  else
    CFG->msglistdate = flag;
}

//  ------------------------------------------------------------------

void CfgMsglistfast() {

  bool flag = GetYesno(val);
  if(cfgingroup)
    CFG->grp.AddItm(GRP_MSGLISTFAST, flag);
  else
    CFG->msglistfast = flag;
}

//  ------------------------------------------------------------------

