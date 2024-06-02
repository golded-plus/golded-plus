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
//  Reader secondary functions
//  ------------------------------------------------------------------

#include <vector>
#include <algorithm>
#include <golded.h>
#include <gwinput.h>
#include <gftnall.h>
#define PROTOTYPES
#include <uudeview.h>
//  ------------------------------------------------------------------
//  Global data from GEREAD
extern GMsg * reader_msg;
extern bool reader_msglistfirst;
//  ------------------------------------------------------------------
void ChangeAttributes()
{
    if(reader_msg->msgno)
    {
        // Reload if hexdumped
        if(AA->attr().hex())
        {
            AA->attr().hex0();
            AA->LoadMsg(reader_msg,
                        reader_msg->msgno,
                        CFG->dispmargin - (int)CFG->switches.get(disppagebar));
        }

        GMsg * msg = new GMsg(*reader_msg);
        throw_new(msg);
        AskAttributes(reader_msg);

        if(memcmp(msg, reader_msg, sizeof(GMsg)))
        {
            msg->charsetlevel = LoadCharset(CFG->xlatlocalset, msg->charset);
            DoKludges(MODE_CHANGE, reader_msg, GKLUD_FLAGS);
            reader_msg->LinesToText();
            AA->SaveMsg(GMSG_UPDATE, reader_msg);
        }

        throw_delete(msg);
    }
}

//  ------------------------------------------------------------------
void DecMargin()
{
    if(CFG->dispmargin > (MAXCOL / 3))
    {
        CFG->dispmargin--;
    }
    else
    {
        SayBibi();
        reader_keyok = YES;
    }
}

//  ------------------------------------------------------------------
void IncMargin()
{
    if(CFG->dispmargin < MAXCOL)
    {
        CFG->dispmargin++;
    }
    else
    {
        SayBibi();
        reader_keyok = YES;
    }
}

//  ------------------------------------------------------------------
void ToggleMark()
{
    uint tmp = AA->Mark.Find(reader_msg->msgno);

    if(tmp)
    {
        AA->Mark.DelReln(tmp);
    }
    else
    {
        AA->Mark.Add(reader_msg->msgno);
    }

    HeaderView->Use(AA, reader_msg);
    HeaderView->Paint();
    reader_keyok = YES;
}

//  ------------------------------------------------------------------
void ToggleBookMark()
{
    if(AA->bookmark == reader_msg->msgno)
    {
        AA->bookmark = 0;
    }
    else
    {
        AA->bookmark = reader_msg->msgno;
    }

    HeaderView->Use(AA, reader_msg);
    HeaderView->Paint();
    reader_keyok = YES;
}

//  ------------------------------------------------------------------
void ToggleHiddKlud()
{
    AA->ToggleViewhidden();
    AA->ToggleViewkludge();
    MsgLineReIndex(reader_msg);
    reader_topline = 0;
    reader_keyok   = YES;
}

//  ------------------------------------------------------------------
void ToggleKludge()
{
    AA->ToggleViewkludge();
    MsgLineReIndex(reader_msg);
    reader_topline = 0;
    reader_keyok   = YES;
}

//  ------------------------------------------------------------------
void ToggleHidden()
{
    AA->ToggleViewhidden();
    MsgLineReIndex(reader_msg);
    reader_topline = 0;
    reader_keyok   = YES;
}

//  ------------------------------------------------------------------
void ToggleStripHTML()
{
    AA->ToggleStripHTML();
    MsgLineReIndex(reader_msg);
    reader_topline = 0;
}

//  ------------------------------------------------------------------
void ToggleQuote()
{
    AA->ToggleViewquote();
    MsgLineReIndex(reader_msg);
    reader_topline = 0;
    reader_keyok   = YES;
}

//  ------------------------------------------------------------------
void ToggleROT13()
{
    if(AA->Msgn.Count())
    {
        reader_msg->attr.rotX();
        Rot13(reader_msg);
        reader_keyok = YES;
    }
}

//  ------------------------------------------------------------------
void TogglePageBar()
{
    CFG->switches.set(disppagebar, not CFG->switches.get(disppagebar));
}

