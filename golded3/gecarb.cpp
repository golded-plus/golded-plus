
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
//  Carbon Copy.
//  ------------------------------------------------------------------

#include <golded.h>

//  ------------------------------------------------------------------

void AddCCList(const char *ptr, bool cchide, GMsg* msg, GMsg** carbon, int &cc, Area* A, Area* AA, int &xc2to)
{
  Addr addr;
  char name[256];

  addr.reset_fast();
  strtrim(strcpy(name, strskip_wht(ptr)));
  (*carbon) = (GMsg*)throw_realloc((*carbon), sizeof(GMsg)*(cc+2));
  (*carbon)[cc] = *msg;
  (*carbon)[cc].dest.reset_fast();
  (*carbon)[cc].odest.reset_fast();
  (*carbon)[cc].attr.nwm1();
  if(strchr(name, ':')) {
    char* d = strchr(name, '@');
    if(d) *d = NUL;
  }
  if(strchr(name, '@')) {
    ParseInternetAddr(name, (*carbon)[cc].realto, (*carbon)[cc].iaddr);
    strxcpy((*carbon)[cc].to, (*carbon)[cc].iaddr, sizeof(Name));
  }
  else {
    (*carbon)[cc].iaddr[0] = NUL;
    strxcpy((*carbon)[cc].to, name, sizeof(Name));
  }
  HeaderView->Use(AA, &(*carbon)[cc]);
  HeaderView->Paint();
  if(AA->isnet()) {
    gsetaddr toname, toaddr, fromaddr;
    IAdr addr1, addr2;

    addr = (*carbon)[cc].orig;
    toname.buf = name; toname.update = false;
    toaddr.buf = addr1; toaddr.update = false; *addr1 = NUL;
    fromaddr.buf = addr2; fromaddr.update = false; *addr2 = NUL;
    if(set_to_address(&(*carbon)[cc], &toname, &toaddr, &fromaddr, NULL, 6, LNG->DestinationCC, true))
      addr.net = 0;
    else
      addr.set(addr1, (*carbon)[cc].odom);
  }
  else
    addr = AA->Aka().addr;
  update_statusline(LNG->ProcessCC);
  if(addr.net or AA->islocal()) {
    // Create a CC if either address or name is different than the original.
    if(memcmp(&addr, &msg->dest, sizeof(Addr)) or stricmp(name, msg->to) or (A != AA and not xc2to++)) {
      (*carbon)[cc].odest = addr;
      (*carbon)[cc].dest = addr;
      if(cchide)
        (*carbon)[cc].attr.tou0();
      else
        (*carbon)[cc].attr.tou1();
      strxcpy((*carbon)[cc].to, name, sizeof(Name));
      cc++;
    }
  }
}


//  ------------------------------------------------------------------

