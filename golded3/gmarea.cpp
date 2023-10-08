
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
//  Area functions.
//  ------------------------------------------------------------------

#include <golded.h>


//  ------------------------------------------------------------------

int SearchTaglist(Echo* taglist, char* tag)
{

    int n = 0;

    while(*taglist[n])
    {
        if(strieql(taglist[n], tag))
            break;
        n++;
    }
    return(n);
}


//  ------------------------------------------------------------------
//  Update exportlist scanning file

void WriteNoDupes(const char* file, const char* line)
{

    gfile fp;
    Path buf;

    bool found = false;
    long tries = 0;

    do
    {
        fp.Fopen(file, "at+", SH_DENYRW);

        if (!fp.isopen())
        {
            if((errno != EACCES) or (PopupLocked(++tries, false, file) == false))
            {
                LOG.ErrOpen();
                LOG.printf("! A semaphore file could not be opened.");
                LOG.printf(": %s", file);
                LOG.ErrOSInfo();
                OpenErrorExit();
            }
        }
    }
    while(!fp.isopen());

    if (tries)
        PopupLocked(0, 0, NULL);

    fp.FseekSet(0);
    while (fp.Fgets(buf, sizeof(buf)))
    {
        if(strieql(strtrim(buf), line))
        {
            found = true;
            break;
        }
    }

    if (not found)
    {
        fp.Fseek(0, SEEK_END);
        fp.Printf("%s\n", line);
    }
}


//  ------------------------------------------------------------------

void FreqWaZOO(const char* files, const Addr& dest, const Attr& attr)
{

    char* buf = throw_strdup(files);
    Path filename, outbound, tmp;

    StripBackslash(strcpy(outbound, CFG->outboundpath));
    strcpy(filename, outbound);

    if(dest.zone != CFG->aka[0].addr.zone)
    {
        sprintf(tmp, ".%03x", dest.zone);
        strcat(filename, tmp);
        if(not is_dir(filename))
            mkdir(filename, S_IWUSR);
    }

    AddBackslash(filename);
    sprintf(tmp, "%04x%04x", dest.net, dest.node);
    strcat(filename, tmp);

    if(dest.point)
    {
        strcat(filename, ".pnt");
        if(not is_dir(filename))
            mkdir(filename, S_IWUSR);
        AddBackslash(filename);
        sprintf(tmp, "%08x", dest.point);
        strcat(filename, tmp);
    }

    strcpy(tmp, filename);

    // filename now contains everything but the extension, and we are sure
    // that the directory exists

    strcat(tmp, ".req");

    int i = 0;
    while(buf[i])
    {
        if(buf[i] == ' ')
        {
            if(buf[i+1] != '!' and buf[i+1] != '$')
                buf[i] = '\n';
        }
        i++;
    }

    gfile fcs(tmp, "at");
    if (fcs.isopen())
    {
        fcs.Printf("%s\n", buf);
        fcs.Fclose();
    }

    strcpy(tmp, filename);

    char m;
    if(attr.imm())
        m = 'i';
    else if(attr.cra())
        m = 'c';
    else if(attr.dir())
        m = 'd';
    else if(attr.hld())
        m = 'h';
    else
        m = 'f';

    char buf2[5];
    sprintf(buf2, ".%clo", m);
    strcat(tmp, buf2);

    if(*tmp)
        TouchFile(tmp);

    throw_free(buf);
}


//  ------------------------------------------------------------------

void RenumberArea()
{

    if(not AA->Renumber())
    {
        HandleGEvent(EVTT_JOBFAILED);
        update_statusline(LNG->NoRenum);
        waitkeyt(5000);
        reader_keyok = YES;
    }
    else
    {

        AA->Mark.ResetAll();
        AA->Expo.ResetAll();

        // Touch the netmail rescan semaphore
        if(AA->isnet())
            TouchFile(AddPath(CFG->areapath, CFG->semaphore.netscan));

        // Tell user we are finished
        update_statuslinef("%u %s", "", AA->Msgn.Count(), LNG->Renumbered);
        waitkeyt(5000);
    }
}