//  ------------------------------------------------------------------
void ToggleRealMsgno()
{
    CFG->switches.set(disprealmsgno, not CFG->switches.get(disprealmsgno));
    HeaderView->Use(AA, reader_msg);
    HeaderView->Paint();
    reader_keyok = YES;
}

//  ------------------------------------------------------------------
void ToggleTwits()
{
    gkey k;

    do
    {
        switch(AA->Twitmode())
        {
            case TWIT_SHOW:
                AA->SetTwitmode(TWIT_BLANK);
                w_info(LNG->TwitBlanked);
                break;

            case TWIT_BLANK:
                AA->SetTwitmode(TWIT_SKIP);
                w_info(LNG->TwitSkipped);
                break;

            case TWIT_SKIP:
                AA->SetTwitmode(TWIT_IGNORE);
                w_info(LNG->TwitIgnoreSkip);
                break;

            case TWIT_IGNORE:
                AA->SetTwitmode(TWIT_KILL);
                w_info(LNG->TwitKilled);
                break;

            case TWIT_KILL:
                AA->SetTwitmode(TWIT_SHOW);
                w_info(LNG->TwitDisplayed);
                break;
        } // switch
        k = waitkeyt(3000);
    }
    while(k and k != Key_Ent and k != Key_Esc);
    w_info(NULL);

    if(k == Key_Esc)
    {
        AA->SetTwitmode(CFG->twitmode);
    }
    else
    {
        CFG->twitmode = AA->Twitmode();
    }
} // ToggleTwits

//  ------------------------------------------------------------------
void ToggleStyles()
{
    gkey k;
    int setting = AA->adat->usestylies + AA->adat->hidestylies * 2;

    do
    {
        switch(setting)
        {
            case 2:
            case 0:
                w_info(LNG->StylecodesYes);
                setting = 1;
                break;

            case 1:
                w_info(LNG->StylecodesHide);
                setting = 3;
                break;

            case 3:
                w_info(LNG->StylecodesNo);
                setting = 0;
                break;
        }
        k = waitkeyt(3000);
    }
    while(k and k != Key_Ent and k != Key_Esc);
    w_info(NULL);

    if(k != Key_Esc)
    {
        AA->adat->usestylies  = make_bool(setting & 1);
        AA->adat->hidestylies = make_bool(setting & 2);
    }
} // ToggleStyles

//  ------------------------------------------------------------------
void GotoMsgno()
{
    if(AA->Msgn.Count())
    {
        // Set the active window
        int lastwh = whandle();
        int inhdr  = AA->Msglistheader() or not _in_msglist;

        if(reader_keycode < KK_Macro)
        {
            if(gkbd.kbuf == NULL)
            {
                kbput(reader_keycode); // Put back the key
            }
        }

        char buf[150];
        char * ptr         = buf;
        int list_max       = reader_msg->link.list_max();
        uint32_t * replies = (uint32_t *)throw_calloc(list_max + 1, sizeof(uint32_t));
        uint32_t replyto, replynext;

        if(CFG->switches.get(disprealmsgno))
        {
            ptr += sprintf(ptr,
                           " %-5.5s:       %s %u",
                           LNG->MsgReal,
                           LNG->of,
                           AA->Msgn.Count());
            replyto    = reader_msg->link.to();
            replies[0] = reader_msg->link.first();
            replynext  = reader_msg->link.next();

            for(int replyn = 1; replyn < list_max + 1; replyn++)
            {
                replies[replyn] = reader_msg->link.list(replyn - 1);
            }
        }
        else
        {
            ptr += sprintf(ptr,
                           " %-5.5s:       %s %u",
                           LNG->Msg,
                           LNG->of,
                           AA->Msgn.Count());
            replyto    = AA->Msgn.ToReln(reader_msg->link.to());
            replies[0] = AA->Msgn.ToReln(reader_msg->link.first());
            replynext  = AA->Msgn.ToReln(reader_msg->link.next());

            for(int replyn = 1; replyn < list_max + 1; replyn++)
            {
                replies[replyn] = AA->Msgn.ToReln(reader_msg->link.list(replyn - 1));
            }
        }

        if(replyto)
        {
            ptr += sprintf(ptr, " -%u", replyto);
        }

        for(int replyn = 0, plus = 0; replyn < list_max + 1; replyn++)
        {
            if(replies[replyn])
            {
                ptr += sprintf(ptr, " %s%u", (plus++ ? "" : "+"), replies[replyn]);
            }
        }

        if(replynext)
        {
            sprintf(ptr, " *%u", replynext);
        }

        if(inhdr)
        {
            HeaderView->window.activate_quick();
        }
        else
        {
            wopen_(0, 0, 3, strlen(buf) + 3, W_BHEAD, C_HEADB, C_HEADW);
            w_shadow();
            wtitle(LNG->EnterMsgno, TCENTER, C_HEADT);
        }

        int wrow = inhdr ? 1 : 0;
        wprints(wrow, 0, C_HEADW, buf);
        *buf = NUL;
        gwindow iwindow(whandle());
        std::string fbuf = buf;
        gwinput2 iform(iwindow);
        iform.setup(C_HEADW, C_HEADW, C_HEADI, _box_table(W_BHEAD, 13), true);
        iform.add_field(0, wrow, 8, 5, fbuf, 20, gwinput::cvt_none, gwinput::entry_new);
        vcurshow();
        iform.run(H_InputMsgno);
        vcurhide();

        if(iform.dropped)
        {
            fbuf = "";
        }

        long active = AA->Msgn.Count();
        long msgno  = atol(fbuf.c_str());

        if(CFG->switches.get(disprealmsgno))
        {
            if(msgno > 0)
            {
                msgno = AA->Msgn.ToReln(msgno);
            }
        }

        if((msgno < 0) and ((active + msgno) > 0))
        {
            msgno = active + msgno;
        }

        if(RngV(msgno, 1L, active))
        {
            AA->set_lastread((uint)msgno);
        }

        if(inhdr)
        {
            wactiv_(lastwh);
        }
        else
        {
            wclose();
        }

        throw_free(replies);
    }
} // GotoMsgno

