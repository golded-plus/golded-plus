//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  FrontDoor Nodelist Indexing.
//  Based on code kindly provided by Scott Dudley.
//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gstrall.h>
#include <gftnnlfd.h>
#include <stdlib.h>

//  ------------------------------------------------------------------

inline const _FDUdx& ftn_frontdoor_nodelist_index::namerec() const
{

    return block.node.name[node-1];
}


//  ------------------------------------------------------------------

inline const _FDFdx& ftn_frontdoor_nodelist_index::addrrec() const
{

    return block.node.addr[node-1];
}


//  ------------------------------------------------------------------

inline const _FDGdx& ftn_frontdoor_nodelist_index::noderec() const
{

    return namebrowse ? *((_FDGdx*)&block.node.name[node-1]) : *((_FDGdx*)&block.node.addr[node-1]);
}


//  ------------------------------------------------------------------

int ftn_frontdoor_nodelist_index::namecmp() const
{

#ifdef DEBUG
    printf("[%s] [%-15.15s] ", searchname, namerec().name);
#endif

    const char* a = searchname;
    const char* b = namerec().name;
    int n = 1;
    while(n <= 15)
    {
        int d = (int)((byte)*a) - (int)((byte)*b);
        if(d != 0)
            return d > 0 ? n : -n;
        a++;
        b++;
        n++;
    }
    return 0;
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::getaddr(ftn_addr& addr) const
{

    if(namebrowse)
    {
        addr.zone  = (word)((namerec().zone_hi  << 8) | namerec().zone_lo);
        addr.net   = (word)((namerec().net_hi   << 8) | namerec().net_lo);
        addr.node  = (word)((namerec().node_hi  << 8) | namerec().node_lo);
        addr.point = (word)((namerec().point_hi << 8) | namerec().point_lo);
    }
    else
    {
        addr.zone  = (word)((addrrec().zone_hi  << 8) | addrrec().zone_lo);
        addr.net   = (word)((addrrec().net_hi   << 8) | addrrec().net_lo);
        addr.node  = (word)((addrrec().node_hi  << 8) | addrrec().node_lo);
        addr.point = (word)((addrrec().point_hi << 8) | addrrec().point_lo);
    }
}


//  ------------------------------------------------------------------

int ftn_frontdoor_nodelist_index::addrcmp() const
{

    ftn_addr currentaddr;
    getaddr(currentaddr);

#ifdef DEBUG
    printf("[%d:%d/%d.%d] [%d:%d/%d.%d] ",
           searchaddr.zone,  searchaddr.net,  searchaddr.node,  searchaddr.point,
           currentaddr.zone, currentaddr.net, currentaddr.node, currentaddr.point
          );
#endif

    return searchaddr.compare(currentaddr);
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::getstatus(char* _status, int type) const
{

    switch(type)
    {
    case ISZC:
        strcpy(_status, "Zone");
        break;
    case ISRC:
        strcpy(_status, "Region");
        break;
    case ISNC:
        strcpy(_status, "Host");
        break;
    case ISHUB:
        strcpy(_status, "Hub");
        break;
    case ISPVT:
        strcpy(_status, "Pvt");
        break;
    case ISHOLD:
        strcpy(_status, "Hold");
        break;
    case ISDOWN:
        strcpy(_status, "Down");
        break;
    case ISPOINT:
        strcpy(_status, "Point");
        break;
    default:
        *_status = NUL;
    }
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::fetchdata()
{

    bool infdpvt   = !!(noderec().nlofs & IN_FDNET);
    bool infdpoint = !!(noderec().nlofs & IN_FDPOINT);
    bool infdnode  = !!(noderec().nlofs & IN_FDNODE);

    if(infdnode)
    {

        int fd = ::sopen(AddPath(nlpath, "FDNODE.FDA"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
        if(fd != -1)
        {
            _FDFdn fda;
            while(read(fd, &fda, sizeof(_FDFdn)) == sizeof(_FDFdn))
            {
                if(not fda.erased)
                {
                    if((fda.zone == data.addr.zone) and (fda.net == data.addr.net) and (fda.node == data.addr.node) and (fda.point == data.addr.point))
                    {
                        strnp2cc(data.name, fda.user, 36);
                        strnp2cc(data.system, fda.name, 30);
                        strnp2cc(data.phone, fda.telephone, 40);
                        strnp2cc(data.location, fda.location, 40);
                        switch(fda.maxbaud)
                        {
                        case ISBAUD300   :
                            strcpy(data.baud, "300");
                            break;
                        case ISBAUD1200  :
                            strcpy(data.baud, "1200");
                            break;
                        case ISBAUD2400  :
                            strcpy(data.baud, "2400");
                            break;
                        case ISBAUD4800  :
                            strcpy(data.baud, "4800");
                            break;
                        case ISBAUD7200  :
                            strcpy(data.baud, "7200");
                            break;
                        case ISBAUD9600  :
                            strcpy(data.baud, "9600");
                            break;
                        case ISBAUD12000 :
                            strcpy(data.baud, "12000");
                            break;
                        case ISBAUD14400 :
                            strcpy(data.baud, "14400");
                            break;
                        case ISBAUD16800 :
                            strcpy(data.baud, "16800");
                            break;
                        case ISBAUD19200 :
                            strcpy(data.baud, "19200");
                            break;
                        case ISBAUD38400 :
                            strcpy(data.baud, "38400");
                            break;
                        case ISBAUD57600 :
                            strcpy(data.baud, "57600");
                            break;
                        case ISBAUD64000 :
                            strcpy(data.baud, "64000");
                            break;
                        case ISBAUD76800 :
                            strcpy(data.baud, "76800");
                            break;
                        case ISBAUD115200:
                            strcpy(data.baud, "115200");
                            break;
                        default:
                            *data.baud = NUL;
                        }
                        getstatus(data.status, fda.status);
                        *data.flags = NUL;
                        long c = fda.capability;
                        char* f = data.flags;
                        if(c & CMflag)      strcat(f, "CM,");
                        if(c & MOflag)      strcat(f, "MO,");
                        if(c & LOflag)      strcat(f, "LO,");
                        if(c & V32flag)     strcat(f, "V32,");
                        if(c & V32bflag)    strcat(f, "V32B,");
                        if(c & V33flag)     strcat(f, "V33,");
                        if(c & V34flag)     strcat(f, "V34,");
                        if(c & V42flag)     strcat(f, "V42,");
                        if(c & V42bflag)    strcat(f, "V42B,");
                        if(c & MNPflag)     strcat(f, "MNP,");
                        if(c & H96flag)     strcat(f, "H96,");
                        if(c & HSTflag)     strcat(f, "HST,");
                        if(c & HST14flag)   strcat(f, "H14,");
                        if(c & HST16flag)   strcat(f, "H16,");
                        if(c & MAXflag)     strcat(f, "MAX,");
                        if(c & PEPflag)     strcat(f, "PEP,");
                        if(c & ZYXflag)     strcat(f, "ZYX,");
                        if(c & XAflag)      strcat(f, "XA,");
                        if(c & XBflag)      strcat(f, "XB,");
                        if(c & XCflag)      strcat(f, "XC,");
                        if(c & XPflag)      strcat(f, "XP,");
                        if(c & XRflag)      strcat(f, "XR,");
                        if(c & XWflag)      strcat(f, "XW,");
                        if(c & XXflag)      strcat(f, "XX,");
                        if(c & FAXflag)     strcat(f, "FAX,");
                        if(c & UISDNAflag)  strcat(f, "V110L,");
                        if(c & UISDNBflag)  strcat(f, "V110H,");
                        if(c & UISDNCflag)  strcat(f, "X75,");
                        if(*f)              f[strlen(f)-1] = NUL;
                        break;
                    }
                }
            }
            ::close(fd);
        }
    }
    else
    {

        int fd = infdpvt ? pfd : (infdpoint ? ppfd : nfd);
        long offset = noderec().nlofs & ~(IN_FDNET|IN_FDPOINT);

        char buf[256];
        buf[255] = NUL;

        lseek(fd, offset, SEEK_SET);
        read(fd, buf, 255);
        if(*buf != ';')
        {

            char* end = strchr(buf, '\r');
            if(end)
                *end = NUL;

            strchg(strtrim(buf), '_', ' ');

            *data.name = NUL;
            *data.system = NUL;
            *data.location = NUL;
            *data.phone = NUL;
            *data.flags = NUL;
            *data.baud = NUL;

            char* q = buf;
            char* p = strchr(buf, ',');
            if(p)
            {
                *p++ = NUL;
                strxcpy(data.system, q, sizeof(data.system));
                p = strchr((q=p), ',');
                if(p)
                {
                    *p++ = NUL;
                    strxcpy(data.location, q, sizeof(data.location));
                    p = strchr((q=p), ',');
                    if(p)
                    {
                        *p++ = NUL;
                        strxcpy(data.name, q, sizeof(data.name));
                        p = strchr((q=p), ',');
                        if(p)
                        {
                            *p++ = NUL;
                            strxcpy(data.phone, q, sizeof(data.phone));
                            p = strchr((q=p), ',');
                            sprintf(data.baud, "%lu", atol(q));
                            if(p)
                                strxcpy(data.flags, p+1, sizeof(data.flags));
                        }
                    }
                }
            }
        }

        getstatus(data.status, namebrowse ? namerec().type : addrrec().type);
    }

    data.addr.make_string(data.address);
}


//  ------------------------------------------------------------------

#ifdef DEBUG
void ftn_frontdoor_nodelist_index::printnode() const
{

    printf("<%05d.%05d.%05d.%d> <%02d:%02d> ",
           blockno,
           block.info.index,
           noderec().block_num,
           depth,
           block.info.nodes,
           node
          );
}
#endif


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::getnodedata()
{

#ifdef DEBUG
    printnode();
#endif
    getaddr(data.addr);
    fetchdata();
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::getblock()
{

    lseek(xfd, (long)blockno*(long)blocksize, SEEK_SET);
    read(xfd, &block, blocksize);
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::push()
{

    stack[depth].blockno = blockno;
    stack[depth].block_num = noderec().block_num;
    stack[depth].maxnodes = block.info.nodes;
    stack[depth].node = node;
    depth++;
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::pop()
{

    depth--;
    node = stack[depth].node;
    blockno = stack[depth].blockno;
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::search()
{

    xfd = namebrowse ? ufd : fdfd;
    blocksize = namebrowse ? sizeof(_FDUdb) : sizeof(_FDFdb);

    // Read beginning control block
    lseek(xfd, 0L, SEEK_SET);
    read(xfd, &ctl, sizeof(_FDCtl));

    // Now trace down the tree, starting at the master index
    exactmatch = false;
    int diff = 0;
    int prevdiff = 0;
    blockno = ctl.hdr.master_idx;
    maxblockno = (uint)(lseek(xfd, 0L, SEEK_END)/blocksize);
    depth = 0;
    node = 1;
    push();

    // Do one block at a time...
    while((not exactmatch) and (blockno < maxblockno))
    {

        // Get one block
        node = 0;
        getblock();

        // Scan all nodes in this block to find a match
        lastblockno = block.info.index;
        do
        {
            node++;
            if(namebrowse)
            {
#ifdef DEBUG
                printnode();
#endif
                prevdiff = diff;
                diff = namecmp();
#ifdef DEBUG
                printf("{%d}\n", diff);
#endif
                if(diff <= 0)
                {
                    if(diff == 0)
                    {
                        exactmatch = true;
                    }
                    break;
                }
                lastblockno = namerec().block_num;
            }
            else
            {
#ifdef DEBUG
                printnode();
#endif
                prevdiff = diff;
                diff = addrcmp();
#ifdef DEBUG
                printf("{%d}\n", diff);
#endif
                if(diff <= 0)
                {
                    if(diff == 0)
                    {
                        exactmatch = true;
                    }
                    break;
                }
                lastblockno = addrrec().block_num;
            }
        }
        while(node < block.info.nodes);

        if(not exactmatch)
        {
            if(lastblockno)
            {
                push();
                blockno = lastblockno;
            }
            else
            {
                break;
            }
        }
    }

    if(diff == 0)
    {
#ifdef DEBUG
        printf("Gotcha!\n");
#endif
        while(previous())
        {
            if(not exactmatch)
            {
                next();
                break;
            }
        }
        return true;
    }
    else if((prevdiff > 0) and (diff < 0))
    {
        if(absolute(prevdiff) > absolute(diff))
        {
#ifdef DEBUG
            printf("Begin lookup at previous blockno.\n");
#endif
            prevnode();
        }
    }
    else
    {
#ifdef DEBUG
        printf("Begin lookup at current blockno.\n");
#endif
    }

    getnodedata();

    return exactmatch;
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::prevnode()
{

    if(node == 1)
    {
        if(blockno == 1)
            return false;
        if(depth > 0)
        {
            pop();
            getblock();
            node--;
            if(node <= 0)
            {
                blockno = block.info.index;
                getblock();
                node = block.info.nodes;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        node--;
        if(noderec().block_num)
        {
            node++;
            push();
            node--;
            blockno = noderec().block_num;
            getblock();
            node = block.info.nodes;
        }
    }

    return true;
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::nextnode()
{

    if(noderec().block_num)
    {
        // Current node has a block. Go to it.
        node++;
        push();
        node--;
        blockno = noderec().block_num;
        node = 1;
        getblock();
    }
    else if(node >= block.info.nodes)
    {
        // Reached end of nodes in current block.
        if(depth > 0)
        {
            if((stack[depth-1].node == stack[depth-1].maxnodes) and (stack[depth-1].block_num == blockno))
                return false;
            // Go back to previous block
            pop();
            getblock();
        }
        else
        {
            // Reached final leaf
            return false;
        }
    }
    else
    {
        node++;
    }

    return true;
}


//  ------------------------------------------------------------------

ftn_frontdoor_nodelist_index::ftn_frontdoor_nodelist_index()
{

    fdfd = nfd  = pfd  = ppfd = ufd  = -1;
    is_intermail = false;
    isopen = false;
    blocksize = 0;
    depth = 0;
}


//  ------------------------------------------------------------------

ftn_frontdoor_nodelist_index::~ftn_frontdoor_nodelist_index()
{

    if(isopen)
        close();
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::open()
{

    if(isopen)
        close();

    if(fexist(AddPath(nlpath, "IMNODE.CTL")))
        is_intermail = true;

    // Open the nodelist binary file first
    fdfd = ::sopen(AddPath(nlpath, "NODELIST.FDX"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
    if(fdfd == -1)
    {
        close();
        return false;
    }

    // Read the control block up front
    read(fdfd, &ctl, sizeof(_FDCtl));

    // Now use that info to open the primary nodelist
    Path primary;
    sprintf(nodelist, "NODELIST.%3.3s", ctl.nl_ext);
    sprintf(primary, "%s%s", nlpath, nodelist);
    nfd = ::sopen(primary, O_RDONLY | O_BINARY, SH_DENYNO, S_STDRD);
    // Don't check for failure, since there may not be a primary nodelist!

    // Open the userlist file
    ufd = ::sopen(AddPath(nlpath, "USERLIST.FDX"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);
    if(ufd == -1)
    {
        close();
        return false;
    }

    // Failing to open the private nodelist is not an error, since there may not be one!
    pfd = ::sopen(AddPath(nlpath, is_intermail ? "IMNET.PVT" : "FDNET.PVT"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);

    // Failing to open the point nodelist is not an error, since there may not be one!
    ppfd = ::sopen(AddPath(nlpath, is_intermail ? "IMPOINT.PVT" : "FDPOINT.PVT"), O_RDONLY|O_BINARY, SH_DENYNO, S_STDRD);

    isopen = true;

    return true;
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::close()
{

    if(fdfd != -1)  ::close(fdfd);
    fdfd = -1;
    if(nfd != -1)   ::close(nfd);
    nfd  = -1;
    if(pfd != -1)   ::close(pfd);
    pfd  = -1;
    if(ppfd != -1)  ::close(ppfd);
    ppfd = -1;
    if(ufd != -1)   ::close(ufd);
    ufd  = -1;

    isopen = false;
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::find(const char* lookup_name)
{

    char buf[80], tmp[16];

    strncpy(tmp, lookup_name, sizeof(tmp));
    namebrowse = true;
    char* ptr = strrchr(tmp, ' ');
    if(ptr)
    {
        if(ptr == (tmp+strlen(tmp)-1))
        {
            while((*ptr == ' ') and (ptr > tmp))
                ptr--;
            ptr++;
        }
        *ptr = NUL;
        sprintf(buf, "%s %s", ptr+1, tmp);
        *ptr = ' ';
    }
    else
    {
        strcpy(buf, tmp);
    }
    strupr(strtrim(buf));
    sprintf(searchname, "%-15.15s", buf);

    return search();
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::find(const ftn_addr& addr)
{

    namebrowse = false;
    searchaddr = addr;
    return search();
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::previous()
{

    bool moved = prevnode();
    if(moved)
    {
        getnodedata();
        compare();
    }
    return moved;
}


//  ------------------------------------------------------------------

bool ftn_frontdoor_nodelist_index::next()
{

    bool moved = nextnode();
    if(moved)
    {
        getnodedata();
        compare();
    }
    return moved;
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::first()
{

    if(namebrowse)
    {
        memset(searchname, ' ', sizeof(searchname)-1);
        searchname[sizeof(searchname)-1] = NUL;
    }
    else
    {
        searchaddr.reset();
    }
    search();
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::last()
{

    if(namebrowse)
    {
        memset(searchname, 0xFF, sizeof(searchname)-1);
        searchname[sizeof(searchname)-1] = NUL;
    }
    else
    {
        searchaddr.set_all(0xFFFF);
    }
    search();
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::push_state()
{

    state.node = node;
    state.depth = depth;
    state.blockno = blockno;
    memcpy(state.stack, stack, sizeof(stack));
}


//  ------------------------------------------------------------------

void ftn_frontdoor_nodelist_index::pop_state()
{

    memcpy(stack, state.stack, sizeof(stack));
    blockno = state.blockno;
    depth = state.depth;
    node = state.node;
    getblock();
    getnodedata();
    compare();
}


//  ------------------------------------------------------------------

const char* ftn_frontdoor_nodelist_index::index_name() const
{

    return namebrowse ? "USERLIST.FDX" : "NODELIST.FDX";
}


//  ------------------------------------------------------------------

const char* ftn_frontdoor_nodelist_index::nodelist_name() const
{

    if(noderec().nlofs & IN_FDNODE)
        return "FDNODE.FDA";
    else if(noderec().nlofs & IN_FDNET)
        return is_intermail ? "IMNET.PVT" : "FDNET.PVT";
    else if(noderec().nlofs & IN_FDPOINT)
        return is_intermail ? "IMPOINT.PVT" : "FDPOINT.PVT";
    return nodelist;
}


//  ------------------------------------------------------------------
