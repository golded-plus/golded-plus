
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
//  Configuration functions, part 3.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>


//  ------------------------------------------------------------------

extern char* val;

//  ------------------------------------------------------------------

void CfgDispmargin() {

  CFG->dispmargin = atoi(val);
}

//  ------------------------------------------------------------------

void CfgDispmsgsize() {

  if(strieql("NO", val))
    CFG->dispmsgsize = NO;
  else if(strieql("BYTES", val))
    CFG->dispmsgsize = DISPMSGSIZE_BYTES;
  else if(strieql("KBYTES", val))
    CFG->dispmsgsize = DISPMSGSIZE_KBYTES;
  else if(strieql("LINES", val))
    CFG->dispmsgsize = DISPMSGSIZE_LINES;
}

//  ------------------------------------------------------------------

void CfgDispareano() {

  CFG->dispareano = GetYesno(val);
}

//  ------------------------------------------------------------------

void CfgDisppmfirst() {

  CFG->disppmfirst = make_bool(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgDispsoftcr()
{
  bool flag = make_bool(GetYesno(val));

  if (cfgingroup)
    CFG->grp.AddItm(GRP_DISPSOFTCR, flag);
  else
    CFG->dispsoftcr = flag;
}

//  ------------------------------------------------------------------

void CfgDisptabsize() {

  CFG->disptabsize = atoi(val);
}

//  ------------------------------------------------------------------

void CfgEditautoattach() {

  EDIT->AutoAttach(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEditautosave() {

  EDIT->AutoSave(atoi(val));
}

//  ------------------------------------------------------------------

void CfgEditchangedate() {

  EDIT->ChangeDate(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgEditcharpara() {

  char ch = *StripQuotes(val);
  if(ch)
    EDIT->CharPara(ch);
}

//  ------------------------------------------------------------------

void CfgEditcharspace() {

  char ch = *StripQuotes(val);
  if(ch)
    EDIT->CharSpace(ch);
}

//  ------------------------------------------------------------------

void CfgEditcomment() {

  char* key;
  getkeyval(&key, &val);
  StripQuotes(key);
  StripQuotes(val);
  EDIT->Comment.Add(key, val);
}

//  ------------------------------------------------------------------

void CfgEditcompletion() {

  char* key;
  getkeyval(&key, &val);
  StripQuotes(key);
  StripQuotes(val);
  EDIT->Completion.Add(key, val);
}

//  ------------------------------------------------------------------

void CfgEditcrlfterm() {

  EDIT->CrLfTerm(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEditfieldclear() {

  EDIT->FieldClear(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEdithardline() {

  EDIT->HardLine(StripQuotes(val));
}

//  ------------------------------------------------------------------

void CfgEdithardlines() {

  EDIT->HardLines(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEdithardterm() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_EDITHARDTERM, flag);
  else
    EDIT->HardTerm(flag);
}

//  ------------------------------------------------------------------

void CfgEdithdrnameset() {

  char* key;
  getkeyval(&key, &val);
  EDIT->HdrNamePos(atoi(key));
  EDIT->HdrNameLen(atoi(val));
}

//  ------------------------------------------------------------------

void CfgEdithdrnodeset() {

  char* key;
  getkeyval(&key, &val);
  EDIT->HdrNodePos(atoi(key));
  EDIT->HdrNodeLen(atoi(val));
}

//  ------------------------------------------------------------------

void CfgEditheaderattrs() {

  EDIT->HeaderAttrs(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEditheaderfirst() {

  int v = EDITHEADERFIRST_NO;
  GTok t;
  if(t.First(val)) {
    do {
      if(strieql("NEW", t.Token()))
        v |= EDITHEADERFIRST_NEW;
      else if(strieql("CHANGES", t.Token()))
        v |= EDITHEADERFIRST_CHANGES;
      else if(strieql("REPLIES", t.Token()))
        v |= EDITHEADERFIRST_REPLIES;
      else if(strieql("FORWARDS", t.Token()))
        v |= EDITHEADERFIRST_FORWARDS;
      else
        v = GetYesno(t.Token()) ? EDITHEADERFIRST_YES : EDITHEADERFIRST_NO;
    } while(t.Next());
  }
  EDIT->HeaderFirst(v);
}

//  ------------------------------------------------------------------

void CfgEditinternal() {

  EDIT->Internal(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEditmenu() {

  EDIT->Menu(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEditmixcase() {

  bool flag = make_bool(GetYesno(val));
  if(cfgingroup)
    CFG->grp.AddItm(GRP_EDITMIXCASE, flag);
  else
    EDIT->MixCase(flag);
}

//  ------------------------------------------------------------------

void CfgEditor() {

  EDIT->External(val);
}

//  ------------------------------------------------------------------

void CfgEditorfile() {

  EDIT->File(val);
}

//  ------------------------------------------------------------------

void CfgEditquotemargin() {

  EDIT->QuoteMargin(atoi(val));
}

//  ------------------------------------------------------------------

void CfgEditreplyre() {

  int replyre;
  if(strieql(val, "NUMERIC"))
    replyre = REPLYRE_NUMERIC;
  else
    replyre = GetYesno(val) ? REPLYRE_YES : REPLYRE_NO;

  if(cfgingroup)
    CFG->grp.AddItm(GRP_EDITREPLYRE, replyre);
  else
    EDIT->ReplyRe(replyre);
}

//  ------------------------------------------------------------------

void CfgEditsavemenu() {

  EDIT->SaveMenu(make_bool(GetYesno(val)));
}

//  ------------------------------------------------------------------

void CfgEditsaveutil() {

  char* _key;
  getkeyval(&_key, &val);
  int n = atoi(_key);
  EDIT->SaveUtil.Add(n, StripQuotes(val));
}

//  ------------------------------------------------------------------

void CfgEditsoftcrxlat() {

  EDIT->SoftCrXlat(*val);
}

//  ------------------------------------------------------------------

#if defined(GCFG_NOSPELLDLL)
void CfgEditspellcheck() {

  EDIT->SpellChecker(val);
}
#endif
//  ------------------------------------------------------------------

void CfgEditundelete() {

  EDIT->UnDelete(atoi(val));
}

//  ------------------------------------------------------------------

void CfgEncodeemailheaders() {

  CFG->encodeemailheaders = make_bool(GetYesno(val));
}

//  ------------------------------------------------------------------

void CfgEndgroup() {

  cfgingroup = false;
}

//  ------------------------------------------------------------------

void CfgEvent(char* v)  { val = v; CfgEvent(); }
void CfgEvent() {

  char* key;
  word x;
  std::vector<GEvent>::iterator n;
  GEvent tmp;

  x = getkeyvalcrc(&key, &val);

  for(n = CFG->event.begin(); n != CFG->event.end(); n++)
    if(x == n->type)
      break;

  tmp.type = x;
  getkeyval(&key, &val);
  if(strieql(key, "PLAY")) {
    tmp.command = EVTC_PLAY;
    FileToGPlay(&tmp.play, val);
  }

  if(n == CFG->event.end())
    CFG->event.push_back(tmp);
  else
    *n = tmp;
}

//  ------------------------------------------------------------------

