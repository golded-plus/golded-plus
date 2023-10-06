
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
//  Conversion of a raw message to a linked list of lines.
//  ------------------------------------------------------------------

#include <cstdarg>
#include <limits.h>
#include <golded.h>
#include <gstrmail.h>
#include <gutlcode.h>
#include <ghdrmime.h>

#if defined(__USE_ALLOCA__)
    #include <malloc.h>
#endif

#ifdef HAS_ICONV
    #include <iconv.h>
#endif


//  ------------------------------------------------------------------

#ifdef __UNIX__
    #define GOLDMARK '^' // xwindow fonts has no square sign :(
#else
    #define GOLDMARK '\xFD'
#endif


//  ------------------------------------------------------------------

const int BODYLINE = 0;
const int HEADERLINE = 1;
const int USERDEFINED = 2;
const int RFC_X = 3;

enum
{
    MASK_FTS = 0x1000,
    FTS_AREA,
    FTS_INTL,
    FTS_FMPT,
    FTS_TOPT,
    FTS_MSGID,
    FTS_REPLY,
    FTS_SEENBY,
    FTS_PATH,
    FTS_ZZZZ
};

enum
{
    MASK_FSC = 0x2000,
    FSC_CHARSET,
    FSC_CHRC,
    FSC_CHRS,
    FSC_CODEPAGE,
    FSC_DOMAIN,
    FSC_EID,
    FSC_ENC,
    FSC_ENCLFILE,
    FSC_FLAGS,
    FSC_FWDFROM,
    FSC_FWDORIG,
    FSC_FWDTO,
    FSC_FWDDEST,
    FSC_FWDSUBJ,
    FSC_FWDAREA,
    FSC_FWDMSGID,
    FSC_I51,
    FSC_MSGTO,
    FSC_PID,
    FSC_PTH,
    FSC_REPLYADDR,
    FSC_REPLYTO,
    FSC_SPLIT,
    FSC_SPTH,
    FSC_TID,
    FSC_ZZZZ
};

enum
{
    MASK_XXX = 0x8000,
    XXX_ACUPDATE,
    XXX_DESTADDR,
    XXX_ENCRYPTION,
    XXX_EOT,
    XXX_GATECHK,
    XXX_GID,
    XXX_GIF,
    XXX_GMD,
    XXX_GROUP,
    XXX_MOOD,
    XXX_MSGSEQ,
    XXX_NOTE,
    XXX_ORIGID,
    XXX_ORIGINAL,
    XXX_ORIGREF,
    XXX_RECD,
    XXX_RFC,
    XXX_RFD,
    XXX_RID,
    XXX_ROUTE,
    XXX_SN,
    XXX_SOT,
    XXX_TCL1,
    XXX_TCL2,
    XXX_TZUTCINFO,
    XXX_TZUTC,
    XXX_TZ,
    XXX_VIA,
    XXX_XID,
    XXX_ZZZZ
};

enum
{
    MASK_RFC = 0x4000,
    RFC_ALSO_CONTROL,
    RFC_APPARENTLY_TO,
    RFC_APPROVED,
    RFC_ARTICLE_NAMES,
    RFC_ARTICLE_UPDATES,
    RFC_BCC,
    RFC_CC,
    RFC_COMMENT,
    RFC_COMMENTS,
    RFC_CONTENT_DESCRIPTION,
    RFC_CONTENT_DISPOSITION,
    RFC_CONTENT_ID,
    RFC_CONTENT_LENGTH,
    RFC_CONTENT_TRANSFER_ENCODING,
    RFC_CONTENT_TYPE,
    RFC_CONTROL,
    RFC_DATE,
    RFC_DELIVERED_TO,
    RFC_DELIVERY_DATE,
    RFC_DISTRIBUTION,
    RFC_ENCRYPTED,
    RFC_ERRORS_TO,
    RFC_EXPIRES,
    RFC_FOLLOWUP_TO,
    RFC_FROM,
    RFC_FROMX,
    RFC_IN_REPLY_TO,
    RFC_KEYWORDS,
    RFC_LINES,
    RFC_MAILING_LIST,                   // This one is actually _not_ RFC
    RFC_MESSAGE_ID,
    RFC_MIME_VERSION,
    RFC_NEWSGROUPS,
    RFC_NEWS_SOFTWARE,
    RFC_NNTP_POSTING_DATE,
    RFC_NNTP_POSTING_HOST,
    RFC_NNTP_POSTING_USER,
    RFC_OLD_DATE,
    RFC_ORGANIZATION,
    RFC_ORIGINATOR,
    RFC_PATH,
    RFC_PRECEDENCE,
    RFC_PRIORITY,
    RFC_RECEIVED,
    RFC_REFERENCES,
    RFC_REPLY_TO,
    RFC_RETURN_PATH,
    RFC_RETURN_RECEIPT_TO,
    RFC_SEE_ALSO,
    RFC_SENDER,
    RFC_STATUS,
    RFC_SUBJECT,
    RFC_SUMMARY,
    RFC_SUPERSEDES,
    RFC_TO,
    RFC_VERSION,
    RFC_XREF,
    RFC_X_CHARSET,
    RFC_X_CHAR_ESC,
    RFC_X_FTN_TO,
    RFC_X_MAILER,
    RFC_X_NEWSREADER,
    RFC_X_TO,
    RFC_RNEWS,
    RFC_ZZZZ
};

#define MASK_ALL (MASK_FTS|MASK_FSC|MASK_RFC|MASK_XXX)


//  ------------------------------------------------------------------

struct Kludges
{
    const char* key;
    uint  num;
    byte  req;
};


//  ------------------------------------------------------------------

const byte KCRQ_NONE  = 0x0000;
const byte KCRQ_COLON = 0x0001;
const byte KCRQ_CASE  = 0x0002;


//  ------------------------------------------------------------------

static const Kludges fts_list[] =
{

    { "AREA", FTS_AREA, KCRQ_CASE  },
    { "INTL", FTS_INTL, KCRQ_CASE  },
    { "FMPT", FTS_FMPT, KCRQ_CASE  },
    { "TOPT", FTS_TOPT, KCRQ_CASE  },
    { "MSGID", FTS_MSGID, KCRQ_CASE  },
    { "REPLY", FTS_REPLY, KCRQ_CASE  },
    { "SEEN-BY", FTS_SEENBY, KCRQ_CASE  },
    { "PATH", FTS_PATH, KCRQ_CASE  },
    { "", FTS_ZZZZ, KCRQ_NONE  },
};


//  ------------------------------------------------------------------

static const Kludges fsc_list[] =
{

    { "CHARSET", FSC_CHARSET, KCRQ_CASE  },
    { "CHRC", FSC_CHRC, KCRQ_CASE  },
    { "CHRS", FSC_CHRS, KCRQ_CASE  },
    { "CODEPAGE", FSC_CODEPAGE, KCRQ_CASE  },
    { "DOMAIN", FSC_DOMAIN, KCRQ_CASE  },
    { "EID", FSC_EID, KCRQ_CASE  },
    { "ENC", FSC_ENC, KCRQ_CASE  },
    { "ENCLFILE", FSC_ENCLFILE, KCRQ_CASE  },
    { "FLAGS", FSC_FLAGS, KCRQ_CASE  },
    { "FWDFROM", FSC_FWDFROM, KCRQ_CASE  },
    { "FWDORIG", FSC_FWDORIG, KCRQ_CASE  },
    { "FWDTO", FSC_FWDTO, KCRQ_CASE  },
    { "FWDDEST", FSC_FWDDEST, KCRQ_CASE  },
    { "FWDSUBJ", FSC_FWDSUBJ, KCRQ_CASE  },
    { "FWDAREA", FSC_FWDAREA, KCRQ_CASE  },
    { "FWDMSGID", FSC_FWDMSGID, KCRQ_CASE  },
    { "I51", FSC_I51, KCRQ_CASE  },
    { "MSGTO", FSC_MSGTO, KCRQ_CASE  },
    { "PID", FSC_PID, KCRQ_CASE  },
    { "PTH", FSC_PTH, KCRQ_CASE  },
    { "REPLYADDR", FSC_REPLYADDR, KCRQ_CASE  },
    { "REPLYTO", FSC_REPLYTO, KCRQ_CASE  },
    { "SPLIT", FSC_SPLIT, KCRQ_CASE  },
    { "SPTH", FSC_SPTH, KCRQ_CASE  },
    { "TID", FSC_TID, KCRQ_CASE  },
    { "", FSC_ZZZZ, KCRQ_NONE  },
};


//  ------------------------------------------------------------------

static const Kludges xxx_list[] =
{

    { "ACUPDATE", XXX_ACUPDATE, KCRQ_CASE  },
    { "DESTADDR", XXX_DESTADDR, KCRQ_CASE  },
    { "ENCRYPTION", XXX_ENCRYPTION, KCRQ_CASE  },
    { "EOT", XXX_EOT, KCRQ_CASE  },
    { "GATECHK", XXX_GATECHK, KCRQ_CASE  },
    { "GID", XXX_GID, KCRQ_CASE  },
    { "GIF", XXX_GIF, KCRQ_CASE  },
    { "GMD", XXX_GMD, KCRQ_CASE  },
    { "GROUP", XXX_GROUP, KCRQ_CASE  },
    { "MOOD", XXX_MOOD, KCRQ_CASE  },
    { "MSGSEQ", XXX_MSGSEQ, KCRQ_CASE  },
    { "NOTE", XXX_NOTE, KCRQ_CASE  },
    { "ORIGID", XXX_ORIGID, KCRQ_CASE  },
    { "Original", XXX_ORIGINAL, KCRQ_NONE  },
    { "ORIGREF", XXX_ORIGREF, KCRQ_CASE  },
    { "Recd", XXX_RECD, KCRQ_CASE  },
    { "RFC", XXX_RFC, KCRQ_CASE  },
    { "RFD", XXX_RFD, KCRQ_CASE  },
    { "RID", XXX_RID, KCRQ_CASE  },
    { "#ROUTE", XXX_ROUTE, KCRQ_CASE  },
    { "SN", XXX_SN, KCRQ_CASE  },
    { "SOT", XXX_SOT, KCRQ_CASE  },
    { "TCL1", XXX_TCL1, KCRQ_CASE  },
    { "TCL2", XXX_TCL2, KCRQ_CASE  },
    { "TZUTCINFO", XXX_TZUTCINFO, KCRQ_CASE  },
    { "TZUTC", XXX_TZUTC, KCRQ_CASE  },
    { "TZ", XXX_TZ, KCRQ_CASE  },
    { "Via", XXX_VIA, KCRQ_NONE  },
    { "XID", XXX_XID, KCRQ_CASE  },
    { "", XXX_ZZZZ, KCRQ_NONE  }
};


//  ------------------------------------------------------------------

