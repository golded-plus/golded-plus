//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 2000 Alexander S. Aganichev
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
//  Synchronet message base
//  ------------------------------------------------------------------

#include <cerrno>
#include <gcrcall.h>
#include <gstrall.h>
#include <gmemall.h>
#include <gmemdbg.h>
#include <gdbgerr.h>
#include <gdbgtrk.h>
#include <gmosmb.h>


//  ------------------------------------------------------------------

smb_t* smbdata = NULL;
int smbdatano = 0;


//  ------------------------------------------------------------------

void SMBExit() {

  throw_release(smbdata);
}


//  ------------------------------------------------------------------

void SMBInit() {

  smbdata = (smb_t *)throw_calloc(3, sizeof(smb_t));
}


//  ------------------------------------------------------------------

void SMBArea::data_open() {

  data = smbdata + (smbdatano++);
  strxcpy(data->file, path(), sizeof(data->file) - 3);
  data->sdt_fp = data->shd_fp = data->sid_fp = data->sda_fp = data->sha_fp = NULL;
  data->retry_time = 1;
  data->last_error[0] = NUL;
}


//  ------------------------------------------------------------------

void SMBArea::data_close() {

  smbdatano--;
  data = NULL;
}


//  ------------------------------------------------------------------
//  Open the Synchronet message base

