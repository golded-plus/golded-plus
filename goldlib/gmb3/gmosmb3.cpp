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
//  Based on freeware sources from Digital Dynamics
//  ------------------------------------------------------------------
//  Synchronet message base
//  ------------------------------------------------------------------

#include <cerrno>
#include <gmemdbg.h>
#include <gmosmb.h>


//  ------------------------------------------------------------------
//  Open a message base
//  If retry_time is 0, fast open method (no compatibility/validity check)
//  Opens files for READing messages or updating message indices only

int SMBArea::smb_open(int retry_time)
{
  int file;
  smbhdr_t hdr;

  data->shd_fp = data->sdt_fp = data->sid_fp = NULL;
  if ((data->shd_fp = fsopen(AddPath(path(), ".shd"), "rb+", WideSharemode)) == NULL)
    return (2);
  file = fileno(data->shd_fp);
  if (retry_time && filelength(file) >= sizeof(smbhdr_t)) {
    setvbuf(data->shd_fp, data->shd_buf, _IONBF, SHD_BLOCK_LEN);
    if (smb_locksmbhdr(retry_time)) {
      smb_close();
      return (-1);
    }
    memset(&hdr, 0, sizeof(smbhdr_t));
    fread(&hdr, sizeof(smbhdr_t), 1, data->shd_fp);
    if (memcmp(hdr.id, "SMB\x1a", 4)) {
      smb_close();
      return (-2);
    }
    if (hdr.version < 0x110) { // Compatibility check
      smb_close();
      return (-3);
    }
    smb_unlocksmbhdr();
    rewind(data->shd_fp);
  }
  if ((data->sdt_fp = fsopen(AddPath(path(), ".sdt"), "rb+", WideSharemode)) == NULL) {
    smb_close();
    return (1);
  }
  if ((data->sid_fp = fsopen(AddPath(path(), ".sid"), "rb+", WideSharemode)) == NULL) {
    smb_close();
    return (3);
  }
  return (0);
}


//  ------------------------------------------------------------------
//  Closes the currently open message base

void SMBArea::smb_close(void)
{
  if (data->shd_fp != NULL) {
    // In case it's been locked
    smb_unlocksmbhdr();
    fclose(data->shd_fp);
  }
  if (data->sid_fp != NULL)
    fclose(data->sid_fp);
  if (data->sdt_fp != NULL)
    fclose(data->sdt_fp);
  data->sid_fp = data->shd_fp = data->sdt_fp = NULL;
}


//  ------------------------------------------------------------------
//  Opens the data block allocation table message base 'smb_file'
//  Retrys for retry_time number of seconds
//  Return 0 on success, non-zero otherwise

int SMBArea::smb_open_da(int retry_time)
{
  data->sda_fp = smb_openexlusively(AddPath(path(), ".sda"), retry_time);
  return data->sda_fp == NULL ? 1 : 0;
}


//  ------------------------------------------------------------------
//  Opens the header block allocation table for message base 'smb_file'
//  Retrys for retry_time number of seconds
//  Return 0 on success, non-zero otherwise

int SMBArea::smb_open_ha(int retry_time)
{
  data->sha_fp = smb_openexlusively(AddPath(path(), ".sha"), retry_time);
  return data->sha_fp == NULL ? 1 : 0;
}


//  ------------------------------------------------------------------
//  Truncates header file
//  Retrys for retry_time number of seconds
//  Return 0 on success, non-zero otherwise

int SMBArea::smb_trunchdr(int retry_time)
{
  long start;

  start = time(NULL);
  rewind(data->shd_fp);
  while (1) {
    if (not chsize(fileno(data->shd_fp), 0L))
      break;
    if (errno != EACCES)
      return (-1);
    if (time(NULL) - start >= retry_time) // Time-out
      return (-2);
  }
  return (0);
}


//  ------------------------------------------------------------------
//  Message Base Header Functions

//  ------------------------------------------------------------------
//  Attempts for retry_time number of seconds to lock the message base hdr

