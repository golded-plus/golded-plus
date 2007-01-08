
//  ------------------------------------------------------------------
//  Route Diagram Drawing Tool.
//  Copyright (C) 1999 Odinn Sorensen
//  Copyright (C) 1999-2001 Alexander S. Aganichev
//  Copyright (C) 2005 Stas Degteff
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
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  Designed to work with the Goldware library. May need some work..
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------


#include <clocale>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>

using namespace std;

#include <gstrall.h>
#include <gftnall.h>
#include <golded3.h>
#include <gmemdbg.h>

int debug = false;


//  ------------------------------------------------------------------

class node {

public:

  node() { order = 0; depth = 0; used = false; posts = 0; }

  bool                used;
  int                 order;
  int                 depth;
  int                 posts;
  ftn_addr           address;
  string             name;
  vector<ftn_addr>   links;
  string             display;

  void add(const char* s) { ftn_addr n = s; add(n); }
  void add(ftn_addr& addr);

  node& operator=(const node& a);

};


//  ------------------------------------------------------------------

typedef list<node> list_node;

//  ------------------------------------------------------------------

class nodetree {

public:

  nodetree() { maxname = 0; maxdisp = 0; order = 0; indent = 2; posts = 0; }

  list_node nodes;

  int order;
  int maxname;
  int maxdisp;
  int indent;
  int posts;

  void prepare(ftn_addr& previous, ftn_addr& current, int depth);
  void sort();
  void print();

  void add(node& a);

};


//  ------------------------------------------------------------------

node& node::operator=(const node& a) {

  used = a.used;
  order = a.order;
  depth = a.depth;
  address = a.address;
  name = a.name;
  display = a.display;
  links = a.links;
  return *this;
}


//  ------------------------------------------------------------------

bool equals2d(ftn_addr& a, ftn_addr& b) {

  if(a == b)
    return true;
  else if((a.net == b.net) and (a.node == b.node) and (a.point == b.point))
    return true;
  return false;
}


//  ------------------------------------------------------------------

void node::add(ftn_addr& addr) {

  if(equals2d(addr, address))
    return;

  for(int l=0; l<links.size(); l++) {
    if(equals2d(addr, links[l]))
      return;
  }

  if(debug) {
    char buf1[50], buf2[50];
    cout << address.make_string(buf1) << " += " << addr.make_string(buf2) << endl;
  }

  links.push_back(addr);
}


//  ------------------------------------------------------------------

void nodetree::add(node& anode) {

  for(list_node::iterator n=nodes.begin(); n != nodes.end(); n++) {
    if(equals2d(anode.address, n->address)) {
      if(anode.address.zone) {
        n->address.zone = anode.address.zone;
        n->name = anode.name;
      }
      for(int l=0; l<anode.links.size(); l++)
        n->add(anode.links[l]);
      return;
    }
  }

  if(debug) {
    char buf[50];
    cout << anode.address.make_string(buf) << endl;
  }
  nodes.push_back(anode);
}


//  ------------------------------------------------------------------

void nodetree::prepare(ftn_addr& previous, ftn_addr& current, int depth) {

  if(debug) {
    char buf[50];
    cout << "prepare(" << previous.make_string(buf);
    cout << ", " << current.make_string(buf);
    cout << ", " << depth << ")" << endl;
  }

  // Find top node
  for(list_node::iterator n=nodes.begin(); n != nodes.end(); n++) {
    if(not n->used) {
      if(equals2d(n->address, current)) {
        n->order = ++order;
        n->depth = depth;
        n->used = true;
        for(int l=0; l<n->links.size(); l++) {
          //if(not equals2d(n->links[l], previous))
          if(n->links[l] != previous)
            prepare(current, n->links[l], depth+1);
        }
        break;
      }
    }
  }
}


//  ------------------------------------------------------------------

bool _compare_nodes(const node& a, const node& b) {

  int cmp = compare_two(a.order ? 0 : 1, b.order ? 0 : 1);
  if(cmp)
    return (cmp < 0);
  cmp = compare_two(a.order, b.order);
  if(cmp)
    return (cmp < 0);
  return (a.address.compare(b.address) < 0);
}

