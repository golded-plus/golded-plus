//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999-2002 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Read areas from Fidoconfig
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gcrcall.h>
#include <gstrall.h>
#include <gmemdbg.h>
#undef GCFG_NOFIDOCONF
#include <gedacfg.h>


//  ------------------------------------------------------------------

static bool comment_char = '#';

//  ------------------------------------------------------------------

bool gareafile::ReadHPTLine(FILE* f, std::string* s, bool add, int state) {

  std::string str;
  char buf[81];

  if(fgets(buf, 81, f) == NULL)
    return false; // eof

  str = buf;

  if(buf[strlen(buf)-1] != '\n') {
    while(fgets(buf, 81, f) != NULL) {
      str += buf;
      if(buf[strlen(buf)-1] == '\n')
        break;
    }
  }

  std::string::iterator ptr = str.begin();

  // state 0: normal state
  //       1: between ""
  //       2: end
  //       3: whitespace
  while((ptr != str.end()) and (state != 2)) {
    if(*ptr == comment_char) {
      if(state != 1) {
        str.erase(ptr, str.end());
        state = 2;
      }
    }
    else {
      switch(*ptr) {
        case '\"':
          state = (state == 1) ? 0 : 1;
          break;
        default:
          break;
      }
    }
    ++ptr;
  }

  while(not str.empty() and ((*(str.end()-1) == '\n') or (*(str.end()-1) == '\r')))
    str.erase(str.end()-1);

  const char *p = strskip_wht(str.c_str());

  if(add)
    *s += p;
  else
    *s = p;

  return true;  
}


//  ------------------------------------------------------------------

void gareafile::gettok(char** key, char** val) {

  char* p = strskip_wht(*val);

  // Get the keyword
  switch(*p) {

    // Strip "double" quotes
    case '\"':
      *key = ++p;
      do {
        p = strskip_to(p, '\"');
        if(not *p or (*(p-1) != '\\'))
          break;
        else
          p++;
      } while(*p);
      break;

    // Strip 'single' quotes
    case '\'':
      *key = ++p;
      do {
        p = strskip_to(p, '\'');
        if(not *p or (*(p-1) != '\\'))
          break;
        else
          p++;
      } while(*p);
      break;

    // Strip round brackets
    case '(': /*)*/
      *key = ++p;
      p = strskip_to(p, /*(*/ ')');
      break;

    // Get straight keyword
    default:
      *key = p;
      while(*p and not isspace(*p))
        p++;
  }

  // Nul-terminate keyword
  if(*p)
    *p++ = NUL;

  // Skip whitespace if any
  p = strskip_wht(p);

  // Get the value
  *val = p;
}


//  ------------------------------------------------------------------