int SMBArea::smb_locksmbhdr(int retry_time)
{
  dword start;

  if(WideCanLock) {
    start = time(NULL);
    while (1) {
      if (not ::lock(fileno(data->shd_fp), 0L, sizeof(smbhdr_t) + sizeof(smbstatus_t)))
        return (0);
      if (time(NULL) - start >= retry_time)
        break; // Incase we've already locked it
      ::unlock(fileno(data->shd_fp), 0L, sizeof(smbhdr_t) + sizeof(smbstatus_t));
    }
    return (-1);
  }
  return (0);
}


//  ------------------------------------------------------------------
//  Read the SMB header from the header file and place into "status"

int SMBArea::smb_getstatus(smbstatus_t *status)
{
  int i;

  clearerr(data->shd_fp);
  fseek(data->shd_fp, sizeof(smbhdr_t), SEEK_SET);
  i = fread(status, 1, sizeof(smbstatus_t), data->shd_fp);
  if (i == sizeof(smbstatus_t))
    return (0);
  return (1);
}


//  ------------------------------------------------------------------
//  Writes message base header

int SMBArea::smb_putstatus(smbstatus_t status)
{
  int i;

  clearerr(data->shd_fp);
  fseek(data->shd_fp, sizeof(smbhdr_t), SEEK_SET);
  i = fwrite(&status, 1, sizeof(smbstatus_t), data->shd_fp);
  fflush(data->shd_fp);
  if (i == sizeof(smbstatus_t))
    return (0);
  return (1);
}


//  ------------------------------------------------------------------
//  Unlocks previously locks message base header

int SMBArea::smb_unlocksmbhdr()
{
  if(not WideCanLock) return 0;
  return (::unlock(fileno(data->shd_fp), 0L, sizeof(smbhdr_t) + sizeof(smbstatus_t)));
}


//  ------------------------------------------------------------------
//  Individual Message Functions

//  ------------------------------------------------------------------
//  Attempts for retry_time number of seconds to lock the header for 'msg'

int SMBArea::smb_lockmsghdr(smbmsg_t msg, int retry_time)
{
  dword start;

  if(WideCanLock) {
    start = time(NULL);
    while (1) {
      if (not ::lock(fileno(data->shd_fp), msg.idx.offset, sizeof(msghdr_t)))
        return (0);
      if (time(NULL) - start >= retry_time)
        break;
      ::unlock(fileno(data->shd_fp), msg.idx.offset, sizeof(msghdr_t));
    }
    return (-1);
  }
  return 0;
}


//  ------------------------------------------------------------------
//  Fills msg->idx with message index based on msg->hdr.number
//  OR if msg->hdr.number is 0, based on msg->offset (record offset).
//  if msg.hdr.number does not equal 0, then msg->offset is filled too.
//  Either msg->hdr.number or msg->offset must be initialized before
//  calling this function
//  Returns 1 if message number wasn't found, 0 if it was

int SMBArea::smb_getmsgidx(smbmsg_t *msg)
{
  idxrec_t idx;
  dword l, length, total, bot, top;

  clearerr(data->sid_fp);
  if (not msg->hdr.number) {
    fseek(data->sid_fp, msg->offset * sizeof(idxrec_t), SEEK_SET);
    if (not fread(&msg->idx, sizeof(idxrec_t), 1, data->sid_fp))
      return (1);
    return (0);
  }
  length = filelength(fileno(data->sid_fp));
  if (not length)
    return (1);
  total = length / sizeof(idxrec_t);
  if (not total)
    return (1);

  bot = 0;
  top = total;
  l = total / 2; // Start at middle index
  while (1) {
    fseek(data->sid_fp, l * sizeof(idxrec_t), SEEK_SET);
    if (not fread(&idx, sizeof(idxrec_t), 1, data->sid_fp))
      return (1);
    if (bot == top - 1 and idx.number != msg->hdr.number)
      return (1);
    if (idx.number > msg->hdr.number) {
      top = l;
      l = bot + ((top - bot) / 2);
      continue;
    }
    if (idx.number < msg->hdr.number) {
      bot = l;
      l = top - ((top - bot) / 2);
      continue;
    }
    break;
  }
  msg->idx = idx;
  msg->offset = l;
  return (0);
}


//  ------------------------------------------------------------------
//  Reads the last index record in the open message base

