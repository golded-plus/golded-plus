
//  ------------------------------------------------------------------
//  The Goldware Utilities.
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2001 Alexander S. Aganichev
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
//  $Id$
//  ------------------------------------------------------------------
//  GoldNODE - A nodelist compiler for GoldED.
//  ------------------------------------------------------------------

// using namespace std;

#include <gdbgerr.h>
#include <list>
#include <map>
#include <clocale>
#include <gdefs.h>
#include <gcrcall.h>
#include <gmemall.h>
#include <gfilutil.h>
#include <gftnall.h>
#include <gftnnlge.h>
#include <gstrall.h>
#include <gshare.h>
#include <gtimall.h>
#include <gutlmisc.h>
#include <gccfgg.h>
#include <gdirposx.h>
#include <gutlos.h>
#include <glog.h>
#include <iomanip>
#include <iostream>
#include <fstream>

#define GOLDNODE_STATS 1

#ifdef GOLDNODE_STATS
#include <math.h>
#endif


//  ------------------------------------------------------------------
//  Config versions

#if defined(__OS2__)
#define __GPID__ "GoldNode+/2"
#elif defined(__WIN32__)
#define __GPID__ "GoldNode+/W32"
#elif defined(__MSDOS__)
#define __GPID__ "GoldNode+/386"
#elif defined(__linux__)
#define __GPID__ "GoldNODE+/LNX"
#elif defined(__FreeBSD__) || defined(__OpenBSD__)
#define __GPID__ "GoldNODE+/BSD"
#else
#define __GPID__ "GoldNODE+"
#endif

#define __GVER__ "1.1.5"         // Visible version


//  ------------------------------------------------------------------
// 32-bit versions

const size_t maxnodes = 262000;


//  ------------------------------------------------------------------

typedef std::vector<Addr>::iterator addr_iter;
typedef std::vector<Stamp>::iterator stamp_iter;
typedef std::list<_GEIdx> geidxlist;

// Nodelists
std::vector<Stamp> nodelist; // nodelist files,stamps,update marker
std::vector<Addr> nodezone;  // nodelist zones
std::vector<Stamp> userlist; // Userlist files,stamps,update marker
std::vector<Addr> userzone;  // Userlist zones
std::vector< std::pair<std::string, std::string> > mappath;

// Exclude/Include nodes
std::vector<Addr> excludenode;
std::vector<Addr> includenode;

// Index files
std::string  addrindex;
std::string  nodeindex;
std::string  listindex;

//  ------------------------------------------------------------------

const word _POINT = 0x0001;
const word _NODE  = 0x0002;
const word _NET   = 0x0003;
const word _ZONE  = 0x0004;
const word _TEST  = 777;

static std::string nodepath;                // Path to the nodelist files

static time_t runtime = 0;
static int    sh_mod = SH_DENYWR;
static bool   fidouser = false;
static Path   fidouserlst;
static bool   ignoredups = false;
static size_t dups = 0;
static bool   quiet = false;

#ifdef GOLDNODE_STATS
bool make_stats = false;
Path statfilename = "goldnode.stt";

struct nl_stat {
  int nodename[100];
  int location[100];
  int sysopname[100];
};

nl_stat statistic;
#endif

//  ------------------------------------------------------------------
//  Display a "twirly"

#define TWIRLY_FACTOR 511
#define ISTWIRLY(n) (not quiet and (((n)&TWIRLY_FACTOR)==0))

static void twirly() {

  static int n=0;

  n = (++n)%4;
  switch(n) {
    case 0: std::cout << "|\b" << std::flush; break;
    case 1: std::cout << "/\b" << std::flush; break;
    case 2: std::cout << "-\b" << std::flush; break;
    case 3: std::cout << "\\\b" << std::flush; break;
  }
}


//  ------------------------------------------------------------------

static bool match_addr_mask(Addr* mask, Addr* addr) {

  if(mask->zone == GFTN_ALL or mask->zone == addr->zone)
    if(mask->net == GFTN_ALL or mask->net == addr->net)
      if(mask->node == GFTN_ALL or mask->node == addr->node)
        if(mask->point == GFTN_ALL or mask->point == addr->point)
          return true;

  return false;
}


//  ------------------------------------------------------------------

static bool macro_addr(char* str, int ap, Addr* addr) {

  word part = 0xFFFD;

  if(*str == NUL)
    part = 0;
  else if(isdigit(*str))
    part = atow(str);
  else if(*str == '?' or *str == '*')
    part = GFTN_ALL;

  if(ap == _TEST) {
    if(part == 0xFFFD)
      return false;
    else
      return true;
  }

  switch(ap) {
    case _ZONE:
      addr->zone = part;
      break;
    case _NET:
      addr->net = part;
      break;
    case _NODE:
      addr->node = part;
      break;
    case _POINT:
      addr->point = part;
      break;
    default:
      return false;
  }
  return true;
}


//  ------------------------------------------------------------------