//  ------------------------------------------------------------------
void PrevArea()
{
    AA->attr().hex0();
    AA->Close();
    int currarea = AL.AreaIdToNo(CurrArea);
    int testarea = currarea;

    while(testarea > 0)
    {
        testarea--;

        if(not AL[testarea]->isseparator())
        {
            currarea = testarea;
            break;
        }
    }

    if(currarea == testarea)
    {
        AL.SetActiveAreaNo(currarea);
        OrigArea = CurrArea;
    }
    else
    {
        HandleGEvent(EVTT_ENDOFMSGS);
    }

    AA->Open();
    AA->RandomizeData();
    AA->SetBookmark(AA->lastread());

    if(CFG->disppmfirst && AA->PMrk.Tags())
    {
        AA->isreadpm = false;
        ToggleMarkRead();
    }
} // PrevArea

//  ------------------------------------------------------------------
void NextArea()
{
    AA->attr().hex0();
    AA->Close();
    uint currarea = AL.AreaIdToNo(CurrArea);
    uint testarea = currarea;

    while(testarea < (AL.size() - 1))
    {
        testarea++;

        if(not AL[testarea]->isseparator())
        {
            currarea = testarea;
            break;
        }
    }

    if(currarea == testarea)
    {
        AL.SetActiveAreaNo(currarea);
        OrigArea = CurrArea;
    }
    else
    {
        HandleGEvent(EVTT_ENDOFMSGS);
    }

    AA->Open();
    AA->RandomizeData();
    AA->SetBookmark(AA->lastread());

    if(CFG->disppmfirst && AA->PMrk.Tags())
    {
        AA->isreadpm = false;
        ToggleMarkRead();
    }
} // NextArea

//  ------------------------------------------------------------------
void QuitNow()
{
    AA->attr().hex0();
    reader_finished = YES;
    reader_done     = YES;
}

//  ------------------------------------------------------------------
void ExitAsk()
{
    AA->attr().hex0();
    GMenuQuit MenuQuit;
    reader_done = reader_finished = MenuQuit.Run();
}

