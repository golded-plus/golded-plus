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
//  Configuration functions, part 6.
//  ------------------------------------------------------------------

#include <golded.h>
#include <gcprot.h>
//  ------------------------------------------------------------------
extern char * val;
//  ------------------------------------------------------------------
void CfgMsglistfirst()
{
    bool flag = make_bool(GetYesno(val));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_MSGLISTFIRST, flag);
    }
    else
    {
        CFG->msglistfirst = flag;
    }
}

//  ------------------------------------------------------------------
void CfgMsglistheader()
{
    bool flag = make_bool(GetYesno(val));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_MSGLISTHEADER, flag);
    }
    else
    {
        CFG->msglistheader = flag;
    }
}

//  ------------------------------------------------------------------
void CfgMsglistwidesubj()
{
    bool flag = make_bool(GetYesno(val));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_MSGLISTWIDESUBJ, flag);
    }
    else
    {
        CFG->msglistwidesubj = flag;
    }
}

//  ------------------------------------------------------------------
void CfgNamesfile()
{
    strcpy(CFG->namesfile, val);
}

//  ------------------------------------------------------------------
void CfgNickname()
{
    Name buf;
    strxcpy(buf, val, sizeof(buf));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_NICKNAME, buf, strlen(buf) + 1);
    }
    else
    {
        strcpy(CFG->nickname, buf);
    }
}

//  ------------------------------------------------------------------
void CfgNetname()
{
    Name buf;
    strxcpy(buf, StripQuotes(val), sizeof(buf));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_NETNAME, buf, strlen(buf) + 1);
    }
    else
    {
        strcpy(CFG->netname, buf);
    }
}

//  ------------------------------------------------------------------
void CfgNodelist()
{
    // Only used by GoldNODE
}

//  ------------------------------------------------------------------
void CfgNodepath()
{
    MapPath(PathCopy(CFG->nodepath, val));
}

//  ------------------------------------------------------------------
void CfgNodepathfd()
{
    MapPath(PathCopy(CFG->nodepathfd, val));
}

//  ------------------------------------------------------------------
void CfgNodepathv7()
{
    MapPath(PathCopy(CFG->nodepathv7, val));
}

//  ------------------------------------------------------------------
extern char v7nodeflags[16][9];
void CfgNodev7flags()
{
    GTok t;
    char * flagstring = t.First(val);
    char * flagvals   = t.Next();
    uint flagval;

    if(g_tolower(*flagvals) == 'b')
    {
        flagval = atoi(flagvals + 1);
    }
    else
    {
        flagval = atoi(flagvals);

        switch(flagval)
        {
            case 32:
                flagval = 5;
                break;

            case 64:
                flagval = 6;
                break;

            case 128:
                flagval = 7;
                break;

            case 256:
                flagval = 8;
                break;

            case 512:
                flagval = 9;
                break;

            case 1024:
                flagval = 10;
                break;

            case 2048:
                flagval = 11;
                break;

            case 8192:
                flagval = 13;
                break;

            case 16384:
                flagval = 14;
                break;

            default:
                flagval = 0;
        } // switch
    }

    if(flagval)
    {
        strcpy(stpcpy(v7nodeflags[flagval], flagstring), ",");
    }
} // CfgNodev7flags

//  ------------------------------------------------------------------
extern char v7modemtype[8][9];
void CfgNodev7modem()
{
    GTok t;
    char * modemstring = t.First(val);
    char * modemvals   = t.Next();
    int modemval;

    if(g_tolower(*modemvals) == 'b')
    {
        modemval = atoi(modemvals + 1);
    }
    else
    {
        modemval = atoi(modemvals);

        switch(modemval)
        {
            case 1:
                modemval = 0;
                break;

            case 2:
                modemval = 1;
                break;

            case 4:
                modemval = 2;
                break;

            case 8:
                modemval = 3;
                break;

            case 16:
                modemval = 4;
                break;

            case 32:
                modemval = 5;
                break;

            case 64:
                modemval = 6;
                break;

            case 128:
                modemval = 7;
                break;

            default:
                modemval = -1;
        } // switch
    }

    if(modemval >= 0)
    {
        strcat(stpcpy(v7modemtype[modemval], modemstring), ",");
    }
} // CfgNodev7modem

//  ------------------------------------------------------------------
void CfgOrganization()
{
    INam buf;
    strxcpy(buf, val, sizeof(buf));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_ORGANIZATION, buf, strlen(buf) + 1);
    }
    else
    {
        strcpy(CFG->organization, buf);
    }
}

//  ------------------------------------------------------------------
void CfgOrigin(const char * v)
{
    char buf[256];
    val = strxcpy(buf, v, sizeof(buf));
    CfgOrigin();
}

void CfgOrigin()
{
    if(not strblank(val))
    {
        StripQuotes(val);

        if(cfgingroup)
        {
            CFG->grp.AddItm(GRP_ORIGIN, val, strlen(val) + 1);
        }
        else
        {
            CFG->origin.push_back(val);
        }
    }
}