static char* fast_parse_addr(char* str, Addr* addr) {

  char* net;
  char* node;
  char* point;
  char* domain;
  char* space;

  space = strchr(str, ' ');
  if(space)
    *space = NUL;

  net = strchr(str, ':');
  node = strchr(str, '/');
  point = strchr(str, '.');
  domain = strchr(str, '@');
  if(domain and point)
    if((dword)point > (dword)domain)
      point = NULL;

  if(space)
    *space = ' ';

  if(net) {
    addr->zone = atow(str);
    addr->net = atow(net+1);
    if(node)
      addr->node = atow(node+1);
  }
  else {
    if(node) {
      if(*str != '/')
        addr->net = atow(str);
      addr->node = atow(node+1);
    }
    else {
      if(point != str)
        addr->node = atow(str);
    }
  }
  if(point)
    addr->point = atow(point+1);

  return domain;
}


//  ------------------------------------------------------------------

static char* parse_address(char* str, Addr* addr, Addr* mainaka) {

  char* domain = NULL;

  str = strskip_wht(str);

  if(isdigit(*str) or *str == '.' or macro_addr(str, _TEST, addr)) {

    char* net = strchr(str, ':');
    char* node = strchr(str, '/');
    char* point = strchr(str, '.');
    domain = strchr(str, '@');
    if(domain and point)
      if((dword)point > (dword)domain)
        point = NULL;

    if(net) {
      macro_addr(str, _ZONE, addr);
      macro_addr(net+1, _NET, addr);
      if(node)
        macro_addr(node+1, _NODE, addr);  // zone:net/node
      else
        addr->node = mainaka->node;                // zone:net
    }
    else {
      addr->zone = mainaka->zone;
      if(node) {
        if(*str != '/')
          macro_addr(str, _NET, addr);      // net/node
        else
          addr->net = mainaka->net;                  // /node
        macro_addr(node+1, _NODE, addr);
      }
      else {
        if(point == str)
          addr->node = mainaka->node;                // .point
        else
          macro_addr(str, _NODE, addr);     // node.point
        addr->net = mainaka->net;
      }
    }
    if(point)
      macro_addr(point+1, _POINT, addr);
  }
  if(domain == NULL)
    domain = str+strlen(str);   // point at NUL char

  return(domain);
}


//  ------------------------------------------------------------------

static char* make_addr_str(char* str, Addr* addr, char* domain) {

  char* ptr = str;
  static char buf[20];

  *ptr = NUL;

  if(addr->zone) {
    if(addr->zone == GFTN_ALL)
      ptr = stpcpy(ptr, "*");
    else {
      sprintf(buf, "%u", addr->zone);
      ptr = stpcpy(ptr, buf);
    }
    *ptr++ = ':';
  }

  if(addr->net == GFTN_ALL)
    ptr = stpcpy(ptr, "*");
  else {
    sprintf(buf, "%u", addr->net);
    ptr = stpcpy(ptr, buf);
  }
  *ptr++ = '/';

  if(addr->node == GFTN_ALL)
    ptr = stpcpy(ptr, "*");
  else {
    sprintf(buf, "%u", addr->node);
    ptr = stpcpy(ptr, buf);
  }

  if(addr->point) {
    *ptr++ = '.';
    if(addr->point == GFTN_ALL)
      ptr = stpcpy(ptr, "*");
    else {
      sprintf(buf, "%u", addr->point);
      ptr = stpcpy(ptr, buf);
    }
  }

  if(domain and *domain) {
    *ptr++ = '@';
    ptr = stpcpy(ptr, domain);
  }

  *ptr = NUL;

  return(str);
}


//  ------------------------------------------------------------------
//  Compare two nodes by name/address/file/pos

static bool cmp_nnlsts(_GEIdx A, _GEIdx B) {

  int cmp;

  if((cmp = stricmp(A.name, B.name)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.addr.zone, B.addr.zone)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.addr.net, B.addr.net)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.addr.node, B.addr.node)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.addr.point, B.addr.point)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.pos, B.pos)) != 0)
    return(cmp < 0);
  return false;
}


//  ------------------------------------------------------------------
//  Compare two nodes by address/name/file/pos

static bool cmp_anlsts(_GEIdx A, _GEIdx B) {

  int cmp;

  if((cmp = CmpV(A.addr.zone, B.addr.zone)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.addr.net, B.addr.net)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.addr.node, B.addr.node)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.addr.point, B.addr.point)) != 0)
    return(cmp < 0);
  if((cmp = stricmp(A.name, B.name)) != 0)
    return(cmp < 0);
  if((cmp = CmpV(A.pos, B.pos)) != 0)
    return(cmp < 0);
  return false;
}


//  ------------------------------------------------------------------

static char* CvtName(char* inp) {

  char buf[300];
  char* p;
  char* q;

  // Convert underlines to spaces

  p = inp;
  while(*p)
    if(*(p++) == '_')
      *(p-1) = ' ';

  // Strip leading spaces

  p = inp;
  while(isspace(*p))
    p++;
  q = &inp[strlen(p)-1];

  // Strip trailing spaces

  while(isspace(*q))
    *q-- = NUL;

  // Search for last space or point

  while(*q != ' ' and *q != '.' and q > p)
    q--;

  // If last char is a point, find last space instead

  if(*(q+1) == 0)
    while(*q != ' ' and q > p)
      q--;

  // Exchange last name and first name(s)

  if(p != q) {
    strcpy(stpcpy(buf, q+1), ", ");
    *(q+(*q == '.' ? 1 : 0)) = 0;
    strcat(buf, p);
    strcpy(inp, buf);
  }
  struplow(inp);
  return inp;
}


//  ------------------------------------------------------------------