//  ------------------------------------------------------------------
void NewArea(bool jumpnext)
{
    AA->attr().hex0();

    if(AA->isopen())
    {
        AA->Close();
    }

    int newarea = CurrArea;

    if(jumpnext)
    {
        uint idx  = AL.AreaIdToNo(CurrArea);
        uint oidx = idx;

        do
        {
            idx++;

            // Wrap around
            if(idx >= AL.size())
            {
                idx = 0;
            }

            // Exit loop if no area with new mail found
            if(idx == oidx)
            {
                newarea = AreaPick(LNG->NewArea, 0, &newarea);

                if(newarea != -1)
                {
                    idx = AL.AreaIdToNo(newarea);
                }

                break;
            }
        }
        while(AL.AreaNoToPtr(idx)->unread == 0);
        CurrArea = AL.AreaNoToId(idx);
    }
    else
    {
        newarea = AreaPick(LNG->NewArea, 0, &newarea);

        if(newarea != -1)
        {
            CurrArea = newarea;
        }
    }

    AL.Sort();
    reader_done = YES;

    if(gkbd.quitall)
    {
        QuitNow();
    }
    else
    {
        AL.SetActiveAreaId(CurrArea);
        OrigArea = CurrArea;
        strcpy(stecho, AA->echoid());
        AA->RandomizeData();

        if(AA->Msglistfirst() && (kbput(KK_ReadMessageList) != -1))
        {
            reader_msglistfirst = true;
        }
        else
        {
            reader_msglistfirst = false;
        }
    }
} // NewArea

//  ------------------------------------------------------------------
void DosShell()
{
    char * shell = getenv(GOLD_SHELL_ENV);

    if(shell && *shell)
    {
        ShellToDos(shell, LNG->DOS_Shell, LGREY_ | _BLACK, YES);
    }
}

//  ------------------------------------------------------------------
void TouchNetscan(int popup)
{
    gfile fp(AddPath(CFG->areapath, CFG->semaphore.netscan), "wb", CFG->sharemode);

    if(fp.isopen())
    {
        if(popup)
        {
            w_infof(" %s ", CFG->semaphore.netscan);
            HandleGEvent(EVTT_JOBDONE);
            waitkeyt(10000);
            w_info(NULL);
        }
    }
}

//  ------------------------------------------------------------------
int ExternUtil(GMsg * msg, const ExtUtil & extutil)
{
    Path editorfile, tmpfile, buf;
    std::string cmdline = extutil.cmdline;
    int mode            = (extutil.options &
                           EXTUTIL_KEEPCTRL) ? MODE_SAVE : MODE_SAVENOCTRL;

    if(striinc("@tmpfile", cmdline.c_str()))
    {
        if((*CFG->temppath == NUL) or is_dir(CFG->temppath))
        {
            mktemp(strcpy(tmpfile, AddPath(CFG->temppath, "GDXXXXXX")));
            SaveLines(mode, tmpfile, msg, 79);
            strcpy(buf, tmpfile);
            strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
            strischg(cmdline, "@tmpfile", buf);
        }
        else
        {
            w_info(LNG->TMPinvalidpath);
            SayBibi();
            waitkeyt(10000);
            w_info("");
            return NO;
        }
    }
    else
    {
        tmpfile[0] = NUL;
    }

    strxcpy(editorfile, AddPath(CFG->goldpath, EDIT->File()), sizeof(Path));
    SaveLines(mode, editorfile, msg, 79);
    strcpy(buf, editorfile);
    strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
    strischg(cmdline, "@file", buf);
    strcpy(buf, CFG->goldpath);
    strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
    strischg(cmdline, "@path", buf);
    TokenXlat(MODE_NEW, cmdline, msg, msg, CurrArea);
    int pauseval = 0;

    if(extutil.options & EXTUTIL_PAUSEONERROR)
    {
        pauseval = -1;
    }

    if(extutil.options & EXTUTIL_PAUSE)
    {
        pauseval = 1;
    }

    ShellToDos(cmdline.c_str(),
               "",
               extutil.options & EXTUTIL_CLS ? LGREY_ | _BLACK : BLACK_ | _BLACK,
               extutil.options & EXTUTIL_CURSOR,
               pauseval);

    if(extutil.options & EXTUTIL_RELOAD)
    {
        if(not (extutil.options & EXTUTIL_KEEPCTRL))
        {
            if(*msg->tearline or * msg->origin)
            {
                gfile fp(editorfile, "at");

                if(fp.isopen())
                {
                    if(*msg->tearline)
                    {
                        fp.Printf("--- %s\n", msg->tearline);
                    }

                    if(*msg->origin)
                    {
                        fp.Printf(" * Origin: %s\n", msg->origin);
                    }
                }
            }
        }

        bool hardlines = EDIT->HardLines();
        EDIT->HardLines(false);
        LoadText(msg, editorfile);
        EDIT->HardLines(hardlines);

        if(not strstr(msg->txt, "\001CHRS:") and not strstr(msg->txt, "\001CHARSET:"))
        {
            // Prepend "@CHRS: XLATLOCALSET 2\n" to force proper encoding
            const char * charset = AA->Viewkludge() ? msg->charset : CFG->xlatlocalset;
            char * msg_txt_chrs  = (char *)throw_malloc(strlen(msg->txt) +
                                                        strlen(charset) +
                                                        sizeof("\001CHRS:  2\r") + 16);
            sprintf(msg_txt_chrs, "\001CHRS: %s 2\r%s", charset, msg->txt);
            throw_free(msg->txt);
            msg->txt = msg_txt_chrs;
        }

        // Ignore any kludge address found
        msg->TextToLines(CFG->dispmargin - (int)CFG->switches.get(disppagebar), false);

        if(AA->Viewkludge())
        {
            // Try to convert to the original codepage
            char * inpcharset = msg->charset;
            int level         = LoadCharset(CFG->xlatlocalset,
                                            IsQuotedPrintable(inpcharset) ?
                                            ExtractPlainCharset(inpcharset) :
                                            inpcharset);

            if(level)
            {
                // do recode
                const std::string convTxt = XlatStr(msg->txt, level, CharTable);
                throw_free(msg->txt);
                msg->txt = (char *)throw_malloc(convTxt.size() + 16);
                strcpy(msg->txt, convTxt.c_str());
            }
        }

        msg->charsetlevel = LoadCharset(msg->charset, CFG->xlatlocalset);
    }

    if(extutil.options & EXTUTIL_WIPE)
    {
        WipeFile(editorfile, 0);
    }

    if(tmpfile[0] != NUL)
    {
        WipeFile(tmpfile, 0);
    }

    return YES;
} // ExternUtil