int DoCarboncopy(GMsg* msg, GMsg** carbon) {

  int n, cc = 0;
  const char* ptr;
  Attr attr;
  bool cchide;
  bool ignorecc = false;
  char buf[256], buf2[256];
  Line* line;
  Line* newline;
  Line* ccline = NULL;
  int margintext;

  if(AA->isinternet() and CFG->soupexportmargin <= CFG->dispmargin)
    margintext = CFG->soupexportmargin;
  else
    margintext = CFG->dispmargin;

  // Insert empty line at the top first for practical purposes

  newline = new Line("");
  throw_xnew(newline);
  msg->lin = newline = line = InsertLine(newline, msg->lin, DIR_PREV);

  // Deal with carbon copies

  Area* A = AA;

  if(AA->isecho()) {
    AA = AL.AreaEchoToPtr(AA->Areareplyto());
    if(AA and (AA != A)) {
      AA->Open();
    }
    else
      AA = A;
  }

  attr = msg->attr;
  if(not AA->isecho() and line) {
    bool ask = true;
    int xc2to = 0;

    do {
      ptr = newline->txt.c_str();
      if(strnieql(ptr, "CC:", 3)) {
        if(not ignorecc) {
          if(ask) {
            msg->attr.nwm1();

            AttrAdd(&msg->attr, &CFG->attribscc);

            HeaderView->Use(AA, msg);
            HeaderView->Paint();
            GMenuCarbon MenuCarbon;
            ignorecc = MenuCarbon.Run(msg) ? false : true;
            if(ignorecc)                    // Do not process carcon copies
              break;
            if(newline)
              ccline = newline->prev;       // Store the position of first line
            else
              ccline = NULL;

            ask = false;
          }
          update_statusline(LNG->ProcessCC);

          gstrarray xposts;
          tokenize(xposts, ptr+3, ",");

          for(int i=0; i < xposts.size(); i++) {

            ptr = strskip_wht(strcpy(buf, xposts[i].c_str()));
            if(*ptr == '#') {
              ++ptr;
              cchide = true;
            }
            else
              cchide = false;

            if(*ptr == '@') {
              FILE* fp = fsopen(AddPath(CFG->goldpath, ++ptr), "rt", CFG->sharemode);
              if(fp) {
                gstrarray fc;

                while(fgets(buf, sizeof(buf), fp))
                  tokenize(fc, buf, ",\n");

                fclose(fp);

                for(int j = 0; j < fc.size(); j++)
                  if(fc[j][0] != '@')  // we don't like nesting!
                    AddCCList(fc[j].c_str(), cchide, msg, carbon, cc, A, AA, xc2to);
              }
            }
            else
              AddCCList(ptr, cchide, msg, carbon, cc, A, AA, xc2to);
          }

          if(CFG->carboncopylist != CC_KEEP) {
            newline = DeleteLine(newline);  // Now at the next line
            if(newline and newline->prev)
              newline = newline->prev;      // Go back one line
          }
        }
      }
      newline = newline->next;
    } while(newline != NULL);

    string temp;
    // Fix the CC list in the message
    if(cc and ccline) {
      switch(CFG->carboncopylist) {
        case CC_REMOVE:
          // No list
          break;
        case CC_KEEP:
          // Keep list as it was entered
          break;
        case CC_HIDDEN:
          if(A == AA) {
            sprintf(buf, "\001CC: %s %s", msg->To(), msg->dest.make_string(temp).c_str());
            ccline = AddKludge(ccline, buf);
          }
          for(n=0; n<cc; n++) {
            if((*carbon)[n].attr.tou()) {
              sprintf(buf, "\001CC: %s %s", (*carbon)[n].To(), (*carbon)[n].dest.make_string(temp).c_str());
              ccline = AddKludge(ccline, buf);
            }
          }
          break;
        case CC_VISIBLE:
          sprintf(buf2, LNG->CCTo, LNG->ListCC);
          if(A == AA) {
            sprintf(buf, buf2, msg->To(), msg->dest.make_string(temp).c_str());
            ccline = AddLine(ccline, buf);
          }
          for(n=0; n<cc; n++) {
            if((*carbon)[n].attr.tou()) {
              sprintf(buf, buf2, (*carbon)[n].To(), (*carbon)[n].dest.make_string(temp).c_str());
              ccline = AddLine(ccline, buf);
            }
          }
          break;
        case CC_NAMES:
          // Expand in line
          {
            string hline = "";
            int line_items = 0;

            if(A == AA) {
              hline += msg->To();
              ++line_items;
            }
            for(n=0; n < cc; n++)
              if((*carbon)[n].attr.tou()) {
                const char *user = (*carbon)[n].To();
                if((hline.length() + strlen(user) + 2 > margintext) and line_items) {
                  sprintf(buf, LNG->CCTo, hline.c_str());
                  ccline = AddLine(ccline, buf);
                  line_items = 0;
                  hline = "";
                }
                if(line_items != 0)
                  hline += ", ";
                hline += user;
                ++line_items;
              }
            if(line_items) {
              sprintf(buf, LNG->CCTo, hline.c_str());
              ccline = AddLine(ccline, buf);
            }
          }
          break;
      }
    }
  }

  // Reset line pointer

  msg->lin = DeleteLine(msg->lin);  // Delete the empty top line again
  msg->attr = attr;

  if(*carbon and not cc)
    throw_release(*carbon);

  if(A != AA) {
    AA->Close();
    AA = A;
  }

  return cc;
}


//  ------------------------------------------------------------------

static bool AddXCAreas(const char* mask, bool attr, vector<int> &postareas, vector<bool> &postareas_attrs, int local_xps) {

  bool rv = false;

  for(int j=0; j < AL.size(); j++) {
    if(not AL[j]->isseparator() and strwild(AL[j]->echoid(), mask)) {

      if(strieql(AA->echoid(), AL[j]->echoid()))
        rv = attr;
      else {
        int i;
        // We don't want twice crossposting to same echo...
        for(i=local_xps; i < postareas.size(); i++)
          if(postareas[i] == j)
            break;
        if(i == postareas.size()) {
          postareas.push_back(j);
          postareas_attrs.push_back(attr);
        }
      }
    }
  }
  return rv;
}


//  ------------------------------------------------------------------