#ifdef GOLDNODE_STATS
void calc_statistic(std::ofstream &ofp, int* observation, float N) {

  int i;
  float mean = 0.0;
  float sumfrekvens = 0.0;
  float varians = 0.0;

  //         12   12345   12345   123456   123456789012
  ofp << ".---------------------------------------------." << std::endl
      << "|   x |  h(x) |  f(x) | x*f(x) | (x-m)^2*f(x) |" << std::endl
      << "|-----+-------+-------+--------+--------------|" << std::endl;

  for(i=0; i<100; i++) {
    float x = i;
    if(observation[i]) {
      float hyppighed = observation[i];
      float frekvens = hyppighed / N;
      mean += x * frekvens;
      sumfrekvens += frekvens;
    }
  }

  for(i=0; i<100; i++) {
    float x = i;
    if(observation[i]) {
      float hyppighed = observation[i];
      float frekvens = hyppighed / N;
      float vartmp =  (x-mean)*(x-mean)*frekvens;
      varians += vartmp;
      ofp << "| " << std::setw(3) << i << " | " << std::setw(5) << observation[i] << " | " << std::setprecision(3) << std::setw(5) << frekvens << " | " << std::setw(6) << x*frekvens << " | " << std::setw(12) << vartmp << " | " << std::endl;
    }
  }

  ofp << "|-----+-------+-------+--------+--------------|" << std::endl
      << "| sum | " << std::setprecision(0) << std::setw(5) << N << " | " << std::setprecision(3) << std::setw(5) << sumfrekvens << " | " << std::setw(6) << mean << " | " << std::setw(12) << varians << " |" << std::endl
      << "`---------------------------------------------'" << std::endl
      << std::endl
      << "Mean: " << std::setprecision(1) << mean << std::endl
      << "Variance = " << varians << std::endl
      << "Standard deviation = " << sqrt(varians) << std::endl
      << std::endl;
}
#endif


//  ------------------------------------------------------------------
//  some useful string operations

inline void index_line(char* p, char* ptrs[5]) {

  for(int i=0; i<5; i++) {
    char* q = p;
    while(*q != ',' and *q) {
      if(*q == '_')
        *q = ' ';
      q++;
    }
    if(*q) {
      ptrs[i] = p;
      *q++ = NUL;
      p = q;
    }
  }
}


//  ------------------------------------------------------------------
//  Read the nodelists and userlists