//  ------------------------------------------------------------------
int ExternUtil(GMsg * msg, uint32_t utilno)
{
    std::vector<ExtUtil>::iterator it  = CFG->externutil.begin();
    std::vector<ExtUtil>::iterator end = CFG->externutil.end();

    for( ; it != end; it++)
    {
        if(it->utilno != utilno)
        {
            continue;
        }

        return ExternUtil(msg, *it);
    }
    SayBibi();
    return YES;
}

//  ------------------------------------------------------------------
void ExternUtilMenu(GMsg * msg)
{
    static int startat        = 0;
    static char cmdline[1024] = "";
    gstrarray Listi;
    Listi.push_back(LNG->EnterCmdLine);
    std::vector<ExtUtil>::iterator it  = CFG->externutil.begin();
    std::vector<ExtUtil>::iterator end = CFG->externutil.end();

    for( ; it != end; it++)
    {
        char buf[100];
        sprintf(buf, " %02d %.59s%s ", it->utilno, it->cmdline.c_str(),
                (it->cmdline.length() > 59) ? ">" : "");
        Listi.push_back(buf);
    }
    size_t n = MinV(Listi.size(), (MAXROW - 10));
    set_title(LNG->ExternUtil, TCENTER, C_ASKT);
    update_statusline(LNG->SelectExternUtil);
    whelppcat(H_ReadExternUtil);
    n = wpickstr(6,
                 0,
                 6 + n + 1,
                 -1,
                 W_BASK,
                 C_ASKB,
                 C_ASKW,
                 C_ASKS,
                 Listi,
                 startat,
                 title_shadow);
    whelpop();

    if(n != -1)
    {
        if(n == 0)
        {
            if(edit_string(cmdline, sizeof(cmdline), LNG->ExecCmdLine, H_ReadExternUtil))
            {
                ExtUtil extutil;
                extutil.utilno  = 0;
                extutil.cmdline = cmdline;
                extutil.options = CFG->externoptions;
                reader_keyok    = ExternUtil(msg, extutil);
            }
        }
        else
        {
            reader_keyok = ExternUtil(msg, CFG->externutil[n - 1]);
        }
    }
} // ExternUtilMenu

