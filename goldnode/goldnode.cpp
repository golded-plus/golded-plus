
//  ------------------------------------------------------------------
//  The Goldware Utilities.
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
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  GoldNODE - A nodelist compiler for GoldED.
//  ------------------------------------------------------------------


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

//#define GOLDNODE_STATS 1

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
#elif defined(__FreeBSD__) or defined(__OpenBSD__)
#define __GPID__ "GoldNODE+/BSD"
#else
#define __GPID__ "GoldNODE+"
#endif

#define __GVER__ "1.1.4.1"         // Visible version


//  ------------------------------------------------------------------
// 32-bit versions

const size_t maxnodes = 262000;


//  ------------------------------------------------------------------

typedef vector<Addr>::iterator addr_iter;
typedef vector<Stamp>::iterator stamp_iter;
typedef list<_GEIdx> geidxlist;

// Nodelists
vector<Stamp> nodelist; // nodelist files,stamps,update marker
vector<Addr> nodezone;  // nodelist zones
vector<Stamp> userlist; // Userlist files,stamps,update marker
vector<Addr> userzone;  // Userlist zones

// Exclude/Include nodes
vector<Addr> excludenode;
vector<Addr> includenode;

// Index files
string  addrindex;
string  nodeindex;
string  listindex;

//  ------------------------------------------------------------------

const word _POINT = 0x0001;
const word _NODE  = 0x0002;
const word _NET   = 0x0003;
const word _ZONE  = 0x0004;
const word _TEST  = 777;

static string nodepath;                // Path to the nodelist files

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

#define fast_printf    if(not quiet) printf
#define fast_putchar   if(not quiet) putchar

//  ------------------------------------------------------------------
//  Display a "twirly"

#define TWIRLY_FACTOR 511
#define ISTWIRLY(n) (((n)&TWIRLY_FACTOR)==0)