static void read_nodelists() {

  long pos;
  FILE* lfp;
  char* ptr;
  _GEIdx nlst;
  Addr nlstz;
  char buf[512], buf2[100];
  int point;
  uint line, realfno;
  size_t no, nodes;
  const char* name;
  char* lp[5];
  geidxlist nodeidx;
  stamp_iter fno;
  addr_iter zno;

  nodes = 0;

  if(not quiet) std::cout << std::endl << "* Compiling nodelists:" << std::endl;

  // Delete the current indexfiles so they don't take up space
  remove(addrindex.c_str());
  remove(nodeindex.c_str());

  // Compile nodelists
  for(realfno=0, fno=nodelist.begin(), zno=nodezone.begin(); fno != nodelist.end(); fno++, zno++) {

    if(nodes < maxnodes) {

      lfp = fsopen(fno->fn, "rb", sh_mod);
      if(lfp) {

        setvbuf(lfp, NULL, _IOFBF, 32000);
        fno->ft = GetFiletime(fno->fn);

        // Initialize for each nodelist file
        no = 0;
        pos = 0;
        line = 0;
        point = YES;
        nlst.reset();
        nlstz = nlst.addr = *zno;
        name = CleanFilename(fno->fn);

        // Read all nodes
        while(fgets(buf, sizeof(buf), lfp)) {
          line++;

          // Break out if eof-marker is found
          if(*buf == '\x1A')
            break;

          // Note file position
          nlst.pos = pos;

          // Get line length and fix possible errors
          uint llen = strlen(buf);
          ptr = buf+llen-1;
          while(llen and not (*ptr == '\r' or *ptr == '\n' or *ptr == '\x1A')) {
            buf[llen] = ' ';
            if(not quiet) {
              int len = 16-strlen(name);
              std::cout << "\r* |--" << name << std::setw((len > 0) ? len : 1) << " " << "Warning line " << line << " - Invalid NUL char encountered." << std::endl;
            }
            llen = strlen(buf);
            ptr = buf+llen-1;
          }
          pos += llen;

          // Skip whitespace
          ptr = buf;
          while(isspace(*ptr))
            ptr++;

          if(*ptr != ';' and *ptr) {

            // First test for FD pvt extension
            if(toupper(*ptr) == 'B') {   // Boss
              nlst.addr.reset();
              parse_address(ptr+5, &nlst.addr, &nlstz);
              point = YES;
              continue;
            }

            // Test for Goldware extension
            if(isdigit(*ptr)) {
              nlst.addr.reset();
              parse_address(ptr+5, &nlst.addr, &nlstz);
              point = YES;
            }

            // Hold,32,TriCom,Hornbaek,Lars_Joergensen,45-12345678,2400,XX

            // Form the full node address
            index_line(ptr, lp);

            // NOTE: I use the fact that the third letter in lp[0] is unique
            //       for all valid attrs to speed up processing

            switch(*lp[0] ? toupper(lp[0][2]) : 0) {
              case 'N':   // zone
                nlst.addr.zone = nlst.addr.net = atow(lp[1]);
                nlst.addr.node = nlst.addr.point = 0;
                point = NO;
                break;

              case 'G':   // Region
                nlst.addr.net = atow(lp[1]);
                nlst.addr.node = nlst.addr.point = 0;
                point = NO;
                if(nlst.addr.net >= 10000)
                  continue;
                break;

              case 'S':   // Host
                {
                  nlst.addr.net = atow(lp[1]);
                  nlst.addr.node = nlst.addr.point = 0;
                  point = NO;
                  Addr a;
                  fast_parse_addr(lp[2],&a);
                  if(a.net) {                   // Is POINTS24 format ?
                    nlst.addr.net = a.net;
                    nlst.addr.node = a.node;
                    nlst.addr.point = 0;
                    point = YES;
                  }
                }
                break;

              case 'B':   // Hub
                nlst.addr.node = atow(lp[1]);
                nlst.addr.point = 0;
                point = NO;
                break;

              case 'I':   // point
                nlst.addr.point = atow(lp[1]);
                break;

              case 'T':   // Pvt
              case 'W':   // Down
              case 'L':   // Hold
              default:
                if(point)
                  nlst.addr.point = atow(lp[1]);
                else {
                  nlst.addr.node = atow(lp[1]);
                  nlst.addr.point = 0;
                }
                break;
            }

            if(ISTWIRLY(no)) {
              int len = 16-strlen(name);
              std::cout << "\r* \\--" << name << std::setw((len > 0) ? len : 1) << " " << "Zone " << nlst.addr.zone << "   \tNet " << nlst.addr.net << "   \tNodes " << (ulong)no << "        " << std::flush;
            }

            bool include = true;

            // Check address against the exclude masks
            for(addr_iter n=excludenode.begin(); n != excludenode.end(); n++) {
              if(match_addr_mask(&(*n), &nlst.addr)) {
                include = false;
                break;
              }
            }

            // Check address against the include masks
            if(not include) {
              for(addr_iter n=includenode.begin(); n != includenode.end(); n++) {
                if(match_addr_mask(&(*n), &nlst.addr)) {
                  include = true;
                  break;
                }
              }
            }

            if(include) {   // Address was okay

              // Convert name to Goldware standard
              strxcpy(nlst.name, CvtName(lp[4]), sizeof(nlst.name));

              // Prepare the rest
              nlst.pos |= ((((dword)realfno) << 24) & 0xFF000000L);

              // Append to end of list
              nodeidx.push_back(nlst);
              ++nodes;

              // Count the node
              no++;

              // Stop if limit is reached
              if(nodes >= maxnodes)
                break;
            }
          }
        }

        if(not quiet) {
          int len = 16-strlen(name);
          std::cout << "\r* " << ((fno == nodelist.end()-1) ? '\\' : '|') << "--" << name << std::setw((len > 0) ? len : 1) << " " << "Nodes read: " << (ulong)no << "\tTotal read: " << (ulong)nodes << ((nodes >= maxnodes) ? " (Limit reached)" : "                ") << std::endl;
        }

        fclose(lfp);
        ++realfno;
      }
      else {
        if(not quiet) std::cout << "Error opening nodelist " << fno->fn << '!' << std::endl;
        *(fno->fn) = NUL;
      }
    }
  }

  // Compile userlists
  if(userlist.size()) {
    if(not quiet) std::cout << std::endl << "* Compiling userlists:" << std::endl;
  }

  pos = 0;

  for(fno=userlist.begin(), zno=userzone.begin(); fno != userlist.end() and nodes < maxnodes; fno++, zno++) {

    no = 0;

    lfp = fsopen(fno->fn, "rt", sh_mod);
    if(lfp) {

      setvbuf(lfp, NULL, _IOFBF, 32000);

      name = CleanFilename(fno->fn);

      if(nodes < maxnodes) {

        while(fgets(buf, sizeof(buf), lfp)) {

          // Get node data
          strbtrim(buf);
          ptr = buf + strlen(buf) - 1;
          while(*ptr != ' ')
            ptr--;
          nlst.reset();
          nlst.addr = *zno;
          fast_parse_addr(ptr+1, &nlst.addr);
          *ptr = NUL;
          strbtrim(buf);

          // Convert "lastname, firstname" to "firstname lastname"
          ptr = strchr(buf, ',');
          if(ptr) {
            *ptr++ = NUL;
            strxmerge(buf2, 100, strskip_wht(ptr), " ", buf, NULL);
            ptr = buf2;
          }
          else {
            ptr = buf;
          }

          // Convert name to Goldware standard
          strxcpy(nlst.name, CvtName(ptr), sizeof(nlst.name));

          bool include = true;

          // Check address against the exclude masks
          for(addr_iter n=excludenode.begin(); n != excludenode.end(); n++) {
            if(match_addr_mask(&(*n), &nlst.addr)) {
              include = false;
              break;
            }
          }

          // Check address against the include masks
          if(not include) {
            for(addr_iter n=includenode.begin(); n != includenode.end(); n++) {
              if(match_addr_mask(&(*n), &nlst.addr)) {
                include = true;
                break;
              }
            }
          }

          if(include) {   // Address was okay

            if(ISTWIRLY(nodes)) {
              int len = 16-strlen(name);
              std::cout << "\r* \\--" << name << std::setw((len > 0) ? len : 1) << " " << "Nodes: " << (ulong)nodes << "        " << std::flush;
            }

            // Indicate userlist
            nlst.pos = (long)0xFF000000L | (pos++);

            // Append to end of list
            nodeidx.push_back(nlst);
            ++nodes;

            // Count the node
            no++;

            // Stop if limit is reached
            if(nodes >= maxnodes)
              break;
          }
        }
      }

      if(not quiet) {
        int len = 16-strlen(name);
        std::cout << "\r* " << ((fno == userlist.end()-1) ? '\\' : '|') << "--" << name << std::setw((len > 0) ? len : 1) << " " << "Nodes read: " << (ulong)no << "\tTotal read: " << (ulong)nodes << ((nodes >= maxnodes) ? " (Limit reached)" : "                ") << std::endl;
      }

      fclose(lfp);
    }
    else {
      if(not quiet) std::cout << "Error opening userlist " << fno->fn << '!' << std::endl;
    }
  }

  #ifdef GOLDNODE_STATS
  if(make_stats) {

    if(not quiet) std::cout << "* Writing statistics to " << statfilename << std::endl;

    std::ofstream ofp(statfilename);
    if(not ofp) {
      if(not quiet) std::cout << "Error opening statfile " << statfilename << '!' << std::endl;
    }
    else {
      ofp << "Nodename size statistics:" << std::endl;
      calc_statistic(ofp, statistic.nodename, nodes);

      ofp << std::endl << "Location size statistics:" << std::endl;
      calc_statistic(ofp, statistic.location, nodes);

      ofp << std::endl << "Sysopname size statistics:" << std::endl;
      calc_statistic(ofp, statistic.sysopname, nodes);
    }
  }
  else {
  #endif
    // At last, sort the nodes
    FILE *fp, *fido;
    geidxlist::iterator curr, prev;
    std::map<long, dword> namepos;

    // Sort by name
    if(not quiet) std::cout << std::endl << "* Sorting by name " << std::flush;
    nodeidx.sort(cmp_nnlsts);

    // Write the name-sorted .GXN
    fp = fsopen(nodeindex.c_str(), "wb", sh_mod);
    if(fp) {
      name = CleanFilename(nodeindex.c_str());
      fido = NULL;
      if(fidouser)
        fido = fsopen(fidouserlst, "wt", sh_mod);
      if(fido == NULL) {
        if(not quiet) std::cout << "\b, writing " << name << ' ' << std::flush;
        fidouser = false;
      }
      else {
        if(not quiet) std::cout << "\b, writing " << name << " and " << fidouserlst << ' ' << std::flush;
      }

      int nn = 0;
      dword nodenum = 0;
      for(prev = nodeidx.end(), curr = nodeidx.begin(); curr != nodeidx.end(); prev = curr++) {

        if(ISTWIRLY(nn++))
          twirly();

        if(ignoredups) {
          if(prev != nodeidx.end() && match_addr_mask(&curr->addr, &prev->addr)) {
            if(strieql(curr->name, prev->name)) {
              #ifdef DEBUG
              if(not quiet) std::cout << "* Dupe: " << curr->addr.zone << ':' << curr->addr.net << '/' << curr->addr.node << '.' << curr->addr.point << ' ' << curr->name << std::endl;
              #endif
              nodeidx.erase(curr);
              curr = prev;
              ++dups;
              continue;
            }
          }
          fwrite(&(*curr), sizeof(_GEIdx), 1, fp);
        }
        else
          fwrite(&(*curr), sizeof(_GEIdx), 1, fp);
        namepos[curr->pos] = nodenum++;
        if(fidouser) {
          char buf[256];
          fprintf(fido, "%-36.36s%24.24s\n", curr->name, make_addr_str(buf, &curr->addr, ""));
        }
      }
      if(fido)
        fclose(fido);
      fclose(fp);
    }

    // Sort by address
    if(not quiet) std::cout << ' ' << std::endl << "* Sorting by node " << std::flush;
    nodeidx.sort(cmp_anlsts);

    // Write the address-sorted .GXA
    fp = fsopen(addrindex.c_str(), "wb", sh_mod);
    if(fp) {
      name = CleanFilename(addrindex.c_str());
      if(not quiet) std::cout << "\b, writing " << name << ' ' << std::flush;
      int nn = 0;
      for(curr = nodeidx.begin(); curr != nodeidx.end(); curr++) {
        if(ISTWIRLY(nn++))
          twirly();
        fwrite(&namepos[curr->pos], sizeof(dword), 1, fp);
      }
      fclose(fp);
    }

    // Write the list index in .GXL
    fp = fsopen(listindex.c_str(), "wt", sh_mod);
    if(fp) {
      name = CleanFilename(listindex.c_str());
      if(not quiet) std::cout << ' ' << std::endl << "* Writing " << name << std::endl;
      for(fno=nodelist.begin(); fno != nodelist.end(); fno++) {
        if(*(fno->fn))
          fprintf(fp, "%s %lu\n", fno->fn, fno->ft);
      }
      fclose(fp);
    }

    // Note compile time
    runtime = time(NULL) - runtime;

    if(not quiet) {
      if(dups) {
        std::cout << std::endl << "* Total duplicate nodes: " << (ulong)dups << '.' << std::endl;
      }
      std::cout << std::endl << "* Nodelist compile completed. Compile time: " << (ulong)(runtime/60) << " min, " << (ulong)(runtime%60) << " sec." << std::endl;
    }
  #ifdef GOLDNODE_STATS
  }
  #endif
}