//  ------------------------------------------------------------------
static bool PeekURLCmp(const std::string & a, const std::string b)
{
    return stricmp(a.c_str(), b.c_str()) < 0;
}

//  ------------------------------------------------------------------
void ReadPeekURLs(GMsg * msg)
{
    if(CFG->urlhandler.empty())
    {
        w_info(LNG->PeekInfoNoHandler);
        waitkeyt(10000);
        w_info(NULL);
        return;
    }

    w_info(LNG->Wait);
    // Scan the current msg for urls
    int tline = reader_topline;

    if(CFG->peekurloptions & PEEK_FROMTOP)
    {
        tline = 0;
    }

    Line ** lin = msg->line;
    std::vector<char *> urls;

    for(int n = tline; n < msg->lines; n++)
    {
        const char * ptr = lin[n]->txt.c_str();

        while(*ptr)
        {
            const char * urlBegin = url_begin(ptr);

            if(urlBegin and not strneql(ptr, "mailto:", 7))
            {
                const char * urlEnd = urlBegin + strcspn(urlBegin, " \t\"\'<>()[]");

                if(ispunct(urlEnd[-1]) and (urlEnd[-1] != '/'))
                {
                    --urlEnd;
                }

                if(urlBegin < urlEnd)
                {
                    char * bufurl = (char *)throw_malloc(urlEnd - ptr + 3);
                    bufurl[0] = ' ';
                    strxcpy(bufurl + 1, ptr, (urlEnd - ptr) + 1);
                    strcat(bufurl, " ");
                    std::vector<char *>::iterator it  = urls.begin();
                    std::vector<char *>::iterator end = urls.end();

                    for( ; it != end; it++)
                    {
                        if(strieql(*it, bufurl))
                        {
                            break;
                        }
                    }

                    if(it == end)
                    {
                        urls.push_back(bufurl);
                    }
                    else
                    {
                        throw_free(bufurl);
                    }

                    ptr = urlEnd - 1;
                }
            }

            ptr++;
        }
    }
    w_info(NULL);

    if(not urls.empty())
    {
        gstrarray Listi;
        std::vector<char *>::iterator it  = urls.begin();
        std::vector<char *>::iterator end = urls.end();

        for( ; it != end; it++)
        {
            Listi.push_back(*it);
        }

        // Sort list if requested
        if(CFG->peekurloptions & FREQ_SORT)
        {
            std::sort(Listi.begin(), Listi.end(), PeekURLCmp);
        }

        // Run the picker
        size_t n = MinV(urls.size(), (MAXROW - 10));
        set_title(LNG->PeekURLMenuTitle, TCENTER, C_ASKT);
        update_statusline(LNG->PeekURLStat);
        whelppcat(H_PeekURL);
        n = wpickstr(6,
                     0,
                     6 + n + 1,
                     -1,
                     W_BASK,
                     C_ASKB,
                     C_ASKW,
                     C_ASKS,
                     Listi,
                     0,
                     title_shadow);
        whelpop();

        if(n != -1)
        {
            std::vector<UrlHandler>::iterator it  = CFG->urlhandler.begin();
            std::vector<UrlHandler>::iterator end = CFG->urlhandler.end();
#if 0   // will be implemented later

            for( ; it != end; it++)
            {
                if(it->scheme.match(Listi[n].c_str(), gregex::extended | gregex::icase))
                {
                    break;
                }
            }
#else
            it = end;
#endif

            if(it == end)
            {
                it = CFG->urlhandler.begin();
            }

            std::string cmdline = it->handler.cmdline;
            std::string buf     = "\"" + strtrim(strltrim(Listi[n])) + "\"";
            strischg(cmdline, "@url", buf.c_str());
            buf = CFG->goldpath;
            strchg(buf, GOLD_WRONG_SLASH_CHR, GOLD_SLASH_CHR);
            strischg(cmdline, "@path", buf.c_str());
            TokenXlat(MODE_NEW, cmdline, msg, msg, CurrArea);
            int pauseval = 0;

            if(it->handler.options & EXTUTIL_PAUSEONERROR)
            {
                pauseval = -1;
            }

            if(it->handler.options & EXTUTIL_PAUSE)
            {
                pauseval = 1;
            }

            ShellToDos(cmdline.c_str(),
                       "",
                       it->handler.options & EXTUTIL_CLS ? LGREY_ | _BLACK : BLACK_ |
                       _BLACK,
                       it->handler.options & EXTUTIL_CURSOR,
                       pauseval);
        }
    }
    else
    {
        w_info(LNG->PeekInfoNoURLs);
        waitkeyt(10000);
        w_info(NULL);
    }

    while(!urls.empty())
    {
        throw_free(urls.back());
        urls.pop_back();
    }
} // ReadPeekURLs