int SMBArea::smb_getlastidx(idxrec_t *idx)
{
  long length;

  clearerr(data->sid_fp);
  length = filelength(fileno(data->sid_fp));
  if (length < sizeof(idxrec_t))
    return (-1);
  fseek(data->sid_fp, length - sizeof(idxrec_t), SEEK_SET);
  if (not fread(idx, sizeof(idxrec_t), 1, data->sid_fp))
    return (-2);
  return (0);
}


//  ------------------------------------------------------------------
//  Figures out the total length of the header record for 'msg'
//  Returns length

uint SMBArea::smb_getmsghdrlen(smbmsg_t msg)
{
  int i;

  // fixed portion
  msg.hdr.length = sizeof(msghdr_t);
  // data fields
  msg.hdr.length += msg.hdr.total_dfields * sizeof(dfield_t);
  // header fields
  for (i = 0; i < msg.total_hfields; i++) {
    msg.hdr.length += sizeof(hfield_t);
    msg.hdr.length += msg.hfield[i].length;
  }
  return (msg.hdr.length);
}


//  ------------------------------------------------------------------
//  Figures out the total length of the data buffer for 'msg'
//  Returns length

dword SMBArea::smb_getmsgdatlen(smbmsg_t msg)
{
  int i;
  dword length = 0L;

  for (i = 0; i < msg.hdr.total_dfields; i++)
    length += msg.dfield[i].length;
  return (length);
}


//  ------------------------------------------------------------------
//  Read header information into 'msg' structure
//  msg->idx.offset must be set before calling this function
//  Must call smb_freemsgmem() to free memory allocated for var len strs
//  Returns 0 on success, non-zero if error

int SMBArea::smb_getmsghdr(smbmsg_t *msg)
{
  word i;
  dword l, offset;
  idxrec_t idx;

  rewind(data->shd_fp);
  fseek(data->shd_fp, msg->idx.offset, SEEK_SET);
  idx = msg->idx;
  offset = msg->offset;
  memset(msg, 0, sizeof(smbmsg_t));
  msg->idx = idx;
  msg->offset = offset;
  if (not fread(&msg->hdr, sizeof(msghdr_t), 1, data->shd_fp))
    return (-1);
  if (memcmp(msg->hdr.id, "SHD\x1a", 4))
    return (-2);
  if (msg->hdr.version < 0x110)
    return (-9);
  l = sizeof(msghdr_t);
  msg->dfield = (dfield_t *)throw_xmalloc(msg->hdr.total_dfields*sizeof(dfield_t));
  i = 0;
  while (i < msg->hdr.total_dfields and l < msg->hdr.length) {
    if (not fread(&msg->dfield[i], sizeof(dfield_t), 1, data->shd_fp)) {
      smb_freemsgmem(*msg);
      return (-4);
    }
    i++;
    l += sizeof(dfield_t);
  }
  if (i < msg->hdr.total_dfields) {
    smb_freemsgmem(*msg);
    return (-8);
  }
  while (l < msg->hdr.length) {
    i = msg->total_hfields++;
    msg->hfield_dat = (void **)throw_xrealloc(msg->hfield_dat, msg->total_hfields*sizeof(void *));
    msg->hfield = (hfield_t *)throw_xrealloc(msg->hfield, msg->total_hfields*sizeof(hfield_t));
    if (not fread(&msg->hfield[i], sizeof(hfield_t), 1, data->shd_fp)) {
      smb_freemsgmem(*msg);
      return (-5);
    }
    l += sizeof(hfield_t);
    msg->hfield_dat[i] = (char *)throw_xmalloc(msg->hfield[i].length + 1);
    memset(msg->hfield_dat[i], 0, msg->hfield[i].length + 1); // init to NULL
    if (msg->hfield[i].length and not fread(msg->hfield_dat[i], msg->hfield[i].length, 1, data->shd_fp)) {
      smb_freemsgmem(*msg);
      return (-6);
    }
    switch (msg->hfield[i].type) { // convenience variables
      case SENDER:
        if (not msg->from) {
          msg->from = (uchar *)msg->hfield_dat[i];
          break;
        }
      case FORWARDED: // fall through
        msg->forwarded = 1;
        break;
      case SENDERAGENT:
        if (not msg->forwarded)
          msg->from_agent = *(word *)msg->hfield_dat[i];
        break;
      case SENDEREXT:
        if (not msg->forwarded)
          msg->from_ext = (uchar *)msg->hfield_dat[i];
        break;
      case SENDERNETTYPE:
        if (not msg->forwarded)
          msg->from_net.type = *(word *)msg->hfield_dat[i];
        break;
      case SENDERNETADDR:
        if (not msg->forwarded)
          msg->from_net.addr = (char *)msg->hfield_dat[i];
        break;
      case REPLYTO:
        msg->replyto = (uchar *)msg->hfield_dat[i];
        break;
      case REPLYTOEXT:
        msg->replyto_ext = (uchar *)msg->hfield_dat[i];
        break;
      case REPLYTOAGENT:
        msg->replyto_agent = *(word *)msg->hfield_dat[i];
        break;
      case REPLYTONETTYPE:
        msg->replyto_net.type = *(word *)msg->hfield_dat[i];
        break;
      case REPLYTONETADDR:
        msg->replyto_net.addr = (char *)msg->hfield_dat[i];
        break;
      case RECIPIENT:
        msg->to = (uchar *)msg->hfield_dat[i];
        break;
      case RECIPIENTEXT:
        msg->to_ext = (uchar *)msg->hfield_dat[i];
        break;
      case RECIPIENTAGENT:
        msg->to_agent = *(word *)msg->hfield_dat[i];
        break;
      case RECIPIENTNETTYPE:
        msg->to_net.type = *(word *)msg->hfield_dat[i];
        break;
      case RECIPIENTNETADDR:
        msg->to_net.addr = (char *)msg->hfield_dat[i];
        break;
      case SUBJECT:
        msg->subj = (uchar *)msg->hfield_dat[i];
        break;
    }
    l += msg->hfield[i].length;
  }

  if (not msg->from or not msg->to or not msg->subj) {
    smb_freemsgmem(*msg);
    return (-7);
  }
  return (0);
}