static void twirly() {

  static int n=0;

  n = (++n)%4;
  switch(n) {
    case 0:   fast_putchar('|');   break;
    case 1:   fast_putchar('/');   break;
    case 2:   fast_putchar('-');   break;
    case 3:   fast_putchar('\\');  break;
  }

  fast_putchar('\b');
  fflush(stdout);
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
void calc_statistic(FILE* ofp, int* observation, float N) {

  int i;
  float mean = 0.0;
  float sumfrekvens = 0.0;
  float varians = 0.0;

  //              12   12345   12345   123456   123456789012
  fprintf(ofp, ".---------------------------------------------.\n");
  fprintf(ofp, "|   x |  h(x) |  f(x) | x*f(x) | (x-m)^2*f(x) |\n");
  fprintf(ofp, "|-----+-------+-------+--------+--------------|\n");

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
      fprintf(ofp, "| %3i | %5i | %5.3f | %6.3f | %12.3f | \n", i, observation[i], frekvens, x*frekvens, vartmp);
    }
  }

  fprintf(ofp, "|-----+-------+-------+--------+--------------|\n");
  fprintf(ofp, "| sum | %5.0f | %5.3f | %5.3f | %12.3f |\n", N, sumfrekvens, mean, varians);
  fprintf(ofp, "`---------------------------------------------'\n");
  fprintf(ofp, "\n");
  fprintf(ofp, "Mean: %.1f\n", mean);
  fprintf(ofp, "Variance = %.1f\n", varians);
  fprintf(ofp, "Standard deviation = %.1f\n", sqrt(varians));
  fprintf(ofp, "\n");
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

  fast_printf("\n* Compiling nodelists:\n");

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
            fast_printf("\r* |-%-12s     Warning line %u - Invalid NUL char encountered.\n", name, line);
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
              fast_printf("\r* \\--%-12s     Zone %-5u  Net %-5u  Nodes %6lu", name, nlst.addr.zone, nlst.addr.net, (ulong)no);
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

        fast_printf("\r* %c--%-12s   Nodes read: %6lu    Total read: %6lu", fno==nodelist.end()-1?'\\':'|', name, (ulong)no, (ulong)nodes);

        if(nodes >= maxnodes) {
          fast_printf("  (Limit reached)\n");
        }
        else {
          fast_printf("         \n");
        }

        fclose(lfp);
        ++realfno;
      }
      else {
        fast_printf("Error Opening nodelist %s!\n", fno->fn);
        *(fno->fn) = NUL;
      }
    }
  }

  // Compile userlists
  if(userlist.size()) {
    fast_printf("\n* Compiling userlists:\n");
  }

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

            if(ISTWIRLY(nodes))
              fast_printf("\r* \\--%s: %6lu", name, (ulong)nodes);

            // Indicate userlist
            nlst.pos = (long)0xFFFFFFFFL;

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

      fast_printf("\r* %c--%-12s     Nodes read: %6lu    Total read: %6lu", fno==userlist.end()-1?'\\':'|', name, (ulong)no, (ulong)nodes);

      if(nodes >= maxnodes) {
        fast_printf("  (Limit reached)\n");
      }
      else {
        fast_printf("         \n");
      }

      fclose(lfp);
    }
    else {
      fast_printf("Error Opening Userlist %s!\n", fno->fn);
    }
  }

  #ifdef GOLDNODE_STATS
  if(make_stats) {

    fast_printf("* Writing statistics to %s\n", statfilename);

    FILE *ofp = fopen(statfilename, "wt");
    if(ofp) {

      fprintf(ofp, "Nodename size statistics:\n");
      calc_statistic(ofp, statistic.nodename, nodes);

      fprintf(ofp, "\nLocation size statistics:\n");
      calc_statistic(ofp, statistic.location, nodes);

      fprintf(ofp, "\nSysopname size statistics:\n");
      calc_statistic(ofp, statistic.sysopname, nodes);

      fclose(ofp);
    }
  }
  else {
  #endif
    // At last, sort the nodes
    FILE *fp, *fido;
    geidxlist::iterator curr, prev;
    map<long, dword> namepos;

    // Sort by name
    fast_printf("\n* Sorting by name ");
    nodeidx.sort(cmp_nnlsts);

    // Write the name-sorted .GXN
    fp = fsopen(nodeindex.c_str(), "wb", sh_mod);
    if(fp) {
      name = CleanFilename(nodeindex.c_str());
      fido = NULL;
      if(fidouser)
        fido = fsopen(fidouserlst, "wt", sh_mod);
      if(fido == NULL) {
        fast_printf("\b, writing %s ", name);
        fidouser = false;
      }
      else {
        fast_printf("\b, writing %s and %s ", name, fidouserlst);
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
              fast_printf("* Dupe: %d:%d/%d.%d %s\n",curr->addr.zone,curr->addr.net,curr->addr.node,curr->addr.point,curr->name);
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
    fast_printf(" \n* Sorting by node ");
    nodeidx.sort(cmp_anlsts);

    // Write the address-sorted .GXA
    fp = fsopen(addrindex.c_str(), "wb", sh_mod);
    if(fp) {
      name = CleanFilename(addrindex.c_str());
      fast_printf("\b, writing %s ", name);
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
      fast_printf(" \n* Writing %s\n", name);
      for(fno=nodelist.begin(); fno != nodelist.end(); fno++) {
        if(*(fno->fn))
          fprintf(fp, "%s %lu\n", fno->fn, fno->ft);
      }
      fclose(fp);
    }

    // Note compile time
    runtime = time(NULL) - runtime;

    if(dups) {
      fast_printf("\n* Total duplicate nodes: %6lu.\n", (ulong)dups);
    }
    fast_printf("\n* Nodelist compile completed. Compile time: %lu min, %lu sec.\n", (ulong)(runtime/60), (ulong)(runtime%60));
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
            strxmerge(nodelist[n].fn, sizeof(Path), f.fullpath(), "/", de->name.c_str(), NULL);
          }
        }
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

  if(compilen) {
    fast_printf("* %u new nodelist file%s found.\n", compilen, compilen==1?"":"s");
  }
  else if(nodelist.size()) {
    fast_printf("* The nodelist file%s up-to-date.\n", nodelist.size()==1?" is":"s are");
  }

  // Check userlists
  for(n=0,compileu=0; n<userlist.size(); n++) {
    if(abs(long(GetFiletime(userlist[n].fn) - userlist[n].ft)) > 1) {
      userlist[n].fc = YES;
      compileu++;
    }
  }

  if(compileu) {
    fast_printf("* %u new userlist file%s found.\n", compileu, compileu==1?"":"s");
  }
  else if(userlist.size()) {
    fast_printf("* The userlist file%s up-to-date.\n", userlist.size()==1?" is":"s are");
  }

  if(force or compilen or compileu)
    read_nodelists();
}


