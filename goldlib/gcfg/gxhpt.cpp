//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1999-2000 Alexander S. Aganichev
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

static bool lame = false;
static bool comment_char = '#';

//  ------------------------------------------------------------------

bool gareafile::ReadHPTLine(FILE* f, string* s, bool add, int state) {

  string str;
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

  string::iterator ptr = str.begin();

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
        case '\\':
          if(lame)
            break;
          if(ptr == str.end()-1) {
            str.erase(ptr);
            const char *p = strskip_wht(str.c_str());
            if(add)
              *s += p;
            else
              *s = p;
            ReadHPTLine(f, s, true, state);
            return true;
          } else
            if(ptr[1] == comment_char) {
              str.erase(ptr, str.end());
              const char *p = strskip_wht(str.c_str());
              if(add)
                *s += p;
              else
                *s = p;
              ReadHPTLine(f, s, true, state);
              return true;
            }
            switch(ptr[1]) {
              case ' ':
              case '\t':
                {
                  string::iterator i = ptr;
                  while((i != str.end()) and isspace(*i))
                    ++i;
                  if(*i != '#')
                    break;
                }
              case NUL:
              case '\n':
                str.erase(ptr, str.end());
                {
                  const char *p = strskip_wht(str.c_str());
                  if(add)
                    *s += p;
                  else
                    *s = p;
                }
                ReadHPTLine(f, s, true, state);
                return true;
              default:
                break;
            }
          ++ptr;
          break;
        case '\"':
          state = 1;
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

void gareafile::replace_slashes(char **key) {

  if(lame)
    return;

  char* p = *key;

  while(*p) {
    if(*p == '\\') {

      int size = 1;
      uint chr = p[1];

      if(isxdigit(p[1])) {
        if(isxdigit(p[2])) {
          size = 2;
          sscanf(p, "\\%02x", &chr);
        }
        else
          sscanf(p, "\\%1x", &chr);
      }
      strcpy(p, p+size);
      *p = chr;
      if(chr == NUL)
        break;
    }
    ++p;
  }
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

  replace_slashes(key);
}


//  ------------------------------------------------------------------

void gareafile::ReadHPTFile(char* path, char* file, char* options, char* origin, int group) {

  const word CRC_ADDRESS = 0xFDD6;
  const word CRC_INCLUDE = 0x379B;
  const word CRC_NETAREA = 0xD2D7;
  const word CRC_LOCALAREA = 0xAEC1;
  const word CRC_ECHOAREA = 0x0D63;
  const word CRC_DUPEAREA = 0xD8B9;
  const word CRC_BADAREA = 0x8DA5;
  const word CRC_SYSOP = 0x967F;
  const word CRC_VERSION = 0xF78F;
  const word CRC_COMMENTCHAR = 0xE2CC;

  AreaCfg aa;
  Path buf2;

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {
    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    aa.reset();

    string s;
    while(ReadHPTLine(fp, &s)) {

      if(not s.empty()) {

        char *alptr = throw_xstrdup(s.c_str());
        char *ptr = alptr;

        aa.type = 0xFF;

        char* key;
        char* val = ptr;
        gettok(&key, &val);
        switch(strCrc16(key)) {
          case CRC_VERSION:
            replace_slashes(&val);
            {
              int ver_maj, ver_min;
              sscanf(val, "%d.%d", &ver_maj, &ver_min);
              if((ver_maj != 0) and (ver_min != 15)) {
                cout << "* Error: Unknown fidoconfig version " << ver_maj << '.' << ver_min << " - Skipping." << endl;
                throw_xfree(alptr);
                goto skip_config;
              }
            }
            break;
          case CRC_ADDRESS:
            replace_slashes(&val);
            CfgAddress(val);
            break;
          case CRC_SYSOP:
            replace_slashes(&val);
            CfgUsername(val);
            break;
          case CRC_INCLUDE:
            replace_slashes(&val);
            strxcpy(buf2, val, sizeof(buf2));
            MakePathname(buf2, path, buf2);
            ReadHPTFile(path, buf2, options, origin, group);
            break;
          case CRC_COMMENTCHAR:
            replace_slashes(&val);
            if((strlen(val) == 3) and (val[0] == val[2]) and strpbrk(val, "\'\""))
              comment_char = val[1];
            else if(*val)
              comment_char = val[0];
            break;
          case CRC_NETAREA:
            aa.type = GMB_NET;
            break;
          case CRC_LOCALAREA:
          case CRC_DUPEAREA:
          case CRC_BADAREA:
            aa.type = GMB_LOCAL;
            break;
          case CRC_ECHOAREA:
            aa.type = GMB_ECHO;
            break;
        }

        if(aa.type != 0xFF) {

          // Get echoid
          gettok(&key, &val);
          aa.setechoid(key);

          // Get path
          gettok(&key, &val);
          strxcpy(buf2, key, sizeof(buf2));

          // If not pass-through
          if(not strieql("Passthrough", buf2)) {

            aa.setpath(MapPath(buf2));
            aa.msgbase = fidomsgtype;
            aa.groupid = group;

            gettok(&key, &val);

            while(*key == '-' || strieql(key, "Squish") || strieql(key, "Jam")) {

              if(strieql(key, "Squish"))
                aa.msgbase = GMB_SQUISH;
              else if(strieql(key, "Jam"))
                aa.msgbase = GMB_JAM;
              else {

                char *opt = key + 1;
    
                if(strieql(opt, "p") or strieql(opt, "$m") or strieql(opt, "lr")
                    or strieql(opt, "lw") or strieql(opt, "tinysb")
                    or strieql(opt, "dupeCheck") or strieql(opt, "dupehistory")
                    or strieql(opt, "r") or strieql(opt, "w")
                    or strieql(opt, "l")) {
    
                  gettok(&key, &val);
                }
                else if(strieql(opt, "a")) {
    
                  gettok(&key, &val);
                  CfgAddress(key);
                  aa.aka.set(key);
                }
                else if(strieql(opt, "h") or strieql(opt, "manual")
                        or strieql(opt, "nopause") or strieql(opt, "mandatory")
                        or strieql(opt, "dosfile") or strieql(opt, "ccoff")) {
                }
                else if (strieql(opt, "g")) {
    
                  gettok(&key, &val);
    
                  if(isalpha(*key))
                    aa.groupid = toupper(*key);
                }
                else if (strieql(opt, "d")) {
    
                  gettok(&key, &val);
                  aa.setdesc(key);
                }
              }

              gettok(&key, &val);
            }
                                
            switch(aa.type) {
              case GMB_NET:
                aa.attr = attribsnet;
                break;
              case GMB_ECHO:
                aa.attr = attribsecho;
                break;
              case GMB_LOCAL:
                aa.attr = attribslocal;
                break;
            }
            aa.setorigin(origin);
            AddNewArea(aa);
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

  lame = false;
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
      else if(strieql(ptr, "lame"))
        lame = true;
    }
    ptr = strtok(NULL, " \t");
  }

  if(not *file and ((ptr = getenv("FIDOCONFIG"))!=NULL))
    strcpy(file, ptr);

  extractdirname(path, file);

  if(*squishuserpath == NUL)
    PathCopy(squishuserpath, path);

  ReadHPTFile(path, file, options, origin, defaultgroup);
}


//  ------------------------------------------------------------------