//  ------------------------------------------------------------------
//  Frees memory allocated for 'msg'

void SMBArea::smb_freemsgmem(smbmsg_t msg)
{
  word i;

  throw_xfree(msg.dfield);
  for (i = 0; i < msg.total_hfields; i++)
    throw_xfree(msg.hfield_dat[i]);
  throw_xfree(msg.hfield);
  throw_xfree(msg.hfield_dat);
}


//  ------------------------------------------------------------------
//  Unlocks header for 'msg'

int SMBArea::smb_unlockmsghdr(smbmsg_t msg)
{
  if(not WideCanLock) return 0;
  return (::unlock(fileno(data->shd_fp), msg.idx.offset, sizeof(msghdr_t)));
}


//  ------------------------------------------------------------------
//  Adds a header field to the 'msg' structure (in memory only)

int SMBArea::smb_hfield(smbmsg_t * msg, word type, word length, void *data)
{
  int i;

  i = msg->total_hfields;
  msg->hfield = (hfield_t *)throw_xrealloc(msg->hfield, (i+1)*sizeof(hfield_t));
  msg->hfield_dat = (void **)throw_xrealloc(msg->hfield_dat, (i+1)*sizeof(void *));
  msg->total_hfields++;
  msg->hfield[i].type = type;
  msg->hfield[i].length = length;
  if (length) {
    msg->hfield_dat[i] = (void *)throw_xmalloc(length);
    memcpy(msg->hfield_dat[i], data, length);
  } else
    msg->hfield_dat[i] = NULL;
  return (0);
}


//  ------------------------------------------------------------------
//  Adds a data field to the 'msg' structure (in memory only)
//  Automatically figures out the offset into the data buffer from existing
//  dfield lengths

int SMBArea::smb_dfield(smbmsg_t * msg, word type, dword length)
{
  int i, j;

  i = msg->hdr.total_dfields;
  msg->dfield = (dfield_t *)throw_xrealloc(msg->dfield, (i+1)*sizeof(dfield_t));
  msg->hdr.total_dfields++;
  msg->dfield[i].type = type;
  msg->dfield[i].length = length;
  for (j = msg->dfield[i].offset = 0; j < i; j++)
    msg->dfield[i].offset += msg->dfield[j].length;
  return (0);
}