static const Kludges rfc_list[] =
{

    { "Also-Control", RFC_ALSO_CONTROL, KCRQ_COLON },
    { "Apparently-To", RFC_APPARENTLY_TO, KCRQ_COLON },
    { "Approved", RFC_APPROVED, KCRQ_COLON },
    { "Article-Names", RFC_ARTICLE_NAMES, KCRQ_COLON },
    { "Article-Updates", RFC_ARTICLE_UPDATES, KCRQ_COLON },
    { "Bcc", RFC_BCC, KCRQ_COLON },
    { "Cc", RFC_CC, KCRQ_COLON },
    { "Comment", RFC_COMMENT, KCRQ_COLON },
    { "Comments", RFC_COMMENTS, KCRQ_COLON },
    { "Content-Description", RFC_CONTENT_DESCRIPTION, KCRQ_COLON },
    { "Content-Disposition", RFC_CONTENT_DISPOSITION, KCRQ_COLON },
    { "Content-ID", RFC_CONTENT_ID, KCRQ_COLON },
    { "Content-Length", RFC_CONTENT_LENGTH, KCRQ_COLON },
    { "Content-Transfer-Encoding", RFC_CONTENT_TRANSFER_ENCODING, KCRQ_COLON },
    { "Content-Type", RFC_CONTENT_TYPE, KCRQ_COLON },
    { "Control", RFC_CONTROL, KCRQ_COLON },
    { "Date", RFC_DATE, KCRQ_COLON },
    { "Delivered-To", RFC_DELIVERED_TO, KCRQ_COLON },
    { "Delivery-Date", RFC_DELIVERY_DATE, KCRQ_COLON },
    { "Distribution", RFC_DISTRIBUTION, KCRQ_COLON },
    { "Encrypted", RFC_ENCRYPTED, KCRQ_COLON },
    { "Errors-To", RFC_ERRORS_TO, KCRQ_COLON },
    { "Expires", RFC_EXPIRES, KCRQ_COLON },
    { "Followup-To", RFC_FOLLOWUP_TO, KCRQ_COLON },
    { "From", RFC_FROM, KCRQ_COLON },
    { "From", RFC_FROMX, KCRQ_NONE  },
    { "In-Reply-To", RFC_IN_REPLY_TO, KCRQ_COLON },
    { "Keywords", RFC_KEYWORDS, KCRQ_COLON },
    { "Lines", RFC_LINES, KCRQ_COLON },
    { "Message-ID", RFC_MESSAGE_ID, KCRQ_COLON },
    { "Mailing-List", RFC_MAILING_LIST, KCRQ_COLON },
    { "MIME-Version", RFC_MIME_VERSION, KCRQ_COLON },
    { "Newsgroups", RFC_NEWSGROUPS, KCRQ_COLON },
    { "News-Software", RFC_NEWS_SOFTWARE, KCRQ_COLON },
    { "NNTP-Posting-Date", RFC_NNTP_POSTING_DATE, KCRQ_COLON },
    { "NNTP-Posting-Host", RFC_NNTP_POSTING_HOST, KCRQ_COLON },
    { "NNTP-Posting-User", RFC_NNTP_POSTING_USER, KCRQ_COLON },
    { "Old-Date", RFC_OLD_DATE, KCRQ_COLON },
    { "Organization", RFC_ORGANIZATION, KCRQ_COLON },
    { "Originator", RFC_ORIGINATOR, KCRQ_COLON },
    { "Path", RFC_PATH, KCRQ_COLON },
    { "Precedence", RFC_PRECEDENCE, KCRQ_COLON },
    { "Priority", RFC_PRIORITY, KCRQ_COLON },
    { "Received", RFC_RECEIVED, KCRQ_COLON },
    { "References", RFC_REFERENCES, KCRQ_COLON },
    { "Reply-To", RFC_REPLY_TO, KCRQ_COLON },
    { "Return-Path", RFC_RETURN_PATH, KCRQ_COLON },
    { "Return-Receipt-To", RFC_RETURN_RECEIPT_TO, KCRQ_COLON },
    { "See-Also", RFC_SEE_ALSO, KCRQ_COLON },
    { "Sender", RFC_SENDER, KCRQ_COLON },
    { "Status", RFC_STATUS, KCRQ_COLON },
    { "Subject", RFC_SUBJECT, KCRQ_COLON },
    { "Summary", RFC_SUMMARY, KCRQ_COLON },
    { "Supersedes", RFC_SUPERSEDES, KCRQ_COLON },
    { "To", RFC_TO, KCRQ_COLON },
    { "Version", RFC_VERSION, KCRQ_COLON },
    { "Xref", RFC_XREF, KCRQ_COLON },
    { "X-Charset", RFC_X_CHARSET, KCRQ_COLON },
    { "X-Char-Esc", RFC_X_CHAR_ESC, KCRQ_COLON },
    { "X-FTN-To", RFC_X_FTN_TO, KCRQ_COLON },
    { "X-Mailer", RFC_X_MAILER, KCRQ_COLON },
    { "X-Mailreader", RFC_X_MAILER, KCRQ_COLON },
    { "X-Newsreader", RFC_X_NEWSREADER, KCRQ_COLON },
    { "X-To", RFC_X_TO, KCRQ_COLON },
    { "#!", RFC_RNEWS, KCRQ_NONE  },
    { "", RFC_ZZZZ, KCRQ_NONE  },
};


char* mime_header_decode(char* decoded, const char* encoded, char *charset)
{

    char dbuf[200], cbuf[100], ebuf[50], tbuf[200];
    char* dptr = decoded;
    const char* eptr = encoded;
    if(charset) *charset = NUL;
    while(*eptr)
    {
        if(*eptr == '=')
        {
            const char* mptr = mime_crack_encoded_word(eptr, cbuf, ebuf, tbuf);
            if(mptr)
            {
                if(charset)
                {
                    strxcpy(charset, cbuf, 100);
                    charset = NULL;
                }
                bool okay = false;
                strchg(tbuf, '_', ' ');
                if(strieql(ebuf, "Q"))
                {
                    quoted_printable_engine qb;
                    qb.decode(dbuf, tbuf);
                    dptr = stpcpy(dptr, dbuf);
                    okay = true;
                }
                else if(strieql(ebuf, "B"))
                {
                    base64_engine b64;
                    b64.decode(dbuf, tbuf);
                    dptr = stpcpy(dptr, dbuf);
                    okay = true;
                }
                if(okay)
                {
                    eptr = mptr;
                    mptr = strskip_lwsp(mptr);
                    if(mime_crack_encoded_word(mptr, cbuf, ebuf, tbuf))
                        eptr = mptr;
                    continue;
                }
            }
        }
        *dptr++ = *eptr++;
    }
    *dptr = NUL;

    return decoded;
}


//  ------------------------------------------------------------------

char* strxmimecpy(char* dest, const char* source, int level, int size, bool detect)
{

    ISub buf, buf2;
    char charset[100];
    int table = -1;

    strxcpy(buf, source, sizeof(buf));
    mime_header_decode(buf2, buf, charset);

    if(charset[0] == NUL)
        detect = false;

    if(detect)
    {
        table = LoadCharset(NULL, NULL, 1);
        level = LoadCharset(charset, CFG->xlatlocalset);
        if(not level)
        {
            level = LoadCharset(AA->Xlatimport(), CFG->xlatlocalset);
        }
    }

    const std::string converted = XlatStr(buf2, level, CharTable);

    if(detect)
    {
        if(table == -1)
            LoadCharset("N/A", "N/A");
        else
            LoadCharset(CFG->xlatcharset[table].imp, CFG->xlatcharset[table].exp);
    }

    strxcpy(dest, converted.c_str(), size);

    return dest;
}


//  ------------------------------------------------------------------

static void KludgeAREA(GMsg* msg, const char* echoid)
{

    if(AA->Usearea())
    {
        Area* ap = AL.AreaEchoToPtr(echoid);
        if(ap)
            msg->areakludgeid = ap->echoid();
    }
}


//  ------------------------------------------------------------------

static void KludgeINTL(GMsg* msg, const char* ptr)
{

    char buf1[201], buf2[201];
    word fmpt = msg->orig.point;
    word topt = msg->dest.point;
    sscanf(ptr, "%s %s", buf1, buf2);
    msg->dest.set(buf1);
    msg->orig.set(buf2);
    msg->orig.point = fmpt;
    msg->dest.point = topt;
}


//  ------------------------------------------------------------------

static void KludgeFMPT(GMsg* msg, const char* ptr)
{

    msg->orig.point = atow(ptr);
}


//  ------------------------------------------------------------------

static void KludgeTOPT(GMsg* msg, const char* ptr)
{

    msg->dest.point = atow(ptr);
}


//  ------------------------------------------------------------------

static void KludgeMSGID(GMsg* msg, const char* ptr)
{

    strxcpy(msg->msgids, ptr, sizeof(msg->msgids));
    msg->msgid.reset(msg->msgids, msg->odom);
}


//  ------------------------------------------------------------------

static void KludgeREPLY(GMsg* msg, const char* ptr)
{

    strxcpy(msg->replys, ptr, sizeof(msg->replys));
}


//  ------------------------------------------------------------------

static void KludgeDOMAIN(GMsg* msg, const char* ptr)
{

    char buf1[201], buf2[201];
    sscanf(ptr, "%s %s %s %s", msg->ddom, buf1, msg->odom, buf2);
    msg->dest.reset(buf1);
    msg->orig.reset(buf2);
}


//  ------------------------------------------------------------------

static void KludgeFLAGS(GMsg* msg, const char* ptr)
{

    GetAttribstr(&msg->attr, ptr);
}


//  ------------------------------------------------------------------

static void KludgeMSGTO(GMsg* msg, const char* ptr)
{

    msg->dest.reset(ptr, msg->ddom);
}


//  ------------------------------------------------------------------

static void KludgePID(GMsg* msg, const char* ptr)
{

    strxcpy(msg->pid, ptr, sizeof(msg->pid));

    if(CFG->gedhandshake)
    {
        // Recognize another GoldED msg
        if(striinc(__gver_name__, ptr) or striinc(__gver_shortname__, ptr))
            goldmark = GOLDMARK;
    }
}


//  ------------------------------------------------------------------

static void KludgeREPLYADDR(GMsg* msg, const char* ptr)
{

    INam name;
    char *buf=throw_strdup(ptr);
    *name = NUL;
    ParseInternetAddr(buf, *msg->realby ? name : msg->realby, msg->iaddr);
    if(*name)
        strxcpy(msg->realby, name, sizeof(INam));
    throw_free(buf);
}


//  ------------------------------------------------------------------

static void KludgeREPLYTO(GMsg* msg, const char* ptr)
{

    strcpy(msg->igate, ptr);
}


//  ------------------------------------------------------------------

static void KludgeFROM(GMsg* msg, const char* ptr)
{

    INam _fromname;
    IAdr _fromaddr;
    char* buf = throw_strdup(ptr);
    strxmimecpy(msg->ifrom, buf, 0, sizeof(msg->ifrom), true);
    ParseInternetAddr(buf, _fromname, _fromaddr);
    throw_free(buf);
    if(*_fromaddr)
        strcpy(msg->iorig, _fromaddr);
    if(*_fromname)
        strxcpy(msg->realby, _fromname, sizeof(msg->realby));
}


//  ------------------------------------------------------------------

static void KludgeTO(GMsg* msg, const char* ptr)
{

    INam _toname;
    IAdr _toaddr;
    char* buf = throw_strdup(ptr);
    strxmimecpy(msg->ito, buf, 0, sizeof(msg->ito), true);
    ParseInternetAddr(buf, _toname, _toaddr);
    throw_free(buf);
    if(*_toaddr)
        strcpy(msg->idest, _toaddr);
    if(*_toname)
        strxcpy(msg->realto, _toname, sizeof(msg->realto));
}


//  ------------------------------------------------------------------

static void KludgeBCC(GMsg* msg, const char* ptr)
{

    INam _toname;
    IAdr _toaddr, buf;

    gstrarray bccs;
    tokenize(bccs, ptr, ",");
    for(int i=0; i < bccs.size(); i++)
    {
        strxcpy(buf, strskip_wht(bccs[i].c_str()), sizeof(IAdr));
        ParseInternetAddr(buf, _toname, _toaddr);
        if(*_toaddr and not striinc(_toaddr, msg->ibcc))
        {
            if(*msg->ibcc)
                strxcat(msg->ibcc, ", ", sizeof(msg->ibcc));
            if(_toname[0] != NUL)
            {
                IAdr buf2;
                mime_header_encode(buf2, _toname, msg);
                char quot[2] = "\"";
                if ((buf2[0] == '\"') or (strpbrk(buf2, " \t") == NULL))
                    quot[0] = NUL;
                gsprintf(PRINTF_DECLARE_BUFFER(buf), "%s%s%s <%s>", quot, buf2, quot, _toaddr);
            }
            else
                gsprintf(PRINTF_DECLARE_BUFFER(buf), "%s", _toaddr);

            strxcat(msg->ibcc, buf, sizeof(msg->ibcc));
        }
    }
}


//  ------------------------------------------------------------------

static void KludgeCC(GMsg* msg, const char* ptr)
{

    INam _toname;
    IAdr _toaddr, buf;

    gstrarray ccs;
    tokenize(ccs, ptr, ",");
    for(int i=0; i < ccs.size(); i++)
    {
        strxcpy(buf, strskip_wht(ccs[i].c_str()), sizeof(IAdr));
        ParseInternetAddr(buf, _toname, _toaddr);
        if(*_toaddr and not striinc(_toaddr, msg->icc))
        {
            if(*msg->icc)
                strxcat(msg->icc, ", ", sizeof(msg->icc));
            if(_toname[0] != NUL)
            {
                IAdr buf2;
                mime_header_encode(buf2, _toname, msg);
                char quot[2] = "\"";
                if ((buf[0] == '\"') or (strpbrk(buf2, " \t") == NULL))
                    quot[0] = NUL;
                gsprintf(PRINTF_DECLARE_BUFFER(buf), "%s%s%s <%s>", quot, buf2, quot, _toaddr);
            }
            else
                gsprintf(PRINTF_DECLARE_BUFFER(buf), "%s", _toaddr);

            strxcat(msg->icc, buf, sizeof(msg->icc));
        }
    }
}


//  ------------------------------------------------------------------

static void KludgeREPLY_TO(GMsg* msg, const char* ptr)
{

    INam _rtname;
    IAdr _rtaddr;
    char *buf=throw_strdup(ptr);
    ParseInternetAddr(buf, _rtname, _rtaddr);
    throw_free(buf);
    if(*_rtaddr)
        strcpy(msg->ireplyto, _rtaddr);
}


//  ------------------------------------------------------------------

static void KludgeSUBJECT(GMsg* msg, const char* ptr)
{

    if(not (msg->attr.frq() or msg->attr.att() or msg->attr.urq()))
        strxmimecpy(msg->re, ptr, 0, sizeof(msg->re), true);
}