//  ------------------------------------------------------------------

static void check_nodelists(bool force) {

  FILE *fp;
  uint n;
  int compilen, compileu;
  static Path buf, newpath;

  // Find newest nodelists
  for(n=0,compilen=0; n<nodelist.size(); n++) {

    strcpy(buf, CleanFilename(nodelist[n].fn));
    char *ext = strchr(buf, '.');
    if(ext and ((atoi(ext+1) == 999) or (ext[1] == '*'))) {
      extractdirname(newpath, nodelist[n].fn);
      int extpos = ext-buf+1;
      strcpy(ext, ".*");
      gposixdir f(newpath);
      const gdirentry *de;
      time_t listtime = 0;
      bool listdefined = false;
      while((de = f.nextentry(buf)) != NULL)
        if(atoi(de->name.c_str()+extpos)) {
          if(not listdefined or (de->stat_info.st_mtime-listtime > 0)) {
            listtime = de->stat_info.st_mtime;
            listdefined = true;
            strxmerge(nodelist[n].fn, sizeof(Path), f.fullpath(), GOLD_SLASH_STR, de->name.c_str(), NULL);
          }
        }
      strchg(nodelist[n].fn, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
    }
  }

  // Get timestamps from .GXL file
  fp = fsopen(listindex.c_str(), "rt", sh_mod);
  if(fp) {
    while(fgets(buf, sizeof(buf), fp)) {
      char* key;
      char* val=buf;
      getkeyval(&key, &val);
      key = strxcpy(newpath, strbtrim(key), sizeof(Path));
      MapPath(key);
      strchg(key, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
      for(n=0; n<nodelist.size(); n++) {
        if(strieql(nodelist[n].fn, key)) {
          nodelist[n].ft = atol(val);
          break;
        }
      }
      for(n=0; n<userlist.size(); n++) {
        if(strieql(userlist[n].fn, key)) {
          userlist[n].ft = atol(val);
          break;
        }
      }
    }
    fclose(fp);
  } else
    perror("error opening .gxl file");

  // Check nodelists
  for(n=0,compilen=0; n<nodelist.size(); n++) {
    if(abs(long(GetFiletime(nodelist[n].fn) - nodelist[n].ft)) > 1) {
      nodelist[n].fc = YES;
      compilen++;
    }
  }

  if(not quiet) {
    if(compilen) {
      std::cout << "* " << compilen << " new nodelist file" << ((compilen == 1) ? "" : "s") << " found." << std::endl;
    }
    else if(nodelist.size()) {
      std::cout << "* The nodelist file" << ((nodelist.size() == 1) ? " is" : "s are") << " up-to-date." << std::endl;
    }
  }

  // Check userlists
  for(n=0,compileu=0; n<userlist.size(); n++) {
    if(abs(long(GetFiletime(newpath) - userlist[n].ft)) > 1) {
      userlist[n].fc = YES;
      compileu++;
    }
  }

  if(not quiet) {
    if(compileu) {
      std::cout << "* " << compileu << " new userlist file" << ((compileu == 1) ? "" : "s") << " found." << std::endl;
    }
    else if(userlist.size()) {
      std::cout << "* The userlist file" << ((userlist.size() == 1) ? " is" : "s are") << " up-to-date." << std::endl;
    }
  }

  if(force or compilen or compileu)
    read_nodelists();
}


//  ------------------------------------------------------------------

static void fatal_error(const char* what) {

  if(not quiet) std::cout << what << std::endl;
  exit(5);
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

char* MapPath(char* fmap, bool reverse) {

  Path buf, cmap;

  strxcpy(cmap, fmap, sizeof(Path));
  if(reverse)
    strchg(cmap, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);

  std::vector< std::pair<std::string, std::string> >::iterator i;
  for(i = mappath.begin(); i != mappath.end(); i++) {
    const char* p = reverse ? i->second.c_str() : i->first.c_str();
    const char* q = reverse ? i->first.c_str() : i->second.c_str();
    if(strnieql(cmap, p, strlen(p))) {
      strxcpy(buf, fmap, sizeof(Path));
      strxmerge(fmap, sizeof(Path), q, buf+strlen(p), NULL);
      char sl1, sl2;
      char* ptr;

      ptr = strpbrk(p, "/\\");
      sl1 = ptr ? *ptr : NUL;
      ptr = strpbrk(q, "/\\");
      sl2 = ptr ? *ptr : NUL;

      if(sl1 and sl2 and (sl1 != sl2))
        strchg(fmap, sl1, sl2);

      break;
    }
  }
  return fmap;
}


//  ------------------------------------------------------------------

static int parse_config(const char *__configfile, Addr& zoneaddr) {

  FILE* fp;
  char buf[512];
  char* ptr;
  char* key;
  word crc;
  char* value;
  char* value2;
  int _gotcond,line = 0;
  static int in_if = NO;
  static int in_else = NO;
  static int cond_status = YES;

  fp = fsopen(__configfile, "rt", sh_mod);
  if(fp) {
    while(fgets((ptr=buf), sizeof(buf), fp)) {
      line++;
      // Replace TABs with SPACEs
      ptr = strskip_wht(ptr);
      if(*ptr != ';' and *ptr) {
        crc=getkeyvalcrc(&key, &ptr);
        getkeyval(&value, &ptr);
        getkeyval(&value2, &ptr);

        _gotcond = YES;
        switch(crc) {
          case CRC_IF:
            if(in_if) {
              if(not quiet) std::cout << "* " << __configfile << ": Misplaced IF at line " << line << ". IF's cannot be nested." << std::endl;
            }
            in_if = YES;
            cond_status = do_if(value);
            break;
          case CRC_ELIF:
          case CRC_ELSEIF:
            if((not in_if) or in_else) {
              if(not quiet) std::cout << "* " << __configfile << ": Misplaced ELIF/ELSEIF at line " << line <<  '.' << std::endl;
            }
            cond_status = do_if(value);
            break;
          case CRC_ELSE:
            if((not in_if) or in_else) {
              if(not quiet) std::cout << "* " << __configfile << "Misplaced ELSE at line " << line <<  '.' << std::endl;
            }
            in_else = YES;
            cond_status ^= YES;
            break;
          case CRC_ENDIF:
            if(not in_if) {
              if(not quiet) std::cout << "* " << __configfile << ": Misplaced ENDIF at line " << line << '.' << std::endl;
            }
            in_if = in_else = NO;
            cond_status = YES;
            break;
          default:
            _gotcond = NO;
            break;
        }

        if((not _gotcond) and cond_status) {
          switch(crc) {
            case CRC_NODEPATH:
              MapPath(value);
              PathCopy(nodepath, value);
              break;
            case CRC_ADDRESS:
            case CRC_AKA:
              if(not zoneaddr.net) {
                parse_address(value, &zoneaddr, &zoneaddr);
                zoneaddr.point = 0;
              }
              break;
            case CRC_NODELIST:
              {
                Stamp ndl;
                Addr ndz;

                if(atoi(value2)) {
                  parse_address(value2, &ndz, &ndz);
                  if(ndz.zone == 0) {
                    ndz.zone  = ndz.node;
                    ndz.net   = 0;
                    ndz.node  = 0;
                  }
                }
                else
                  ndz = zoneaddr;
                ndz.point = 0;
                ndl.ft = (dword)-1;
                ndl.fc = NO;
                strschg_environ(value);
                MapPath(value);
                strcpy(ndl.fn, value);
                nodelist.push_back(ndl);
                nodezone.push_back(ndz);
              }
              break;
            case CRC_USERLIST:
              {
                Stamp ndl;
                Addr ndz;

                if(atoi(value2)) {
                  parse_address(value2, &ndz, &ndz);
                  if(ndz.zone == 0) {
                    ndz.zone  = ndz.node;
                    ndz.net   = 0;
                    ndz.node  = 0;
                  }
                }
                else
                  ndz = zoneaddr;
                ndz.point = 0;
                ndl.ft = (dword)-1;
                ndl.fc = NO;
                strschg_environ(value);
                MapPath(value);
                strcpy(ndl.fn, value);
                userlist.push_back(ndl);
                userzone.push_back(ndz);
              }
              break;
            case CRC_EXCLUDENODES:
              {
                Addr exn;
                parse_address(value, &exn, &zoneaddr);
                excludenode.push_back(exn);
              }
              break;
            case CRC_INCLUDENODES:
              {
                Addr inn;
                parse_address(value, &inn, &zoneaddr);
                includenode.push_back(inn);
              }
              break;
            case CRC_SHAREMODE:
              if(atoi(value))
                sh_mod = atoi(value);
              else
                sh_mod = GetYesno(value) ? SH_DENYNO : SH_COMPAT;
              break;
            case CRC_INCLUDE:
              strschg_environ(value);
              MapPath(value);
              if(not parse_config(value,zoneaddr))     // NOTE! This is a recursive call!
                if(not quiet) std::cout << "* Could not read configuration file " << value << '!' << std::endl;
              break;
            case CRC_MAPPATH:
              {
                std::pair<std::string, std::string> mapentry;

                mapentry.first = value;
                mapentry.second = value2;
                mappath.push_back(mapentry);
              }
              break;
            default:
              break;
          }
        }
      }
    }

    fclose(fp);
    return(YES);
  }
  else {
    return(NO);
  }
}


//  ------------------------------------------------------------------

static bool ExistCfg(char* path, char* file) {

  bool found = fexist(AddPath(path, file));
  if(found)
    strcat(path, file);
  return found;
}


//  ------------------------------------------------------------------

static bool FindCfg(char* path) {

  bool found = false;
  
  if(!is_dir(path)) {
    if(fexist(path))
      return true;
    else
      return false;
  }
  AddBackslash(path);
  #if defined(__OS2__)
  found = ExistCfg(path, "ged2.cfg");
  #elif defined(__WIN32__)
  found = ExistCfg(path, "gedw32.cfg");
  #endif
  if(not found)
    found = ExistCfg(path, "golded.cfg");
  return found;
}


//  ------------------------------------------------------------------

static bool read_config(const char *cfg, const char *argv_0) {

  Addr zoneaddr;
  Path buf;

  bool found = (*cfg != NUL) ? true : false;
  if(not found) {
    // Look for configfilename in the environment
    const char *ptr = getenv("GOLDNODE");
    #if defined(__OS2__)
    if(not(ptr and *ptr))
      ptr = getenv("GED2");
    #elif defined(__WIN32__)
    if(not(ptr and *ptr))
      ptr = getenv("GEDW32");
    #endif
    if(not(ptr and *ptr))
      ptr = getenv("GOLDED");
    if(not(ptr and *ptr))
      ptr = getenv("GED");
    if(ptr and *ptr) {
      strxcpy(buf, ptr, sizeof(buf));
      found = FindCfg(buf);
    }

    // Get it in current directory
    if(not found) {
      getcwd(buf, sizeof(buf));
      found = FindCfg(buf);
    }

    // Get it where the the .EXE file is
    if(not found) {
      extractdirname(buf, argv_0);
      found = FindCfg(buf);

      // If we still could not find config name...
      if(not found)
        strcat(buf, "golded.cfg");
    }
  }
  else
    strxcpy(buf, cfg, sizeof(Path));

  nodelist.clear();
  nodezone.clear();
  userlist.clear();
  userzone.clear();
  mappath.clear();
  if(not parse_config(buf, zoneaddr)) {
    errorlevel = 1;
    return false;
  }

  if(nodelist.empty() and userlist.empty() == 0)
    fatal_error("* Error: No NODELISTs or USERLISTs defined!");

  if(zoneaddr.net == 0)
    fatal_error("* Error: No ADDRESS or AKAs defined!");

  if(nodepath.empty())
    nodepath = getcwd(buf, sizeof(buf));

  AddBackslash(nodepath);
  MakePathname(listindex, nodepath, "goldnode.gxl");
  MakePathname(nodeindex, nodepath, "goldnode.gxn");
  MakePathname(addrindex, nodepath, "goldnode.gxa");
  size_t n;
  for(n=0; n<nodelist.size(); n++)
    MakePathname(nodelist[n].fn, nodepath.c_str(), nodelist[n].fn);
  for(n=0; n<userlist.size(); n++)
    MakePathname(userlist[n].fn, nodepath.c_str(), userlist[n].fn);

  return true;
}


//  ------------------------------------------------------------------

static void run_gn(int argc, char* argv[]) {

  int n;
  char* ptr;
  bool force=false, conditional=false;

  // Note start time
  runtime = time(NULL);

  const char *cfg = "";

  for(n=1; n<argc; n++) {
    if(strchr("-", *argv[n])) {
      ptr = argv[n]+2;
      if(*ptr == '=')
        ptr++;
      switch(toupper(argv[n][1])) {

        case 'C':
          conditional = true;
          break;

        case 'D':
          ignoredups = true;
          break;

        case 'F':
          force = true;
          break;

        case 'Q':
          quiet = true;
          break;

        #ifdef GOLDNODE_STATS
        case 'T':
          make_stats = true;
          break;
        #endif

        case 'U':
          fidouser = true;
          strcpy(fidouserlst, ptr);
          break;

      }
    }
    else
      cfg = argv[n];
  }

  if(not quiet) {
    std::cout << __GPID__ " " __GVER__ " Nodelist Compiler." << std::endl
         << "Copyright (C) 1990-1999 Odinn Sorensen" << std::endl
         << "Copyright (C) 1999-2001 Alexander S. Aganichev" << std::endl
         << "-------------------------------------------------------------------------------" << std::endl
         << std::endl;
  }

  if(not(force or conditional)) {
    if(not quiet) {
      std::cout << "Commandline syntax: " << CleanFilename(argv[0]) << " [-options] [configfile]" << std::endl
           << std::endl
           << "[-options] =\t-C\t  Conditional compile." << std::endl
           << "\t\t-F\t  Forced compile." << std::endl
           << "\t\t-D\t  Remove duplicate nodes from index while compiling." << std::endl
           << "\t\t-Q\t  Quiet compile. No screen output improves speed." << std::endl
           << "\t\t-S<size>  Set max size of a name in the index." << std::endl
           << "\t\t-U<file>  Create sorted FIDOUSER.LST userlist file." << std::endl
      #ifdef GOLDNODE_STATS
           << "\t\t-T\t  Make statistics." << std::endl
      #endif
           << std::endl
           << "[configfile] =\t\t  The path AND filename of GOLDED.CFG" << std::endl
           << "\t\t\t  configuration file to read." << std::endl
           << std::endl;
    }
  }
  else {

    if(force)
      if(not quiet) std::cout << "* Forced compile." << std::endl;

    if(read_config(cfg, argv[0])) {
      if(force or conditional)
        check_nodelists(force);
    }
    else {
      if(not quiet) std::cout << std::endl << "Could not find the configuration file!" << std::endl;
    }
  }
}


//  ------------------------------------------------------------------

int main(int argc, char *argv[]) {

  throw_init();

  // set locale
  setlocale(LC_CTYPE, "");
  #if defined(GUTLOS_FUNCS)
  g_init_os(0);
  #endif

  #ifdef GOLDNODE_STATS
  memset(&statistic, 0, sizeof(nl_stat));
  #endif

  run_gn(argc, argv);

  #if defined(GUTLOS_FUNCS)
  g_deinit_os();
  #endif

  THROW_CHECK();

  return errorlevel;
}


//  ------------------------------------------------------------------
