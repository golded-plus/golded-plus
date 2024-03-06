
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
//  Misc. msg handling.
//  ------------------------------------------------------------------

#include <cstdarg>
#include <golded.h>
#include <gtimall.h>

#if defined(__USE_ALLOCA__)
    #include <malloc.h>
#endif


//  ------------------------------------------------------------------

inline const char *it2str(std::string &str, std::string::iterator it)
{
    return &(str.c_str()[it-str.begin()]);
}


//  ------------------------------------------------------------------

static bool tokenxchg(std::string &input, std::string::iterator &pos,
                      const char* tok, const char* src,
                      size_t len = 0, size_t cnt = 0, ...)
{
    size_t toklen = strlen(tok);
    const char* inp_str_tok=it2str(input, pos);

    if ( (strnieql(inp_str_tok, tok, toklen)) && (isspace(inp_str_tok[toklen]) || ispunct(inp_str_tok[toklen]) || inp_str_tok[toklen]=='\0') )
    {
        std::string str = src;
        std::string::iterator tokend = pos+toklen;

        if (cnt)
        {
            va_list a;
            va_start(a, cnt);

            for (int i = 0; i < cnt; i++)
            {
                std::string::iterator beg = tokend;
                if ((beg == input.end()) || (*beg++ != '{'))
                    break;

                std::string::iterator end;
                for (end = beg; end != input.end(); end++)
                {
                    if (*end == '}')
                    {
                        if (va_arg(a, int))
                            str = input.substr(beg-input.begin(), end-beg);

                        end++;
                        break;
                    }
                }

                tokend = end;
            }

            va_end(a);
        }

        size_t strlen = str.length();
        if (!len || (len > strlen)) len = strlen;

        size_t idx = pos - input.begin();
        input.replace(pos, tokend, str.c_str(), len);
        pos = input.begin() + idx;

        return true;
    }

    return false;
}


//  ------------------------------------------------------------------

static void translate(std::string &text)
{
    std::map<std::string, std::string>::iterator it = CFG->translate.begin();
    for (; it != CFG->translate.end(); ++it)
    {
        const std::string& str1 = it->first;
        const std::string& str2 = it->second;

        size_t length = text.length();

        for (size_t pos = 0; (pos < length) && (text[pos] != '}'); )
        {
            if (strnieql(&(text.c_str()[pos]), str1.c_str(), str1.length()))
            {
                text.replace(pos, str1.length(), str2);
                pos += str2.length();
                length = text.length();
            }
            else
                pos++;
        }
    }
}


//  ------------------------------------------------------------------

inline bool domain_requested(const char *str, size_t pos)
{
    if (str[1] == '_') pos++;
    return strnieql(str+pos, "{domain}", 8);
}


//  ------------------------------------------------------------------