//  ------------------------------------------------------------------

static void KludgeTZUTC(GMsg* msg, const char* ptr)
{

    msg->tzutc = atoi(ptr);
}


//  ------------------------------------------------------------------

void KludgeDATE(GMsg* msg, const char* ptr)
{

    // RFC822 Date: BNF
    //
    // date-time   =  [ day "," ] date time        ; dd mm yy
    //                                             ;  hh:mm:ss zzz
    //
    // day         =  "Mon"  / "Tue" /  "Wed"  / "Thu"
    //             /  "Fri"  / "Sat" /  "Sun"
    //
    // date        =  1*2DIGIT month 2DIGIT        ; day month year
    //                                             ;  e.g. 20 Jun 82
    //
    // month       =  "Jan"  /  "Feb" /  "Mar"  /  "Apr"
    //             /  "May"  /  "Jun" /  "Jul"  /  "Aug"
    //             /  "Sep"  /  "Oct" /  "Nov"  /  "Dec"
    //
    // time        =  hour zone                    ; ANSI and Military
    //
    // hour        =  2DIGIT ":" 2DIGIT [":" 2DIGIT]
    //                                             ; 00:00:00 - 23:59:59
    //
    // zone        =  "UT"  / "GMT"                ; Universal Time
    //                                             ; North American : UT
    //             /  "EST" / "EDT"                ;  Eastern:  - 5/ - 4
    //             /  "CST" / "CDT"                ;  Central:  - 6/ - 5
    //             /  "MST" / "MDT"                ;  Mountain: - 7/ - 6
    //             /  "PST" / "PDT"                ;  Pacific:  - 8/ - 7
    //             /  1ALPHA                       ; Military: Z = UT;
    //                                             ;  A:-1; (J not used)
    //                                             ;  M:-12; N:+1; Y:+12
    //             / ( ("+" / "-") 4DIGIT )        ; Local differential
    //                                             ;  hours+min. (HHMM)

    bool date_ok = false;
    int year=0, month=0, day=0;
    int hour=0, minute=0, second=0;

    ptr = strskip_wht(ptr);
    if(not isdigit(*ptr))
    {
        // Skip past weekday string
        ptr = strskip_wht(strskip_txt(ptr));
    }
    if(*ptr)
    {
        if(isdigit(*ptr))
        {
            day = atoi(ptr);
            ptr = strskip_wht(strskip_txt(ptr));
            if(g_isalpha(*ptr))
            {
                month = str2mon(ptr);
                if(month)
                {
                    ptr = strskip_wht(strskip_txt(ptr));
                    if(isdigit(*ptr))
                    {
                        year = atoi(ptr);
                        ptr = strskip_wht(strskip_txt(ptr));
                        if(isdigit(*ptr))
                        {
                            hour = atoi(ptr);
                            ptr = strskip_digits(ptr);
                            if(*ptr and isdigit(ptr[1]))
                            {
                                minute = atoi(++ptr);
                                date_ok = true;
                                // The seconds part is optional
                                ptr = strskip_digits(ptr);
                                if(*ptr and isdigit(ptr[1]))
                                {
                                    second = atoi(++ptr);
                                    // Setting timezone
                                    ptr = strskip_wht(strskip_digits(ptr));
                                    if(*ptr)
                                    {
                                        if(*ptr == '(' /*)*/ ) ++ptr;
                                        msg->tzutc = atoi(ptr);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (date_ok)
    {
        struct tm t;
        t.tm_year  = (year < 80) ? (year+100) : (year > 1900) ? (year-1900) : year;
        t.tm_mon   = month - 1;
        t.tm_mday  = day;
        t.tm_hour  = hour;
        t.tm_min   = minute;
        t.tm_sec   = second;
        t.tm_isdst = -1;
        time32_t a = gmktime(&t);
        struct tm tp;
        ggmtime(&tp, &a);
        tp.tm_isdst = -1;
        time32_t b = gmktime(&tp);
        msg->written = a + a - b;
    }
}


//  ------------------------------------------------------------------

static void KludgeMESSAGE_ID(GMsg* msg, const char* ptr)
{

    char buf[201];
    throw_free(msg->messageid);
    msg->messageid = throw_strdup(ptr);
    CvtMessageIDtoMSGID(ptr, buf, AA->echoid(), "MSGID");
    strcpy(msg->msgids, buf+8);
}


//  ------------------------------------------------------------------

static void KludgeREFERENCES(GMsg* msg, const char* ptr)
{

    throw_free(msg->references);
    msg->references = throw_strdup(ptr);
}


//  ------------------------------------------------------------------

static void KludgeIN_REPLY_TO(GMsg* msg, const char* ptr)
{

    throw_free(msg->inreplyto);
    msg->inreplyto = throw_strdup(ptr);
}


//  ------------------------------------------------------------------

static void KludgeORGANIZATION(GMsg* msg, const char* ptr)
{

    strxcpy(msg->organization, ptr, sizeof(msg->organization));
}


//  ------------------------------------------------------------------

static void KludgeX_FTN_TO(GMsg* msg, const char* ptr)
{

    strxmimecpy(msg->realto, ptr, 0, sizeof(msg->realto), true);
}


//  ------------------------------------------------------------------

static void KludgeFWDFROM(GMsg* msg, const char* ptr)
{

    if(AA->Usefwd())
        strxcpy(msg->fwdfrom, ptr, sizeof(msg->fwdfrom));
}


//  ------------------------------------------------------------------

static void KludgeFWDORIG(GMsg* msg, const char* ptr)
{

    if(AA->Usefwd())
        msg->fwdorig.reset(ptr);
}


//  ------------------------------------------------------------------

static void KludgeFWDTO(GMsg* msg, const char* ptr)
{

    if(AA->Usefwd())
        strxcpy(msg->fwdto, ptr, sizeof(msg->fwdto));
}


//  ------------------------------------------------------------------

static void KludgeFWDDEST(GMsg* msg, const char* ptr)
{

    if(AA->Usefwd())
        msg->fwddest.reset(ptr);
}


//  ------------------------------------------------------------------

static void KludgeFWDSUBJ(GMsg* msg, const char* ptr)
{

    if(AA->Usefwd())
        strxcpy(msg->fwdsubj, ptr, sizeof(msg->fwdsubj));
}


//  ------------------------------------------------------------------

static void KludgeFWDAREA(GMsg* msg, const char* ptr)
{

    if(AA->Usefwd())
        strxcpy(msg->fwdarea, ptr, sizeof(msg->fwdarea));
}


//  ------------------------------------------------------------------

static void KludgeFWDMSGID(GMsg* msg, const char* ptr)
{

    if(AA->Usefwd())
        strxcpy(msg->fwdmsgid, ptr, sizeof(msg->fwdmsgid));
}


//  ------------------------------------------------------------------

char* UnwrapLine(Line* line, const char* ptr, int addspace = false)
{

    if(line->type & GLINE_WRAP)
    {
        uint len = strlen(ptr);
        char* uptr = throw_strdup(ptr);
        while(line and (line->type & GLINE_WRAP))
        {
            if(line->next)
            {
                uint nextlen = line->next->txt.length();
                uptr = (char*)throw_realloc(uptr, len+nextlen+2);
                if(addspace and len and (uptr[len-1] != ' '))
                {
                    strcat(uptr, " ");
                    len++;
                }
                strcpy(uptr+len, line->next->txt.c_str());
                len += nextlen;
            }
            line = line->next;
        }
        return uptr;
    }

    return NULL;
}


//  ------------------------------------------------------------------

static int HandleKludges(GMsg* msg, Line* line, int kludgenum, const char* ptr, int getvalue)
{

    switch(kludgenum)
    {

    case FTS_AREA:
        line->kludge = GKLUD_AREA;
        if(getvalue)
            KludgeAREA(msg, ptr);
        return true;

    case FTS_INTL:
        line->kludge = GKLUD_INTL;
        if(getvalue)
            KludgeINTL(msg, ptr);
        return true;

    case FTS_FMPT:
        line->kludge = GKLUD_FMPT;
        if(getvalue)
            KludgeFMPT(msg, ptr);
        return true;

    case FTS_TOPT:
        line->kludge = GKLUD_TOPT;
        if(getvalue)
            KludgeTOPT(msg, ptr);
        return true;

    case FTS_MSGID:
        line->kludge = GKLUD_MSGID;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeMSGID(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case FTS_REPLY:
        line->kludge = GKLUD_REPLY;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeREPLY(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case FTS_SEENBY:
        line->kludge = GKLUD_SEENBY;
        return true;

    case FTS_PATH:
        line->kludge = GKLUD_PATH;
        return true;

    ////////////////////////////////////////////////////////////

    case FSC_CHARSET:
        line->kludge = GKLUD_CHARSET;
        return true;

    case FSC_CHRS:
        line->kludge = GKLUD_CHARSET;
        return true;

    case FSC_CODEPAGE:
        line->kludge = GKLUD_CHARSET;
        return true;

    case FSC_DOMAIN:
        //line->kludge = GKLUD_DOMAIN;
        if(getvalue)
            KludgeDOMAIN(msg, ptr);
        return true;

    case FSC_ENCLFILE:
        if(getvalue)
            strxcpy(msg->re, ptr, sizeof(msg->re));
        return true;

    case FSC_FLAGS:
        line->kludge = GKLUD_FLAGS;
        if(getvalue)
            KludgeFLAGS(msg, ptr);
        return true;

    case FSC_I51:
        line->kludge = GKLUD_CHARSET;
        msg->i51 = true;
        return true;

    case FSC_MSGTO:
        //line->kludge = GKLUD_MSGTO;
        if(getvalue)
            KludgeMSGTO(msg, ptr);
        return true;

    case FSC_PID:
        line->kludge = GKLUD_PID;
        if(getvalue)
            KludgePID(msg, ptr);
        return true;

    case FSC_REPLYADDR:
        line->kludge = GKLUD_REPLYADDR;
        if(getvalue)
            KludgeREPLYADDR(msg, ptr);
        return true;

    case FSC_REPLYTO:
        line->kludge = GKLUD_REPLYTO;
        if(getvalue)
            KludgeREPLYTO(msg, ptr);
        return true;

    case FSC_FWDFROM:
        line->kludge = GKLUD_FWD;
        if(getvalue)
            KludgeFWDFROM(msg, ptr);
        return true;

    case FSC_FWDORIG:
        line->kludge = GKLUD_FWD;
        if(getvalue)
            KludgeFWDORIG(msg, ptr);
        return true;

    case FSC_FWDTO:
        line->kludge = GKLUD_FWD;
        if(getvalue)
            KludgeFWDTO(msg, ptr);
        return true;

    case FSC_FWDDEST:
        line->kludge = GKLUD_FWD;
        if(getvalue)
            KludgeFWDDEST(msg, ptr);
        return true;

    case FSC_FWDSUBJ:
        line->kludge = GKLUD_FWD;
        if(getvalue)
            KludgeFWDSUBJ(msg, ptr);
        return true;

    case FSC_FWDAREA:
        line->kludge = GKLUD_FWD;
        if(getvalue)
            KludgeFWDAREA(msg, ptr);
        return true;

    case FSC_FWDMSGID:
        line->kludge = GKLUD_FWD;
        if(getvalue)
            KludgeFWDMSGID(msg, ptr);
        return true;

    case FSC_CHRC:
    case FSC_EID:
    case FSC_ENC:
    case FSC_PTH:
    case FSC_SPLIT:
    case FSC_SPTH:
    case FSC_TID:
        // Recognized but not processed
        return true;

    ////////////////////////////////////////////////////////////

    case XXX_DESTADDR:
        //line->kludge = GKLUD_DESTADDR;
        if(getvalue)
            KludgeMSGTO(msg, ptr);
        return true;

    case XXX_TZUTCINFO:
    case XXX_TZUTC:
        line->kludge = GKLUD_KNOWN;
        if(getvalue)
            KludgeTZUTC(msg, ptr);
        return true;

    case XXX_ACUPDATE:
    case XXX_ENCRYPTION:
    case XXX_EOT:
    case XXX_GATECHK:
    case XXX_GID:
    case XXX_GIF:
    case XXX_GMD:
    case XXX_GROUP:
    case XXX_MOOD:
    case XXX_MSGSEQ:
    case XXX_NOTE:
    case XXX_ORIGID:
    case XXX_ORIGINAL:
    case XXX_ORIGREF:
    case XXX_RECD:
    case XXX_RFC:
    case XXX_RFD:
    case XXX_RID:
    case XXX_ROUTE:
    case XXX_SN:
    case XXX_SOT:
    case XXX_TCL1:
    case XXX_TCL2:
    case XXX_TZ:
    case XXX_VIA:
    case XXX_XID:
        // Recognized but not processed
        return true;
    }

    return false;
}


//  ------------------------------------------------------------------

int HandleRFCs(GMsg* msg, Line* line, int kludgenum, const char* ptr, int getvalue)
{

    switch(kludgenum)
    {

    case RFC_FROM:
        line->kludge = GKLUD_RFC;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeFROM(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_TO:
    case RFC_X_TO:
        line->kludge = GKLUD_RFC;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeTO(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_BCC:
        line->kludge = GKLUD_RFC;
        if(true /* getvalue */)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeBCC(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_CC:
        line->kludge = GKLUD_RFC;
        if(true /* getvalue */)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeCC(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_DATE:
        line->kludge = GKLUD_RFC;
        if(getvalue)
            KludgeDATE(msg, ptr);
        return true;

    case RFC_SUBJECT:
        line->kludge = GKLUD_RFC;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeSUBJECT(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_REPLY_TO:
        line->kludge = GKLUD_RFC;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeREPLY_TO(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_MESSAGE_ID:
        line->kludge = GKLUD_RFC;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeMESSAGE_ID(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_REFERENCES:
        line->kludge = GKLUD_RFC;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeREFERENCES(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_IN_REPLY_TO:
        line->kludge = GKLUD_RFC;
        if(getvalue)
        {
            char* tmp = UnwrapLine(line, ptr);
            KludgeIN_REPLY_TO(msg, tmp ? tmp : ptr);
            if(tmp)
                throw_free(tmp);
        }
        return true;

    case RFC_ORGANIZATION:
        line->kludge = GKLUD_RFC;
        if(getvalue)
            KludgeORGANIZATION(msg, ptr);
        return true;

    case RFC_X_FTN_TO:
        line->kludge = GKLUD_RFC;
        if(getvalue)
            KludgeX_FTN_TO(msg, ptr);
        return true;

    case RFC_X_MAILER:
        line->kludge = GKLUD_RFC;
        if(getvalue)
            KludgePID(msg, ptr);
        return true;

    case RFC_X_NEWSREADER:
        line->kludge = GKLUD_RFC;
        if(getvalue)
            KludgePID(msg, ptr);
        return true;

    case RFC_NEWSGROUPS:
    case RFC_SENDER:
    case RFC_CONTENT_TRANSFER_ENCODING:
    case RFC_CONTENT_TYPE:
    case RFC_MIME_VERSION:
    case RFC_X_CHARSET:
    case RFC_X_CHAR_ESC:
        // Mark RFC's that we add ourselves
        line->kludge = GKLUD_RFC;
        return true;

    case RFC_ALSO_CONTROL:
    case RFC_APPARENTLY_TO:
    case RFC_APPROVED:
    case RFC_ARTICLE_NAMES:
    case RFC_ARTICLE_UPDATES:
    case RFC_COMMENT:
    case RFC_COMMENTS:
    case RFC_CONTENT_DESCRIPTION:
    case RFC_CONTENT_DISPOSITION:
    case RFC_CONTENT_ID:
    case RFC_CONTENT_LENGTH:
    case RFC_CONTROL:
    case RFC_DELIVERED_TO:
    case RFC_DELIVERY_DATE:
    case RFC_DISTRIBUTION:
    case RFC_ENCRYPTED:
    case RFC_ERRORS_TO:
    case RFC_EXPIRES:
    case RFC_FOLLOWUP_TO:
    case RFC_FROMX:
    case RFC_KEYWORDS:
    case RFC_LINES:
    case RFC_MAILING_LIST:
    case RFC_NEWS_SOFTWARE:
    case RFC_NNTP_POSTING_DATE:
    case RFC_NNTP_POSTING_HOST:
    case RFC_NNTP_POSTING_USER:
    case RFC_OLD_DATE:
    case RFC_ORIGINATOR:
    case RFC_PATH:
    case RFC_PRECEDENCE:
    case RFC_PRIORITY:
    case RFC_RECEIVED:
    case RFC_RETURN_PATH:
    case RFC_RETURN_RECEIPT_TO:
    case RFC_SEE_ALSO:
    case RFC_STATUS:
    case RFC_SUMMARY:
    case RFC_SUPERSEDES:
    case RFC_VERSION:
    case RFC_XREF:
    case RFC_RNEWS:
        // Recognized but not processed
        return true;
    }

    return false;
}


//  ------------------------------------------------------------------

int ScanCtrlList(const Kludges *k, const char *kludge, char endchar)
{

    while(*k->key)
    {
        if((k->req & KCRQ_CASE) ? streql(kludge, k->key) : strieql(kludge, k->key))
        {
            if(k->req & KCRQ_COLON)
            {
                if(endchar == ':')
                    return k->num;
            }
            else
            {
                return k->num;
            }
        }
        k++;
    }

    return 0;
}


//  ------------------------------------------------------------------

int ScanLine(GMsg* msg, Line* line, const char* ptr, int getvalue, int mask)
{

    // Kludge number
    int kludgenum = 0;

    // Pointer to kludge id
    const char* kludge1 = ptr;

    // Skip past "RFC" string, if any
    if(strnieql(kludge1, "RFC", 3) and (kludge1[3] != ':'))
    {
        kludge1 += 3;
        if(not g_isalpha(*kludge1))
            kludge1++;
    }

    // Keep copy of id terminating char
    while((*ptr != ' ') and (*ptr != ':') and *ptr)
        ptr++;
    char endchar = *ptr;

#if defined(__USE_ALLOCA__)
    char *kludge = (char*)alloca(ptr-kludge1+1);
#else
    __extension__ char kludge[ptr-kludge1+1];
#endif
    strxcpy(kludge, kludge1, ptr-kludge1+1);

    // Search for it in the known kludges list
    if(*kludge)
    {
        while(1)
        {
            if(mask & MASK_FTS)
            {
                kludgenum = ScanCtrlList(fts_list, kludge, endchar);
                if(kludgenum)
                    break;
            }
            if(mask & MASK_FSC)
            {
                kludgenum = ScanCtrlList(fsc_list, kludge, endchar);
                if(kludgenum)
                    break;
            }
            if(mask & MASK_RFC)
            {
                kludgenum = ScanCtrlList(rfc_list, kludge, endchar);
                if(kludgenum)
                    break;
            }
            if(mask & MASK_XXX)
            {
                kludgenum = ScanCtrlList(xxx_list, kludge, endchar);
                if(kludgenum)
                    break;
            }
            break;
        }
    }

    // Restore terminating char
    if(*ptr != ' ')
        ptr++;

    if(kludgenum)
    {
        ptr = strskip_wht(ptr);
        line->type |= GLINE_KLUD;
        if(kludgenum & (MASK_FTS|MASK_FSC|MASK_XXX))
            HandleKludges(msg, line, kludgenum, ptr, getvalue);
        else if(kludgenum & MASK_RFC)
            HandleRFCs(msg, line, kludgenum, ptr, getvalue);
    }
    else
    {
        gstrarray::iterator k;
        for(k = CFG->kludge.begin(); k != CFG->kludge.end(); k++)
        {
            if(strnieql(kludge, k->c_str(), k->length()))
            {
                line->type |= GLINE_KLUD;
                kludgenum = USERDEFINED;
                break;
            }
        }
        if(not (line->type & GLINE_KLUD))
            if((strnieql(kludge, "X-", 2) or strnieql(kludge, "Resent-", 7)) and (endchar == ':') and (mask & MASK_RFC))
            {
                line->type |= GLINE_KLUD;
                kludgenum = RFC_X;
            }
        if(not (line->type & GLINE_KLUD))
            kludgenum = (endchar == ':') ? HEADERLINE : BODYLINE;
    }

    return kludgenum;
}


//  ------------------------------------------------------------------

Line* next_non_empty(Line *line)
{
    Line* nl = line;

    while(nl)
    {
        if(nl->txt.empty())
            nl = nl->next;
        else
            break;
    }
    return nl;
}

//  ------------------------------------------------------------------

void ScanKludges(GMsg* msg, int getvalue)
{

    const char* ptr;
    int tearlineno = INT_MAX;
    int originlineno = INT_MAX;
    int gotorig=NO, gottear=NO, gottag=NO;

    // Scan for kludge-, tear- and originlines

    msg->tzutc = -32767; // Default value, means TZUTC kludge was not found

    if(getvalue)
        goldmark = ' ';   // Reset the "recognizer" code

    int lineno = 0;
    Line* line = LastLine(msg->lin);

    do
    {
        ptr = line->txt.c_str();
        if(*ptr == CTRL_A)
        {

            // Set kludge/hidden color
            line->color = C_READK;

            int kludgenum = ScanLine(msg, line, ptr+1, getvalue, MASK_ALL);
            if((kludgenum == BODYLINE) or (kludgenum == HEADERLINE))
            {
                line->type |= GLINE_HIDD;
                line->color = C_READKH;
            }
        }
        else
        {

            if(strneql(ptr, "AREA:", 5) and (line->prev == NULL or *line->prev->txt.c_str() == CTRL_A))
            {
                line->color = C_READK;
                line->kludge = GKLUD_AREA;
                line->type |= GLINE_KLUD;
                const char* areakludgeptr = ptr+5;   // Extract echoid from kludge
                areakludgeptr = strskip_wht(areakludgeptr);
                KludgeAREA(msg, areakludgeptr);
            }

            if(not (line->type & GLINE_KLUD))
            {

                // Check if it is a tagline
                if(not gottag and (strneql("...", ptr, 3) or strneql("___", ptr, 3)))
                {

                    // It's only a tagline if it's just above the tearline or origin.
                    // Or if it's the last line in the message.
                    if(not lineno or ((lineno-1) == tearlineno) or ((lineno-1) == originlineno))
                    {
                        gottag = YES;
                        line->type |= GLINE_TAGL;
                        line->color = C_READG;
                        if(AA->Taglinesupport())
                            strbtrim(strxcpy(msg->tagline, ptr+3, sizeof(msg->tagline)));
                    }
                }

                // Check if it's a tearline
                else if(not (gottear or gottag) and strneql("---", ptr, 3) and (ptr[3] == ' ' or ptr[3] == NUL))
                {

                    Line* tearln = line;
                    int tearlnno = lineno;
                    while (tearln->type & GLINE_WRAP)
                    {
                        tearln = tearln->next;
                        tearlnno--;
                    }
                    Line* nnel = next_non_empty(tearln->next);
                    if(not tearlnno or ((tearlnno-1) == originlineno) or not nnel or nnel->type & GLINE_KLUDGE)
                    {
                        // Found Tearline
                        gottear = YES;
                        tearlineno = lineno;
                        for (tearln = line; tearln->type & GLINE_WRAP; tearln = tearln->next)
                        {
                            tearln->type |= GLINE_TEAR;
                            tearln->color = C_READT;
                        }
                        tearln->type |= GLINE_TEAR;
                        tearln->color = C_READT;
                        strbtrim(strxcpy(msg->tearline, ptr+3, sizeof(msg->tearline)));

                        if(getvalue and CFG->gedhandshake)
                        {
                            char* tearid[] =
                            {
                                "GoldED",
                                " GED ",
                                " GED2 ",
                                " GED3 ",
                                " GED386 ",
                                " GED/2 ",
                                " GED/386 ",
                                " GEDP16 ",
                                " GEDP32 ",
                                " GEDW32 ",
                                " GEDLNX ",
                                " GED/W32 ",
                                NULL
                            };
                            int n = 0;
                            while(tearid[n])
                            {
                                if(striinc(tearid[n], ptr))
                                {
                                    goldmark = GOLDMARK;          // Recognize another GoldED msg
                                    break;
                                }
                                n++;
                            }
                        }
                    }
                }

                // Check if it's an originline
                else if(not (gotorig or gottear or gottag) and strneql(" * Origin: ", ptr, 11))
                {

                    // Found Origin line
                    bool cnd = line->next != NULL;
                    Line* nnel = next_non_empty(line->next);
                    bool nextkl = cnd ? not nnel or nnel->type & GLINE_KLUDGE : false;
                    nnel = cnd ? next_non_empty(line->next->next) : NULL;
                    bool nextor = cnd ? (not nnel or nnel->type & GLINE_KLUDGE) and (line->next->txt.find(/*(*/')') != line->next->txt.npos) : false;
                    if(not line->next or nextkl or nextor)
                    {

                        gotorig = YES;
                        originlineno = lineno;
                        line->type |= GLINE_ORIG;
                        line->color = C_READO;
                        strxcpy(msg->origin, line->txt.c_str()+11, sizeof(msg->origin));
                        if(nextor)    // Get the next line too
                        {
                            strxcat(msg->origin, line->next->txt.c_str(), sizeof(msg->origin));
                            line->next->color = C_READO;
                            line->next->type |= GLINE_ORIG;     // Mark next line as Origin too
                        }
                    }
                }

                else if(strneql(ptr, "SEEN-BY:", 8))
                {
                    line->kludge = GKLUD_SEENBY;
                    line->color = C_READK;
                    line->type |= GLINE_KLUD;
                }

                // Check if it's a signature indicator
                else if(AA->isinternet() or *msg->ito or *msg->ifrom)
                {
                    if(streql(ptr, "-- "))
                    {
                        for(Line* q = line; q; q = q->next)
                        {
                            if((q->type & (GLINE_KLUDGE|GLINE_ORIG|GLINE_TEAR)) == 0)
                            {
                                q->color = C_READS;
                                q->type |= GLINE_SIGN;
                                if(q != line)
                                    q->type |= GLINE_HARD;
                                if(strneql("----", q->txt.c_str(), 4))
                                    break;
                            }
                        }
                    }
                }
            }
        }
        if(line->type & GLINE_WRAP)
        {
            Line* linep = line;
            while(linep and (linep->type & GLINE_WRAP))
            {
                if(linep->next)
                {
                    linep->next->type |= linep->type & GLINE_KLUDGE;
                    linep->next->kludge = linep->kludge;
                    linep->next->color = linep->color;
                }
                linep = linep->next;
            }
        }

        lineno++;

    }
    while((line = line->prev) != NULL);

    if(not gottag)
        *msg->tagline = NUL;
    if(not gottear)
        *msg->tearline = NUL;
    if(not gotorig)
        *msg->origin = NUL;

    if(getvalue)
    {
        // This is the new code (experimental)
        // It looks for an Origin before taking MSGID
        // Trust msg->orig if valid and we're in netmail area.
        // (msg->orig is already merged with INTL/FMPT/TOPT)

        if(not (AA->isnet() and msg->orig.valid()))
        {
            if(CFG->addresslookupfirst and msg->msgid.valid())
                msg->orig = msg->msgid;
            else if((ptr = strrchr(msg->origin, '(' /*)*/ )) != NULL)
            {
                while(not isdigit(*ptr) and *ptr)
                    ptr++;
                msg->orig.reset(ptr);
            }
            if(msg->msgid.valid())
                msg->orig = msg->msgid;
        }

        if(msg->orig.zone == 0)
            msg->orig.zone = msg->msgid.zone ? msg->msgid.zone : AA->Aka().addr.zone;

        if(msg->dest.zone == 0)
            msg->dest.zone = msg->orig.zone;
    }
}


//  ------------------------------------------------------------------

void  Latin2Local(char *str)
{
    if (!CFG->latin2local || !str) return;

    for (size_t i = 0; str[i]; i++)
    {
        byte chr = str[i];
        byte xch = CFG->latintolocal[chr];

        if (xch && (xch != chr))
        {
            byte left = i ? str[i-1] : 0;
            byte right = str[i+1];

            if (((left  >= 0x80) && g_isalpha(left )) ||
                    ((right >= 0x80) && g_isalpha(right)))
            {
                str[i] = xch;

                for (size_t j = i-1; j < i; j--)
                {
                    chr = str[j];
                    xch = CFG->latintolocal[chr];

                    if (xch && (xch != chr))
                        str[j] = xch;
                    else
                        break;
                }
            }
        }
    }
}


//  ------------------------------------------------------------------

void  Latin2Local(std::string &str)
{
#if defined(__USE_ALLOCA__)
    char *temp = (char *)alloca(str.length()+1);
#else
    __extension__ char temp[str.length()+1];
#endif
    strcpy(temp, str.c_str());
    Latin2Local(temp);
    str = temp;
}

//  ------------------------------------------------------------------

#ifdef HAS_ICONV
void IconvClear(void)
{
    if( iconv_cd!=(iconv_t)(-1) )
    {
        iconv_close(iconv_cd);
        iconv_cd = (iconv_t)(-1);
    }
}
#endif

//  ------------------------------------------------------------------

std::string XlatStr(const char* src, int level, Chs* chrtbl, int qpencoded, bool i51)
{

    if( src==NULL )
        return std::string();

    if( not chrtbl
#ifdef HAS_ICONV
            && iconv_cd==(iconv_t)(-1)
#endif
      )
        return src;

    std::string result;
    const char* sptr = src;
    char dochar;
    ChsTab* chrs = chrtbl ? chrtbl->t : (ChsTab*)NULL;

#ifdef HAS_ICONV
    size_t iconvrc=(size_t)(-1);
    if( iconv_cd!=(iconv_t)(-1) )
    {
        iconvrc=iconv( iconv_cd, NULL, NULL, NULL, NULL ); // init iconv
    }
#endif
    while(*sptr)
    {
        switch(*sptr)
        {
        case 0x02:
            if(i51 and I51Table)
            {
                bool charFound = false;
                for(uint n=0; n<I51Table->size; n++)
                {
                    const char* tptr = (char*)I51TP[n];
                    if(*(sptr+1) == tptr[0])
                    {
                        if(*(sptr+2) == tptr[1])
                        {
                            const char* escp = &tptr[2];
                            for (size_t c = 0; c < 3 && *escp; ++escp)
                            {
                                result += *escp;
                            }
                            sptr += 2;
                            charFound = true;
                            break;
                        }
                    }
                }
                if(!charFound)   // I51 char not found, use fallback method
                    sptr++;
            }
            sptr++;
            break;

        case 29:
            if(MNETable)
            {
                bool charFound = false;
                for(uint n=0; n<MNETable->size; n++)
                {
                    const char* tptr = (char*)MNETP[n];
                    if(*(sptr+1) == tptr[0])
                    {
                        if(*(sptr+2) == tptr[1])
                        {
                            const char* escp = &tptr[2];
                            for (size_t c = 0; c < 3 && *escp; ++escp)
                            {
                                result += *escp;
                            }
                            sptr += 2;
                            charFound = true;
                            break;
                        }
                    }
                }
                if(!charFound)   // MNEMONIC char not found, use fallback method
                    sptr++;
            }
            sptr++;
            break;

        case SOFTCR:
            {
                bool translated = false;
                if (WideDispsoftcr)
                    goto defaultchardo;
                else if (CompTable)
                {
                    if(sptr > src)
                    {
                        if(not (isspace(*(sptr-1)) or isspace(*(sptr+1))))
                        {
                            for(uint n=0; n<CompTable->size; n++)
                            {
                                const char* tptr = (char*)CompTP[n];
                                if(*(sptr-1) == tptr[0])
                                {
                                    if(*(sptr+1) == tptr[1])
                                    {
                                        const char* escp = &tptr[2];
                                        if(*escp)
                                        {
                                            result.erase(result.end() - 1);
                                            result += *escp++;
                                            if(*escp)
                                            {
                                                result += *escp++;
                                                if(*escp)
                                                {
                                                    result += *escp;
                                                    translated = true;
                                                }
                                            }
                                        }
                                        sptr += 2;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if(not translated)
                    result += *sptr++;
            }
            break;

        case '=':
            if(qpencoded)
            {
                if(isxdigit(sptr[1]) and isxdigit(sptr[2]))
                {
                    dochar = (char)((xtoi(sptr[1]) << 4) | xtoi(sptr[2]));
                    sptr += 3;
                    goto chardo;
                }
            }
            goto defaultchardo;

        default:
defaultchardo:
            dochar = *sptr++;
chardo:
#ifdef HAS_ICONV
            if( iconv_cd!=(iconv_t)(-1) )
            {
                size_t srcleft=1;
                size_t dstleft=3;
                char* tsptr = &dochar;
                char encBuf[3];
                char* encPtr = encBuf;

                iconvrc=iconv( iconv_cd, &tsptr, &srcleft, &encPtr, &dstleft );
                if( iconvrc==((size_t)-1) )
                {
                    switch( errno )
                    {
                    case EINVAL:
                        LOG.printf("! An incomplete multibyte sequence has been encountered before:");
                        LOG.printf("+ %s",sptr);
                    case EILSEQ:
                        LOG.printf("! An invalid multibyte sequence has been encountered in the input before:");
                        LOG.printf("+ %s",sptr);
                    case E2BIG:
                        LOG.printf("! There is not sufficient destination size before '%s'", sptr);
                    default:
                        LOG.printf("! Unknown error %u in iconv() before %s", errno, sptr);
                    }
                }
                else
                {
                    result.append(encBuf, encPtr - encBuf);
                }
            }
            else
#endif

                if ((level > 0) && chrs)
                {
                    const char* tptr = (const char*)chrs[(byte)dochar];
                    char clen = *tptr++;
                    while(clen--)
                        result += *tptr++;
                }
                else
                {
                    result += dochar;
                }
        }
    }
    return result;
}


//  ------------------------------------------------------------------

int cmp_quotes(char* q1, char* q2)
{

    q1--;
    q2--;

    do
    {
        q1 = spanspaces(++q1);
        q2 = spanspaces(++q2);
        if(*q1 != *q2)
            return NO;
    }
    while(*q1 and *q2);

    return *q1 == *q2;
}


//  ------------------------------------------------------------------

void GMsg::TextToLines(int __line_width, bool getvalue, bool header_recode)
{

    MakeLineIndex(this, __line_width, getvalue, header_recode);
}


//  ------------------------------------------------------------------

static bool check_multipart(const char* ptr, char* boundary)
{

    if(striinc("multipart", ptr))
    {
        const char* boundptr = striinc("boundary=", ptr);
        if(boundptr)
        {
            boundptr += 9;
            const char* boundend;
            if(*boundptr == '\"')
            {
                boundptr++;
                boundend = strchr(boundptr, '\"');
            }
            else
            {
                boundend = strpbrk(boundptr, " \r\n");
            }
            if(boundend)
            {
                strxcpy(boundary, boundptr, 1+boundend-boundptr);
                return true;
            }
        }
    }
    return false;
}


//  ------------------------------------------------------------------

inline bool put_on_new_line(const char *ptr)
{

    if((*ptr == CR) or
            (*ptr == CTRL_A) or
            is_quote(ptr) or
            ((ptr[0] == ptr[1]) and (ptr[0] == ptr[2])) or
            (strneql(ptr, "-- ", 3) and (ptr[3] == CR)) or
            strneql(ptr, " * Origin: ", 11))
        return true;
    // Put RFC kludges and SEEN-BY on new line
    while(*ptr and (isxalnum(*ptr) or (*ptr == '-')))
        ptr++;
    if(*ptr == ':')
        return true;
    return false;
}


//  ------------------------------------------------------------------

void MakeLineIndex(GMsg* msg, int margin, bool getvalue, bool header_recode)
{

    uint idx=0, idxadjust=0;
    uint len;
    int level=0;
    uint n;
    char ch, chln = 0, dochar;
    Line* line;
    Line* nextline=NULL;
    char* bp;
    char* btmp=NULL;
    char* tptr;
    char* escp;
    char* bptr;
    ISub buf;
    char qbuf[MAXQUOTELEN], qbuf2[MAXQUOTELEN], chsbuf[100];
    char* ptr;
    char* qptr;
    char* tmp=NULL;
    char* linetmp=NULL;
    uint qlen=0, qlen2=0;
    int wraps=0, para=0, chslev;
    bool reflow = false, quoteflag = false;
    bool quotewraphard = AA->Quotewraphard();
    bool qpencoded = getvalue and (IsQuotedPrintable(AA->Xlatimport()) or AA->StripHTML());
    bool gotmime = false;
    bool gotmultipart = false;
    bool inheader = false;
    gstrarray boundary_set;
    char boundary[100];

    *buf = *qbuf = *qbuf2 = NUL;

    if(margin < 0)
    {
        margin = -margin;
        quoteflag = true;
    }

    // Free all previously allocated lines
    line = msg->lin;
    while(line)
    {
        nextline = line->next;
        throw_xdelete(line);
        line = nextline;
    }

    msg->icc[0] = NUL;
    msg->ibcc[0] = NUL;

    msg->lines = 0;
    msg->lin = NULL;

    if(AA->attr().hex())
    {

        // Make a complete hexdump as a list of message lines
        w_info(LNG->GenHexdump);
        throw_release(msg->txt);
        line = AA->MakeDumpMsg(msg, LNG->Hexdumphead);
        if(line and msg->txt)
        {
            line = AddLine(line, "");
            line = AddLine(line, LNG->Hexdumptext);
            line = AddLine(line, "");
            ptr = msg->txt;
            uint _size = strlen(msg->txt);
            if ((AA->basetype() == "OPUS") || (AA->basetype() == "FTS1"))
                idxadjust = 190;
            for (idx=0; idx < _size; ptr+=16,idx+=16)
            {
                gsprintf(PRINTF_DECLARE_BUFFER(buf), "%04X   ", idx+idxadjust);
                HexDump16(buf+7, ptr, MinV((int)(_size-idx), 16), HEX_DUMP2);
                line = AddLine(line, buf);
            }
        }

        w_info(NULL);
    }
    else    // Convert the message text to a list of separately allocated lines
    {
        if(AA->StripHTML())
            RemoveHTML(msg->txt);
        ptr = spanfeeds(msg->txt);

        // Set default conversion table for area
        if(getvalue)
        {
            if(not strieql(AA->Xlatimport(), CFG->xlatlocalset))
            {
                strxcpy(msg->charset, AA->Xlatimport(), sizeof(msg->charset));
                level = msg->charsetlevel = LoadCharset(msg->charset, CFG->xlatlocalset);
            }
        }

        if(*ptr != NUL)
        {
            line = msg->lin = new Line();
            throw_xnew(line);

            // Alloc space for one line
            linetmp = (char*)throw_calloc(1, margin+512);
            while(*ptr)
            {

                bptr = linetmp;
                bp = bptr;
                len = 0;

                // Link previous line to this one

                if(line->prev)
                {
                    line->prev->next = line;
                    if((line->prev->type & (GLINE_HARD|GLINE_WRAP|GLINE_QUOT)) == (GLINE_HARD|GLINE_WRAP))
                    {
                        line->prev->type &= ~GLINE_HARD;
                        line->type |= GLINE_HARD;
                    }
                }

                // Reflow quotes

                if(reflow)
                {
                    len = qlen;
                    qptr = qbuf;
                    reflow = false;
                    // Insert previous quotestring
                    for(n=0; n<qlen; n++)
                    {
                        if ((level > 0) && ChsTP)
                        {
                            tptr = (char*)ChsTP[(byte)(*qptr++)];
                            chln = *tptr++;
                            while(chln--)
                            {
                                *(++bp) = *tptr++;
                            }
                        }
                        else
                        {
                            *(++bp) = *qptr++;
                        }
                    }
                    if(quotewraphard)
                    {
                        *qbuf = NUL;
                        qlen = 0;
                    }
                    ptr = spanfeeds(ptr);
                    line->type |= GLINE_QUOT|GLINE_HARD;
                }

                // Get type of line
                int isq_flag1 = 0;
                bool isq_flag2 = false;

                if(wraps == 0)
                {

                    if(gotmultipart)
                    {
                        if(*ptr == '-' and ptr[1] == '-')
                        {
                            gstrarray::iterator ib;
                            for(ib = boundary_set.begin(); ib != boundary_set.end(); ib++)
                                if(strneql(ptr+2, (*ib).c_str(), (*ib).length()))
                                {
                                    inheader = true;
                                    break;
                                }
                        }
                        else if(*ptr == '\n' or *ptr == '\r')
                            inheader = false;
                    }

                    if(inheader and (*ptr != '-'))
                        line->type |= GLINE_HIDD;

                    para = 0;
                    if(*ptr == CTRL_A or inheader)    // Found kludge/hidden line
                    {
                        para = GLINE_KLUD;
                        line->type |= GLINE_HARD;
                        if(getvalue and not CFG->ignorecharset)
                        {
                            tptr = ptr;
                            char* kludge = ptr + (*ptr == CTRL_A ? 1 : 0);
                            if(strnieql(kludge, "RFC", 3))
                            {
                                kludge += 3;
                                if(not g_isalpha(*kludge))
                                    kludge++;
                            }
                            while((*ptr != ' ') and (*ptr != ':') and *ptr)
                                ptr++;
                            char endchar = *ptr;
                            *ptr = NUL;
                            int kludgetype = -1;
                            if(strieql(kludge, "I51"))
                                kludgetype = FSC_I51;
                            else if(strieql(kludge, "CHRS") or strieql(kludge, "CHARSET"))
                                kludgetype = FSC_CHARSET;
                            else if(strieql(kludge, "CODEPAGE"))
                                kludgetype = FSC_CODEPAGE;
                            else if(strieql(kludge, "Content-Type"))
                                kludgetype = RFC_CONTENT_TYPE;
                            else if(strieql(kludge, "Content-Transfer-Encoding"))
                                kludgetype = RFC_CONTENT_TRANSFER_ENCODING;
                            else if(strieql(kludge, "X-Charset"))
                                kludgetype = RFC_X_CHARSET;
                            else if(strieql(kludge, "X-Char-Esc"))
                                kludgetype = RFC_X_CHAR_ESC;
                            *ptr = endchar;
                            if(*ptr != ' ')
                                ptr++;
                            ptr = strskip_wht(ptr);
                            char* keptr = strpbrk(ptr, "\r\n");
                            if(keptr)
                            {
                                endchar = *keptr;
                                *keptr = NUL;
                            }
                            if(kludgetype == FSC_I51)
                            {
                                msg->i51 = true;
                                // Convert FSC-0051.003 to FSC-0054.003
                                strxcpy(chsbuf, "LATIN-1", sizeof(chsbuf));
                                chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                if(not chslev)
                                {
                                    strxcpy(chsbuf, AA->Xlatimport(), sizeof(chsbuf));
                                    chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                }
                                if(chslev)
                                {
                                    level = msg->charsetlevel = chslev;
                                    strcpy(msg->charset, chsbuf);
                                }
                            }
                            else if((kludgetype == FSC_CHARSET) or (kludgetype == FSC_CODEPAGE))
                            {
                                *chsbuf = NUL;
                                qpencoded = IsQuotedPrintable(ptr);
                                if(kludgetype == FSC_CODEPAGE)
                                    strxmerge(chsbuf, sizeof(chsbuf), "CP", ptr, NULL);
                                else
                                    strxcpy(chsbuf, qpencoded ? ExtractPlainCharset(ptr) : ptr, sizeof(chsbuf));
                                // Workaround for buggy mailreaders which stores '_' in charset name
                                strchg(chsbuf,'_',' ');
                                chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                if(not chslev)
                                {
                                    strxcpy(chsbuf, AA->Xlatimport(), sizeof(chsbuf));
                                    chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                }
                                if(chslev)
                                {
                                    level = msg->charsetlevel = chslev;
                                    strcpy(msg->charset, chsbuf);
                                }
                                if(*msg->charset == NUL)
                                    strcpy(msg->charset, chsbuf);
                            }
                            else if(kludgetype == RFC_CONTENT_TYPE)
                            {
                                // Content-Type: text/plain; charset="us-ascii"
                                while(keptr and ((keptr[1] == ' ') or (keptr[1] == '\t')))
                                {
                                    *keptr = endchar;
                                    keptr = strpbrk(keptr+1, "\r\n");
                                    if(keptr)
                                    {
                                        endchar = *keptr;
                                        *keptr = NUL;
                                    }
                                }
                                const char *mime_charset = striinc("charset=", ptr);
                                if(mime_charset != NULL)
                                {
                                    if(mime_charset[8] == '\"')
                                    {
                                        strxcpy(chsbuf, mime_charset+9, sizeof(chsbuf));
                                        char *quote = strchr(chsbuf, '\"');
                                        if(quote != NULL) *quote = NUL;
                                    }
                                    else
                                        strxcpy(chsbuf, strrword(mime_charset+8), sizeof(chsbuf));
                                    if(striinc("8859", chsbuf))
                                        ISO2Latin(chsbuf, chsbuf);
                                    chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                    if(not chslev)
                                    {
                                        strxcpy(chsbuf, AA->Xlatimport(), sizeof(chsbuf));
                                        chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                    }
                                    if(chslev)
                                    {
                                        level = msg->charsetlevel = chslev;
                                        strcpy(msg->charset, chsbuf);
                                    }
                                    if(*msg->charset == NUL)
                                        strcpy(msg->charset, chsbuf);
                                    gotmime = true;
                                }
                                else if(check_multipart(ptr, boundary))
                                {
                                    boundary_set.push_back(boundary);
                                    gotmultipart = true;
                                    gotmime = true;
                                }
                            }
                            else if(kludgetype == RFC_CONTENT_TRANSFER_ENCODING)
                            {
                                if(striinc("quoted-printable", ptr))
                                {
                                    qpencoded = true;
                                    msg->charsetencoding |= GCHENC_QP;
                                    strcpy(chsbuf, MakeQuotedPrintable(msg->charset));
                                    chslev = LoadCharset(msg->charset, CFG->xlatlocalset);
                                    if(not chslev)
                                    {
                                        strxcpy(chsbuf, AA->Xlatimport(), sizeof(chsbuf));
                                        chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                    }
                                    if(chslev)
                                    {
                                        level = msg->charsetlevel = chslev;
                                        strcpy(msg->charset, chsbuf);
                                    }
                                }
                            }
                            else if(kludgetype == RFC_X_CHARSET)
                            {
                                if(not gotmime)
                                {
                                    if(striinc("8859", ptr))
                                        ISO2Latin(chsbuf, ptr);
                                    else
                                        strxcpy(chsbuf, ptr, sizeof(chsbuf));
                                    chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                    if(not chslev)
                                    {
                                        strxcpy(chsbuf, AA->Xlatimport(), sizeof(chsbuf));
                                        chslev = LoadCharset(chsbuf, CFG->xlatlocalset);
                                    }
                                    if(chslev)
                                    {
                                        level = msg->charsetlevel = chslev;
                                        strcpy(msg->charset, chsbuf);
                                    }
                                }
                            }
                            else if(kludgetype == RFC_X_CHAR_ESC)
                            {
                                if(not gotmime)
                                    msg->charsetencoding |= GCHENC_MNE;
                            }
                            if(keptr)
                                *keptr = endchar;
                            ptr = tptr;
                        }
                    }
                    else if ((isq_flag1 = is_quote(ptr)) &&
                             (isq_flag2 = is_quote2(line, ptr)))
                    {
                        para = GLINE_QUOT;
                        line->type |= GLINE_QUOT|GLINE_HARD;
                        GetQuotestr(ptr, qbuf, &qlen);
                    }
                }

                if (CFG->quoteusenewai && isq_flag1 && !isq_flag2)
                {
                    uint bad_qlen;
                    char bad_qbuf[MAXQUOTELEN];
                    char *bad_ptr;

                    GetQuotestr(bad_ptr = ptr, bad_qbuf, &bad_qlen);

                    while (true)
                    {
                        for (; *bad_ptr && (*bad_ptr != CR); bad_ptr++);
                        if (!*bad_ptr) break;

                        char *bad_head = bad_ptr;

                        bad_ptr++;
                        if (*bad_ptr == LF) bad_ptr++;

                        if (strneql(bad_qbuf, bad_ptr, bad_qlen))
                        {
                            *bad_head = ' ';

                            memmove(bad_head+1, bad_ptr+bad_qlen, strlen(bad_ptr));
                            bad_ptr = bad_head;
                        }
                        else
                            break;
                    }
                }

                // Get one line

                ch = '\0';
                tmp = NULL;
                while(*ptr and (len < (uint)margin))
                {
                    switch(*ptr)
                    {
                    case CR:
do_cr:
                        ch = CR;
                        ptr = spanfeeds(ptr+1);
                        if(wraps and not ((line->type & GLINE_HARD) and not (line->type & GLINE_QUOT)))
                        {
                            if(para != GLINE_QUOT)
                            {
                                if(quoteflag)
                                {
                                    if(para == GLINE_KLUD)
                                    {
                                        wraps = 0;
                                        break;
                                    }
                                    if(put_on_new_line(ptr))
                                    {
                                        wraps = 0;
                                        break;
                                    }
                                    char* lp = ptr;
                                    while((*lp == ' ') or (*lp == '\t'))
                                        lp++;
                                    if(*lp == CR)
                                    {
                                        wraps = 0;
                                        break;
                                    }
                                    else
                                    {
                                        ptr = lp;
                                        if(*bp != ' ')
                                        {
                                            *(++bp) = ' ';
                                            len++;
                                        }
                                    }
                                    ch = '\0';
                                    continue;
                                }
                                else
                                {
                                    wraps=0;
                                    break;
                                }
                            }
                            else
                            {
                                GetQuotestr(ptr, qbuf2, &qlen2);
                                if((*ptr != CR) and cmp_quotes(qbuf2, qbuf))
                                {
                                    char* lp = ptr + qlen2;
                                    if(is_quote(lp))
                                    {
                                        wraps = 0;
                                        para = 0;
                                        break;
                                    }
                                    else
                                    {
                                        ptr = lp;
                                        para = GLINE_QUOT;
                                        if((*ptr != ' ') and (*bp != ' '))
                                        {
                                            if(put_on_new_line(ptr))
                                            {
                                                ptr -= qlen2;
                                                wraps = 0;
                                                break;
                                            }
                                            tmp = ptr-1;
                                            btmp = bp;
                                            *(++bp) = ' ';
                                            len++;
                                        }
                                        else if(*ptr == ' ')
                                        {
                                            ptr -= qlen2;
                                            wraps = 0;
                                            break;
                                        }
                                        ch = 0;
                                        continue;
                                    }
                                }
                                else
                                {
                                    wraps = 0;
                                    para = 0;
                                    break;
                                }
                            }
                        }
                        break;
                    case 0x02:  // The I51 escape character
                        if(msg->i51 and I51Table)
                        {
                            for(n=0; n<I51Table->size; n++)
                            {
                                tptr = (char*)I51TP[n];
                                if(*(ptr+1) == tptr[0])
                                {
                                    if(*(ptr+2) == tptr[1])
                                    {
                                        escp = &tptr[2];
                                        if(*escp)
                                        {
                                            *(++bp) = *escp++;
                                            if(*escp)
                                            {
                                                *(++bp) = *escp++;
                                                len++;
                                                if(*escp)
                                                {
                                                    *(++bp) = *escp;
                                                    len++;
                                                }
                                            }
                                        }
                                        ptr += 2;
                                        n = (uint)-1;
                                        break;
                                    }
                                }
                            }
                            if(n != (uint)-1)   // I51 char not found, use fallback method
                                ptr++;
                        }
                        ptr++;
                        break;
                    case 29:  // The MNE escape character
                        if(MNETable)
                        {
                            for(n=0; n<MNETable->size; n++)
                            {
                                tptr = (char*)MNETP[n];
                                if(*(ptr+1) == tptr[0])
                                {
                                    if(*(ptr+2) == tptr[1])
                                    {
                                        escp = &tptr[2];
                                        if(*escp)
                                        {
                                            *(++bp) = *escp++;
                                            if(*escp)
                                            {
                                                *(++bp) = *escp++;
                                                len++;
                                                if(*escp)
                                                {
                                                    *(++bp) = *escp;
                                                    len++;
                                                }
                                            }
                                        }
                                        ptr += 2;
                                        n = (uint)-1;
                                        break;
                                    }
                                }
                            }
                            if(n != (uint)-1)   // MNE char not found, use fallback method
                                ptr++;
                        }
                        ptr++;
                        break;
                    case SOFTCR:
                        if (WideDispsoftcr)
                            goto defaultchardo;
                        else
                        {
                            if(CompTable)
                            {
                                if(not (isspace(*(ptr-1)) or isspace(*(ptr+1))))
                                {
                                    for(n=0; n<CompTable->size; n++)
                                    {
                                        tptr = (char*)CompTP[n];
                                        if(*(ptr-1) == tptr[0])
                                        {
                                            if(*(ptr+1) == tptr[1])
                                            {
                                                escp = &tptr[2];
                                                if(*escp)
                                                {
                                                    *(bp) = *escp++;
                                                    if(*escp)
                                                    {
                                                        *(++bp) = *escp++;
                                                        len++;
                                                        if(*escp)
                                                        {
                                                            *(++bp) = *escp;
                                                            len++;
                                                        }
                                                    }
                                                }
                                                ptr++;
                                                break;
                                            }
                                        }
                                    }
                                }
                                else if(para == GLINE_QUOT)
                                {
                                    *ptr-- = CR;   // Fake a CR
                                }
                            }
                            else if(para == GLINE_QUOT)
                            {
                                *ptr-- = CR;   // Fake a CR
                            }
                            ptr++;
                        }
                        break;
                    case LF:
                        ptr++;
                        break;
                    case CTRL_A:
                        *(++bp) = *ptr++;
                        ++len;
                        break;
                    case '=':
                        if(qpencoded)
                        {
                            if(isxdigit(ptr[1]) and isxdigit(ptr[2]))
                            {
                                // Decode the character
                                dochar = (char)((xtoi(ptr[1]) << 4) | xtoi(ptr[2]));
                                ptr += 3;
                                if(dochar == '\t')
                                {
                                    if(len >= qlen)
                                    {
                                        tmp = ptr-3;
                                        btmp = bp;
                                    }
                                    goto do_ht;
                                }
                                else if(dochar == CR)
                                    goto do_cr;
                                goto chardo;
                            }
                            else if((ptr[1] == CR) or (ptr[1] == LF))
                            {
                                // Skip soft line break
                                ptr = spanfeeds(ptr+2);
                                break;
                            }
                        }
                        goto defaultchardo;
                    case '\t':
                        if(len >= qlen)
                        {
                            tmp = ptr;
                            btmp = bp;
                        }
                        ptr++;
do_ht:
                        if(CFG->disptabsize)
                        {
                            int llen = (int)(bp-linetmp);
                            for(n=0; n<(CFG->disptabsize-(llen%CFG->disptabsize)); n++)
                            {
                                *(++bp) = ' ';
                                len++;
                            }
                        }
                        else
                        {
                            *(++bp) = ' ';
                            len++;
                        }
                        break;
                    case ' ':
                        if(len >= qlen)
                        {
                            tmp = ptr;
                            btmp = bp;
                        }
                    default:
defaultchardo:
                        dochar = *ptr++;
chardo:
                        if ((level > 0) && ChsTP)
                        {
                            tptr = (char*)ChsTP[(byte)dochar];
                            chln = *tptr++;
                            while(chln--)
                            {
                                *(++bp) = *tptr++;
                                ++len;
                            }
                        }
                        else
                        {
                            *(++bp) = dochar;
                            ++len;
                        }
                        break;
                    }
                    if(ch == CR)
                        break;
                }

                if(len == (uint)margin)
                {
                    if(*ptr == CR)
                    {
                        line->type |= GLINE_HARD;
                        wraps = 0;
                        ptr++;
                    }
                    else
                    {
                        wraps++;
                        if(para == GLINE_QUOT)
                            reflow = true;
                        line->type |= GLINE_WRAP;
                        ptr = spanfeeds(ptr);
                        if((*bp == ' ') or (isspace(*ptr) and (*ptr != LF)))
                            ptr = spanspaces(ptr);
                        else
                        {
                            if(tmp)
                            {
                                bp = btmp+1;
                                ptr = tmp+1;
                            }
                        }
                    }
                }
                else
                    line->type |= GLINE_HARD;

                *(++bp) = NUL;

                // Get line length
                uint tmplinelength = (uint)(bp-bptr);
                if(tmplinelength > (uint)(margin + 512))
                {
                    LOG.ErrPointer();
                    LOG.printf("! A message line length (%u bytes) exceeded an internal buffer limit of %u bytes", tmplinelength, margin+512);
                    LOG.printf(": Message line text: %s", bptr+1);
                    PointerErrorExit();
                }

                // Store line
                line->txt = linetmp+1;

                // Set line color and type
                if((line->type & GLINE_QUOT) and not is_quote(line->txt.c_str()))
                    line->type ^= GLINE_QUOT;
                if(line->type & GLINE_QUOT)
                    line->color = quotecolor(line->txt.c_str());
                else if(inheader)
                    line->color = C_READT;
                else
                    line->color = C_READW;

                Line* prevline = line;
                line = new Line();
                throw_xnew(line);
                line->prev = prevline;

                ptr = spanfeeds(ptr);
            }

            throw_release(linetmp);
            throw_xdelete(line);

            // Charset translate header fields
            if(header_recode)
            {
                strxmimecpy(msg->by, msg->by, level, sizeof(INam), true);
                strxmimecpy(msg->to, msg->to, level, sizeof(INam), true);
                if(not (msg->attr.frq() or msg->attr.att() or msg->attr.urq()))
                    strxmimecpy(msg->re, msg->re, level, sizeof(ISub), true);
            }

            // Scan msg body top for RFC headerlines
            int irfcbody = AA->Internetrfcbody();
            if(irfcbody != 0)
            {
                if(msg->lin)
                {
                    Line* linep = msg->lin;
                    int headerlines = 0;
                    while(linep)
                    {
                        if(not linep->txt.empty())
                        {
                            const char* tptr = linep->txt.c_str();
                            if(*tptr != CTRL_A)
                            {
                                int kludgetype = ScanLine(msg, linep, tptr, getvalue, MASK_RFC);
                                if(kludgetype)
                                {
                                    if(kludgetype == HEADERLINE)
                                    {
                                        linep->type |= GLINE_HIDD;
                                    }
                                    headerlines++;
                                    if(linep->type & GLINE_HIDD)
                                        linep->color = C_READKH;
                                    else
                                        linep->color = C_READK;
                                    if(linep->next)
                                    {
                                        char lwsp = *linep->next->txt.c_str();
                                        while((lwsp == ' ') or (lwsp == '\t') or (linep->type & GLINE_WRAP))
                                        {
                                            linep = linep->next;
                                            linep->type |= linep->prev->type & (GLINE_KLUD|GLINE_HIDD);
                                            linep->color = linep->prev->color;
                                            if(linep->next)
                                                lwsp = *linep->next->txt.c_str();
                                            else
                                                break;
                                        }
                                    }
                                }
                                else
                                    break;
                            }
                        }
                        else
                        {
                            if(headerlines)
                            {
                                linep->type |= GLINE_KLUD;
                                linep->kludge = GKLUD_RFC;
                                linep->color = C_READK;
                            }
                            if(--irfcbody == 0)
                                break;
                        }
                        if(linep->type & GLINE_WRAP)
                        {
                            while(linep and (linep->type & GLINE_WRAP))
                            {
                                linep = linep->next;
                                if(linep)
                                {
                                    linep->type |= linep->prev->type & (GLINE_KLUD|GLINE_HIDD);
                                    linep->color = linep->prev->color;
                                }
                            }
                        }
                        if(linep)
                            linep = linep->next;
                    }
                }
            }

            // Scan for kludge-, tear- and originlines
            ScanKludges(msg, getvalue);

            // Try to fix the following situation:
            // Messagebase From field: fido7@da.ru
            // @REPLYADDR: fido7@da.ru
            // @REPLYTO: 2:5020/52 Sergey Kitsya
            // From: fido7@da.ru
            if(getvalue and streql(msg->by, msg->iaddr) and not *msg->realby and *msg->igate)
            {
                ptr = strchr(msg->igate, ' ');
                if(ptr)
                {
                    ptr = strskip_wht(ptr);
                    if(not isuucp(ptr))
                        strcpy(msg->realby, ptr);
                }
            }
        }
    }

    // Make the index to the line index as allowed by config
    MsgLineReIndex(msg);
}


//  ------------------------------------------------------------------

void MsgLineReIndex(GMsg* msg, int viewhidden, int viewkludge, int viewquote)
{

    if(viewhidden == -1)
        viewhidden = AA->Viewhidden();
    if(viewkludge == -1)
        viewkludge = AA->Viewkludge();
    if(viewquote == -1)
        viewquote = AA->Viewquote();

    int x;
    Line* line;

    throw_xrelease(msg->line);
    line = msg->lin;
    msg->lines = 0;
    while(line)
    {
        msg->lines++;
        if(line->next)
        {
            if(line->type & GLINE_QUOT)
            {
                if(not (line->next->type & GLINE_QUOT) and not strblank(line->next->txt.c_str()))
                {
                    if(CFG->switches.get(quotespacing))
                    {
                        line = AddLine(line, "");
                        msg->lines++;
                    }
                }
            }
            else
            {
                const char *posn, *posn2 = line->txt.c_str();
                if(msg->attr.pos() and ((posn = striinc("@position", posn2)) != NULL))
                {
                    line->txt.erase(posn - posn2, 9);
                    line->type |= GLINE_POSI;
                }
                if((line->next->type & GLINE_QUOT) and not strblank(line->txt.c_str()))
                {
                    if(CFG->switches.get(quotespacing))
                    {
                        line = AddLine(line, "");
                        msg->lines++;
                    }
                }
            }
        }
        line = line->next;
    }

    msg->line = (Line**)throw_xcalloc(msg->lines+2, sizeof(Line*)); // FIXME: Memory Leak

    x = 0;
    msg->lines = 0;

    char qbuf[MAXQUOTELEN];
    char qbuf0[MAXQUOTELEN];
    uint qlen = 0;
    int qmatches = 0;

    *qbuf0 = NUL;
    for(line = msg->lin; line != NULL; line = line->next)
    {
        if(line->type & GLINE_KLUD)
        {
            *qbuf0 = NUL;
            qmatches = 0;
            if(not viewkludge)
            {
                continue;
            }
        }
        else if(line->type & GLINE_HIDD)
        {
            *qbuf0 = NUL;
            qmatches = 0;
            if(not viewhidden)
            {
                continue;
            }
        }
        else if (line->type & GLINE_TXTH)
        {
            *qbuf0 = NUL;
            qmatches = 0;
            continue;
        }
        else if(line->type & GLINE_QUOT)
        {
            if(not viewquote)
            {
                GetQuotestr(line->txt.c_str(), qbuf, &qlen);
                if(not cmp_quotes(qbuf0, qbuf))
                {
                    strcpy(qbuf0, qbuf);
                    qmatches = 0;
                }
                const char *p = line->txt.c_str()+qlen;
                while(*p != NUL)
                {
                    if(isxalnum(*p))
                    {
                        qmatches++;
                        break;
                    }
                    ++p;
                }
                if(qmatches != 1)
                {
                    continue;
                }
            }
        }
        else
        {
            *qbuf0 = NUL;
            qmatches = 0;
        }
        msg->line[x++] = line;
        msg->lines++;
    }

    msg->line[x] = NULL;  // Mark end of index

    // Calculate quote percent
    int quotes = 0, nonquotes = 0;

    line = msg->lin;
    while(line)
    {
        if(not (line->type & (GLINE_KLUDGE|GLINE_TEAR|GLINE_ORIG)))
        {
            if(line->txt.c_str())
            {
                int n = line->txt.length();
                nonquotes += n;
                if(line->type & GLINE_QUOT)
                    quotes += n;
            }
        }
        line = line->next;
    }
    msg->quotepct = 100-Pct(nonquotes, quotes);
}


//  ------------------------------------------------------------------

bool IsQuotedPrintable(const char *encoding)
{

    if(striinc("LATIN1QP", encoding))
        return true;
    else
    {
        const char *lencoding = strlword(encoding);
        int len = strlen(lencoding);
        if((len > 2) and strnieql("QP", lencoding+len-2, 2))
            return true;
    }
    return false;
}


//  ------------------------------------------------------------------

static char qpencodingbuf[100];

char *MakeQuotedPrintable(const char *encoding)
{

    strxmerge(qpencodingbuf, sizeof(qpencodingbuf), strlword(encoding), "QP", NULL);
    return qpencodingbuf;
}

//  ------------------------------------------------------------------

char *ExtractPlainCharset(const char *encoding)
{

    strxcpy(qpencodingbuf, strlword(encoding), sizeof(qpencodingbuf));
    int len = strlen(qpencodingbuf);
    if(len > 2)
        qpencodingbuf[len-2] = NUL;
    return qpencodingbuf;
}


//  ------------------------------------------------------------------

char *Latin2ISO(char *iso_encoding, const char *latin_encoding)
{

    static const char *isono[] = { "15", "1", "2", "3", "4", "9", "10", "13", "14", "15" };
    int chsno = atoi(latin_encoding+5);
    if(chsno < 0) chsno = -chsno; // support for both latin-1 and latin1
    chsno = chsno > sizeof(isono)/sizeof(const char *) ? 0 : chsno;
    return strxmerge(iso_encoding, 12, "iso-8859-", isono[chsno], NULL);
}


//  ------------------------------------------------------------------

char *ISO2Latin(char *latin_encoding, const char *iso_encoding)
{

    static const char *latinno[] = { NULL, "1", "2", "3", "4", NULL, NULL, NULL, NULL, "5", "6", NULL, NULL, "7", "8", "9" };
    int chsno = atoi(strstr(iso_encoding, "8859")+5);
    chsno = chsno > sizeof(latinno)/sizeof(const char *) ? 0 : chsno;
    if(latinno[chsno] == NULL)
        return strxmerge(latin_encoding, 12, iso_encoding, NULL);
    else
        return strxmerge(latin_encoding, 8, "latin-", latinno[chsno], NULL);
}


//  ------------------------------------------------------------------

static bool CheckLevel(const char* imp, const char* imp2, int n, int &current_table)
{
    const char *ptr = striinc(imp2, imp);
    ptr += strlen(imp2);
    strskip_wht(ptr);

    int level = atoi(ptr);

    if (CharTable && (n == current_table) && (level <= CharTable->level))
        return true;

    gfile fp(AddPath(CFG->goldpath, CFG->xlatged), "rb", CFG->sharemode);
    if (fp.isopen())
    {
        if (!CharTable) CharTable = (Chs*)throw_calloc(1, sizeof(Chs));
        fp.FseekSet(n, sizeof(Chs));
        fp.Fread(CharTable, sizeof(Chs));

        ChsTP = CharTable->t;
        current_table = n;

        // Disable softcr translation unless DISPSOFTCR is enabled
        if (not WideDispsoftcr)
        {
            char* tptr = (char*)ChsTP[SOFTCR];
            *tptr++ = 1;
            *tptr = SOFTCR;
        }

        if (level <= CharTable->level) return true;
    }

    return false;
}


//  ------------------------------------------------------------------

int LoadCharset(const char* imp, const char* exp, int query)
{

    static int current_table = -1;
    int n;

    switch(query)
    {
    case 1:
        return current_table;
    default:
        break;
    }

#ifdef HAS_ICONV
    if( iconv_cd != (iconv_t)(-1) )
        iconv_close(iconv_cd);
    iconv_cd = iconv_open(exp, imp);
    if(iconv_cd != (iconv_t)(-1) )
        LOG.printf("+ iconv is initialised to convert from %s to %s", imp, exp);
    else
        LOG.printf("+ Can't initialise iconv to convert from %s to %s", imp, exp);
#endif

    // Find and load charset table
    std::vector<Map>::iterator xlt;
    for(n = 0, xlt = CFG->xlatcharset.begin(); xlt != CFG->xlatcharset.end(); xlt++, n++)
    {
        if (!striinc(xlt->exp, exp)) continue;

        bool imp_found = make_bool(strnieql(xlt->imp, imp, strlen(xlt->imp)));
        if (imp_found) imp_found = CheckLevel(imp, xlt->imp, n, current_table);

        std::vector< std::pair<std::string, gstrarray> >::iterator als;
        for (als = CFG->xlatcharsetalias.begin();
                !imp_found && (als != CFG->xlatcharsetalias.end()); als++)
        {
            if (strieql(xlt->imp, als->first.c_str()))
            {
                for (gstrarray::iterator it = als->second.begin(); !imp_found && (it != als->second.end()); it++)
                    if (striinc(it->c_str(), imp))
                        imp_found = CheckLevel(imp, it->c_str(), n, current_table);
            }
        }

        if (imp_found) return CharTable->level;
    }

    // No matching table found
    throw_release(CharTable);
    ChsTP = NULL;
    current_table = -1;
    return 0;
}


//  ------------------------------------------------------------------

Line* DeleteLine(Line* line)
{

    Line* nextline = NULL;

    // Link next and previous lines and release this line

    if(line)
    {
        if(line->prev)
        {
            line->prev->next = line->next;
            nextline = line->prev;
        }
        if(line->next)
        {
            line->next->prev = line->prev;
            nextline = line->next;
        }
        throw_xdelete(line);
    }

    return nextline;
}


//  ------------------------------------------------------------------

Line* InsertLine(Line* newline, Line* oldline, int pos)
{

    if(oldline)
    {
        if(pos >= DIR_BELOW)
        {
            newline->prev = oldline;
            newline->next = oldline->next;
            if(oldline->next)
                oldline->next->prev = newline;
            oldline->next = newline;
        }
        else
        {
            newline->prev = oldline->prev;
            newline->next = oldline;
            if(oldline->prev)
                oldline->prev->next = newline;
            oldline->prev = newline;
        }
    }
    return newline;
}


//  ------------------------------------------------------------------

Line* FirstLine(Line* line)
{

    if(line)
        while(line->prev)
            line = line->prev;
    return line;
}


//  ------------------------------------------------------------------

Line* LastLine(Line* line)
{

    if(line)
        while(line->next)
            line = line->next;
    return line;
}


//  ------------------------------------------------------------------

Line* AddLine(Line* line, const char* buf, int where)
{

    Line* newline = new Line(buf);
    throw_xnew(newline);
    newline->type = GLINE_HARD;
    newline->color = C_READW;

    return InsertLine(newline, line, where);
}


//  ------------------------------------------------------------------

Line* AddLine(Line* line, const char* buf)
{

    return AddLine(line, buf, DIR_BELOW);
}


//  ------------------------------------------------------------------

Line* AddLineFast(Line* oldline, const char* text)
{

    Line* newline = new Line(text);
    throw_xnew(newline);
    newline->type = GLINE_HARD;
    newline->color = C_READW;
    newline->prev = oldline;
    newline->next = oldline->next;
    if(oldline->next)
        oldline->next->prev = newline;
    oldline->next = newline;
    return newline;
}


//  ------------------------------------------------------------------

Line* AddKludge(Line* line, const char* buf, int where)
{

    Line* newline = new Line(buf);
    throw_xnew(newline);
    newline->type = GLINE_HARD|GLINE_KLUD;
    newline->color = C_READK;
    return InsertLine(newline, line, where);
}


//  ------------------------------------------------------------------

Line* AddLineF(Line*& line, const char* format, ...)
{

    char buf[256];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buf, format, argptr);
    va_end(argptr);
    line = AddLine(line, buf);
    return line;
}


//  ------------------------------------------------------------------

Line* AddHexdump(Line*& line, void* data, size_t datalen)
{

    char buf[256];
    uint pos = 0;
    char* ptr = (char*)data;

    while (pos < datalen)
    {
        uint dataleft = datalen - pos;
        gsprintf(PRINTF_DECLARE_BUFFER(buf), "%04X   ", pos);
        HexDump16(buf+7, ptr, (dataleft < 16) ? dataleft : 16, HEX_DUMP2);
        line = AddLine(line, buf);
        ptr += 16;
        pos += 16;
    }

    return line;
}


//  ------------------------------------------------------------------

char* ParseInternetAddr(char* __string, char* __name, char* __addr, bool detect_charset)
{

    *__name = *__addr = NUL;
    char* commaptr = NULL;

    if(strchr(__string, ','))
    {
        bool inquotes = false;
        commaptr = __string;
        while(commaptr)
        {
            if(*commaptr == '\"')
                inquotes = not inquotes;
            else if((*commaptr == ',') and not inquotes)
            {
                *commaptr = NUL;
                break;
            }
            if(not *commaptr)
                commaptr = NULL;
            else
                commaptr++;
        }
    }

    char* p;
    if((p = strrchr(__string, '>')) != NULL)
        *(++p) = NUL;

    char* endchar = __string + strlen(__string) - 1;
    if(*endchar == /*(*/ ')')
    {
        char* begchar = endchar;
        int pcnt = 0;
        while(begchar > __string)
        {
            if(*begchar == /*(*/ ')')
                pcnt++;
            else if(*begchar == '(' /*)*/)
                pcnt--;
            if(pcnt == 0)
                break;
            begchar--;
        }
        if(*begchar == '(' /*)*/)
            begchar++;
        strbtrim(strxcpy(__name, begchar, MinV((size_t)(endchar-begchar)+1, (size_t)sizeof(INam))));
        strbtrim(strxcpy(__addr, __string, MinV((size_t)(begchar-__string), (size_t)sizeof(IAdr))));
    }
    else if(*endchar == '>')
    {
        char* endaddr = endchar;
        while(*endchar != '<' and endchar > __string)
            endchar--;
        char* begaddr = endchar;
        if(*endchar == '<')
        {
            begaddr++;
            if (endchar > __string) endchar--;
        }
        __string = strskip_wht(__string);
        strbtrim(strxcpy(__name, __string, MinV((size_t)(endchar-__string)+1, (size_t)sizeof(INam))));
        strbtrim(strxcpy(__addr, begaddr, MinV((size_t)(endaddr-begaddr)+1, (size_t)sizeof(IAdr))));
    }
    else
    {
        strxcpy(__addr, __string, sizeof(IAdr));
    }

    if(*__addr == '@')
    {
        char* ptr = strchr(__addr, ':');
        if(ptr)
            memmove(__addr, ptr+1, strlen(ptr));
    }

    if(commaptr)
        *commaptr = ',';

    StripQuotes(__name);

    if(not strchr(__addr, '@'))
        *__addr = NUL;

    strxmimecpy(__name, __name, 0, strlen(__name)+1, detect_charset);

    return __name;
}


//  ------------------------------------------------------------------

void InvalidateControlInfo(GMsg* msg)
{

    Line* line = msg->lin;
    while(line)
    {

        if(not (line->type & (GLINE_TEAR | GLINE_ORIG)))
        {
            bool invalidated = false;
            // Invalidate tearline
            if(not CFG->invalidate.tearline.first.empty())
                invalidated |= doinvalidate(line->txt, CFG->invalidate.tearline.first, CFG->invalidate.tearline.second, true);
            else
                invalidated |= doinvalidate(line->txt, "---", "-+-", true);

            // Invalidate originline
            if(not CFG->invalidate.origin.first.empty())
                invalidated |= doinvalidate(line->txt, CFG->invalidate.origin.first.c_str(), CFG->invalidate.origin.second.c_str());
            else
                invalidated |= doinvalidate(line->txt, " * Origin: ", " + Origin: ");

            // Invalidate SEEN-BY's
            if(not CFG->invalidate.seenby.first.empty())
                invalidated |= doinvalidate(line->txt, CFG->invalidate.seenby.first.c_str(), CFG->invalidate.seenby.second.c_str());
            else
                invalidated |= doinvalidate(line->txt, "SEEN-BY: ", "SEEN+BY: ");

            if(invalidated)
            {
                line->type &= ~GLINE_KLUDGE;
                line->kludge = 0;
                line->color = C_READW;
            }

        }
        line = line->next;
    }
}


//  ------------------------------------------------------------------
