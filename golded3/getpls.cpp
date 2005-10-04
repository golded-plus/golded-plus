
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
//  Template handling.
//  ------------------------------------------------------------------

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <malloc.h>
#endif

#if defined(_MSC_VER)
    /* C4786: 'identifier' : identifier was truncated to 'number'
          characters in the debug information
    */
  #pragma warning(disable: 4786)
#endif
#include <algorithm>
#include <golded.h>


//  ------------------------------------------------------------------

extern GMsg* reader_msg;
extern bool reader_gen_confirm;

const int REALLOC_CACHE_SIZE = 4096;

//  ------------------------------------------------------------------

bool is_user(const char* name) {

  if(strieql(name, AA->Username().name))
    return true;
  // We should check all misspells too
  for(std::vector<Node>:: iterator u = CFG->username.begin(); u != CFG->username.end(); u++)
    if(strieql(name, u->name))
      return true;
  return false;
}


//  ------------------------------------------------------------------

inline int IsInitial(char c) {

  return isalpha(c) or (c > 127);
}


//  ------------------------------------------------------------------

int TemplateToText(int mode, GMsg* msg, GMsg* oldmsg, const char* tpl, int origarea) {

  FILE* fp;
  long fpos;
  Path tplfile;
  int n;
  int x;
  FILE *tfp;
  FILE *ifp;
  char* tptr;
  char* ptr;
  char* ptr2;
  const char* quote;
  uint size;
  uint pos;
  uint ctrlinfo;
  char textfile[GMAXPATH];
  char indexfile[GMAXPATH];
#if defined(__MINGW32__) || defined(_MSC_VER)
  size_t sizeofbuf = CFG->quotemargin + 256;
  char *buf = (char*)alloca(sizeofbuf);
#else
  __extension__ char buf[CFG->quotemargin + 256];
#endif
  char initials[10];
  char quotestr[100];
  char qbuf[100];
  uint len;
  int y;
  int tmptpl = NO;
  int robotchk = NO;
  int disphdr = NO;
  int quotebufline;

  enum TPL_TOKEN_IDS {
    TPLTOKEN_FORWARD,
    TPLTOKEN_NOTFORWARD,
    TPLTOKEN_CHANGED,
    TPLTOKEN_NET,
    TPLTOKEN_ECHO,
    TPLTOKEN_LOCAL,
    TPLTOKEN_MOVED,
    TPLTOKEN_NEW,
    TPLTOKEN_REPLY,
    TPLTOKEN_QUOTED,
    TPLTOKEN_COMMENT,
    TPLTOKEN_QUOTEBUF,
    TPLTOKEN_ATTRIB,
    TPLTOKEN_SPELLCHECKER,
    TPLTOKEN_SETSUBJ,
    TPLTOKEN_SETFROM,
    TPLTOKEN_SETTO,
    TPLTOKEN_FORCESUBJ,
    TPLTOKEN_FORCEFROM,
    TPLTOKEN_FORCETO,
    TPLTOKEN_XLATEXPORT,
    TPLTOKEN_LOADLANGUAGE,
    TPLTOKEN_RANDOM,
    TPLTOKEN_QUOTE,
    TPLTOKEN_INCLUDE,
    TPLTOKEN_MESSAGE,
    TPLTOKEN_MODERATOR,
    TPLTOKEN_WRITE,
    TPLTOKEN_HEADER
  };

  #define CSTR_COMMA_SIZEOF_CSTR(s) s, (sizeof(s)-1)

  struct tpl_token {
    char* token;
    int length;
  };

  static const tpl_token token_list[] = {
    { CSTR_COMMA_SIZEOF_CSTR("forward")      },
    { CSTR_COMMA_SIZEOF_CSTR("notforward")   },
    { CSTR_COMMA_SIZEOF_CSTR("changed")      },
    { CSTR_COMMA_SIZEOF_CSTR("net")          },
    { CSTR_COMMA_SIZEOF_CSTR("echo")         },
    { CSTR_COMMA_SIZEOF_CSTR("local")        },
    { CSTR_COMMA_SIZEOF_CSTR("moved")        },
    { CSTR_COMMA_SIZEOF_CSTR("new")          },
    { CSTR_COMMA_SIZEOF_CSTR("reply")        },
    { CSTR_COMMA_SIZEOF_CSTR("quoted")       },
    { CSTR_COMMA_SIZEOF_CSTR("comment")      },
    { CSTR_COMMA_SIZEOF_CSTR("quotebuf")     },
    { CSTR_COMMA_SIZEOF_CSTR("attrib")       },
    { CSTR_COMMA_SIZEOF_CSTR("spellchecker") },
    { CSTR_COMMA_SIZEOF_CSTR("setsubj")      },
    { CSTR_COMMA_SIZEOF_CSTR("setfrom")      },
    { CSTR_COMMA_SIZEOF_CSTR("setto")        },
    { CSTR_COMMA_SIZEOF_CSTR("forcesubj")    },
    { CSTR_COMMA_SIZEOF_CSTR("forcefrom")    },
    { CSTR_COMMA_SIZEOF_CSTR("forceto")      },
    { CSTR_COMMA_SIZEOF_CSTR("xlatexport")   },
    { CSTR_COMMA_SIZEOF_CSTR("loadlanguage") },
    { CSTR_COMMA_SIZEOF_CSTR("random")       },
    { CSTR_COMMA_SIZEOF_CSTR("quote")        },
    { CSTR_COMMA_SIZEOF_CSTR("include")      },
    { CSTR_COMMA_SIZEOF_CSTR("message")      },
    { CSTR_COMMA_SIZEOF_CSTR("moderator")    },
    { CSTR_COMMA_SIZEOF_CSTR("write")        },
    { CSTR_COMMA_SIZEOF_CSTR("header")       }
  };

  int end_token = sizeof(token_list) / sizeof(tpl_token);

  *initials = NUL;

  // Check for msg to AreaFix
  if(AA->isnet() and mode == MODE_NEW) {
    for(gstrarray::iterator r = CFG->robotname.begin(); r != CFG->robotname.end(); r++)
      if(striinc(r->c_str(), msg->to)) {
        robotchk = YES;
        break;
      }
    if(robotchk) {
      EDIT->HardLines(false);
      msg->txt = (char*)throw_realloc(msg->txt, 256);
      strcpy(msg->txt, LNG->RobotMsg);
      TokenXlat(mode, msg->txt, msg, oldmsg, origarea);
      return 0;
    }
  }

  strcpy(tplfile, tpl);

  if((mode != MODE_WRITEHEADER) and (mode != MODE_WRITE) and (mode != MODE_HEADER)) {
    if(AA->Templatematch() and not (CFG->tplno or AA->isnewsgroup() or AA->isemail())) {
      if(not ((mode == MODE_NEW or mode == MODE_REPLYCOMMENT or mode == MODE_FORWARD)
              and (AA->isecho() or AA->islocal()))) {
        std::vector<Tpl>::iterator tp;
        for(tp = CFG->tpl.begin(); tp != CFG->tpl.end(); tp++)
          if(tp->match.net and msg->dest.match(tp->match)) {
            strcpy(tplfile, tp->file);
            break;
          }
      }
    }
  }

  if(not strieql(tplfile, "built-in")) {
    if(tplfile == CleanFilename(tplfile))
      strcpy(tplfile, AddPath(CFG->templatepath, tplfile));

    if((mode != MODE_WRITEHEADER) and (mode != MODE_WRITE) and (mode != MODE_HEADER)) {
      if(not fexist(tplfile) and not CFG->tpl.empty())
        strcpy(tplfile, AddPath(CFG->templatepath, CFG->tpl[CFG->tplno].file));
    }
  }
  if(strieql(tplfile, "built-in") or not fexist(tplfile) or CFG->tpl.empty()) {
    tmptpl = YES;   // Create a temporary template
    mktemp(strcpy(tplfile, AddPath(CFG->temppath, "GDXXXXXX")));
    fp = fsopen(tplfile, "wt", CFG->sharemode);
    if(fp) {
      fputs("@header= @oecho (@caddr) @align{79}{=}\n", fp);
      fputs("@header Msg  : @msgno of @msgs@align{44}@attr\n", fp);
      if(AA->isinternet()) {
        fputs("@header From : @ofrom@align{60}@odtime\n", fp);
        fputs("@header To   : @oto\n", fp);
      }
      else {
        fputs("@header From : @oname@align{44}@oaddr@align{60}@odtime\n", fp);
        fputs("@header To   : @dname\n", fp);
      }
      fputs("@header Subj : @subject\n", fp);
      fputs("@header@align{79}{=}\n", fp);
      fputs("@moved* Replying to a msg in @oecho (@odesc)\n@moved\n", fp);
      fputs("@changed* Changed by @cname (@caddr), @cdate @ctime.\n@changed\n", fp);
      fputs("@forward* Forwarded from @oecho by @fname (@faddr).\n", fp);
      fputs("@forward* Originally by: @oname (@oaddr), @odate @otime.\n", fp);
      fputs("@forward* Originally to: @dname{}{}{all}.\n", fp);
      fputs("@forward\n", fp);
      fputs("@message\n", fp);
      fputs("@forward\n", fp);
      fputs("Hello @pseudo{}{}{everybody}.\n", fp);
      fputs("@new\n", fp);
      fputs("@position\n", fp);
      fputs("@replyReplying to a msg dated @odate @otime, from @oname{me}{you} to @dname{me}{you}{all}.\n", fp);
      fputs("@reply@position\n", fp);
      fputs("@quoted@odate @otime, @oname{I}{you} wrote to @dname{me}{you}{all}:\n", fp);
      fputs("@quoted@position\n", fp);
      fputs("@comment@odate @otime, @oname{I}{you} wrote to @dname{me}{you}{all}:\n", fp);
      fputs("@comment@position\n", fp);
      fputs("@quotebuf\n", fp);
      fputs("@quotebuf@odate @otime, @oname{I}{you} wrote to @dname{me}{you}{all}:\n", fp);
      fputs("@quotebuf\n", fp);
      fputs("@quote\n\n", fp);
      fputs("@cfname\n\n", fp);
      fclose(fp);
    }
  }

  fp = fsopen(tplfile, "rt", CFG->sharemode);
  if(fp == NULL) {
    LOG.ErrOpen();
    LOG.printf("! A template file could not be opened.");
    LOG.printf(": %s", tplfile);
    LOG.ErrOSInfo();
    OpenErrorExit();
  }

  oldmsg->you_and_I = msg->you_and_I = 0;

  if (strieql(msg->By(), oldmsg->By()) or is_user (oldmsg->By()))
    msg->you_and_I |= BY_ME;
  if (strieql(msg->By(), oldmsg->to) or is_user (oldmsg->to))
    msg->you_and_I |= TO_ME;

  if (not msg->by_me() and strieql(msg->to, oldmsg->By()))
    msg->you_and_I |= BY_YOU;
  if (not msg->to_me() and strieql(msg->to, oldmsg->to))
    msg->you_and_I |= TO_YOU;

  if(not msg->to_me() and strieql(AA->Whoto(), oldmsg->to))
    oldmsg->you_and_I |= TO_ALL;

  if(strieql(AA->Whoto(), msg->to))
    msg->you_and_I |= TO_ALL;

  // build @tpseudo
  if(is_user(msg->to))
    strcpy(msg->pseudoto, *AA->Nickname() ? AA->Nickname() : strlword(msg->to, " @."));
  else
    *(msg->pseudoto) = NUL;

  // build @fpseudo
  if(is_user(msg->By()))
    strcpy(msg->pseudofrom, *AA->Nickname() ? AA->Nickname() : strlword(msg->By(), " @."));
  else
    *(msg->pseudofrom) = NUL;

  // build @dpseudo
  if(msg->to_me())
    strcpy(oldmsg->pseudoto, msg->pseudofrom);
  else if(msg->to_you())
    strcpy(oldmsg->pseudoto, msg->pseudoto);
  else
    *(oldmsg->pseudoto) = NUL;

  // build @opseudo
  if(msg->by_me())
    strcpy(oldmsg->pseudofrom, msg->pseudofrom);
  else if(msg->by_you())
    strcpy(oldmsg->pseudofrom, msg->pseudoto);
  else
    *(oldmsg->pseudofrom) = NUL;

  throw_release(msg->txt);

  *buf = NUL;
  msg->txt = throw_strdup(buf);
  len = size = pos = 0;

  size_t oldmsg_size = oldmsg->txt ? strlen(oldmsg->txt) : REALLOC_CACHE_SIZE;
  size_t msg_txt_realloc_cache = 0;

#if defined(__MINGW32__) || defined(_MSC_VER)
  while(fgets(buf, sizeofbuf, fp)) {
#else
  while(fgets(buf, sizeof(buf), fp)) {
#endif
    ptr = strskip_wht(buf);
    if(*ptr != ';') {
      bool chg = false;
      quotebufline = NO;
      ptr = buf;
      int token = end_token;
      do {
        do {
          // Find next '@' or NUL
          while(*ptr and (*ptr != '@'))
            ptr++;
          // Skip past double '@'s
          if(*ptr and (ptr[1] == '@'))
            ptr += 2;
        } while(*ptr and (*ptr != '@'));
        if(*ptr) {
          ptr++;
          for(token=0; token<end_token; token++) {
            if(strnieql(token_list[token].token, ptr, token_list[token].length)) {
              break;
            }
          }
        }
        else {
          break;
        }

        if(token < end_token) {

          // Remove the token
          ptr--;
          ptr2 = ptr + token_list[token].length + 1;
          memmove(ptr, ptr2, strlen(ptr2)+1);

          switch(token) {

            case TPLTOKEN_FORWARD:
              if(mode != MODE_FORWARD)
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_NOTFORWARD:
              if(mode == MODE_FORWARD)
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_CHANGED:
              if(mode != MODE_CHANGE)
                goto loop_next;
              chg = true;
              token = end_token;
              break;

            case TPLTOKEN_HEADER:
              if((mode != MODE_HEADER) and (mode != MODE_WRITEHEADER))
                goto loop_next;
              chg = true;
              token = end_token;
              break;

            case TPLTOKEN_WRITE:
              if((mode != MODE_WRITE) and (mode != MODE_WRITEHEADER))
                goto loop_next;
              chg = true;
              token = end_token;
              break;

            case TPLTOKEN_NET:
              if(not AA->isnet())
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_ECHO:
              if(not AA->isecho())
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_LOCAL:
              if(not AA->islocal())
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_MOVED:
              if(CurrArea == origarea)
                goto loop_next;
              if(mode == MODE_FORWARD)
                goto loop_next;
              if(AL.AreaIdToPtr(origarea)->Areareplydirect() and oldmsg->areakludgeid and strieql(oldmsg->areakludgeid, AA->echoid()))
                goto loop_next;
              if(AL.AreaIdToPtr(origarea)->Areareplydirect() and strieql(AL.AreaIdToPtr(origarea)->Areareplyto(), AA->echoid()))
                goto loop_next;
              if(strieql(oldmsg->fwdarea, AA->echoid()))
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_NEW:
              if(mode != MODE_NEW)
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_REPLY:
              if(mode != MODE_REPLY)
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_QUOTED:
              if(mode != MODE_QUOTE)
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_COMMENT:
              if(mode != MODE_REPLYCOMMENT)
                goto loop_next;
              token = end_token;
              break;

            case TPLTOKEN_QUOTEBUF:
              if(mode != MODE_QUOTEBUF)
                goto loop_next;
              quotebufline = YES;
              token = end_token;
              break;

            case TPLTOKEN_MODERATOR:
              if(not striinc("moderator", msg->By()))
                goto loop_next;
              token = end_token;
              break;
          }
        }

      } while(*ptr and (token>=end_token));

      if(token < end_token) {
        TokenXlat(mode, buf, msg, oldmsg, origarea);

        switch(token) {

          case TPLTOKEN_ATTRIB:
            if(mode != MODE_QUOTEBUF) {
              GetAttribstr(&msg->attr, ptr);
              disphdr = YES;
            }
            continue;

          case TPLTOKEN_SPELLCHECKER:
            if(mode != MODE_QUOTEBUF)
              EDIT->SpellChecker(strskip_wht(ptr));
            continue;

          case TPLTOKEN_SETSUBJ:
          case TPLTOKEN_FORCESUBJ:
            if(mode != MODE_QUOTEBUF) {
              if(strblank(msg->re) or (token == TPLTOKEN_FORCESUBJ)) {
                strbtrim(ptr);
                StripQuotes(ptr);
                strcpy(msg->re, ptr);
                disphdr = YES;
              }
            }
            continue;

          case TPLTOKEN_SETFROM:
          case TPLTOKEN_FORCEFROM:
            if(mode != MODE_QUOTEBUF) {
              if(strblank(msg->by) or (token == TPLTOKEN_FORCEFROM)) {
                strbtrim(ptr);
                StripQuotes(ptr);
                strcpy(msg->by, ptr);
                disphdr = YES;
              }
            }
            continue;

          case TPLTOKEN_SETTO:
          case TPLTOKEN_FORCETO:
            if(mode != MODE_QUOTEBUF) {
              if(strblank(msg->to) or (token == TPLTOKEN_FORCETO)) {
                strbtrim(ptr);
                StripQuotes(ptr);
                strcpy(msg->to, ptr);
                disphdr = YES;
              }
            }
            continue;

          case TPLTOKEN_XLATEXPORT:
            if(mode != MODE_QUOTEBUF)
              AA->SetXlatexport(strbtrim(ptr));
            continue;

          case TPLTOKEN_LOADLANGUAGE:
            strbtrim(ptr);
            LoadLanguage(ptr);  // Load a GoldED language file
            disphdr = YES;
            continue;

          case TPLTOKEN_RANDOM:
            if((mode != MODE_QUOTEBUF) and
               (not chg and ((mode != MODE_CHANGE) and
                             (mode != MODE_WRITEHEADER) and
                             (mode != MODE_WRITE) and
                             (mode != MODE_HEADER)))) {
              indexfile[0] = '\0';
              strtrim(ptr);
              ptr = strskip_wht(strskip_txt(ptr));
              if(*ptr) {
                tptr = ptr;
                ptr = strskip_txt(ptr);
                if(*ptr) {
                  *ptr++ = NUL;
                  strcpy(textfile, tptr);
                  ptr = strskip_wht(ptr);
                  if(*ptr) {
                    tptr = ptr;
                    ptr = strskip_txt(ptr);
                    *ptr = NUL;
                    strcpy(indexfile, tptr);
                  }
                }
                else {
                  strcpy(textfile, tptr);
                }
              }
              else {
                strcpy(textfile, "random.txt");
              }

              if(indexfile[0] == '\0') {
                replaceextension(indexfile, textfile, ".mdx");
              }

              MakePathname(textfile, CFG->cookiepath, textfile);
              MakePathname(indexfile, CFG->cookiepath, indexfile);

              // Check if index exists or if it is older than the textfile
              int idxexist = fexist(indexfile);
              if(idxexist)
                if(FiletimeCmp(textfile, indexfile) > 0)
                  idxexist = false;

              // If index file is missing, make one
              if(not idxexist)
                CookieIndex(textfile, indexfile);

              // Get a random cookie
              tfp = fsopen(textfile, "rt", CFG->sharemode);
              if(tfp) {
                ifp = fsopen(indexfile, "rb", CFG->sharemode);
                if(ifp) {
                  fseek(ifp, 0L, SEEK_END);
                  int idxs = (int)(ftell(ifp)/sizeof(long));
                  if(idxs) {
                    fseek(ifp, (long)(rand()%idxs)*sizeof(long), SEEK_SET);
                    fread(&fpos, sizeof(long), 1, ifp);
                    fseek(tfp, fpos, SEEK_SET);
                    while(fgets(buf, 255, tfp)) {
                      strtrim(buf);
                      if(*buf) {
                        if(*buf == '+' and buf[1] == NUL)
                          *buf = ' ';
                        TokenXlat(mode, buf, msg, oldmsg, origarea);
                        strtrim(buf);
                        strcat(buf, "\r");
                        len = strlen(buf);
                        size += len;
                        if(msg_txt_realloc_cache >= len) {
                          msg_txt_realloc_cache -= len;
                        }
                        else {
                          msg_txt_realloc_cache += REALLOC_CACHE_SIZE;
                          msg->txt = (char*)throw_realloc(msg->txt, size+10+msg_txt_realloc_cache);
                        }
                        strcpy(&(msg->txt[pos]), buf);
                        pos += len;
                      }
                      else
                        break;
                    }
                  }
                  fclose(ifp);
                }
                fclose(tfp);
              }
            }
            continue;

          case TPLTOKEN_INCLUDE:
            if(mode != MODE_QUOTEBUF) {
              strbtrim(ptr);
              strcpy(textfile, ptr);
              MakePathname(textfile, CFG->templatepath, textfile);
              tfp = fsopen(textfile, "rt", CFG->sharemode);
              if(tfp) {
                while(fgets(buf, 255, tfp)) {
                  TokenXlat(mode, buf, msg, oldmsg, origarea);
                  strtrim(buf);
                  strcat(buf, "\r");
                  len = strlen(buf);
                  size += len;
                  if(msg_txt_realloc_cache >= len) {
                    msg_txt_realloc_cache -= len;
                  }
                  else {
                    msg_txt_realloc_cache += REALLOC_CACHE_SIZE;
                    msg->txt = (char*)throw_realloc(msg->txt, size+10+msg_txt_realloc_cache);
                  }
                  strcpy(&(msg->txt[pos]), buf);
                  pos += len;
                }
                fclose(tfp);
              }
            }
            continue;

          case TPLTOKEN_QUOTE:
            if((mode == MODE_QUOTE) or (mode == MODE_REPLYCOMMENT) or
               (mode == MODE_QUOTEBUF)) {
              y = 0;
              ptr = strskip_wht(oldmsg->By());
              while(*ptr) {
                while(not IsInitial(*ptr) and (*ptr != '@') and *ptr)
                  ptr++;
                if(*ptr == '@')
                  break;
                if(*ptr) {
                  initials[y++] = *ptr;
                  if(y == 9)
                    break;
                  ptr++;
                }
                while(IsInitial(*ptr) and *ptr)
                  ptr++;
              }

              initials[y] = NUL;
              *buf = NUL;
              if(y > 2) {
                for(x=1; x<(y-1); x++)
                  buf[x-1] = initials[x];
                buf[x-1] = NUL;
              }
              for(n=0,x=0; n<strlen(AA->Quotestring()); n++) {
                switch(AA->Quotestring()[n]) {
                  case 'F':
                  case 'f':
                    if(*initials) {
                      quotestr[x++] = *initials;
                      quotestr[x] = NUL;
                    }
                    break;
                  case 'M':
                  case 'm':
                    strcat(quotestr, buf);
                    x += strlen(buf);
                    break;
                  case 'L':
                  case 'l':
                    if(y > 1) {
                      quotestr[x++] = initials[y-1];
                      quotestr[x] = NUL;
                    }
                    break;
                  default:
                    quotestr[x++] = AA->Quotestring()[n];
                    quotestr[x] = NUL;
                }
              }
              ptr = strskip_wht(quotestr);
              y = (int)((long)ptr-(long)quotestr);
              n = 0;
              *buf = NUL;
              while(oldmsg->line[n]) {
                strtrim(oldmsg->line[n]->txt);
                if(oldmsg->line[n]->type & GLINE_TEAR) {
                  // Invalidate tearline
                  oldmsg->line[n]->type &= ~GLINE_TEAR;
                  if(not (AA->Quotectrl() & CI_TEAR)) {
                    n++;
                    continue;
                  }
                }
                else if(oldmsg->line[n]->type & GLINE_ORIG) {
                  // Invalidate originline
                  oldmsg->line[n]->type &= ~GLINE_ORIG;
                  if(not (AA->Quotectrl() & CI_ORIG)) {
                    n++;
                    continue;
                  }
                }

                // Invalidate kludge chars
                std::string& tempref = strtrim(oldmsg->line[n]->txt);
                std::replace(tempref.begin(), tempref.end(), CTRL_A, '@');
                quote = tempref.c_str();

                if(is_quote(oldmsg->line[n]->txt.c_str())) {
                  quote += GetQuotestr(quote, qbuf, &len);
                  strbtrim(qbuf);
                  ptr = qbuf;
                  if(not IsQuoteChar(ptr)) {
                    ptr = qbuf+strlen(qbuf);
                    while(ptr >= qbuf) {
                      if(IsQuoteChar(ptr))
                        break;
                      ptr--;
                    }
                  }
                  x = (int)((dword)ptr-(dword)qbuf);
                  sprintf(buf, "%*.*s%*.*s>%s %s",
                    y, y, quotestr, x, x, qbuf, qbuf+x, quote
                  );
                }
                else if((not strblank(quote)) or CFG->switches.get(quoteblank))
                  sprintf(buf, "%s%s", quotestr, quote);
                else
                  *buf = NUL;
                n++;
                strtrim(buf);
                strcat(buf, "\r");
                len = strlen(buf);
                size += len;
                if(msg_txt_realloc_cache >= len) {
                  msg_txt_realloc_cache -= len;
                }
                else {
                  msg_txt_realloc_cache += REALLOC_CACHE_SIZE;
                  msg->txt = (char*)throw_realloc(msg->txt, size+10+msg_txt_realloc_cache);
                }
                strcpy(&(msg->txt[pos]), buf);
                pos += len;
              }
            }
            continue;

          case TPLTOKEN_MESSAGE:
            if((mode == MODE_FORWARD) or (mode == MODE_CHANGE) or
               (mode == MODE_WRITEHEADER) or (mode == MODE_WRITE)) {
              n = 0;
              while(oldmsg->line[n]) {
                if(oldmsg->line[n]->txt.c_str()) {
                  strcpy(buf, oldmsg->line[n]->txt.c_str());
                  if(mode == MODE_FORWARD) {
                    // Invalidate tearline
                    if(not CFG->invalidate.tearline.first.empty())
                      doinvalidate(buf, CFG->invalidate.tearline.first.c_str(), CFG->invalidate.tearline.second.c_str(), true);
                    // Invalidate originline
                    if(not CFG->invalidate.origin.first.empty())
                      doinvalidate(buf, CFG->invalidate.origin.first.c_str(), CFG->invalidate.origin.second.c_str());
                    // Invalidate SEEN-BY's
                    if(not CFG->invalidate.seenby.first.empty())
                      doinvalidate(buf, CFG->invalidate.seenby.first.c_str(), CFG->invalidate.seenby.second.c_str());
                    // Invalidate CC's
                    if(not CFG->invalidate.cc.first.empty())
                      doinvalidate(buf, CFG->invalidate.cc.first.c_str(), CFG->invalidate.cc.second.c_str());
                    // Invalidate XC's
                    if(not CFG->invalidate.xc.first.empty())
                      doinvalidate(buf, CFG->invalidate.xc.first.c_str(), CFG->invalidate.xc.second.c_str());
                    // Invalidate XP's
                    if(not CFG->invalidate.xp.first.empty())
                      doinvalidate(buf, CFG->invalidate.xp.first.c_str(), CFG->invalidate.xp.second.c_str());
                    // Invalidate kludge chars
                    strchg(buf, CTRL_A, '@');
                  }
                  len = strlen(buf);
                  size += len;
                  if(msg_txt_realloc_cache >= (len+1)) {
                    msg_txt_realloc_cache -= (len+1);
                  }
                  else {
                    msg_txt_realloc_cache += (size <= oldmsg_size) ? oldmsg_size : REALLOC_CACHE_SIZE;
                    msg->txt = (char*)throw_realloc(msg->txt, size+10+msg_txt_realloc_cache);
                  }
                  strcpy(&(msg->txt[pos]), buf);
                  pos += len;
                  if(oldmsg->line[n]->type & GLINE_HARD) {
                    msg->txt[pos++] = CR;
                    size++;
                  }
                  else {
                    if(oldmsg->line[n+1]) {
                      if(msg->txt[pos-1] != ' ' and *oldmsg->line[n+1]->txt.c_str() != ' ') {
                        msg->txt[pos++] = ' ';
                        size++;
                      }
                    }
                  }
                }
                n++;
              }
            }
            continue;
        }
      }

      if(not chg and ((mode == MODE_CHANGE) or (mode == MODE_WRITEHEADER) or
                      (mode == MODE_WRITE) or (mode == MODE_HEADER)))
        continue;
      if((mode == MODE_QUOTEBUF) and not quotebufline)
        continue;
      TokenXlat(mode, buf, msg, oldmsg, origarea);
      len = strlen(buf);
      size += len;
      if(msg_txt_realloc_cache >= len) {
        msg_txt_realloc_cache -= len;
      }
      else {
        msg_txt_realloc_cache += REALLOC_CACHE_SIZE;
        msg->txt = (char*)throw_realloc(msg->txt, size+10+msg_txt_realloc_cache);
      }
      strcpy(&(msg->txt[pos]), buf);
      pos += len;
    }
    loop_next:
    ;
  }
  fclose(fp);

  if((mode != MODE_CHANGE) and (mode != MODE_QUOTEBUF) and
     (mode != MODE_HEADER) and (mode != MODE_WRITEHEADER) and
     (mode != MODE_WRITE) and (mode != MODE_WRITEHEADER)) {
    ctrlinfo = AA->Ctrlinfo();
    ctrlinfo |= CI_TAGL;
    if(ctrlinfo & (CI_TAGL|CI_TEAR|CI_ORIG)) {

      // Add CR if it missed in template
      if(msg->txt[pos-1] != CR) {
        size++;
        msg->txt[pos++] = CR;
      }

      do {
        if((ctrlinfo & CI_ORIG) and not (ctrlinfo & CI_TEAR) and not (ctrlinfo & CI_TAGL)) {
          sprintf(buf, " * Origin: %s", msg->origin);
          ctrlinfo &= ~CI_ORIG;
        }
        if((ctrlinfo & CI_TEAR) and not (ctrlinfo & CI_TAGL)) {
          MakeTearline(msg, buf);
          ctrlinfo &= ~CI_TEAR;
        }
        if(ctrlinfo & CI_TAGL) {
          strbtrim(msg->tagline);
          if(AA->Taglinesupport() and *msg->tagline)
            sprintf(buf, "%c%c%c %s", AA->Taglinechar(), AA->Taglinechar(), AA->Taglinechar(), msg->tagline);
          else
            *buf = NUL;
          ctrlinfo &= ~CI_TAGL;
        }
        strtrim(buf);
        if(*buf) {
          strcat(buf, "\n");
          TokenXlat(mode, buf, msg, oldmsg, origarea);
          len = strlen(buf);
          size += len;
          if(msg_txt_realloc_cache >= len) {
            msg_txt_realloc_cache -= len;
          }
          else {
            msg_txt_realloc_cache += REALLOC_CACHE_SIZE;
            msg->txt = (char*)throw_realloc(msg->txt, size+10+msg_txt_realloc_cache);
          }
          strcpy(&(msg->txt[pos]), buf);
          pos += len;
        }
      } while(ctrlinfo & (CI_TAGL|CI_TEAR|CI_ORIG));
    }
  }
  msg->txt[pos] = NUL;

  if(tmptpl)
    remove(tplfile);

  return disphdr;
}


//  ------------------------------------------------------------------

void ChangeMsg() {

  int changemsg=NO;

  if(AA->Msgn.Count()) {

    if(AA->attr().r_o()) {
      GMenuReadonly MenuReadonly;
      reader_keyok = not MenuReadonly.Run();
      if(reader_keyok)
        return;
    }

    reader_keyok = YES;

    if(AA->attr().hex()) {
      AA->attr().hex0();
      AA->LoadMsg(reader_msg, reader_msg->msgno, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
    }

    for(std::vector<Node>:: iterator u = CFG->username.begin(); u != CFG->username.end(); u++)
      // Check FROM:
      if(reader_msg->orig.match(u->addr)) {
        if(strieql(u->name, reader_msg->By())) {
          if(u->addr.net != GFTN_ALL or reader_msg->orig.net == 0 or not AA->isnet())
            reader_keyok = NO;
          else {
            for(std::vector<gaka>::iterator x = CFG->aka.begin(); x != CFG->aka.end(); x++) {
              if(reader_msg->orig.match(x->addr)) {
                reader_keyok = NO;
                break;
              }
            }
          }
        }
      }

    if(reader_keyok == YES) {
      GMenuChange MenuChange;
      reader_keyok = not MenuChange.Run(LNG->ChangeWarn);
      if(reader_keyok == NO)
        changemsg = true;
    }
    else
      changemsg = true;

    if(changemsg) {
      if(reader_msg->attr.loc() and not AA->islocal()) {
        if(reader_msg->attr.snt() or not reader_msg->attr.uns()) {
          GMenuChange MenuChange;
          reader_keyok = not MenuChange.Run(LNG->WarnAlreadySent);
        }
      }
      if(reader_keyok == NO)
        MakeMsg(MODE_CHANGE, reader_msg);
    }
  }
}


//  ------------------------------------------------------------------

void NewMsg() {

  reader_topline = 0;
  AA->attr().hex0();
  if(AA->attr().r_o()) {
    GMenuReadonly MenuReadonly;
    reader_keyok = not MenuReadonly.Run();
  }
  if(not reader_keyok)
    MakeMsg(MODE_NEW, reader_msg);
}


//  ------------------------------------------------------------------

void ConfirmMsg() {

  int doit = CFG->confirmresponse;
  if(CFG->confirmresponse == ASK) {
    GMenuConfirm MenuConfirm;
    doit = MenuConfirm.Run();
  }

  reader_gen_confirm = false;
  if(doit and AA->isnet() and reader_msg->attr.cfm()) {
    int a = AL.AreaEchoToNo(CFG->areacfmreplyto);
    if(a != -1) {
      AL.SetActiveAreaNo(a);
      if(CurrArea != OrigArea)
        AA->Open();
    }
    reader_topline = 0;
    AA->attr().hex0();
    update_statusline(LNG->GenCfmReceipt);
    MakeMsg(MODE_CONFIRM, reader_msg);
    reader_topline = 0;
    LoadMessage(reader_msg, CFG->dispmargin-(int)CFG->switches.get(disppagebar));
    if(CurrArea != OrigArea) {
      AA->Close();
      AL.SetActiveAreaId(OrigArea);
    }
  }
  if(not CFG->switches.get(rcvdisablescfm)) {
    reader_msg->attr.cfm0();
    reader_msg->attr.upd1();
    reader_msg->charsetlevel = LoadCharset(CFG->xlatlocalset, reader_msg->charset);
    DoKludges(MODE_CHANGE, reader_msg, GKLUD_FLAGS);
    reader_msg->LinesToText();
    AA->SaveMsg(GMSG_UPDATE, reader_msg);
  }
}


//  ------------------------------------------------------------------

bool _allow_pick = true;

void OtherAreaReplyMsg() {

  if(AA->Msgn.Count()) {
    int destarea = CurrArea;
    if(CurrArea == OrigArea) {
      if(*AA->Areareplyto()) {
        int a = AL.AreaEchoToNo(AA->Areareplyto());
        if(a != -1)
          destarea = AL.AreaNoToId(a);
      }
      reader_topline = 0;
      AA->attr().hex0();
      const char* destinationecho = *reader_msg->fwdarea ? reader_msg->fwdarea : reader_msg->areakludgeid;
      if(destinationecho and *destinationecho) {
        for(uint n=0; n<AL.size(); n++) {
          if(strieql(AL[n]->echoid(), destinationecho)) {
            destarea = AL[n]->areaid();
            break;
          }
        }
      }
      if(_allow_pick or not AA->Areareplydirect())
        destarea = AreaPick(LNG->ReplyArea, 6, &destarea);
    }
    if(destarea != -1) {
      int adat_viewhidden = AA->Viewhidden();
      int adat_viewkludge = AA->Viewkludge();
      int adat_viewquote  = AA->Viewquote();
      AL.SetActiveAreaId(destarea);
      if(CurrArea != OrigArea) {
        AA->Open();
        if(CurrArea != OrigArea) {
          AA->adat->viewhidden  = adat_viewhidden;
          AA->adat->viewkludge  = adat_viewkludge;
          AA->adat->viewquote   = adat_viewquote;
        }
      }
      ReplyMsg();
      if(CurrArea != OrigArea) {
        AA->Close();
        AL.SetActiveAreaId(OrigArea);
      }
    }
  }
}

//  ------------------------------------------------------------------

void ReplyMsg() {

  if(CurrArea == OrigArea) {
    const char *destarea = reader_msg->areakludgeid;
    if ((destarea == NULL) or (*destarea == NUL))
      destarea = AA->Areareplyto();

    if(AA->Areareplydirect() and (destarea != NULL)) {
      int a = AL.AreaEchoToNo(destarea);
      if(a != -1) {
        CurrArea = AL.AreaNoToId(a);
        if(CurrArea != OrigArea) {
          _allow_pick = false;
          OtherAreaReplyMsg();
          _allow_pick = true;
          return;
        }
      }
    }
  }

  if(AA->Msgn.Count() or (CurrArea != OrigArea)) {
    reader_topline = 0;
    AA->attr().hex0();
    if(AA->attr().r_o()) {
      GMenuReadonly MenuReadonly;
      reader_keyok = not MenuReadonly.Run();
    }
    if(not reader_keyok)
      MakeMsg(MODE_REPLY, reader_msg);
  }
}


//  ------------------------------------------------------------------

void QuoteMsg(bool ignore_replyto) {

  if(CurrArea == OrigArea) {
    const char *destarea = reader_msg->areakludgeid;
    if ((destarea == NULL) or (*destarea == NUL))
      destarea = AA->Areareplyto();

    if(AA->Areareplydirect() and (destarea != NULL)) {
      int a = AL.AreaEchoToNo(destarea);
      if(a != -1) {
        CurrArea = AL.AreaNoToId(a);
        if(CurrArea != OrigArea) {
          _allow_pick = false;
          OtherAreaQuoteMsg();
          _allow_pick = true;
          return;
        }
      }
    }
  }

  if(AA->Msgn.Count() or (CurrArea != OrigArea)) {
    reader_topline = 0;
    AA->attr().hex0();
    if(AA->attr().r_o()) {
      GMenuReadonly MenuReadonly;
      reader_keyok = not MenuReadonly.Run();
    }
    if(not reader_keyok)
      MakeMsg(MODE_QUOTE, reader_msg, ignore_replyto);
  }
}


//  ------------------------------------------------------------------

void CommentMsg() {

  if(CurrArea == OrigArea) {
    const char *destarea = reader_msg->areakludgeid;
    if ((destarea == NULL) or (*destarea == NUL))
      destarea = AA->Areareplyto();

    if(AA->Areareplydirect() and (destarea != NULL)) {
      int a = AL.AreaEchoToNo(destarea);
      if(a != -1) {
        CurrArea = AL.AreaNoToId(a);
        if(CurrArea != OrigArea) {
          _allow_pick = false;
          OtherAreaCommentMsg();
          _allow_pick = true;
          return;
        }
      }
    }
  }

  if(AA->Msgn.Count() or (CurrArea != OrigArea)) {
    reader_topline = 0;
    AA->attr().hex0();
    if(AA->attr().r_o()) {
      GMenuReadonly MenuReadonly;
      reader_keyok = not MenuReadonly.Run();
    }
    if(not reader_keyok)
      MakeMsg(MODE_REPLYCOMMENT, reader_msg);
  }
}


//  ------------------------------------------------------------------

void OtherAreaQuoteMsg(bool ignore_replyto) {

  if(AA->Msgn.Count()) {
    int destarea = CurrArea;
    if(CurrArea == OrigArea) {
      if(*AA->Areareplyto()) {
        int a = AL.AreaEchoToNo(AA->Areareplyto());
        if(a != -1)
          destarea = AL.AreaNoToId(a);
      }
      reader_topline = 0;
      AA->attr().hex0();
      const char* destinationecho = *reader_msg->fwdarea ? reader_msg->fwdarea : reader_msg->areakludgeid;
      if(destinationecho and *destinationecho) {
        for(uint n=0; n<AL.size(); n++) {
          if(strieql(AL[n]->echoid(), destinationecho)) {
            destarea = AL[n]->areaid();
            break;
          }
        }
      }
      if(_allow_pick or not AA->Areareplydirect())
        destarea = AreaPick(LNG->ReplyArea, 6, &destarea);
    }
    if(destarea != -1) {
      int adat_viewhidden = AA->Viewhidden();
      int adat_viewkludge = AA->Viewkludge();
      int adat_viewquote  = AA->Viewquote();
      AL.SetActiveAreaId(destarea);
      if(CurrArea != OrigArea) {
        AA->Open();
        if(CurrArea != OrigArea) {
          AA->adat->viewhidden  = adat_viewhidden;
          AA->adat->viewkludge  = adat_viewkludge;
          AA->adat->viewquote   = adat_viewquote;
        }
      }
      QuoteMsg(ignore_replyto);
      if(CurrArea != OrigArea) {
        AA->Close();
        AL.SetActiveAreaId(OrigArea);
      }
    }
  }
}


//  ------------------------------------------------------------------

void OtherAreaCommentMsg() {

  if(AA->Msgn.Count()) {

    int destarea = CurrArea;
    if(CurrArea == OrigArea) {
      if(*AA->Areareplyto()) {
        int a = AL.AreaEchoToNo(AA->Areareplyto());
        if(a != -1)
          destarea = AL.AreaNoToId(a);
      }
      reader_topline = 0;
      AA->attr().hex0();
      const char* destinationecho = *reader_msg->fwdarea ? reader_msg->fwdarea : reader_msg->areakludgeid;
      if(destinationecho and *destinationecho) {
        for(uint n=0; n<AL.size(); n++) {
          if(strieql(AL[n]->echoid(), destinationecho)) {
            destarea = AL[n]->areaid();
            break;
          }
        }
      }
      if(_allow_pick or not AA->Areareplydirect())
        destarea = AreaPick(LNG->ReplyArea, 6, &destarea);
    }
    if(destarea != -1) {
      int adat_viewhidden = AA->Viewhidden();
      int adat_viewkludge = AA->Viewkludge();
      int adat_viewquote  = AA->Viewquote();
      AL.SetActiveAreaId(destarea);
      if(CurrArea != OrigArea) {
        AA->Open();
        if(CurrArea != OrigArea) {
          AA->adat->viewhidden  = adat_viewhidden;
          AA->adat->viewkludge  = adat_viewkludge;
          AA->adat->viewquote   = adat_viewquote;
        }
      }
      CommentMsg();
      if(CurrArea != OrigArea) {
        AA->Close();
        AL.SetActiveAreaId(OrigArea);
      }
    }
  }
}


//  ------------------------------------------------------------------