#if defined(_MSC_VER)

inline bool operator<(const node& A,const node& B){ return _compare_nodes(A,B); }

#define compare_nodes
#else
#define compare_nodes _compare_nodes
#endif

//  ------------------------------------------------------------------

void nodetree::sort() {

  nodes.sort(compare_nodes);
}


//  ------------------------------------------------------------------

void nodetree::print() {

  list_node::iterator n;

  for(n=nodes.begin(); n != nodes.end(); n++) {
    char buf[100];
    strsetsz(strcpy(buf, n->name.c_str()), maxname+indent);
    n->display = buf;
    list_node::iterator x;
    bool above, below;
    for(int d=1; d<n->depth; d++) {
      above = false;
      below = false;
      if(d > 0) {
        for(x=n; x-- != nodes.begin();) {
          if(x->depth < d)
            break;
          else if(x->depth == d) {
            above = true;
            break;
          }
        }
        for(x=n; ++x != nodes.end();) {
          if(x->depth < d)
            break;
          else if(x->depth == d) {
            below = true;
            break;
          }
        }
      }

      if(above and below) {
#ifdef KOI8
        strcpy(buf, "");
#else
        strcpy(buf, "³");
#endif
      } else
        *buf = NUL;
      strsetsz(buf, indent);
      n->display += buf;
    }

    if(n->depth) {
#ifdef KOI8
      strcpy(buf, "„");
#else
      strcpy(buf, "À");
#endif
      strsetsz(buf, indent);
#ifdef KOI8
      strchg(buf, ' ', '€');
#else
      strchg(buf, ' ', 'Ä');
#endif
      n->display += buf;
    }

    n->display += n->address.make_string(buf);

    maxdisp = maximum_of_two((size_t)maxdisp, n->display.length());
  }

  for(n=nodes.begin(); n != nodes.end(); n++) {
    list_node::iterator x=n;
    if(++x != nodes.end()) {
#ifdef KOI8
      const char* p = strchr(n->display.c_str(), '„');
#else
      const char* p = strchr(n->display.c_str(), 'À');
#endif
      if(p) {
        int len = p - n->display.c_str();
#ifdef KOI8
        if((x->display[len] == '') or (x->display[len] == '„'))
          n->display[len] = '†';
#else
        if((x->display[len] == '³') or (x->display[len] == 'À'))
          n->display[len] = 'Ã';
#endif
      }
    }
    n->display.resize(maxdisp, ' ');
    char buf[40];
    sprintf(buf, "%*s[%i]", indent, "", n->depth);
    n->display += buf;
  }

  for(n=nodes.begin(); n != nodes.end(); n++)
    cout << n->display.c_str() << endl;
}


//  ------------------------------------------------------------------