void gareafile::ReadHPTFile(char* path, char* file, char* origin, int group) {

  const word CRC_ADDRESS = 0xFDD6;
  const word CRC_INCLUDE = 0x379B;
  const word CRC_NETAREA = 0x8F1C;
  const word CRC_NETMAILAREA = 0xD2D7;
  const word CRC_LOCALAREA = 0xAEC1;
  const word CRC_ECHOAREA = 0x0D63;
  const word CRC_DUPEAREA = 0xD8B9;
  const word CRC_BADAREA = 0x8DA5;
  const word CRC_SYSOP = 0x967F;
  const word CRC_VERSION = 0xF78F;
  const word CRC_COMMENTCHAR = 0xE2CC;
  const word CRC_ECHOAREADEFAULTS = 0x2CBB;

  AreaCfg aa;
  Path buf2;

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      std::cout << "* Reading " << file << std::endl;

    aa.reset();
    aa.type = GMB_NONE;
    aa.msgbase = fidomsgtype;
    aa.groupid = group;

    std::string s;
    while(ReadHPTLine(fp, &s)) {

      if(not s.empty()) {

        char *alptr = throw_xstrdup(s.c_str());
        char *ptr = alptr;

        char* key;
        char* val = ptr;
        gettok(&key, &val);
        switch(strCrc16(key)) {
          case CRC_VERSION:
            {
              int ver_maj, ver_min;
              sscanf(val, "%d.%d", &ver_maj, &ver_min);
              if((ver_maj != 0) and (ver_min != 15)) {
                std::cout << "* Error: Unknown fidoconfig version " << ver_maj << '.' << ver_min << " - Skipping." << std::endl;
                throw_xfree(alptr);
                goto skip_config;
              }
            }
            break;
          case CRC_ADDRESS:
            CfgAddress(val);
            break;
          case CRC_SYSOP:
            CfgUsername(val);
            break;
          case CRC_INCLUDE:
            strxcpy(buf2, val, sizeof(buf2));
            if(strchg(buf2, '[', '%') != 0)
              strchg(buf2, ']', '%');
            MakePathname(buf2, path, buf2);
            ReadHPTFile(path, buf2, origin, group);
            break;
          case CRC_COMMENTCHAR:
            if((strlen(val) == 3) and (val[0] == val[2]) and strpbrk(val, "\'\""))
              comment_char = val[1];
            else if(*val)
              comment_char = val[0];
            break;
          case CRC_NETAREA:
          case CRC_NETMAILAREA:
            aa.type = GMB_NET;
            break;
          case CRC_LOCALAREA:
          case CRC_DUPEAREA:
          case CRC_BADAREA:
            aa = echoareadefaults;
            aa.type = GMB_LOCAL;
            break;
          case CRC_ECHOAREA:
            aa = echoareadefaults;
            aa.type = GMB_ECHO;
            break;
          case CRC_ECHOAREADEFAULTS:
            echoareadefaults.reset();
            aa.type = GMB_DEFAULT;
            aa.msgbase = fidomsgtype;
            aa.groupid = group;
            break;
        }

        if(aa.type != GMB_NONE) {

          if(aa.type == GMB_DEFAULT) {

             *key = NUL;
          }
          else {

            // Get echoid
            gettok(&key, &val);
            aa.setechoid(key);

            // Get path
            gettok(&key, &val);
          }

          // If not pass-through
          if(not strieql("Passthrough", key)) {

            if(strchg(key, '[', '%') != 0)
              strchg(key, ']', '%');
            aa.setpath(key);

            gettok(&key, &val);

            while((*key == '-') or strieql(key, "Squish") or strieql(key, "Jam") or strieql(key, "MSG")) {

              if(strieql(key, "Squish"))
                aa.msgbase = GMB_SQUISH;
              else if(strieql(key, "Jam"))
                aa.msgbase = GMB_JAM;
              else if(strieql(key, "MSG"))
                aa.msgbase = fidomsgtype;
              else {

                char *opt = key + 1;
    
                if(strieql(opt, "p") or strieql(opt, "$m")
                   or strieql(opt, "lr") or strieql(opt, "lw")
                   or strieql(opt, "dupeCheck") or strieql(opt, "dupehistory")
                   or strieql(opt, "r") or strieql(opt, "w")
                   or strieql(opt, "l") or strieql(opt, "fperm")
                   or strieql(opt, "fowner") or strnieql(opt, "sbadd(", 6)
                   or strnieql(opt, "sbign(", 6)) {
    
                  gettok(&key, &val);
                }
                else if(strieql(opt, "a")) {
    
                  gettok(&key, &val);
                  CfgAddress(key);
                  aa.aka.set(key);
                }
                else if(strieql(opt, "h") or strieql(opt, "manual")
                        or strieql(opt, "nopause") or strieql(opt, "mandatory")
                        or strieql(opt, "dosfile") or strieql(opt, "ccoff")
                        or strieql(opt, "b") or strieql(opt, "tinysb")
                        or strieql(opt, "killsb") or strieql(opt, "keepunread")
                        or strieql(opt, "killread") or strieql(opt, "h")
                        or strieql(opt, "nolink") or strieql(opt, "debug")
                        or strieql(opt, "nopack") or strieql(opt, "keepsb")
                        or strieql(opt, "$") or strieql(opt, "0")) {
                }
                else if(strieql(opt, "g")) {
    
                  gettok(&key, &val);
    
                  if(isdigit(*key))
                    aa.groupid = 0x8000+atoi(key);
                  else if(isalpha(*key))
                    aa.groupid = toupper(*key);
                }
                else if (strieql(opt, "d")) {
    
                  gettok(&key, &val);
                  aa.setdesc(key);
                }
              }

              gettok(&key, &val);
            }
                                
            aa.setorigin(origin);

            switch(aa.type) {
              case GMB_NET:
                aa.attr = attribsnet;
                AddNewArea(aa);
                break;
              case GMB_ECHO:
                aa.attr = attribsecho;
                AddNewArea(aa);
                break;
              case GMB_LOCAL:
                aa.attr = attribslocal;
                AddNewArea(aa);
                break;
              case GMB_DEFAULT:
                echoareadefaults = aa;
                break;
              default:
                break;
            }
          }

          aa.reset();
        }
        throw_xfree(alptr);
      }
    }

skip_config:
    fclose(fp);
  }
}


//  ------------------------------------------------------------------
//  Read areas from HPT (echomail processor)

void gareafile::ReadHPT(char* tag) {

  char origin[80];
  char options[80];
  word defaultgroup = 0;
  Path file, path;

  *origin = NUL;
  *file = NUL;
  strcpy(options, tag);
  char* ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-')
      strcpy(file, ptr);
    else {
      if(toupper(*(++ptr)) == 'G') {
        if(*(++ptr) == '=')
          ptr++;
        if(*ptr == '#')
          defaultgroup = (word)(atoi(ptr+1)+0x8000u);
        else
          defaultgroup = (word)(isupper(*ptr) ? *ptr : 0);
      }
    }
    ptr = strtok(NULL, " \t");
  }

  if(not *file and ((ptr = getenv("FIDOCONFIG"))!=NULL))
    strcpy(file, ptr);

  extractdirname(path, file);

  CfgSquishuserpath(path);

  CfgJAMSMAPIHighwater(true);

  echoareadefaults.type = GMB_NONE;
  echoareadefaults.msgbase = fidomsgtype;
  echoareadefaults.groupid = defaultgroup;

  ReadHPTFile(path, file, origin, defaultgroup);
}


//  ------------------------------------------------------------------