//  ------------------------------------------------------------------
//  Checks CRC history file for duplicate crc. If found, returns 1.
//  If no dupe, adds to CRC history and returns 0, or negative if error.

int SMBArea::smb_addcrc(dword max_crcs, dword crc, int retry_time)
{
  int file;
  long length;
  dword l, *buf;

  if (not max_crcs)
    return (0);
  file = smb_openexlusively2(AddPath(path(), ".sch"), retry_time);
  length = filelength(file);
  if (length < 0L) {
    ::close(file);
    return (-4);
  }
  buf = (dword *)throw_xmalloc(max_crcs * sizeof(dword));
  if (length >= max_crcs * 4) { // Reached or exceeds max crcs
    read(file, buf, max_crcs * 4);
    for (l = 0; l < max_crcs; l++)
      if (crc == buf[l])
        break;
    if (l < max_crcs) { // Dupe CRC found
      ::close(file);
      throw_xfree(buf);
      return (1);
    }
    chsize(file, 0L); // truncate it
    lseek(file, 0L, SEEK_SET);
    write(file, buf + 4, (max_crcs - 1) * 4);
  } else if (length / 4) { // Less than max crcs
    read(file, buf, length);
    for (l = 0; l < length / 4; l++)
      if (crc == buf[l])
        break;
    if (l < length / 4) { // Dupe CRC found
      ::close(file);
      throw_xfree(buf);
      return (1);
    }
  }
  lseek(file, 0L, SEEK_END);
  write(file, &crc, 4); // Write to the end
  throw_xfree(buf);
  ::close(file);
  return (0);
}


//  ------------------------------------------------------------------
//  Creates a new message header record in the header file.
//  If storage is SMB_SELFPACK, self-packing conservative allocation is used
//  If storage is SMB_FASTALLOC, fast allocation is used
//  If storage is SMB_HYPERALLOC, no allocation tables are used (fastest)

int SMBArea::smb_addmsghdr(smbmsg_t * msg, smbstatus_t * status, int storage, int retry_time)
{
  int i;
  long l;

  if (smb_locksmbhdr(retry_time))
    return (1);
  if (smb_getstatus(status))
    return (2);

  if (storage != SMB_HYPERALLOC and (i = smb_open_ha(retry_time)) != 0)
    return (i);

  msg->hdr.length = smb_getmsghdrlen(*msg);
  if (storage == SMB_HYPERALLOC)
    l = smb_hallochdr(status->header_offset);
  else if (storage == SMB_FASTALLOC)
    l = smb_fallochdr(msg->hdr.length);
  else
    l = smb_allochdr(msg->hdr.length);
  if (l == -1L) {
    smb_unlocksmbhdr();
    fclose(data->sha_fp);
    return (-1);
  }
  status->last_msg++;
  msg->idx.number = msg->hdr.number = status->last_msg;
  msg->idx.offset = status->header_offset + l;
  msg->idx.time = msg->hdr.when_imported.time;
  msg->idx.attr = msg->hdr.attr;
  msg->offset = status->total_msgs;
  status->total_msgs++;
  smb_putstatus(*status);

  if (storage != SMB_HYPERALLOC)
    fclose(data->sha_fp);
  i = smb_putmsg(*msg);
  smb_unlocksmbhdr();
  return (i);
}


//  ------------------------------------------------------------------
//  Writes both header and index information for msg 'msg'

int SMBArea::smb_putmsg(smbmsg_t msg)
{
  int i;

  i = smb_putmsghdr(msg);
  if (i)
    return (i);
  return (smb_putmsgidx(msg));
}


//  ------------------------------------------------------------------
//  Writes index information for 'msg'
//  msg.idx and msg.offset must be set prior to calling to this function
//  Returns 0 if everything ok