//  ------------------------------------------------------------------

GMsg::GMsg()
    : you_and_I(0)
    , fwdorig()
    , fwddest()
    , i51(false)
    , charsetlevel(0)
    , charsetencoding(0)
    , tzutc(0)
    , lin(NULL)
    , line(NULL)
    , lines(0)
    , quotepct(0)
    , foundwhere(0)
    , foundtopline(0)
    , orig_timesread(0)
    , messageid(NULL)
    , inreplyto(NULL)
    , references(NULL)
    , areakludgeid(NULL)
{
    gmsg::reset();
    iorig[0] = NUL;
    idest[0] = NUL;
    ireplyto[0] = NUL;
    iaddr[0] = NUL;
    igate[0] = NUL;
    ifrom[0] = NUL;
    ito[0] = NUL;
    icc[0] = NUL;
    ibcc[0] = NUL;
    organization[0] = NUL;
    realby[0] = NUL;
    realto[0] = NUL;
    pseudoto[0] = NUL;
    pseudofrom[0] = NUL;
    fwdfrom[0] = NUL;
    fwdto[0] = NUL;
    fwdsubj[0] = NUL;
    fwdarea[0] = NUL;
    fwdmsgid[0] = NUL;
    charset[0] = NUL;
    tagline[0] = NUL;
    tearline[0] = NUL;
    origin[0] = NUL;
}

//  ------------------------------------------------------------------

char* GMsg::By()
{
    return *realby ? realby : by;
}

//  ------------------------------------------------------------------

char* GMsg::To()
{
    return *realto ? realto : to;
}

//  ------------------------------------------------------------------

bool GMsg::to_me()
{
    return make_bool(you_and_I & TO_ME );
}

//  ------------------------------------------------------------------

bool GMsg::to_all()
{
    return make_bool(you_and_I & TO_ALL);
}

//  ------------------------------------------------------------------

bool GMsg::by_me()
{
    return make_bool(you_and_I & BY_ME );
}

//  ------------------------------------------------------------------

bool GMsg::to_you()
{
    return make_bool(you_and_I & TO_YOU);
}

//  ------------------------------------------------------------------

bool GMsg::by_you()
{
    return make_bool(you_and_I & BY_YOU);
}

//  ------------------------------------------------------------------

void GMsg::Reset()
{
    iorig[0] = 0;
    idest[0] = 0;
    ireplyto[0] = 0;
    iaddr[0] = 0;
    igate[0] = 0;
    ifrom[0] = 0;
    ito[0] = 0;
    icc[0] = 0;
    ibcc[0] = 0;
    organization[0] = 0;
    realby[0] = 0;
    realto[0] = 0;
    pseudoto[0] = 0;
    pseudofrom[0] = 0;
    you_and_I = 0;
    fwdfrom[0] = 0;
    fwdorig.reset();
    fwdto[0] = 0;
    fwddest.reset();
    fwdsubj[0] = 0;
    fwdarea[0] = 0;
    fwdmsgid[0] = 0;
    i51 = false;
    charset[0] = 0;
    charsetlevel = 0;
    charsetencoding = 0;
    tzutc = 0;
    tagline[0] = 0;
    tearline[0] = 0;
    origin[0] = 0;
    lines = 0;
    quotepct = 0;
    foundwhere = 0;
    foundtopline = 0;
    orig_timesread = 0;
    areakludgeid = NULL;
    throw_xrelease(references);
    throw_xrelease(inreplyto);
    throw_xrelease(messageid);
    throw_xrelease(line);
    Line* ln = lin;
    lin = NULL;
    while(ln)
    {
        Line* nextline = ln->next;
        throw_delete(ln);
        ln = nextline;
    }
    gmsg::reset();
}

//  ------------------------------------------------------------------

void Area::Open()
{

    if(not adat)
    {
//    adat = (AreaData*)throw_calloc(1, sizeof(AreaData));
        adat = new AreaData;
        InitData();
    }

    area->Msgn = &Msgn;
    area->PMrk = &PMrk;

    area->open();

    isscanned = true;
    UpdateAreadata();
}