//  ------------------------------------------------------------------
void UUDecode(GMsg * msg)
{
    Path infile, outfile;
    bool ok = false;

    if(not ((*CFG->temppath == NUL) or is_dir(CFG->temppath)))
    {
        w_info(LNG->TMPinvalidpath);
        SayBibi();
        waitkeyt(10000);
        w_info("");
        return;
    }

    if((*CFG->uudecodepath == NUL) or is_dir(CFG->uudecodepath))
    {
        mktemp(strcpy(infile, AddPath(CFG->temppath, "GDXXXXXX")));
        strcpy(outfile, CFG->uudecodepath);
        GMenuDomarks MenuDomarks;
        int source     = AA->Mark.Count() ? MenuDomarks.Run(LNG->Decode) : WRITE_CURRENT;
        bool overwrite = true;

        if(source == WRITE_QUIT)
        {
            return;
        }

        bool old_quotespacing = CFG->switches.get(quotespacing);
        CFG->switches.set(quotespacing, false);

        if(source == WRITE_MARKED)
        {
            for(uint n = 0; n < AA->Mark.Count(); n++)
            {
                if(overwrite and n)
                {
                    overwrite = false;  // Overwrite only the first time
                }

                w_progress(MODE_UPDATE, C_INFOW, n + 1, AA->Mark.Count(),
                           LNG->Preparing);
                AA->LoadMsg(msg, AA->Mark[n], 79);
                SaveLines(overwrite ? MODE_WRITE : MODE_APPEND, infile, msg, 79);
            }

            if(AA->Mark.Count())
            {
                w_progress(MODE_QUIT, BLACK_ | _BLACK, 0, 0, NULL);
            }
        }
        else if(source == WRITE_CURRENT)
        {
            AA->LoadMsg(msg, msg->msgno, 79);
            SaveLines(MODE_WRITE, infile, msg, 79);
        }

        CFG->switches.set(quotespacing, old_quotespacing);
        uulist * item;
        int i, res;
        w_info(LNG->WaitUUdecoding);
        UUInitialize();
        UUSetOption(UUOPT_SAVEPATH, 0, outfile);

        if((res = UULoadFile(infile, NULL, 0)) == UURET_OK)
        {
            for(i = 0; (item = UUGetFileListItem(i)) != NULL; i++)
            {
                if((item->state & UUFILE_OK) == 0)
                {
                    continue;
                }

                if((res = UUDecodeFile(item, NULL)) == UURET_OK)
                {
                    ok = true;
                    w_infof(LNG->CompletedUUdecode, item->filename);
                }
            }
        }

        UUCleanUp();
        remove(infile);

        if(not ok)
        {
            w_info(LNG->NotUUencoded);
        }
    }
    else
    {
        w_info(LNG->UUEinvalidpath);
    }

    if(not ok)
    {
        waitkeyt(10000);
    }

    w_info(NULL);
} // UUDecode