void DoCrosspost(GMsg* msg, vector<int> &postareas) {

  if(CFG->crosspost == NO)
    return;
  char buf[256];
  const char *ptr;
  bool ignorexc = false;
  Line* newline;
  Line* xcline = NULL;
  int margintext;
  bool xphide;
  int local_xps = postareas.size();
  vector<bool> postareas_attrs(local_xps);
  bool hideoriginal = false;

  // Insert empty line at the top first for practical purposes

  newline = new Line("");
  throw_xnew(newline);
  msg->lin = newline = InsertLine(newline, msg->lin, DIR_PREV);

  // Deal with carbon copies

  if(AA->isinternet() and CFG->soupexportmargin <= CFG->dispmargin)
    margintext = CFG->soupexportmargin;
  else
    margintext = CFG->dispmargin;
  // Next line is invalid if someone uses format chars like %-20s
  // but we couldn't predict everything...
  margintext -= strlen(LNG->Crosspostedin) - 2;

  if(newline) {
    bool ask = true;

    do {

      if(not (newline->type & (GLINE_KLUDGE|GLINE_TEAR|GLINE_ORIG))) {

        ptr = newline->txt.c_str();
        if(not ignorexc and (strnieql(ptr, "XC:", 3) or strnieql(ptr, "XP:", 3))) {
          if(ask) {
            GMenuCross MenuCross;
            ignorexc = (CFG->crosspost != ASK) or MenuCross.Run(msg) ? false : true;
            if(ignorexc)                // Do not process crossposting
              break;
            if(newline)
              xcline = newline->prev;   // Store the position of first line
            else
              xcline = NULL;

            ask = false;
          }

          gstrarray xposts;
          tokenize(xposts, ptr+3);

          for(int i=0; i < xposts.size(); i++) {

            ptr = strcpy(buf, xposts[i].c_str());
            if(*ptr == '#') {
              ++ptr;
              xphide = true;
            }
            else
              xphide = false;

            if(*ptr == '@') {
              FILE* fp = fsopen(AddPath(CFG->goldpath, ++ptr), "rt", CFG->sharemode);
              if(fp) {
                gstrarray fc;

                while(fgets(buf, sizeof(buf), fp))
                  tokenize(fc, buf, ", \n");

                fclose(fp);

                for(int j = 0; j < fc.size(); j++)
                  if(fc[j][0] != '@')  // we don't like nesting!
                    hideoriginal = AddXCAreas(fc[j].c_str(), xphide, postareas, postareas_attrs, local_xps) or hideoriginal;
              }
            }
            else
              hideoriginal = AddXCAreas(ptr, xphide, postareas, postareas_attrs, local_xps) or hideoriginal;
          }

          if(CFG->crosspostlist != CC_KEEP) {
            newline = DeleteLine(newline);  // Now at the next line
            if(newline and newline->prev)
              newline = newline->prev;      // Go back one line
          }
        }
      }
      newline = newline->next;
    } while(newline != NULL);

    // Fix the XC list in the message, ignore crossposting to itself only
    if((local_xps < postareas.size()+1) and xcline) {
      switch(CFG->crosspostlist) {
        case CC_REMOVE:
          // No list
          break;
        case CC_KEEP:
          // Keep list as it was entered
          break;
        case CC_VISIBLE:
          // Expand in line
          {
            if(not hideoriginal) {
              sprintf(buf, LNG->Originallyin, AA->echoid());
              xcline = AddLine(xcline, buf);
            }

            string hline = "";
            int line_items = 0;

            for(int i=local_xps; i < postareas.size(); i++) {
              const char *echoid = AL[postareas[i]]->echoid();
              if(postareas_attrs[i] or strieql(AA->echoid(), echoid))
                continue;
              if((hline.length() + strlen(echoid) + 2 > margintext) and line_items) {
                sprintf(buf, LNG->Crosspostedin, hline.c_str());
                xcline = AddLine(xcline, buf);
                line_items = 0;
                hline = "";
              }
              if(line_items != 0)
                hline += ", ";
              hline += echoid;
              ++line_items;
            }
            if(line_items) {
              sprintf(buf, LNG->Crosspostedin, hline.c_str());
              xcline = AddLine(xcline, buf);
            }
          }
          break;
        case CC_NAMES:
          // Expand in column
          {
            if(not hideoriginal) {
              sprintf(buf, LNG->Originallyin, AA->echoid());
              xcline = AddLine(xcline, buf);
            }

            for(int i=local_xps; i < postareas.size(); i++) {
              const char *echoid = AL[postareas[i]]->echoid();
              if(postareas_attrs[i] or strieql(AA->echoid(), echoid))
                continue;
              sprintf(buf, LNG->Crosspostedin, echoid);
              xcline = AddLine(xcline, buf);
            }
          }
      }
    }
  }

  // Reset line pointer

  msg->lin = DeleteLine(msg->lin);  // Delete the empty top line again
}


//  ------------------------------------------------------------------