int main(int argc, char** argv) {

  throw_init();

  // set locale
  setlocale(LC_CTYPE, "");

  cout << "Route Diagram Drawing Tool v." << __gver_shortver__ << __gver_platform__ << __gver_postversion__;
#ifdef KOI8
  cout << " (koi8)";
#endif
  cout << endl
       << "Copyright (C) 1999 Odinn Sorensen" << endl
       << "Copyright (C) 1999-2001 Alexander S. Aganichev" << endl
       << "Copyright (C) 2005 Stas Degteff & Golded+ team" << endl
       << "----------------------------------------------------------------------" << endl
       << endl;

  if(argc < 2) {
    cout << "Syntax: RDDT <routefile> [options] [address or name]" << endl
       << "\twhere options may be:" << endl
       << "\t\t-d             - Debug mode" << endl
       << "\t\t-p             - Decode path" << endl
       << "\t\t-n ownaddress  - Your own address" << endl
       << "\t\t-l ownuplink   - Your own uplink" << endl
       << "\t\t-i num         - indent <num> spaces" << endl
       << endl;

    return 1;
  }

  #if defined(GUTLOS_FUNCS)
  g_init_os(0);
  #endif

  node      anode;
  nodetree  atree;
  ifstream  fp(argv[1]);
  ftn_addr address;
  bool decode_path = false;
  char* top = NULL;
  ftn_addr ownaddress;
  ftn_addr ownuplink;

  for(int argn=2; argn<argc; argn++) {
    char* argp = argv[argn];
    if(*argp == '-') {
      switch(argp[1]) {
        case 'd':
          debug = true;
          break;
        case 'p':
          decode_path = true;
          break;
        case 'n':
          ownaddress = argp + 2;
          break;
        case 'l':
          ownuplink = argp + 2;
          break;
        case 'i':
          atree.indent = atoi(argp + 2);
          break;
        default:
          cerr << "Illegal option '" << argp << "', ignored" << endl;
      }
    }
    else
      top = argp;
  }

  if(fp) {

    if(anode.links.size()) {
      atree.add(anode);
      anode.links.resize(0);
    }

    char buf[2048];
    while(not fp.eof()) {
      fp.getline(buf, sizeof(buf));
      if((*buf != ';') and (*buf != '/')) {
        gstrarray links;
        tokenize(links, buf, " ");
        if(links.size() > 1) {
          anode.links.resize(0);
          strchg(&links[0][0], '_', ' ');
          anode.name = links[0];
          atree.maxname = maximum_of_two(anode.name.length(), (size_t)atree.maxname);
          anode.address = links[1];
          if(links.size() > 2) {
            if(decode_path) {
              anode.posts++;
              atree.posts++;
              int n;
              address = links[2];
              if(not equals2d(address, anode.address))
                anode.add(address);
              for(n=3; n<links.size(); n++) {
                char buf[50];
                address.set(links[n]);
                links[n] = address.make_string(buf);
              }
              atree.add(anode);
              for(n=2; n<links.size(); n++) {
                anode.links.resize(0);
                anode.name = "?";
                anode.address = links[n];
                anode.add(links[n-1].c_str());
                if((n+1) < links.size())
                  anode.add(links[n+1].c_str());
                atree.add(anode);
              }
            }
            else {
              address = anode.address;
              for(int n=2; n<links.size(); n++) {
                address.set(links[n]);
                anode.add(address);
              }
              atree.add(anode);
            }
          }
          else if(anode.address == ownaddress) {
            anode.add(ownuplink);
            atree.add(anode);
            anode.name = "?";
            anode.address = ownuplink;
            anode.links.resize(0);
            anode.add(ownaddress);
            atree.add(anode);
          }
        }
      }
    }

    fp.close();

    address = 0;
    node& xnode = anode;
    if(top == NULL) {
      xnode = *atree.nodes.begin();
      address = xnode.address;
    }
    else if(isdigit(*top)) {
      ftn_addr match = top;
      for(list_node::iterator n=atree.nodes.begin(); n != atree.nodes.end(); n++) {
        if(n->address.match(match)) {
          xnode = *n;
          address = n->address;
          break;
        }
      }
    }
    else {
      for(list_node::iterator n=atree.nodes.begin(); n != atree.nodes.end(); n++) {
        if(n->name.find(top) != n->name.npos) {
          xnode = *n;
          address = n->address;
          break;
        }
      }
    }

    if(address.valid()) {

      if(debug) {
        char buf[50];
        for(list_node::iterator n=atree.nodes.begin(); n != atree.nodes.end(); n++) {
          xnode = *n;
          cout << xnode.address.make_string(buf) << endl;
          for(int l=0; l<xnode.links.size(); l++)
            cout << "  " << xnode.links[l].make_string(buf) << endl;
        }
      }

      atree.prepare(address, address, 0);
      atree.sort();
      atree.print();
    }
    else {
      cout << "Address or name not found!" << endl;
    }
  }

  #if defined(GUTLOS_FUNCS)
  g_deinit_os();
  #endif
  return 0;
}


//  ------------------------------------------------------------------