//  ------------------------------------------------------------------
void Make_Userlist(const char * userlist)
{
    word * crclist;
    word crc, crcs = 0;
    uint n, x;
    char userline[80], adrs[40];
    GMsg * msg = new GMsg();
    throw_new(msg);
    crclist = (word *)throw_calloc(AA->Msgn.Count() + 1, sizeof(word));
    gfile fp(userlist, "ab", CFG->sharemode);

    if(fp.isopen())
    {
        w_progress(MODE_NEW, C_INFOW, 0, AA->Msgn.Count(), LNG->GenUserlist);

        for(n = AA->Msgn.Count(); n; n--)
        {
            update_statuslinef(LNG->ReadingMsg, "ST_READINGMSG", n, AA->Msgn.Count());
            w_progress(MODE_UPDATE,
                       C_INFOW,
                       (AA->Msgn.Count() - n) + 1,
                       AA->Msgn.Count(),
                       NULL);
            AA->LoadMsg(msg, AA->Msgn.CvtReln(n), CFG->dispmargin);
            crc = strCrc16(msg->by, false);

            for(x = 0; x < crcs; x++)
            {
                if(crclist[x] == crc)
                {
                    break;  // We have already used it
                }
            }

            if(x >= crcs)
            {
                crclist[crcs++] = crc;
                strrevname(userline, msg->by);
                msg->orig.make_string(adrs);
                fp.Printf("%-36.36s%24.24s\r\n", userline, adrs);
            }
        }
        throw_free(crclist);
        w_progress(MODE_QUIT, BLACK_ | _BLACK, 0, 0, NULL);
    }

    msg->Reset();
    throw_delete(msg);
} // Make_Userlist

//  ------------------------------------------------------------------
void MakeUserList()
{
    if(AA->Msgn.Count())
    {
        uint temp = AA->lastread();

        if(not edit_pathname(CFG->userlistfile, sizeof(Path), LNG->UserlistName, 0))
        {
            return;
        }

        Make_Userlist(AddPath(CFG->goldpath, CFG->userlistfile));
        AA->set_lastread(temp);
    }
}

//  ------------------------------------------------------------------
void TouchSemaphore()
{
    Path file;
    *file = NUL;

    if(edit_string(file, sizeof(Path), LNG->TouchSemaphore, 0))
    {
        TouchFile(file);
    }
}

//  ------------------------------------------------------------------
void make_pathreport(const char * reportfile)
{
    gfile fp(reportfile, "wt");

    if(fp.isopen())
    {
        std::string path;
        ftn_addr address;
        std::vector<ftn_addr> alist;
        GMsg * msg = new GMsg();
        throw_new(msg);
        w_progress(MODE_NEW, C_INFOW, 0, AA->Msgn.Count(), "Generating PATH report");

        for(int n = AA->Msgn.Count(); n >= AA->lastread(); n--)
        {
            update_statuslinef(LNG->ReadingMsg, "ST_READINGMSG", n, AA->Msgn.Count());
            w_progress(MODE_UPDATE, C_INFOW, n, AA->Msgn.Count(), NULL);
            AA->LoadMsg(msg,
                        AA->Msgn.CvtReln(n),
                        CFG->dispmargin - (int)CFG->switches.get(disppagebar));
            address = msg->orig;
            int x;

            for(x = 0; x < alist.size(); x++)
            {
                if(alist[x] == address)
                {
                    break;  // We have already used it
                }
            }

            if(x >= alist.size())
            {
                alist.push_back(address);
                INam buf;
                strcpy(buf, msg->By());
                strchg(buf, ' ', '_');
                std::string temp;
                fp.Printf("%s  %s  ", buf, address.make_string(temp).c_str());
                path = "";
                Line * line = msg->lin;

                while(line)
                {
                    if(line->kludge == GKLUD_PATH)
                    {
                        path += line->txt.c_str() + 6;
                    }

                    line = line->next;
                }

                if(path.length())
                {
                    gstrarray links;
                    tokenize(links, path.c_str(), " ");

                    for(int i = 0; i < links.size(); i++)
                    {
                        address = links[i];
                    }
                }

                fp.Printf("%s\n", path.c_str());
            }
        }
        w_progress(MODE_QUIT, BLACK_ | _BLACK, 0, 0, NULL);
        msg->Reset();
        throw_delete(msg);
    }
} // make_pathreport

//  ------------------------------------------------------------------
void MakePathreport()
{
    if(AA->Msgn.Count())
    {
        uint temp = AA->lastread();

        if(not edit_pathname(CFG->pathreportfile, sizeof(Path), LNG->Pathreport, 0))
        {
            return;
        }

        make_pathreport(AddPath(CFG->goldpath, CFG->pathreportfile));
        AA->set_lastread(temp);
    }
}

//  ------------------------------------------------------------------