void SMBArea::open() {

  GFTRK("SMBOpen");

  isopen++;
  if(isopen > 2) {
    WideLog->ErrTest();
    WideLog->printf("! Trying to open a Synchronet msgbase more than twice.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }
  if(isopen == 1) {
    if(ispacked()) {
      isopen--;
      const char* newpath = Unpack(path());
      if(newpath == NULL)
        packed(false);
      set_real_path(newpath ? newpath : path());
      isopen++;
    }
    data_open();

    int _tries = 0;

    for(;;) {
      if(smb_open(data) != 0) {

        if((errno != EACCES) or (PopupLocked(++_tries, false, data->file) == false)) {

          // User requested to exit
          WideLog->ErrOpen();
          WideLog->printf("! Synchronet message base could not be opened (%s).", data->last_error);
          WideLog->printf(": %s", path());
          WideLog->ErrOSInfo();
          OpenErrorExit();
        }
      }
      else
        break;
    };

    // Remove the popup window
    if(_tries)
      PopupLocked(0, 0, NULL);

    if(not fsize(data->shd_fp)) {
      data->status.max_crcs = 0;
      data->status.max_age = 0;
      data->status.max_msgs = 1000;
      data->status.attr = 0;
      if(smb_create(data) != 0) {
        smb_close(data);

        WideLog->ErrOpen();
        WideLog->printf("! Synchronet message base could not be created (%s).", data->last_error);
        WideLog->printf(": %s", path());
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
    scan();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SMBArea::close()
{
  GFTRK("SMBClose");

  if(isopen) {
    if(isopen == 1) {
      smb_close(data);
      data_close();
      if(ispacked()) {
        CleanUnpacked(real_path());
      }
    }
    isopen--;
  }
  else {
    WideLog->ErrTest();
    WideLog->printf("! Trying to close an already closed Synchronet msgbase.");
    WideLog->printf(": %s, %s.", echoid(), path());
    WideLog->printf("+ Info: This indicates a potentially serious bug.");
    WideLog->printf("+ Advice: Report to the Author immediately.");
    TestErrorExit();
  }

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SMBArea::suspend()
{
  smb_close(data);
}


//  ------------------------------------------------------------------

void SMBArea::resume()
{
  int _tries = 0;

  for(;;) {
    if(smb_open(data) != 0) {

      if((errno != EACCES) or (PopupLocked(++_tries, false, data->file) == false)) {

        // User requested to exit
        WideLog->ErrOpen();
        WideLog->printf("! Synchronet message base could not be opened (%s).", data->last_error);
        WideLog->printf(": %s", path());
        WideLog->ErrOSInfo();
        OpenErrorExit();
      }
    }
    else
      break;
  };

  // Remove the popup window
  if(_tries)
    PopupLocked(0, 0, NULL);

  if(not fsize(data->shd_fp)) {
    data->status.max_crcs = 0;
    data->status.max_age = 0;
    data->status.max_msgs = 1000;
    data->status.attr = 0;
    if(smb_create(data) != 0) {
      smb_close(data);

      WideLog->ErrOpen();
      WideLog->printf("! Synchronet message base could not be created (%s).", data->last_error);
      WideLog->printf(": %s", path());
      WideLog->ErrOSInfo();
      OpenErrorExit();
    }
  }
}


//  ------------------------------------------------------------------

void SMBArea::lock()
{
}


//  ------------------------------------------------------------------

void SMBArea::unlock()
{
}


//  ------------------------------------------------------------------

int SMBArea::load_hdr(gmsg* __msg, smbmsg_t *smsg)
{
  GFTRK("SMBLoadHdr");

  smbmsg_t local_smsg, *smsgp;
  smsgp = smsg ? smsg : &local_smsg;
  ulong reln = Msgn->ToReln(__msg->msgno);
  if(reln == 0) {
    GFTRK(NULL);
    return false;
  }
  fseek(data->sid_fp, (reln - 1L) * sizeof(idxrec_t), SEEK_SET);
  if(not fread(&smsgp->idx, 1, sizeof(idxrec_t), data->sid_fp) or (smb_lockmsghdr(data, smsgp) != 0)) {
    GFTRK(NULL);
    return false;
  }
  int rv = smb_getmsghdr(data, smsgp);
  smb_unlockmsghdr(data, smsgp);
  if(rv != 0) {
    GFTRK(NULL);
    return false;
  }
  __msg->link.to_set(smsgp->hdr.thread_orig);
  __msg->link.next_set(smsgp->hdr.thread_next);
  __msg->link.first_set(smsgp->hdr.thread_first);

  strxcpy(__msg->by, (char *)smsgp->from, 36);
  strxcpy(__msg->to, (char *)smsgp->to, 36);
  strxcpy(__msg->re, (char *)smsgp->subj, 72);

  if(smsgp->from_net.type == NET_FIDO) {
    __msg->orig.zone  = __msg->oorig.zone  = ((fidoaddr_t *)smsgp->from_net.addr)->zone;
    __msg->orig.net   = __msg->oorig.net   = ((fidoaddr_t *)smsgp->from_net.addr)->net;
    __msg->orig.node  = __msg->oorig.node  = ((fidoaddr_t *)smsgp->from_net.addr)->node;
    __msg->orig.point = __msg->oorig.point = ((fidoaddr_t *)smsgp->from_net.addr)->point;
  }
  else {
    __msg->orig.zone  = __msg->oorig.zone  = 0;
    __msg->orig.net   = __msg->oorig.net   = 0;
    __msg->orig.node  = __msg->oorig.node  = 0;
    __msg->orig.point = __msg->oorig.point = 0;
  }
  if(smsgp->to_net.type == NET_FIDO) {
    __msg->dest.zone  = __msg->odest.zone  = ((fidoaddr_t *)smsgp->to_net.addr)->zone;
    __msg->dest.net   = __msg->odest.net   = ((fidoaddr_t *)smsgp->to_net.addr)->net;
    __msg->dest.node  = __msg->odest.node  = ((fidoaddr_t *)smsgp->to_net.addr)->node;
    __msg->dest.point = __msg->odest.point = ((fidoaddr_t *)smsgp->to_net.addr)->point;
  }
  else {
    __msg->dest.zone  = __msg->odest.zone  = 0;
    __msg->dest.net   = __msg->odest.net   = 0;
    __msg->dest.node  = __msg->odest.node  = 0;
    __msg->dest.point = __msg->odest.point = 0;
  }

  // Transfer attributes
  __msg->attr.pvt(smsgp->hdr.attr & MSG_PRIVATE);
  __msg->attr.rcv(smsgp->hdr.attr & MSG_READ);
  __msg->attr.lok(smsgp->hdr.attr & MSG_LOCKED);
  __msg->attr.del(smsgp->hdr.attr & MSG_DELETE);
  __msg->attr.r_o(smsgp->hdr.attr & MSG_PERMANENT);
  __msg->attr.loc(smsgp->hdr.netattr & MSG_LOCAL);
  __msg->attr.cra(smsgp->hdr.netattr & MSG_CRASH);
  __msg->attr.imm(smsgp->hdr.netattr & MSG_IMMEDIATE);
  __msg->attr.dir(smsgp->hdr.netattr & MSG_DIRECT);
  __msg->attr.hld(smsgp->hdr.netattr & MSG_HOLD);
  __msg->attr.snt(smsgp->hdr.netattr & MSG_SENT);
  __msg->attr.trs(smsgp->hdr.netattr & MSG_INTRANSIT);
  __msg->attr.orp(smsgp->hdr.netattr & MSG_ORPHAN);
  __msg->attr.k_s(smsgp->hdr.netattr & MSG_KILLSENT);
  __msg->attr.a_s(smsgp->hdr.netattr & MSG_ARCHIVESENT);
  __msg->attr.zon(smsgp->hdr.netattr & MSG_GATE);
  __msg->attr.att(smsgp->hdr.auxattr & MSG_FILEATTACH);
  __msg->attr.frq(smsgp->hdr.auxattr & MSG_FILEREQUEST);
  __msg->attr.rrq(smsgp->hdr.auxattr & MSG_RECEIPTREQ);
  __msg->attr.cfm(smsgp->hdr.auxattr & MSG_CONFIRMREQ);
  __msg->attr.tfs(smsgp->hdr.auxattr & MSG_TRUNCFILE);

  time_t a = smsgp->hdr.when_written.time;
  struct tm *tp = gmtime(&a);
  tp->tm_isdst = -1;
  time_t b = mktime(tp);
  __msg->written = a + a - b;
  a = smsgp->hdr.when_imported.time;
  b = mktime(gmtime(&a));
  __msg->arrived = a + a - b;
  __msg->received = 0;

  if(not smsg)
    smb_freemsgmem(smsgp);
  GFTRK(NULL);
  return true;
}


//  ------------------------------------------------------------------

int SMBArea::load_msg(gmsg* msg)
{
  smbmsg_t smsg;
  ushort xlat;
  uchar *inbuf;
  long outlen;
  char buf[512];
  int i;
  bool lzh;
  bool tail = true;
  ulong l;
  ulong txt_len = 1;

  GFTRK("SMBLoadMsg");

  if(not load_hdr(msg, &smsg)) {
    GFTRK(NULL);
    return false;
  }

  msg->txt = throw_strdup("");

  if(smsg.from_net.type == NET_INTERNET) {
    sprintf(buf, "\001From: %s (%s)\r", (char *)smsg.from_net.addr, smsg.from);
    outlen = strlen(buf);
    msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
    strcpy(msg->txt+txt_len-1, buf);
    txt_len+=outlen;
  }
  if(smsg.to_net.type == NET_INTERNET) {
    sprintf(buf, "\001To: %s (%s)\r", (char *)smsg.to_net.addr, smsg.to);
    outlen = strlen(buf);
    msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
    strcpy(msg->txt+txt_len-1, buf);
    txt_len+=outlen;
  }

  for(i = 0; i < smsg.total_hfields; i++)
    switch(smsg.hfield[i].type) {
      case RFC822MSGID:
        sprintf(buf, "\001Message-ID: %s\r", (char *)smsg.hfield_dat[i]);
        goto add;
      case FIDOMSGID:
        sprintf(buf, "\001MSGID: %s\r", (char *)smsg.hfield_dat[i]);
        goto add;
      case RFC822REPLYID:
        sprintf(buf, "\001In-Reply-To: %s\r", (char *)smsg.hfield_dat[i]);
        goto add;
      case FIDOREPLYID:
        sprintf(buf, "\001REPLY: %s\r", (char *)smsg.hfield_dat[i]);
        goto add;
      case FIDOPID:
        sprintf(buf, "\001PID: %s\r", (char *)smsg.hfield_dat[i]);
        strcpy(msg->pid, (char *)smsg.hfield_dat[i]);
        goto add;
      case FIDOAREA:
        sprintf(buf, "\001AREA: %s\r", (char *)smsg.hfield_dat[i]);
        goto add;
      case FIDOFLAGS:
        sprintf(buf, "\001FLAGS: %s\r", (char *)smsg.hfield_dat[i]);
        goto add;
      case FIDOCTRL:
        if(not strncmp((char *)smsg.hfield_dat[i], "Via ", 4) or not strncmp((char *)smsg.hfield_dat[i], "Recd ", 5))
          break;
      case RFC822HEADER:
        if(not strncmp((char *)smsg.hfield_dat[i], "Via:", 4))
          break;
        sprintf(buf, "\001%s\r", (char *)smsg.hfield_dat[i]);
add:
        outlen = strlen(buf);
        msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
        strcpy(msg->txt+txt_len-1, buf);
        txt_len+=outlen;
        break;
      default:
        break;
    }

  for(i = 0; i < smsg.hdr.total_dfields; i++)
    switch(smsg.dfield[i].type) {
      case TEXT_BODY:
        if(tail)
          tail = false;
        goto common;
      case TEXT_TAIL:
        if(not tail) {
          tail = true;
          msg->txt[txt_len-1] = '\r';
          txt_len++;
        }
common:
        fseek(data->sdt_fp, smsg.hdr.offset + smsg.dfield[i].offset, SEEK_SET);
        fread(&xlat, 2, 1, data->sdt_fp);
        l = 2;
        lzh = false;
        while(xlat != XLAT_NONE) {
          if(xlat == XLAT_LZH)
            lzh = true;
          fread(&xlat, 2, 1, data->sdt_fp);
          l += 2;
        }
        if(lzh) {
          inbuf = (uchar *)throw_xmalloc(smsg.dfield[i].length);
          fread(inbuf, smsg.dfield[i].length - l, 1, data->sdt_fp);
          outlen = *(long *)inbuf;
          msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
          lzh_decode(inbuf, smsg.dfield[i].length - l, (uchar *)(msg->txt+txt_len-1));
          throw_xfree(inbuf);
        } else {
          outlen = smsg.dfield[i].length - l;
          msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
          fread(msg->txt+txt_len-1, smsg.dfield[i].length - l, 1, data->sdt_fp);
        }
        txt_len+=outlen;
        msg->txt[txt_len-1] = NUL;
        break;
    }

  for(i = 0; i < smsg.total_hfields; i++)
    switch(smsg.hfield[i].type) {
      case FIDOPATH:
        sprintf(buf, "\r\001PATH: %s", (char *)smsg.hfield_dat[i]);
        goto add2;
      case FIDOSEENBY:
        sprintf(buf, "\rSEEN-BY: %s", (char *)smsg.hfield_dat[i]);
        goto add2;
      case FIDOCTRL:
        if(strncmp((char *)smsg.hfield_dat[i], "Via ", 4) and strncmp((char *)smsg.hfield_dat[i], "Recd ", 5))
          break;
        sprintf(buf, "\r\001%s", (char *)smsg.hfield_dat[i]);
add2:
        outlen = strlen(buf);
        msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
        strcpy(msg->txt+txt_len-1, buf);
        txt_len+=outlen;
        break;
      default:
        break;
    }


  smb_freemsgmem(&smsg);

  GFTRK(NULL);
  return true;
}


//  ------------------------------------------------------------------

void SMBArea::save_hdr(int mode, gmsg* msg)
{
  int rv;
  char ch;
  bool done, cr;
  ulong l, m, bodylen, taillen, crc;
  char *fbuf, *sbody, *stail;
  char buf[256];
  smbmsg_t smsg;
  fidoaddr_t destaddr, origaddr;

  GFTRK("SMBSaveHdr");

  smb_getstatus(data);
  memset(&smsg, 0, sizeof(smbmsg_t));
  if(not (mode & GMSG_NEW)) {
    ulong reln = Msgn->ToReln(msg->msgno);
    if(reln == 0) {
      GFTRK(NULL);
      return;
    }
    fseek(data->sid_fp, (reln - 1L) * sizeof(idxrec_t), SEEK_SET);
    if(not fread(&smsg.idx, 1, sizeof(idxrec_t), data->sid_fp) or (smb_lockmsghdr(data, &smsg) != 0)) {
      GFTRK(NULL);
      return;
    }
    int rv = smb_getmsghdr(data, &smsg);
    smb_unlockmsghdr(data, &smsg);
    if(rv != 0) {
      GFTRK(NULL);
      return;
    }
    smsg.hdr.attr = 0;
    smsg.hdr.netattr = 0;
    smsg.hdr.auxattr = 0;
  }
  else {
    memcpy(smsg.hdr.id, "SHD\x1a", 4);
    smsg.hdr.version = smb_ver();
    struct tm *tp = gmtime(&msg->written);
    tp->tm_isdst = -1;
    smsg.hdr.when_written.time = mktime(tp);
  }
  smsg.hdr.when_imported.time = time(NULL);

  // Transfer attributes
  if(msg->attr.pvt()) smsg.hdr.attr |= MSG_PRIVATE;
  if(msg->attr.rcv()) smsg.hdr.attr |= MSG_READ;
  if(msg->attr.lok()) smsg.hdr.attr |= MSG_LOCKED;
  if(msg->attr.del()) smsg.hdr.attr |= MSG_DELETE;
  if(msg->attr.r_o()) smsg.hdr.attr |= MSG_PERMANENT;
  if(msg->attr.loc()) smsg.hdr.netattr |= MSG_LOCAL;
  if(msg->attr.cra()) smsg.hdr.netattr |= MSG_CRASH;
  if(msg->attr.imm()) smsg.hdr.netattr |= MSG_IMMEDIATE;
  if(msg->attr.dir()) smsg.hdr.netattr |= MSG_DIRECT;
  if(msg->attr.hld()) smsg.hdr.netattr |= MSG_HOLD;
  if(msg->attr.snt()) smsg.hdr.netattr |= MSG_SENT;
  if(msg->attr.trs()) smsg.hdr.netattr |= MSG_INTRANSIT;
  if(msg->attr.orp()) smsg.hdr.netattr |= MSG_ORPHAN;
  if(msg->attr.k_s()) smsg.hdr.netattr |= MSG_KILLSENT;
  if(msg->attr.a_s()) smsg.hdr.netattr |= MSG_ARCHIVESENT;
  if(msg->attr.zon()) smsg.hdr.netattr |= MSG_GATE;
  if(msg->attr.att()) smsg.hdr.auxattr |= MSG_FILEATTACH;
  if(msg->attr.frq()) smsg.hdr.auxattr |= MSG_FILEREQUEST;
  if(msg->attr.rrq()) smsg.hdr.auxattr |= MSG_RECEIPTREQ;
  if(msg->attr.cfm()) smsg.hdr.auxattr |= MSG_CONFIRMREQ;
  if(msg->attr.tfs()) smsg.hdr.auxattr |= MSG_TRUNCFILE;

  smsg.hdr.thread_orig = msg->link.to();
  smsg.hdr.thread_next = msg->link.next();
  smsg.hdr.thread_first = msg->link.first();

  if((mode & GMSG_UPDATE) and not (mode & GMSG_TXT)) {
    if(mode & GMSG_NEW)
      smb_addmsghdr(data, &smsg, data->status.attr & SMB_HYPERALLOC);
    else
      smb_putmsghdr(data, &smsg);
    smb_freemsgmem(&smsg);
    GFTRK(NULL);
    return;
  }

  if(not (mode & GMSG_NEW)) {
    if(not (data->status.attr & SMB_HYPERALLOC)) {
      if(smb_open_da(data) == 0) {
        if(smb_open_ha(data) == 0) {
          smb_freemsg(data, &smsg);
          smb_close_ha(data);
        }
        smb_close_da(data);
      }
    }
    else {
      smb_freemsg(data, &smsg);
    }
    smb_freemsgmem(&smsg);
    smsg.dfield = NULL;
    smsg.hdr.total_dfields = 0;
    smsg.total_hfields = 0;
    smsg.hfield = NULL;
    smsg.hfield_dat = NULL;
  }

  ushort net = NET_FIDO;
  origaddr.zone = msg->orig.zone;
  origaddr.net = msg->orig.net;
  origaddr.node = msg->orig.node;
  origaddr.point = msg->orig.point;
  smb_hfield(&smsg, SENDERNETTYPE, sizeof(ushort), &net);
  smb_hfield(&smsg, SENDERNETADDR, sizeof(fidoaddr_t), &origaddr);

  destaddr.zone = msg->dest.zone;
  destaddr.net = msg->dest.net;
  destaddr.node = msg->dest.node;
  destaddr.point = msg->dest.point;
  smb_hfield(&smsg, RECIPIENTNETTYPE, sizeof(ushort), &net);
  smb_hfield(&smsg, RECIPIENTNETADDR, sizeof(fidoaddr_t), &destaddr);

  smb_hfield(&smsg, SENDER, strlen(msg->by), msg->by);
  strcpy(buf, msg->by);
  strlwr(buf);
  smsg.idx.from = strCrc16(buf);

  smb_hfield(&smsg, RECIPIENT, strlen(msg->to), msg->to);
  strcpy(buf, msg->to);
  strlwr(buf);
  smsg.idx.to = strCrc16(buf);

  smb_hfield(&smsg, SUBJECT, strlen(msg->re), msg->re);
  strcpy(buf, msg->re);
  strlwr(buf);
  smsg.idx.subj = strCrc16(buf);

  // calculate maximum possible size of sbody/stail
  for(l = 0, fbuf = msg->txt; *fbuf != NUL; l++, fbuf++)
    if(l == CR)
      ++l;

  fbuf = msg->txt;
  sbody = (char *)throw_malloc(SDT_BLOCK_LEN*smb_datblocks(l));
  stail = (char *)throw_malloc(SDT_BLOCK_LEN*smb_datblocks(l));

  for(l = bodylen = taillen = done = 0, cr = true; (ch = fbuf[l]) != NUL; l++) {
    if(ch == CTRL_A and cr) {
      ++l;
      if(not strncmp(fbuf+l, "MSGID:", 6)) {
        l += 6;
        while(fbuf[l] and fbuf[l] == ' ') l++;
        m = l;
        while(fbuf[m] and not iscntrl(fbuf[m])) m++;
        if(m != l)
          smb_hfield(&smsg, FIDOMSGID, m-l, fbuf+l);
      }
      else if(not strncmp(fbuf+l, "REPLY:", 6)) {
        l += 6;
        while(fbuf[l] and fbuf[l] == ' ') l++;
        m = l;
        while(fbuf[m] and not iscntrl(fbuf[m])) m++;
        if(m != l)
          smb_hfield(&smsg, FIDOREPLYID, m-l, fbuf+l);
      }
      else if(not strncmp(fbuf+l, "FLAGS:", 6)) {
        l += 6;
        while(fbuf[l] and fbuf[l] == ' ') l++;
        m = l;
        while(fbuf[m] and not iscntrl(fbuf[m])) m++;
        if(m != l)
          smb_hfield(&smsg, FIDOFLAGS, m-l, fbuf+l);
      }
      else if(not strncmp(fbuf+l, "PATH:", 5)) {
        l += 5;
        while(fbuf[l] and fbuf[l] == ' ') l++;
        m = l;
        while(fbuf[m] and not iscntrl(fbuf[m])) m++;
        if(m != l)
          smb_hfield(&smsg, FIDOPATH, m-l, fbuf+l);
      }
      else if(not strncmp(fbuf+l, "PID:", 4)) {
        l += 4;
        while(fbuf[l] and fbuf[l] == ' ') l++;
        m = l;
        while(fbuf[m] and not iscntrl(fbuf[m])) m++;
        if(m != l)
          smb_hfield(&smsg, FIDOPID, m-l, fbuf+l);
      }
      else {
        if(strncmp(fbuf+l, "TOPT ", 5) and strncmp(fbuf+l, "FMPT ", 5) and
           strncmp(fbuf+l, "INTL ", 5)) {
          while(fbuf[l] and fbuf[l] == ' ') l++;
          m = l;
          while(fbuf[m] and not iscntrl(fbuf[m])) m++;
          if(m != l)
            smb_hfield(&smsg, FIDOCTRL, m-l, fbuf+l);
        }
      }
      while(fbuf[l] and fbuf[l]!=CR) l++;
      continue;
    }
    if(ch != LF and ch != SOFTCR) {
      if(cr) {
        if(not done and not strncmp(fbuf+l, "--- ", 4) or not strncmp(fbuf+l, "---\r", 4))
          done = true; // tear line and down go into tail
        else if(done and not strncmp(fbuf+l, "SEEN-BY:", 8)) {
          l += 8;
          while(fbuf[l] and fbuf[l] == ' ') l++;
          m = l;
          while(fbuf[m] and not iscntrl(fbuf[m])) m++;
          if(m != l)
            smb_hfield(&smsg, FIDOSEENBY, m-l, fbuf+l);
          while(fbuf[l] and fbuf[l]!=CR) l++;
          continue;
        }
        else if(not done and not strncmp(fbuf+l, " * Origin: ", 11))
          done = true;
      }
      if(done)
        stail[taillen++] = ch;
      else
        sbody[bodylen++] = ch;
      if(ch == CR) {
        cr = true;
        if(done)
          stail[taillen++] = LF;
        else
          sbody[bodylen++] = LF;
      }
      else
        cr = false;
    }
  }
  if(bodylen>=2 && sbody[bodylen-2]==CR && sbody[bodylen-1]==LF)
    bodylen-=2; // remove last CRLF if present

  crc = ~memCrc32(sbody, bodylen, false, CRC32_MASK_CCITT);
  rv = smb_addcrc(data, crc);

  while(taillen and (iscntrl(stail[taillen-1]) or isspace(stail[taillen-1])))
    taillen--;

  l = bodylen+2;
  if(taillen)
    l += (taillen+2);

  if(not (data->status.attr & SMB_HYPERALLOC)) {
    if(smb_open_da(data) == 0) {
      smsg.hdr.offset = smb_allocdat(data, l, 1);
      smb_close_da(data);
    }
    else
      smsg.hdr.offset = (ulong)-1L;
  }
  else {
    smsg.hdr.offset = smb_hallocdat(data);
  }

  if(smsg.hdr.offset != (ulong)-1L) {
    fseek(data->sdt_fp, smsg.hdr.offset, SEEK_SET);
    ushort xlat = XLAT_NONE;
    fwrite(&xlat, 2, 1, data->sdt_fp);
    l = ftell(data->sdt_fp);
    fwrite(sbody, SDT_BLOCK_LEN, smb_datblocks(bodylen), data->sdt_fp);
    if(taillen) {
      fseek(data->sdt_fp, l+bodylen, SEEK_SET);
      fwrite(&xlat, 2, 1, data->sdt_fp);
      fwrite(stail, SDT_BLOCK_LEN, smb_datblocks(taillen), data->sdt_fp);
    }
    fflush(data->sdt_fp);
    smb_dfield(&smsg, TEXT_BODY, bodylen+2);
    if(taillen)
      smb_dfield(&smsg, TEXT_TAIL, taillen+2);

    if(mode & GMSG_NEW) {
      smb_addmsghdr(data, &smsg, data->status.attr & SMB_HYPERALLOC);
      Msgn->Append(smsg.hdr.number);
    }
    else
      smb_putmsghdr(data, &smsg);
  }
  throw_xfree(sbody);
  throw_xfree(stail);
  smb_freemsgmem(&smsg);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SMBArea::save_msg(int mode, gmsg* msg)
{
  GFTRK("SMBSaveMsg");

  save_hdr(mode | GMSG_HDRTXT, msg);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SMBArea::del_msg(gmsg* msg)
{
  GFTRK("SMBDelMsg");

  smbmsg_t smsg;
  ulong reln = Msgn->ToReln(msg->msgno);
  if(reln == 0) {
    GFTRK(NULL);
    return;
  }
  fseek(data->sid_fp, (reln - 1L) * sizeof(idxrec_t), SEEK_SET);
  if(not fread(&smsg.idx, 1, sizeof(idxrec_t), data->sid_fp) or (smb_lockmsghdr(data, &smsg) != 0)) {
    GFTRK(NULL);
    return;
  }
  if(smb_getmsghdr(data, &smsg) == 0) {
    smsg.hdr.attr |= MSG_DELETE;
    int rv = smb_putmsghdr(data, &smsg);
    smb_unlockmsghdr(data, &smsg);
    if(rv == 0)
      msg->attr.del1();
  }
  else
    smb_unlockmsghdr(data, &smsg);
  smb_freemsgmem(&smsg);

  GFTRK(NULL);
}


//  ------------------------------------------------------------------

void SMBArea::new_msgno(gmsg* msg)
{
  int res = smb_getstatus(data);
  smb_unlocksmbhdr(data);
  msg->msgno = (res == 0) ? data->status.last_msg+1 : 0;
}


//  ------------------------------------------------------------------

char* SMBArea::user_lookup(char* lookfor)
{
  NW(lookfor);
  return NULL;
}


//  ------------------------------------------------------------------

int SMBArea::renumber()
{
  return false;
}


//  ------------------------------------------------------------------

void SMBArea::update_timesread(gmsg* msg)
{
  NW(msg);
  return;
}


//  ------------------------------------------------------------------

static char *binstr(char *buf, ushort length)
{
  static char str[128];
  char tmp[128];
  int i;

  str[0] = 0;
  for(i = 0; i < length; i++)
    if(buf[i] and not isprint(buf[i]))
      break;
  if(i == length)		/* not binary */
    return buf;

  if(length > 42)
    length = 42;

  for(i = 0; i < length; i++) {
    sprintf(tmp, "%02X ", buf[i]);
    strcat(str, tmp);
  }
  return str;
}


//  ------------------------------------------------------------------

static char *faddrtoa(fidoaddr_t addr)
{
  static char str[25];
  char point[25];

  sprintf(str, "%u:%u/%u", addr.zone, addr.net, addr.node);
  if (addr.point) {
    sprintf(point, ".%u", addr.point);
    strcat(str, point);
  }
  return str;
}


//  ------------------------------------------------------------------

Line* SMBArea::make_dump_msg(Line*& lin, gmsg* msg, char* lng_head)
{

  GFTRK("SMBMakeDump");

  Line* line = lin =
  AddLine (NULL, "Hexdump of Synchronet-style message header and text");
  AddLineF(line, "------------------------------------------------------------------------------");
  line = AddLine(line, "");

  smbmsg_t smsg;
  char buf[512];
  int i;

  GFTRK("SMBLoadMsg");

  if(not load_hdr(msg, &smsg)) {
    line = AddLine(line, "Error loading header");
    GFTRK(NULL);
    return line;
  }

  line = AddLineF(line, "Subject           : %s", smsg.subj);
  line = AddLineF(line, "From              : %s", smsg.from);
  line = AddLineF(line, "To                : %s", smsg.to);
  line = AddLineF(line, "Type              : %04Xh", smsg.hdr.type);
  line = AddLineF(line, "Version           : %04Xh", smsg.hdr.version);
  line = AddLineF(line, "Length            : %u", smsg.hdr.length);
  line = AddLineF(line, "Attr              : %04Xh", smsg.hdr.attr);
  line = AddLineF(line, "AUXAttr           : %04lXh", smsg.hdr.auxattr);
  line = AddLineF(line, "NetAttr           : %04lXh", smsg.hdr.netattr);
  stpcpy(buf, ctime((time_t *)&smsg.hdr.when_written.time))[-1] = NUL;
  line = AddLineF(line, "Written           : %s", buf);
  stpcpy(buf, ctime((time_t *)&smsg.hdr.when_imported.time))[-1] = NUL;
  line = AddLineF(line, "Imported          : %s", buf);
  line = AddLineF(line, "Number            : %ld (%ld)", smsg.hdr.number, (long)(ftell(data->sid_fp)/sizeof(idxrec_t)));
  line = AddLineF(line, "Thread orig       : %ld", smsg.hdr.thread_orig);
  line = AddLineF(line, "Thread next       : %ld", smsg.hdr.thread_next);
  line = AddLineF(line, "Thread first      : %ld", smsg.hdr.thread_first);
  line = AddLineF(line, "Reserved          : %s", HexDump16(buf, (const char*)smsg.hdr.reserved, 16, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"));
  line = AddLineF(line, "Offset            : %06lXh", smsg.hdr.offset);
  line = AddLineF(line, "Total dfields     : %u", smsg.hdr.total_dfields);

  if(smsg.from_net.type) {
    line = AddLineF(line, "From net type     : %02Xh", smsg.from_net.type);
    line = AddLineF(line, "From net addr     : %s", (char *)(smsg.from_net.type == NET_FIDO ? faddrtoa(*(fidoaddr_t *)smsg.from_net.addr) : smsg.from_net.addr));
  }

  if(smsg.to_net.type) {
    line = AddLineF(line, "To net type       : %02Xh", smsg.to_net.type);
    line = AddLineF(line, "To net addr       : %s", (char *)(smsg.to_net.type == NET_FIDO ? faddrtoa(*(fidoaddr_t *)smsg.to_net.addr) : smsg.to_net.addr));
  }

  if(smsg.replyto_net.type) {
    line = AddLineF(line, "Replyto net type  : %02Xh", smsg.replyto_net.type);
    line = AddLineF(line, "Replyto net addr  : %s", (char *)(smsg.replyto_net.type == NET_FIDO ? faddrtoa(*(fidoaddr_t *)smsg.replyto_net.addr) : smsg.replyto_net.addr));
  }

  line = AddLineF(line, "From agent        : %02Xh", smsg.from_agent);
  line = AddLineF(line, "To agent          : %02Xh", smsg.to_agent);
  line = AddLineF(line, "Replyto agent     : %02Xh", smsg.replyto_agent);

  line = AddLine(line, "");
  AddLineF(line, "dfields:");
  line = AddLine(line, "");

  for (i = 0; i < smsg.hdr.total_dfields; i++) {
    line = AddLineF(line, "dfield[%02u].type   : %02X", i, smsg.dfield[i].type);
    line = AddLineF(line, "dfield[%02u].offset : %lu", i, smsg.dfield[i].offset);
    line = AddLineF(line, "dfield[%02u].length : %lu", i, smsg.dfield[i].length);
  }

  line = AddLine(line, "");
  AddLineF(line, "hfields:");
  line = AddLine(line, "");

  for (i = 0; i < smsg.total_hfields; i++) {
    line = AddLineF(line, "hfield[%02u].type   : %02X", i, smsg.hfield[i].type);
    line = AddLineF(line, "hfield[%02u].length : %d", i, smsg.hfield[i].length);
    line = AddLineF(line, "hfield_dat[%02u]    : %s", i, binstr((char *)smsg.hfield_dat[i], smsg.hfield[i].length));
  }

  line = AddLine(line, "");
  AddLineF(line, lng_head);
  line = AddLine(line, "");

  int _count = 0;
  char* _ptr = (char*)&smsg.hdr;
  while(_count < sizeof(msghdr_t)) {
    sprintf(buf, "%04X   ", _count);
    HexDump16(buf+7, _ptr, 16, HEX_DUMP2);
    line = AddLine(line, buf);
    _count += 16;
    _ptr += 16;
  }
  sprintf(buf, "%04X   ", _count);
  HexDump16(buf+7, _ptr, sizeof(msghdr_t)%16, HEX_DUMP2);
  line = AddLine(line, buf);

  ushort xlat;
  uchar *inbuf;
  long outlen;
  bool lzh;
  bool tail = true;
  ulong l;
  ulong txt_len = 1;

  msg->txt = throw_strdup("");

  for(i = 0; i < smsg.hdr.total_dfields; i++)
    switch(smsg.dfield[i].type) {
      case TEXT_BODY:
        if(tail)
          tail = false;
        goto common;
      case TEXT_TAIL:
        if(not tail) {
          tail = true;
          msg->txt[txt_len-1] = '\r';
          txt_len++;
        }
common:
        fseek(data->sdt_fp, smsg.hdr.offset + smsg.dfield[i].offset, SEEK_SET);
        fread(&xlat, 2, 1, data->sdt_fp);
        l = 2;
        lzh = false;
        while(xlat != XLAT_NONE) {
          if(xlat == XLAT_LZH)
            lzh = true;
          fread(&xlat, 2, 1, data->sdt_fp);
          l += 2;
        }
        if(lzh) {
          inbuf = (uchar *)throw_xmalloc(smsg.dfield[i].length);
          fread(inbuf, smsg.dfield[i].length - l, 1, data->sdt_fp);
          outlen = *(long *)inbuf;
          msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
          lzh_decode(inbuf, smsg.dfield[i].length - l, (uchar *)(msg->txt+txt_len-1));
          throw_xfree(inbuf);
        } else {
          outlen = smsg.dfield[i].length - l;
          msg->txt = (char *)throw_realloc(msg->txt, txt_len+outlen);
          fread(msg->txt+txt_len-1, smsg.dfield[i].length - l, 1, data->sdt_fp);
        }
        txt_len+=outlen;
        msg->txt[txt_len-1] = NUL;
        break;
    }

  smb_freemsgmem(&smsg);

  GFTRK(NULL);

  return line;
}


//  ------------------------------------------------------------------