//  ------------------------------------------------------------------

static void fatal_error(const char* what) {

  fast_printf(what);
  fast_printf("\n");
  exit(5);
}


//  ------------------------------------------------------------------

static int do_if(char* val) {

  if(strieql(val, "OS/2") OR strieql(val, "OS2")) {
    #ifdef __OS2__
    return true;
    #else
    return false;
    #endif
  }
  else if(strieql(val, "NT") OR strieql(val, "W32") OR strieql(val, "WIN32")) {
    #ifdef __WIN32__
    return true;
    #else
    return false;
    #endif
  }
  else if(strieql(val, "386")) {
    #if defined(__MSDOS__)
    return true;
    #else
    return false;
    #endif
  }
  else if(strieql(val, "DOS")) {
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
  else if(strieql(val, "INOS2")) {
    #if defined(__OS2__)
    return true;
    #elif defined(__GNUC__)
    return false;
    #else
    return _osmajor >= 10;
    #endif
  }
  else if(strieql(val, "FIREBIRD")) {
    return true;
  }
  else if(strieql(val, "YES") OR strieql(val, "TRUE") OR strieql(val, "ON"))
    return true;
  return atoi(val) != 0;
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
              fast_printf("* %s: Misplaced IF at line %u. IF's cannot be nested.\n", __configfile, line);
            }
            in_if = YES;
            cond_status = do_if(value);
            break;
          case CRC_ELIF:
          case CRC_ELSEIF:
            if((not in_if) or in_else) {
              fast_printf("* %s: Misplaced ELIF/ELSEIF at line %u.\n", __configfile, line);
            }
            cond_status = do_if(value);
            break;
          case CRC_ELSE:
            if((not in_if) or in_else) {
              fast_printf("* %s: Misplaced ELSE at line %u.\n", __configfile, line);
            }
            in_else = YES;
            cond_status ^= YES;
            break;
          case CRC_ENDIF:
            if(not in_if) {
              fast_printf("* %s: Misplaced ENDIF at line %u.\n", __configfile, line);
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
              nodepath = value;
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
              else if(striinc("NO", value))
                sh_mod = 0;
              break;
            case CRC_INCLUDE:
              if(not parse_config(value,zoneaddr))     // NOTE! This is a recursive call!
                fast_printf("* Could not read configuration file '%s' !\n",value);
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

  fast_printf(
    __GPID__ " " __GVER__ " Nodelist Compiler.\n"
    "Copyright (C) 1990-1999 Odinn Sorensen\n"
    "Copyright (C) 1999-2000 Alexander S. Aganichev\n"
    "-------------------------------------------------------------------------------\n"
    "\n"
  );

  if(not(force or conditional)) {
    fast_printf(
      "Commandline syntax: %s [-options] [configfile]\n"
      "\n"
      "[-options] =   -C        Conditional compile.\n"
      "               -F        Forced compile.\n"
      "               -D        Remove duplicate nodes from index while compiling.\n"
      "               -Q        Quiet compile. No screen output improves speed.\n"
      "               -S<size>  Set max size of a name in the index.\n"
      "               -U<file>  Create sorted FIDOUSER.LST userlist file.\n"
      #ifdef GOLDNODE_STATS
      "               -T        Make statistics.\n"
      #endif
      ""
      "[configfile] =           The path AND filename of GOLDED.CFG\n"
      "                         configuration file to read.\n"
      "\n",
      CleanFilename(argv[0])
    );
  }
  else {

    if(force)
      fast_printf("* Forced compile.\n");

    if(read_config(cfg, argv[0])) {
      if(force or conditional)
        check_nodelists(force);
    }
    else {
      fast_printf("\nCould not find the configuration file!\n");
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
