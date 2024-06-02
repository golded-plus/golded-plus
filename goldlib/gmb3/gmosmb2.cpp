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

#include <gdbgtrk.h>
#include <gmemdbg.h>
#include <gstrall.h>
#include <gmosmb.h>
//  ------------------------------------------------------------------
void SMBArea::raw_scan(bool keep_index, bool scanpm)
{
    GFTRK("SMBArea::raw_scan");
    smb_t * _was_data = data;

    if(_was_data == NULL)
    {
        if(ispacked())
        {
            const char * newpath = Unpack(path());

            if(newpath == NULL)
            {
                packed(false);
            }

            set_real_path(newpath ? newpath : path());
        }

        data_open();
    }

    uint32_t firstmsgno = 0;
    uint32_t lastmsgno  = 0;
    Msgn->Reset();
    PMrk->Reset();

    if(isopen or smb_open(data) == 0)
    {
        if(smb_locksmbhdr(data) == 0)
        {
            int res = smb_getstatus(data);
            smb_unlocksmbhdr(data);
            uint32_t total_msgs = 0;

            if(res == 0)
            {
                total_msgs = data->status.total_msgs;
                lastmsgno  = data->status.last_msg;

                if(keep_index or scanpm)
                {
                    smbmsg_t msg;
                    int umax   = (WidePersonalmail & PM_ALLNAMES) ? WideUsernames : 1;
                    uint32_t l = 1;
                    rewind(data->sid_fp);

                    while(l <= total_msgs)
                    {
                        if(not fread(&msg.idx, 1, sizeof(idxrec_t), data->sid_fp))
                        {
                            break;
                        }

                        if(smb_lockmsghdr(data, &msg) == 0)
                        {
                            int rc = smb_getmsghdr(data, &msg);
                            smb_unlockmsghdr(data, &msg);

                            if(rc == 0)
                            {
                                if(firstmsgno == 0)
                                {
                                    firstmsgno = msg.hdr.number;
                                }

                                if(keep_index)
                                {
                                    Msgn->Append(msg.hdr.number);
                                }

                                if(scanpm)
                                {
                                    bool gotpm = false;

                                    for(int u = 0; u < umax; u++)
                                    {
                                        if(strieql(WideUsername[u], (char *)msg.to))
                                        {
                                            gotpm = true;
                                            break;
                                        }
                                    }

                                    if(gotpm)
                                    {
                                        if(not (msg.idx.attr & MSG_READ))
                                        {
                                            PMrk->Append(msg.hdr.number);
                                        }

                                        gotpm = false;
                                    }
                                }

                                smb_freemsgmem(&msg);
                            }
                        }

                        l++;
                    }
                    total_msgs = l - 1;
                }
            }

            if(not isopen)
            {
                smb_close(data);
            }

            Msgn->SetCount(total_msgs);
        }
    }

    if(WideDebug)
    {
        WideLog->printf("- %s: t:%u, l:%u, fm:%u, hm:%u, lr:%u, u:%u, pm: %i",
                        echoid(),
                        Msgn->Count(),
                        0,
                        firstmsgno,
                        lastmsgno,
                        0,
                        0,
                        scanpm ? (int)PMrk->Count() : -1);
    }

    if(_was_data == NULL)
    {
        data_close();

        if(ispacked())
        {
            CleanUnpacked(real_path());
        }
    }

    GFTRK(0);
} // SMBArea::raw_scan

//  ------------------------------------------------------------------
void SMBArea::scan()
{
    raw_scan(true);
}

//  ------------------------------------------------------------------
void SMBArea::scan_area()
{
    raw_scan();
}

//  ------------------------------------------------------------------
void SMBArea::scan_area_pm()
{
    raw_scan(true, true);
}

//  ------------------------------------------------------------------