int SMBArea::smb_putmsgidx(smbmsg_t msg)
{
  clearerr(data->sid_fp);
  fseek(data->sid_fp, msg.offset * sizeof(idxrec_t), SEEK_SET);
  if (not fwrite(&msg.idx, sizeof(idxrec_t), 1, data->sid_fp))
    return (1);
  fflush(data->sid_fp);
  return (0);
}


//  ------------------------------------------------------------------
//  Writes header information for 'msg'
//  msg.hdr.length
//  msg.idx.offset
//  and msg.offset must be set prior to calling to this function
//  Returns 0 if everything ok

int SMBArea::smb_putmsghdr(smbmsg_t msg)
{
  word i;
  dword l;

  clearerr(data->shd_fp);
  if (fseek(data->shd_fp, msg.idx.offset, SEEK_SET))
    return (-1);

  // Write the fixed portion of the header record
  if (not fwrite(&msg.hdr, sizeof(msghdr_t), 1, data->shd_fp))
    return (-2);

  // Write the data fields (each is fixed length)
  for (i = 0; i < msg.hdr.total_dfields; i++)
    if (not fwrite(&msg.dfield[i], sizeof(dfield_t), 1, data->shd_fp))
      return (-3);

  // Write the variable length header fields
  for (i = 0; i < msg.total_hfields; i++) {
    if (not fwrite(&msg.hfield[i], sizeof(hfield_t), 1, data->shd_fp))
      return (-4);
    if (msg.hfield[i].length and not fwrite(msg.hfield_dat[i], msg.hfield[i].length, 1, data->shd_fp))
      return (-5);
  }

  l = smb_getmsghdrlen(msg);
  while (l % SHD_BLOCK_LEN) {
    if (fputc(0, data->shd_fp) == EOF) // pad block with NULL
      return (-6);
    l++;
  }
  fflush(data->shd_fp);
  return (0);
}


//  ------------------------------------------------------------------
//  Creates a sub-board's initial header file
//  Truncates and deletes other associated SMB files

int SMBArea::smb_create(dword max_crcs, dword max_msgs, word max_age, word attr, int retry_time)
{
  smbhdr_t hdr;
  smbstatus_t status;

  if (filelength(fileno(data->shd_fp)) >= sizeof(smbhdr_t) + sizeof(smbstatus_t)
    and smb_locksmbhdr(retry_time)) // header exists, so lock it
    return (1);
  memset(&hdr, 0, sizeof(smbhdr_t));
  memset(&status, 0, sizeof(smbstatus_t));
  memcpy(hdr.id, "SMB\x1a", 4);
  hdr.version = SMB_VERSION;
  hdr.length = sizeof(smbhdr_t) + sizeof(smbstatus_t);
  status.last_msg = status.total_msgs = 0;
  status.header_offset = sizeof(smbhdr_t) + sizeof(smbstatus_t);
  status.max_crcs = max_crcs;
  status.max_msgs = max_msgs;
  status.max_age = max_age;
  status.attr = attr;
  rewind(data->shd_fp);
  fwrite(&hdr, 1, sizeof(smbhdr_t), data->shd_fp);
  fwrite(&status, 1, sizeof(smbstatus_t), data->shd_fp);
  rewind(data->shd_fp);
  chsize(fileno(data->shd_fp), sizeof(smbhdr_t) + sizeof(smbstatus_t));
  fflush(data->shd_fp);

  rewind(data->sdt_fp);
  chsize(fileno(data->sdt_fp), 0L);
  rewind(data->sid_fp);
  chsize(fileno(data->sid_fp), 0L);

  remove(AddPath(path(), ".sda")); // if it exists, delete it
  remove(AddPath(path(), ".sha")); // if it exists, delete it
  remove(AddPath(path(), ".sch"));
  smb_unlocksmbhdr();
  return (0);
}


//  ------------------------------------------------------------------
//  Returns number of data blocks required to store "length" amount of data

dword SMBArea::smb_datblocks(dword length)
{
  dword blocks;

  blocks = length / SDT_BLOCK_LEN;
  if (length % SDT_BLOCK_LEN)
    blocks++;
  return (blocks);
}


//  ------------------------------------------------------------------
//  Returns number of header blocks required to store "length" size header