//  ------------------------------------------------------------------

void Area::Close()
{

    if(isreadpm)
    {
        set_lastread(Msgn.ToReln(lastreadentry()));
        isreadpm = false;
    }
    PMrk.ResetAll();

    isreadmark = false;

    area->close();

    UpdateAreadata();

//  throw_release(adat);
    delete adat;
    adat = 0;
}


//  ------------------------------------------------------------------

void Area::Scan()
{

    if(cmdlinedebughg)
        LOG.printf("- Scan: %s", echoid());

    scan();

    isscanned = true;
    UpdateAreadata();
}


//  ------------------------------------------------------------------

void Area::SaveMsg(int mode, GMsg* msg)
{

    if(CFG->switches.get(frqwazoo) and msg->attr.frq())
    {
        if(not (mode & GMSG_NOLSTUPD))
            FreqWaZOO(msg->re, msg->dest, msg->attr);
        if(CFG->frqoptions & FREQ_NOWAZOOMSG)
            return;
        msg->attr.frq0();
    }

    if(isinternet() or isecho())             // Adjust fields for compatibility
    {
        if(*msg->realby)
            strcpy(msg->by, msg->realby);
        if(*msg->realto and not isnet())
            strcpy(msg->to, msg->realto);
    }

    // Translate softcr to configured char
    if (adat->usesoftcrxlat && EDIT->SoftCrXlat())
    {
        strchg(msg->by, SOFTCR, EDIT->SoftCrXlat());
        strchg(msg->to, SOFTCR, EDIT->SoftCrXlat());
        strchg(msg->realby, SOFTCR, EDIT->SoftCrXlat());
        strchg(msg->realto, SOFTCR, EDIT->SoftCrXlat());
        if(not (msg->attr.frq() or msg->attr.att() or msg->attr.urq()))
            strchg(msg->re, SOFTCR, EDIT->SoftCrXlat());
        strchg(msg->txt, SOFTCR, EDIT->SoftCrXlat());
    }
    area->save_msg(mode, msg);

    if(not (mode & GMSG_NOLSTUPD) or msg->attr.uns())
    {
        UpdateAreadata();
        if(msg->attr.uns())
        {
            Path file, line;

            if(islocal())
            {
                errorlevel |= EXIT_LOCAL;
                locpost++;
            }
            else if(isnet())
            {
                errorlevel |= EXIT_NET;
                netpost++;
            }
            else
            {
                errorlevel |= EXIT_ECHO;
                echopost++;
            }

            if ((basetype() == "JAM") && (isecho() or isnet()))
            {
                Path p;

                snprintf(file, sizeof(file), "%s%smail.jam", CFG->jampath, isecho() ? "echo" : "net");
                snprintf(line, sizeof(line), "%s %u", ReMapPath(strcpy(p, path())), msg->msgno);
                WriteNoDupes(file, line);
            }
            if(isqwk())
            {
                strcpy(file, AddPath(CFG->goldpath, "goldqwk.lst"));
                sprintf(line, "%s %u", echoid(), msg->msgno);
                WriteNoDupes(file, line);

            }
            else if(isinternet())
            {
                strcpy(file, AddPath(CFG->goldpath, "goldsoup.lst"));
                sprintf(line, "%s %u", echoid(), msg->msgno);
                WriteNoDupes(file, line);
            }
            else
            {
                if(not strblank(CFG->semaphore.exportlist))
                {
                    strcpy(file, AddPath(CFG->areapath, CFG->semaphore.exportlist));
                    sprintf(line, "%s", echoid());
                    WriteNoDupes(file, line);
                }
            }
        }
    }
}


//  ------------------------------------------------------------------

void HudsSizewarn()
{

    whelpcat(H_EWarnMsgtxt);
    call_help();
    whelpcat(H_General);
}


//  ------------------------------------------------------------------

void HGWarnRebuild()
{

    whelpcat(H_EQbaseRebuild);
    call_help();
}


//  ------------------------------------------------------------------

void FidoRenumberProgress(const char* s)
{

    update_statusline(s);
}


//  ------------------------------------------------------------------