void TokenXlat(int mode, std::string &input, GMsg* msg, GMsg* oldmsg, int __origarea)
{
    static char revbuf[5] = "";
    if (revbuf[0] == NUL)
        gsprintf(PRINTF_DECLARE_BUFFER(revbuf), "%02d%02d", str2mon(__gver_date__), atoi(&__gver_date__[4]));

    char attr[80];
    MakeAttrStr(attr, sizeof(attr), &msg->attr);

    const char *xmailer = get_informative_string();
    const char *osver = ggetosstring();

    time32_t t = gtime(NULL);
    struct tm written_tm;
    glocaltime(&written_tm, &t);
    char cdate[80];
    strftimei(cdate, 80, LNG->DateFmt, &written_tm);
    char ctime[80];
    strftimei(ctime, 80, LNG->TimeFmt, &written_tm);
    char cdtime[80];
    strftimei(cdtime, 80, LNG->DateTimeFmt, &written_tm);
    ggmtime(&written_tm, &oldmsg->written);
    char odate[80];
    strftimei(odate, 80, LNG->DateFmt, &written_tm);
    char otime[80];
    strftimei(otime, 80, LNG->TimeFmt, &written_tm);
    char odtime[80];
    strftimei(odtime, 80, LNG->DateTimeFmt, &written_tm);

    const char* origareaid = AL.AreaIdToPtr(__origarea)->echoid();
    bool origareaisinet = make_bool(AL.AreaIdToPtr(__origarea)->isinternet());
    bool currareaisinet = make_bool(AA->isinternet());
    char* modereptr = oldmsg->re;

    char msgno[16];
    gsprintf(PRINTF_DECLARE_BUFFER(msgno), "%u", msg->attr.nwm() ? AA->Msgn.Count() + 1 : AA->Msgn.ToReln(msg->msgno));

    char msgs[16];
    gsprintf(PRINTF_DECLARE_BUFFER(msgs), "%u", AA->Msgn.Count() + (msg->attr.nwm() ? 1 : 0));

    if ((mode == MODE_QUOTE) or (mode == MODE_REPLYCOMMENT) or (mode == MODE_REPLY))
    {
        if (AL.AreaIdToPtr(__origarea)->Areareplydirect() and oldmsg->areakludgeid)
            origareaid = oldmsg->areakludgeid;
    }
    else if (mode != MODE_FORWARD)
        modereptr = msg->re;

    char buf[100];

    std::string::iterator dst;
    for (dst = input.begin(); dst != input.end(); )
    {
        if (*dst == LF) *dst = CR;
        else if (*dst != '@') ;
        else if (*(dst+1) == '@') input.erase(dst);
        else
        {
            if (tokenxchg(input, dst, "@cecho", AA->echoid()))
                continue;

            if (tokenxchg(input, dst, "@cdesc", AA->desc()))
                continue;

            if (tokenxchg(input, dst, "@oecho", origareaid))
                continue;

            if (tokenxchg(input, dst, "@odesc", AL.AreaEchoToPtr(origareaid)->desc()))
                continue;

            if (tokenxchg(input, dst, "@oname", strbtrim(strtmp(oldmsg->By())), 34, 2,
                          (int)msg->by_me(), (int)msg->by_you()))
                continue;

            if (tokenxchg(input, dst, "@ofname", strlword(oldmsg->By()), 0, 2,
                          (int)msg->by_me(), (int)msg->by_you()))
                continue;

            if (tokenxchg(input, dst, "@olname", strrword(oldmsg->By()), 0, 2,
                          (int)msg->by_me(), (int)msg->by_you()))
                continue;

            if (tokenxchg(input, dst, "@odate", odate))
                continue;

            if (tokenxchg(input, dst, "@otime", otime))
                continue;

            if (tokenxchg(input, dst, "@odtime", odtime))
                continue;

            if (tokenxchg(input, dst, "@otzoffset", (oldmsg->tzutc == -32767) ? "" : (gsprintf(PRINTF_DECLARE_BUFFER(buf), "%+05d", oldmsg->tzutc), buf)))
                continue;

            if (tokenxchg(input, dst, "@ofrom", oldmsg->ifrom))
                continue;

            if (tokenxchg(input, dst, "@oto", oldmsg->ito))
                continue;

            if (tokenxchg(input, dst, "@omessageid", oldmsg->messageid ? oldmsg->messageid : ""))
                continue;

            if (tokenxchg(input, dst, "@omsgid", *msg->replys ? msg->replys : ""))
                continue;

            if (tokenxchg(input, dst, "@dname", strbtrim(strtmp(oldmsg->To())), 34, 3,
                          (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all()))
                continue;

            if (tokenxchg(input, dst, "@dpgp", *msg->iaddr ? msg->iaddr : msg->To()))
                continue;

            if (tokenxchg(input, dst, "@dfname", strlword(oldmsg->To()), 0, 3,
                          (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all()))
                continue;

            if (tokenxchg(input, dst, "@dlname", strrword(oldmsg->To()), 0, 3,
                          (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all()))
                continue;

            if (origareaisinet)
            {
                if (tokenxchg(input, dst, "@oaddr", oldmsg->iorig, 19, 1, 0))
                    continue;

                if (tokenxchg(input, dst, "@daddr", oldmsg->iaddr, 19, 1, 0))
                    continue;
            }

            if (currareaisinet)
            {
                if (tokenxchg(input, dst, "@caddr", AA->Internetaddress(), 19, 1, 0))
                    continue;

                if (tokenxchg(input, dst, "@faddr", msg->iorig, 19, 1, 0))
                    continue;

                if (tokenxchg(input, dst, "@taddr", msg->iaddr, 19, 1, 0))
                    continue;
            }

            if ((not origareaisinet or not currareaisinet) and (strlen(it2str(input, dst)) >= 6))
            {
                bool dr = domain_requested(it2str(input, dst), 6);
                if (not origareaisinet)
                {
                    if (tokenxchg(input, dst, "@oaddr", oldmsg->orig.make_string(buf, dr ? oldmsg->odom : NULL), 19, 1, 0))
                        continue;

                    if (strnieql(it2str(input, dst), "@o3daddr", 8))
                    {
                        ftn_addr boss = oldmsg->orig;
                        boss.point = 0;
                        tokenxchg(input, dst, "@o3daddr", boss.make_string(buf, domain_requested(it2str(input, dst), 8) ? oldmsg->odom : NULL), 19, 1, 0);
                        continue;
                    }

                    if (tokenxchg(input, dst, "@daddr", oldmsg->dest.make_string(buf, dr ? oldmsg->ddom : NULL), 19, 1, 0))
                        continue;

                    if (strnieql(it2str(input, dst), "@d3daddr", 8))
                    {
                        ftn_addr boss = oldmsg->dest;
                        boss.point = 0;
                        tokenxchg(input, dst, "@d3daddr", boss.make_string(buf, domain_requested(it2str(input, dst), 8) ? oldmsg->ddom : NULL), 19, 1, 0);
                        continue;
                    }
                }

                if (not currareaisinet)
                {
                    const gaka &caka=AA->Aka();
                    if (tokenxchg(input, dst, "@caddr", caka.addr.make_string(buf, dr ? caka.domain : NULL), 19, 1, 0))
                        continue;

                    if (strnieql(it2str(input, dst), "@c3daddr", 8))
                    {
                        ftn_addr boss = caka.addr;
                        boss.point = 0;
                        tokenxchg(input, dst, "@c3daddr", boss.make_string(buf, domain_requested(it2str(input, dst), 8) ? caka.domain : NULL), 19, 1, 0);
                        continue;
                    }

                    if (tokenxchg(input, dst, "@taddr", msg->dest.make_string(buf, dr ? msg->ddom : NULL), 19, 1, 0))
                        continue;

                    if (strnieql(it2str(input, dst), "@t3daddr", 8))
                    {
                        ftn_addr boss = msg->dest;
                        boss.point = 0;
                        tokenxchg(input, dst, "@t3daddr", boss.make_string(buf, domain_requested(it2str(input, dst), 8) ? msg->ddom : NULL), 19, 1, 0);
                        continue;
                    }

                    if (tokenxchg(input, dst, "@faddr", msg->orig.make_string(buf, dr ? msg->odom : NULL), 19, 1, 0))
                        continue;

                    if (strnieql(it2str(input, dst), "@f3daddr", 8))
                    {
                        ftn_addr boss = msg->orig;
                        boss.point = 0;
                        tokenxchg(input, dst, "@f3daddr", boss.make_string(buf, domain_requested(it2str(input, dst), 8) ? msg->odom : NULL), 19, 1, 0);
                        continue;
                    }
                }
            }

            if (tokenxchg(input, dst, "@tname", strbtrim(strtmp(msg->To())), 34, 3,
                          (int)false, (int)false, (int)msg->to_all()))
                continue;

            if (tokenxchg(input, dst, "@tfname", strlword(msg->To()), 0, 3,
                          (int)false, (int)false, (int)msg->to_all()))
                continue;

            if (tokenxchg(input, dst, "@tlname", strrword(msg->To()), 0, 3,
                          (int)false, (int)false, (int)msg->to_all()))
                continue;

            if (tokenxchg(input, dst, "@cname", AA->Username().name, 34))
                continue;

            if (tokenxchg(input, dst, "@cfname", strlword(strcpy(buf, AA->Username().name))))
                continue;

            if (tokenxchg(input, dst, "@clname", strrword(strcpy(buf, AA->Username().name))))
                continue;

            if (tokenxchg(input, dst, "@cfrom", msg->ifrom))
                continue;

            if (tokenxchg(input, dst, "@cto", msg->ito))
                continue;

            if (tokenxchg(input, dst, "@cdate", cdate))
                continue;

            if (tokenxchg(input, dst, "@ctime", ctime))
                continue;

            if (tokenxchg(input, dst, "@cdtime", cdtime))
                continue;

            if (tokenxchg(input, dst, "@ctzoffset", AA->Usetzutc() ? (gsprintf(PRINTF_DECLARE_BUFFER(buf), "%+05d", tzoffset()), buf) : ""))
                continue;

            if (tokenxchg(input, dst, "@fname", strbtrim(strtmp(msg->By())), 34))
                continue;

            if (tokenxchg(input, dst, "@fpgp", *msg->iorig ? msg->iorig : msg->By()))
                continue;

            if (tokenxchg(input, dst, "@ffname", strlword(msg->By())))
                continue;

            if (tokenxchg(input, dst, "@flname", strrword(msg->By())))
                continue;

            if (strnieql(it2str(input, dst), "@dpseudo", 8))
            {
                if (*(oldmsg->pseudoto) == NUL)
                    build_pseudo(oldmsg);
                tokenxchg(input, dst, "@dpseudo", oldmsg->pseudoto, 0, 3, (int)msg->to_me(), (int)msg->to_you(), (int)oldmsg->to_all());
                continue;
            }

            if (strnieql(it2str(input, dst), "@opseudo", 8))
            {
                if (*(oldmsg->pseudofrom) == NUL)
                    build_pseudo(oldmsg, false);
                tokenxchg(input, dst, "@opseudo", oldmsg->pseudofrom, 0, 2, (int)msg->by_me(), (int)msg->by_you());
                continue;
            }

            if (strnieql(it2str(input, dst), "@tpseudo", 8))
            {
                if (*(msg->pseudoto) == NUL)
                    build_pseudo(msg);
                tokenxchg(input, dst, "@tpseudo", msg->pseudoto, 0, 3, (int)false, (int)false, (int)msg->to_all());
                continue;
            }

            // Same as above (just for backward compatibility)
            if (strnieql(it2str(input, dst), "@pseudo", 7))
            {
                if (*(msg->pseudoto) == NUL)
                    build_pseudo(msg);
                tokenxchg(input, dst, "@pseudo", msg->pseudoto, 0, 3, (int)false, (int)false, (int)msg->to_all());
                continue;
            }

            if (strnieql(it2str(input, dst), "@fpseudo", 8))
            {
                if (*(msg->pseudofrom) == NUL)
                    build_pseudo(msg, false);
                tokenxchg(input, dst, "@fpseudo", msg->pseudofrom);
                continue;
            }

            if (tokenxchg(input, dst, "@msgno", msgno))
                continue;

            if (tokenxchg(input, dst, "@msgs", msgs))
                continue;

            if (tokenxchg(input, dst, "@cpseudo", *AA->Nickname() ? AA->Nickname() : strlword(strcpy(buf, AA->Username().name), " @.")))
                continue;

            if (tokenxchg(input, dst, "@version", __gver_ver__))
                continue;

            if (tokenxchg(input, dst, "@ver", __gver_shortver__))
                continue;

            if (tokenxchg(input, dst, "@rev", revbuf))
                continue;

            if (tokenxchg(input, dst, "@pid", __gver_shortpid__))
                continue;

            if (tokenxchg(input, dst, "@longpid", __gver_longpid__))
                continue;

            if (tokenxchg(input, dst, "@widepid", xmailer))
                continue;

            if (tokenxchg(input, dst, "@osver", osver))
                continue;

            if (tokenxchg(input, dst, "@osslash", __gver_platform__))
                continue;

            if (tokenxchg(input, dst, "@subject", modereptr))
                continue;

            if (tokenxchg(input, dst, "@attr", attr))
                continue;

            if (tokenxchg(input, dst, "@tagline",
                          HandleRandomLine(strxcpy(buf, AA->Tagline(), sizeof(buf)), sizeof(buf))))
                continue;

            if (tokenxchg(input, dst, "@tearline",
                          HandleRandomLine(strxcpy(buf, AA->Tearline(), sizeof(buf)), sizeof(buf))))
                continue;

            if (tokenxchg(input, dst, "@origin",
                          HandleRandomLine(strxcpy(buf, AA->Origin(), sizeof(buf)), sizeof(buf))))
                continue;

            if (strnieql(it2str(input, dst), "@area", 5))
            {
                if (tokenxchg(input, dst, "@areatype", AA->basetype().c_str()))
                    continue;

                char areapath[GMAXPATH];
                char areaname[GMAXPATH];

                strcpy(areapath, AA->path());
                areaname[0] = NUL;

                size_t slashpos;
                size_t pathlen = strlen(areapath);

                for (slashpos = pathlen-1; slashpos < pathlen; slashpos--)
                    if (isslash(areapath[slashpos])) break;

                if (slashpos < pathlen)
                {
                    strcpy(areaname, &areapath[slashpos+1]);
                    areapath[slashpos+1] = NUL;
                }
                else
                {
                    strcpy(areaname, areapath);
                    areapath[0] = NUL;
                }

                if (tokenxchg(input, dst, "@areapath", areapath))
                    continue;

                if (tokenxchg(input, dst, "@areaname", areaname))
                    continue;
            }

            if (strnieql(it2str(input, dst), "@align{", 7))
            {
                std::string::iterator ptr = dst+7;
                while ((*ptr != '}') && (ptr != input.end())) ptr++;

                if (*ptr != '}')
                    tokenxchg(input, dst, "@align", "");
                else
                {
                    int size = atoi(it2str(input, dst)+7) - (dst-input.begin());
                    if (size < 0) size = 0;

                    char filler = ' ';
                    if ((*(ptr+1) == '{') && (*(ptr+3) == '}'))
                    {
                        ptr += 2;
                        filler = *ptr;
                    }

                    std::string fill(size, filler);
                    tokenxchg(input, dst, "@align", fill.c_str(), 0, 2, (int)false, (int)false);
                    dst += size;
                }

                continue;
            }

            if (strnieql(it2str(input, dst), "@pipe{", 6))
            {
                std::string::iterator pbeg = dst+6;
                std::string::iterator pend = pbeg;
                while ((*pend != '}') && (pend != input.end()))
                    pend++;

                if (*pend != '}')
                    tokenxchg(input, dst, "@pipe", "");
                else
                {
                    std::string param = input.substr(pbeg-input.begin(), pend-pbeg);

                    gfile pipe_in;
                    std::string pipe_buff;

                    if ((pipe_in.Popen(param, "rt")) != NULL )
                    {
                        char buffer[1024];
                        while (!pipe_in.feof_())
                        {
                            if (pipe_in.Fgets(buffer, sizeof(buffer)) != NULL)
                                pipe_buff += buffer;
                        }

                        pipe_in.Pclose();
                    }

                    for (size_t i = 0; i < pipe_buff.length(); i++)
                    {
                        if (pipe_buff[i] == LF)
                            pipe_buff[i] = CR;
                    }

                    if (tokenxchg(input, dst, "@pipe", pipe_buff.c_str(), 0, 1, (int)false))
                        continue;
                }
            }

            if (strnieql(it2str(input, dst), "@pad{", 5))
            {
                std::string::iterator pbeg = dst+5;
                std::string::iterator pend = pbeg;
                while ((*pend != '}') && (pend != input.end()))
                    pend++;

                if ((*pend != '}') || ((pend-pbeg) < 3))
                    tokenxchg(input, dst, "@pad", "");
                else
                {
                    pend++;

                    char fill = *pbeg;
                    char align = toupper(*(pbeg+1));
                    int padsize = atoi(it2str(input, pbeg)+2);

                    if (padsize < 0) padsize = 0;

                    std::string text;
                    if (*pend == '{')
                    {
                        std::string buff = input.substr(pend-input.begin());
                        TokenXlat(mode, buff, msg, oldmsg, __origarea);

                        size_t idx1 = dst - input.begin();
                        size_t idx2 = pend - dst;
                        input.replace(pend, input.end(), buff);
                        dst = input.begin() + idx1;
                        pend = pbeg = dst + idx2 + 1;

                        while ((*pend != '}') && (pend != input.end()))
                            pend++;

                        text = input.substr(pbeg-input.begin(), pend-pbeg);
                    }

                    size_t tlen = text.length();
                    if (tlen != padsize)
                    {
                        int diff = padsize - tlen;
                        switch (align)
                        {
                        case 'L':
                            if (diff > 0)
                                text.insert(text.end(), diff, fill);
                            else
                                text.erase(padsize);
                            break;
                        case 'R':
                            if (diff > 0)
                                text.insert(text.begin(), diff, fill);
                            else
                                text = text.substr(-diff);
                            break;
                        default:
                            if (diff > 0)
                            {
                                text.insert(text.begin(), diff/2, fill);
                                text.insert(text.end(), diff-diff/2, fill);
                            }
                            else
                            {
                                text = text.substr((-diff)/2);
                                text.erase(padsize);
                            }
                            break;
                        }
                    }

                    if (tokenxchg(input, dst, "@pad", text.c_str(), 0, 2, (int)false, (int)false))
                        continue;
                }
            }

            if (strnieql(it2str(input, dst), "@tr{", 4))
            {
                std::string buff = input.substr(dst+3-input.begin());
                TokenXlat(mode, buff, msg, oldmsg, __origarea);
                translate(buff);

                size_t idx = dst - input.begin();
                input.replace(dst+3, input.end(), buff);
                dst = input.begin() + idx;

                if (tokenxchg(input, dst, "@tr", "", 0, 1, (int)true))
                    continue;
            }

            if (strnieql(it2str(input, dst), "@uptime", 7))
            {
                size_t days = 0;
                size_t hours = 0;
                size_t minutes = 0;
                size_t seconds = 0;
                size_t useconds = 0;

#ifdef __WIN32__
                LARGE_INTEGER counter;
                LARGE_INTEGER frequency;
                QueryPerformanceCounter(&counter);
                QueryPerformanceFrequency(&frequency);
                seconds = counter.QuadPart / frequency.QuadPart;
                useconds = size_t(double(counter.QuadPart % frequency.QuadPart)*1000 / frequency.QuadPart);
#elif defined(__unix__)
                FILE *fd = fopen("/proc/uptime","r");
                if( fd )
                {
                    char s[20];
                    if( fgets(s, sizeof(s), fd) )
                    {
                        if( sizeof(size_t) == sizeof(long) )
                            sscanf(s, "%lu.%2lu", &seconds, &useconds);
                        /* to future 64-bit unixtime
                                    else if( sizeof(size_t) == sizeof(long long) )
                                    sscanf(s, "%Lu.%2Lu", &seconds, &useconds);
                        */
                        else if( sizeof(size_t) == sizeof(unsigned) )
                            sscanf(s, "%u.%2u", &seconds, &useconds);
                    }
                }
#else
                LOG.printf("! Macros \"@uptime\" is not suppoted on platform %s!", __gver_platform__);
#endif

                if( seconds+useconds>0 )
                {
                    days = seconds/(60*60*24);
                    seconds %= 60*60*24;
                    hours = seconds/(60*60);
                    seconds %= 60*60;
                    minutes = seconds/60;
                    seconds %= 60;

                    std::string uptime(LNG->Uptime);

                    FormatString( FormatString( FormatString( FormatString( FormatString(
                                                    uptime, "%days", days),
                                                "%hours", hours),
                                                "%minutes", minutes),
                                                "%seconds", seconds),
                                  "%useconds", useconds);

                    tokenxchg(input, dst, "@uptime", uptime.c_str());
                }
                else
                {
                    LOG.printf("! Can't determine uptime value, set @uptime empty.");
                    tokenxchg(input, dst, "@uptime", "");
                }
            }
        }
        dst++;
    }
}

//  ------------------------------------------------------------------

void TokenXlat(int mode, char *&input, size_t size, bool resize, GMsg* msg, GMsg* oldmsg, int origarea)
{
    std::string buff = input;
    TokenXlat(mode, buff, msg, oldmsg, origarea);

    if (resize)
    {
        size_t newsize = buff.length() + 1;
        if (size < newsize)
            input = (char*)throw_realloc(input, size = newsize);
    }

    strncpy(input, buff.c_str(), size);
    input[size-1] = 0;
}


//  ------------------------------------------------------------------
//  ROT13 De/crypting function

void Rot13(GMsg* msg)
{

    char c;
    int ptr;
    int n;
    Line* line;
    static char rot13_table[256];
    static char done = NO;
#define ROT13_DECODE(c)   (((c) & 0x80) ? (c) : rot13_table[c])

    if(not done)
    {
        for(n=0; n<256; n++)
        {
            c = (char)n;
            if     (c >= 'a' and c <= 'm') c += '\xD';
            else if(c >= 'n' and c <= 'z') c -= '\xD';
            else if(c >= 'A' and c <= 'M') c += '\xD';
            else if(c >= 'N' and c <= 'Z') c -= '\xD';
            rot13_table[n] = c;
        }
        done = YES;
    }

    line = msg->lin;
    while(line)
    {
        if(not (line->type & (GLINE_KLUDGE|GLINE_TEAR|GLINE_ORIG)))
        {
            for(ptr = 0; ptr != line->txt.length(); ptr++)
            {
                c = line->txt[ptr];
                line->txt[ptr] = ROT13_DECODE(c & 0xff);
            }
        }
        line = line->next;
    }
}


//  ------------------------------------------------------------------

void LoadText(GMsg* msg, const char* textfile)
{
    uint size;
    char* buf;
    char* ptr;
    char* txtptr;
    int hardcr = NO, hardlen;
    char hardline[20];

    gfile fp(textfile, "rt", CFG->sharemode);
    if (fp.isopen())
    {
#define PBUFSIZE 4096   // Allow a 4k long paragraph

        size_t buf_len = PBUFSIZE;
        buf = (char*)throw_malloc(PBUFSIZE);

        int tabsz = CFG->disptabsize ? CFG->disptabsize : 1;
#if defined(__USE_ALLOCA__)
        char *spaces = (char*)alloca(tabsz+1);
#else
        __extension__ char spaces[tabsz+1];
#endif
        memset(spaces, ' ', tabsz);
        spaces[tabsz] = NUL;

        uint tlen = (uint)(fp.FileLength()+512);
        msg->txt = txtptr = (char*)throw_realloc(msg->txt, tlen);
        memset(msg->txt, NUL, tlen);

        if(EDIT->HardLines())
            strcpy(hardline, EDIT->HardLine());
        else
            *hardline = NUL;
        strcat(hardline, "\n");
        hardlen = strlen(hardline);
        *txtptr = NUL;

        while (fp.Fgets(buf, PBUFSIZE-1))
        {
            if(EDIT->HardLines() and strneql(buf, hardline, hardlen))
            {
                hardcr = not hardcr;
                if(*txtptr == LF)
                    *txtptr = CR;
                if(*txtptr)
                    txtptr++;
                continue;
            }
            else
            {
                size_t read_len = strlen(buf);

                char *ht = buf;
                while((ht = strchr(ht, '\t')) != NULL)
                {
                    int rposn = ht-buf;
                    int rstart = rposn%tabsz+1;
                    *ht = ' ';
                    if(tabsz > rstart)
                    {
                        if((read_len + tabsz - rstart) >= (buf_len-1))
                        {
                            buf_len += tabsz;
                            buf = (char*)throw_realloc(buf, buf_len);
                        }
                        strins(spaces+rstart, buf, rposn);
                    }
                }

                ptr = buf;
                while(*ptr == ' ')    // Skip leading spaces
                    ptr++;
                if(*ptr != LF)
                {
                    strsrep(buf, hardline, "\r");
                    read_len = strlen(buf);
                    if(hardcr or *buf == CTRL_A)
                    {
                        if(not isspace(buf[read_len-1])) read_len += 1;
                        buf[read_len-1] = CR;
                        buf[read_len] = NUL;
                        if(*txtptr == LF)
                            *txtptr = CR;
                    }
                    else if(is_quote(buf))
                    {
                        if(not isspace(buf[read_len-1])) read_len += 1;
                        buf[read_len-1] = CR;
                        buf[read_len] = NUL;
                        if(*txtptr == LF)
                            *txtptr = CR;
                    }
                    else if(not isspace(buf[0]) and (buf[0] == buf[1]) and (buf[0] == buf[2]))
                    {
                        if(not isspace(buf[read_len-1])) read_len += 1;
                        buf[read_len-1] = CR;
                        buf[read_len] = NUL;
                        if(*txtptr == LF)
                            *txtptr = CR;
                    }
                    else if(strnieql(buf, " * Origin: ", 11))
                    {
                        if(not isspace(buf[read_len-1])) read_len += 1;
                        buf[read_len-1] = CR;
                        buf[read_len] = NUL;
                        if(*txtptr == LF)
                            *txtptr = CR;
                    }
                    else if(strnieql(ptr, "CC:", 3))
                    {
                        if(not isspace(buf[read_len-1])) read_len += 1;
                        buf[read_len-1] = CR;
                        buf[read_len] = NUL;
                        if(*txtptr == LF)
                            *txtptr = CR;
                    }
                    else if(strnieql(ptr, "XC:", 3))
                    {
                        if(not isspace(buf[read_len-1])) read_len += 1;
                        buf[read_len-1] = CR;
                        buf[read_len] = NUL;
                        if(*txtptr == LF)
                            *txtptr = CR;
                    }
                    else if(strnieql(buf, "SEEN-BY: ", 9))
                    {
                        if(not isspace(buf[read_len-1])) read_len += 1;
                        buf[read_len-1] = CR;
                        buf[read_len] = NUL;
                        if(*txtptr == LF)
                            *txtptr = CR;
                    }
                    if(*txtptr == LF)
                    {
                        if((*(txtptr-1) != ' ') and (*ptr != ' '))
                            *txtptr++ = ' ';
                    }
                    else if(*txtptr != 0)
                        txtptr++;
                }
                else
                {
                    if(*txtptr == LF)
                        *txtptr = CR;
                    if(*txtptr)
                        txtptr++;
                    strcpy(buf, "\r");
                }
            }
            if((*(txtptr-1) == CR) or (*txtptr == NUL))
            {
                size = strlen(buf);
                memcpy(txtptr, buf, size);
                txtptr += size-1;
            }
        }
        if(*txtptr != CR)
            *(++txtptr) = CR;
        *(++txtptr) = NUL;
        throw_free(buf);
    }
}


//  ------------------------------------------------------------------

void GMsg::LinesToText()
{

    Chs* _xlat_table = CharTable;
    int _xlat_level = _xlat_table ? (_xlat_table->level ? _xlat_table->level : 2) : 0;

    strxcpy(realby, XlatStr(realby, _xlat_level, _xlat_table).c_str(), sizeof(realby));
    strxcpy(realto, XlatStr(realto, _xlat_level, _xlat_table).c_str(), sizeof(realto));
    strxcpy(by, XlatStr(by, _xlat_level, _xlat_table).c_str(), sizeof(by));
    strxcpy(to, XlatStr(to, _xlat_level, _xlat_table).c_str(), sizeof(to));
    if(not (attr.frq() or attr.att() or attr.urq()))
    {
        strxcpy(re, XlatStr(re, _xlat_level, _xlat_table).c_str(), sizeof(re));
    }

    bool _lfterm = EDIT->CrLfTerm() and (AA->basetype() == "PCBOARD");
    bool _hardterm = AA->Edithardterm() or AA->requirehardterm();
    bool _softterm = AA->requiresoftterm();

    // Setup variables for the loop
    Line* _line = lin;
    std::string text;

    // Build the text from the lines

    while(_line)
    {
        bool _hterm = true;
        if(_line->isheader())
        {
            text += _line->txt;
            while(_line->next and _line->iswrapped())
            {
                _line = _line->next;
                text += _line->txt;
            }
        }
        else
        {
            text += XlatStr(_line->txt.c_str(), _xlat_level, _xlat_table);

            if(!text.empty() and not _line->ishard())
            {
                if(_line->next)
                {
                    if(text[text.size() - 1] != ' ' and _line->next->txt[0] != ' ')
                    {
                        if(_softterm or not _hardterm)
                            text += ' ';
                    }
                    if (not _hardterm)
                    {
                        if (_softterm and not WideDispsoftcr)
                            text += SOFTCR;
                        _hterm = false;
                    }
                }
            }
        }

        if(_hterm)
        {
            text += CR;
            if(_lfterm)
                text += LF;
        }

        _line = _line->next;
    }

    // Allocate the text
    txt = (char*)throw_realloc(txt, text.size() + 1);
    strcpy(txt, text.c_str());
}


//  ------------------------------------------------------------------