dword SMBArea::smb_hdrblocks(dword length)
{
  dword blocks;

  blocks = length / SHD_BLOCK_LEN;
  if (length % SHD_BLOCK_LEN)
    blocks++;
  return (blocks);
}


//  ------------------------------------------------------------------
//  Finds unused space in data file based on block allocation table and
//  marks space as used in allocation table.
//  File must be opened read/write DENY ALL
//  Returns offset to beginning of data (in bytes, not blocks)
//  Assumes smb_open_da() has been called
//  fclose(data->sda_fp) should be called after
//  Returns negative on error

long SMBArea::smb_allocdat(dword length, word headers)
{
  word i, j;
  dword l, blocks, offset = 0L;

  blocks = smb_datblocks(length);
  j = 0; //  j is consecutive unused block counter
  fflush(data->sda_fp);
  rewind(data->sda_fp);
  while (not feof(data->sda_fp)) {
    if (not fread(&i, 2, 1, data->sda_fp))
      break;
    offset += SDT_BLOCK_LEN;
    if (not i)
      j++;
    else
      j = 0;
    if (j == blocks) {
      offset -= (blocks * SDT_BLOCK_LEN);
      break;
    }
  }
  clearerr(data->sda_fp);
  fseek(data->sda_fp, (offset / SDT_BLOCK_LEN) * 2L, SEEK_SET);
  for (l = 0; l < blocks; l++)
    if (not fwrite(&headers, 2, 1, data->sda_fp))
      return (-1);
  fflush(data->sda_fp);
  return (offset);
}


//  ------------------------------------------------------------------
//  Allocates space for data, but doesn't search for unused blocks
//  Returns negative on error

long SMBArea::smb_fallocdat(dword length, word headers)
{
  dword l, blocks, offset;

  fflush(data->sda_fp);
  clearerr(data->sda_fp);
  blocks = smb_datblocks(length);
  fseek(data->sda_fp, 0L, SEEK_END);
  offset = (ftell(data->sda_fp) / 2L) * SDT_BLOCK_LEN;
  for (l = 0; l < blocks; l++)
    if (not fwrite(&headers, 2, 1, data->sda_fp))
      break;
  fflush(data->sda_fp);
  if (l < blocks)
    return (-1L);
  return (offset);
}


//  ------------------------------------------------------------------
//  De-allocates space for data
//  Returns non-zero on error

int SMBArea::smb_freemsgdat(dword offset, dword length, word headers)
{
  word i;
  dword l, blocks;

  blocks = smb_datblocks(length);

  clearerr(data->sda_fp);
  for (l = 0; l < blocks; l++) {
    if (fseek(data->sda_fp, ((offset / SDT_BLOCK_LEN) + l) * 2L, SEEK_SET))
      return (1);
    if (not fread(&i, 2, 1, data->sda_fp))
      return (2);
    if (headers > i)
      i = 0; // don't want to go negative
    else
      i -= headers;
    if (fseek(data->sda_fp, -2L, SEEK_CUR))
      return (3);
    if (not fwrite(&i, 2, 1, data->sda_fp))
      return (4);
  }
  fflush(data->sda_fp);
  return (0);
}


//  ------------------------------------------------------------------
//  Adds to data allocation records for blocks starting at 'offset'
//  Returns non-zero on error

int SMBArea::smb_incdat(dword offset, dword length, word headers)
{
  word i;
  dword l, blocks;

  clearerr(data->sda_fp);
  blocks = smb_datblocks(length);
  for (l = 0; l < blocks; l++) {
    fseek(data->sda_fp, ((offset / SDT_BLOCK_LEN) + l) * 2L, SEEK_SET);
    if (not fread(&i, 2, 1, data->sda_fp))
      return (1);
    i += headers;
    fseek(data->sda_fp, -2L, SEEK_CUR);
    if (not fwrite(&i, 2, 1, data->sda_fp))
      return (2);
  }
  fflush(data->sda_fp);
  return (0);
}


//  ------------------------------------------------------------------
//  De-allocates blocks for header record
//  Returns non-zero on error