//  ------------------------------------------------------------------
void CfgOutboundpath()
{
    PathCopy(CFG->outboundpath, val);
}

//  ------------------------------------------------------------------
void CfgOutputfile()
{
    Path buf;
    strxcpy(buf, val, sizeof(buf));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_OUTPUTFILE, buf, strlen(buf) + 1);
    }
    else
    {
        strcpy(CFG->outputfile, buf);
    }
}

void CfgPathreportfile()
{
    PathCopy(CFG->pathreportfile, val);
}

//  ------------------------------------------------------------------
void CfgPcboardpath(const char * path, bool force)
{
    if(force or strblank(CFG->pcboardpath))
    {
        MapPath(PathCopy(CFG->pcboardpath, path));
    }
}

void CfgPcboardpath()
{
    CfgPcboardpath(val, true);
}

//  ------------------------------------------------------------------
void CfgPcboarduserno()
{
    CFG->pcboarduserno = atoi(val);
}

//  ------------------------------------------------------------------
void CfgPeekurloptions()
{
    if(striinc("SORT", val))
    {
        CFG->peekurloptions |= PEEK_SORT;
    }

    if(striinc("FROMTOP", val))
    {
        CFG->peekurloptions |= PEEK_FROMTOP;
    }

    if(striinc("NOTFROMTOP", val))
    {
        CFG->peekurloptions &= ~PEEK_FROMTOP;
    }
}

//  ------------------------------------------------------------------
void CfgPersonalmail()
{
    if(striinc("STARTUP", val))
    {
        CFG->personalmail |= PM_STARTUP;
    }

    if(striinc("NOTSTARTUP", val))
    {
        CFG->personalmail &= ~PM_STARTUP;
    }

    if(striinc("ALLNAMES", val))
    {
        CFG->personalmail |= PM_ALLNAMES;
    }

    if(striinc("NOTALLNAMES", val))
    {
        CFG->personalmail &= ~PM_ALLNAMES;
    }

    if(striinc("LISTONLY", val))
    {
        CFG->personalmail |= PM_LISTONLY;
    }

    if(striinc("NOTLISTONLY", val))
    {
        CFG->personalmail &= ~PM_LISTONLY;
    }
} // CfgPersonalmail

//  ------------------------------------------------------------------
void CfgPlay()
{
    Path buf;
    strxcpy(buf, val, sizeof(buf));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_PLAY, buf, strlen(buf) + 1);
    }
    else
    {
        FileToGPlay(&CFG->play, buf);
    }
}

//  ------------------------------------------------------------------
void CfgPrintdevice()
{
    strcpy(CFG->printdevice, val);
}

//  ------------------------------------------------------------------
void CfgPrintinit()
{
    CvtPrnstr(CFG->printinit, val);
}

//  ------------------------------------------------------------------
void CfgPrintlength()
{
    CFG->printlength = atoi(val);
}

//  ------------------------------------------------------------------
void CfgPrintmargin()
{
    CFG->printmargin = atoi(val);
}

//  ------------------------------------------------------------------
void CfgPrintreset()
{
    CvtPrnstr(CFG->printreset, val);
}

//  ------------------------------------------------------------------
void CfgQuotebuffile()
{
    Path buf;
    strxcpy(buf, val, sizeof(buf));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_QUOTEBUFFILE, buf, strlen(buf) + 1);
    }
    else
    {
        strcpy(CFG->quotebuffile, buf);
    }
}

//  ------------------------------------------------------------------
void CfgQuotebufmode()
{
    if(strieql(val, "ASK"))
    {
        CFG->quotebufmode = QBUF_ASK;
    }
    else if(strieql(val, "APPEND"))
    {
        CFG->quotebufmode = QBUF_APPEND;
    }
    else if(strieql(val, "OVERWRITE"))
    {
        CFG->quotebufmode = QBUF_OVERWRITE;
    }
}

//  ------------------------------------------------------------------
void CfgQuotechars()
{
    char buf[11];
    strxcpy(buf, StripQuotes(val), sizeof(buf));

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_QUOTECHARS, buf, strlen(buf) + 1);
    }
    else
    {
        strcpy(CFG->quotechars, buf);
    }
}

//  ------------------------------------------------------------------
void CfgQuotectrl()
{
    int ctrlinfo = CI_NONE;

    if(striinc("NO", val))
    {
        ctrlinfo = CI_NONE;
    }
    else if(striinc("YES", val))
    {
        ctrlinfo = CI_TEAR | CI_ORIG;
    }
    else
    {
        if(striinc("TEARLINE", val))
        {
            ctrlinfo |= CI_TEAR;
        }

        if(striinc("ORIGIN", val))
        {
            ctrlinfo |= CI_ORIG;
        }
    }

    if(cfgingroup)
    {
        CFG->grp.AddItm(GRP_QUOTECTRL, ctrlinfo);
    }
    else
    {
        CFG->quotectrl = ctrlinfo;
    }
} // CfgQuotectrl

//  ------------------------------------------------------------------
void CfgQuotemargin()
{
    CFG->quotemargin = atoi(val);
}

//  ------------------------------------------------------------------