int SMBArea::smb_freemsghdr(dword offset, dword length)
{
  uchar c = 0;
  dword l, blocks;

  clearerr(data->sha_fp);
  blocks = smb_hdrblocks(length);
  fseek(data->sha_fp, offset / SHD_BLOCK_LEN, SEEK_SET);
  for (l = 0; l < blocks; l++)
    if (not fwrite(&c, 1, 1, data->sha_fp))
      return (1);
  fflush(data->sha_fp);
  return (0);
}


//  ------------------------------------------------------------------
//  Frees all allocated header and data blocks for 'msg'

int SMBArea::smb_freemsg(smbmsg_t msg, smbstatus_t status)
{
  int i;
  word x;

  if (status.attr & SMB_HYPERALLOC)
    return (0); // Nothing to do

  for (x = 0; x < msg.hdr.total_dfields; x++) {
    if ((i = smb_freemsgdat(msg.hdr.offset + msg.dfield[x].offset, msg.dfield[x].length, 1)) != 0)
      return (i);
  }
  return (smb_freemsghdr(msg.idx.offset - status.header_offset, msg.hdr.length));
}


//  ------------------------------------------------------------------
//  Finds unused space in header file based on block allocation table and
//  marks space as used in allocation table.
//  File must be opened read/write DENY ALL
//  Returns offset to beginning of header (in bytes, not blocks)
//  Assumes smb_open_ha() has been called
//  fclose(data->sha_fp) should be called after
//  Returns -1L on error

long SMBArea::smb_allochdr(dword length)
{
  uchar c;
  word i;
  dword l, blocks, offset = 0;

  blocks = smb_hdrblocks(length);
  i = 0; // i is consecutive unused block counter
  fflush(data->sha_fp);
  rewind(data->sha_fp);
  while (not feof(data->sha_fp)) {
    if (not fread(&c, 1, 1, data->sha_fp))
      break;
    offset += SHD_BLOCK_LEN;
    if (not c)
      i++;
    else
      i = 0;
    if (i == blocks) {
      offset -= (blocks * SHD_BLOCK_LEN);
      break;
    }
  }
  clearerr(data->sha_fp);
  fseek(data->sha_fp, offset / SHD_BLOCK_LEN, SEEK_SET);
  c = 1;
  for (l = 0; l < blocks; l++)
    if (not fwrite(&c, 1, 1, data->sha_fp))
      return (-1L);
  fflush(data->sha_fp);
  return (offset);
}


//  ------------------------------------------------------------------
//  Allocates space for index, but doesn't search for unused blocks
//  Returns -1L on error

long SMBArea::smb_fallochdr(dword length)
{
  uchar c = 1;
  dword l, blocks, offset;

  blocks = smb_hdrblocks(length);
  fflush(data->sha_fp);
  clearerr(data->sha_fp);
  fseek(data->sha_fp, 0L, SEEK_END);
  offset = ftell(data->sha_fp) * SHD_BLOCK_LEN;
  for (l = 0; l < blocks; l++)
    if (not fwrite(&c, 1, 1, data->sha_fp))
      return (-1L);
  fflush(data->sha_fp);
  return (offset);
}


//  ------------------------------------------------------------------
//  Allocate header blocks using Hyper Allocation
//  this function should be most likely not be called from anywhere but
//  smb_addmsghdr()

long SMBArea::smb_hallochdr(dword header_offset)
{
  long l;

  fflush(data->shd_fp);
  fseek(data->shd_fp, 0L, SEEK_END);
  l = ftell(data->shd_fp);
  if (l < header_offset) // Header file truncated?!?
    return (header_offset);
  while ((l - header_offset) % SHD_BLOCK_LEN) // Make sure even block boundry
    l++;
  return (l - header_offset);
}


//  ------------------------------------------------------------------
//  Allocate data blocks using Hyper Allocation
//  smb_locksmbhdr() should be called before this function and not
//  unlocked until all data fields for this message have been written
//  to the SDT file

long SMBArea::smb_hallocdat()
{
  long l;

  fflush(data->sdt_fp);
  fseek(data->sdt_fp, 0L, SEEK_END);
  l = ftell(data->sdt_fp);
  if (l <= 0)
    return (l);
  while (l % SDT_BLOCK_LEN) // Make sure even block boundry
    l++;
  return (l);
}
