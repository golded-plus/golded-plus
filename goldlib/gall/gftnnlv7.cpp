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
//  Version 7 nodelist processing module.
//  Based on source code from Binkley 2.50.
//  V7+ support based on draft #2 by Thomas Waldmann.
//  ------------------------------------------------------------------

#include <gfilutil.h>
#include <gstrall.h>
#include <gutlmisc.h>
#include <gftnnlv7.h>
//  ------------------------------------------------------------------
char v7nodeflags[16][9] =
{   {"Hub"}, {"Host"}, {"Region"}, {"Zone"}, {"CM,"}, {""}, {""}, {""}, {""}, {""}, {""},
    {""   }, {""    }, {""      }, {""    }, {""   }
};
//  ------------------------------------------------------------------
char v7modemtype[8][9] = {{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}
};
//  ------------------------------------------------------------------
//  Unpack a dense version of a symbol (base 40 polynomial)
static void v7unpack(char * src, char * dest, uint count)
{
    // This table has been modified to minimize searches
    //                     1234567890123456789012345678901234567890
    static char unwrk[] = " EANROSTILCHBDMUGPKYWFVJXZQ-\'0123456789";

    union
    {
        word w;
        byte c[2];
    } u;

    int i, j;
    char obuf[4];
    *dest = NUL;

    while(count)
    {
        u.c[0] = *src++;
        u.c[1] = *src++;
        count -= 2;

        for(j = 2; j >= 0; j--)
        {
            i       = u.w % 40;
            u.w    /= (word)40;
            obuf[j] = unwrk[i];
        }
        obuf[3] = NUL;
        strcat(dest, obuf);
    }

  #ifdef DEBUG
    printf("{%s}", dest);
  #endif
} // v7unpack

//  ------------------------------------------------------------------
const char * ftn_version7_nodelist_index::namekey() const
{
    return key;
}

//  ------------------------------------------------------------------
const ftn_addr & ftn_version7_nodelist_index::addrkey() const
{
    return *((ftn_addr *)key);
}

//  ------------------------------------------------------------------
int ftn_version7_nodelist_index::namecmp() const
{
  #ifdef DEBUG
    printf("[%s] [%s] ", searchname, namekey());
  #endif

    const char * a = searchname;
    const char * b = namekey();
    int n          = 1;
    int d;

    while(1)
    {
        d = g_tolower((uint8_t)*a) - g_tolower((uint8_t)*b);

        if((d != 0) or (*a == NUL) or (*b == NUL))
        {
            break;
        }

        a++;
        b++;
        n++;
    }
    return d != 0 ? (d > 0 ? n : -n) : 0;
}

//  ------------------------------------------------------------------
int ftn_version7_nodelist_index::addrcmp() const
{
  #ifdef DEBUG
    printf("[%d:%d/%d.%d] [%d:%d/%d.%d] ",
           searchaddr.zone,
           searchaddr.net,
           searchaddr.node,
           searchaddr.point,
           addrkey()->zone,
           addrkey()->net,
           addrkey()->node,
           keylength == 6 ? 0 : addrkey()->point);
  #endif

    ftn_addr currentaddr;
    currentaddr = addrkey();

    if(keylength == 6)
    {
        currentaddr.point = 0;
    }

    return searchaddr.compare(currentaddr);
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::fetchdata()
{
    if(node)
    {
        // Get data package
        _V7Data v7data;
        lseek(dfh, block.ndx.lnodeblk.leafref[node - 1].keyval, SEEK_SET);
        read(dfh, &v7data, sizeof(_V7Data));
        // Reset data buffers
        char buf[160], buf2[1024];
        memset(buf, 0, sizeof(buf));
        memset(buf2, 0, sizeof(buf2));
        // Get phone
        read(dfh, data.phone, v7data.phone_len);
        data.phone[v7data.phone_len] = NUL;
        // Skip password
        lseek(dfh, v7data.password_len, SEEK_CUR);
        // Get packed data and unpack it
        read(dfh, buf2, v7data.pack_len);
        v7unpack(buf2, buf, v7data.pack_len);
        // Get system name
        struplow(strxcpy(data.system, buf, v7data.bname_len + 1));

        // Get name
        if(namebrowse)
        {
            strunrevname(data.name, namekey());
        }
        else
        {
            char * namep = buf + v7data.bname_len;
            strxcpy(data.name, namep, v7data.sname_len + 1);
        }

        struplow(data.name);
        // Get location
        char * locationp = buf + v7data.bname_len + v7data.sname_len;
        struplow(strxcpy(data.location, locationp, v7data.cname_len + 1));
        // Check if V7+ data is available
        char * v7plus = locationp + v7data.cname_len;
        dword v7p = 0;
        int i, j;

        for(i = 0; (i < 8) and (*v7plus); v7plus++, i++)
        {
            if(not isxdigit(*v7plus))
            {
                break;
            }
            else
            {
                v7p = (v7p << 4) | (xtoi(*v7plus));
            }
        }
        use_v7plus = (tfh > 0) and (i == 8);

        if(use_v7plus)
        {
            lseek(tfh, v7p, SEEK_SET);

            if(v7data.nodeflags & V7_B_Point) // node is a point
            {
                lseek(tfh, (long)dtpctl.AllFixSize, SEEK_CUR);
            }
            else
            {
                lseek(tfh, (long)dtpctl.AllFixSize + (long)dtpctl.AddFixSize, SEEK_CUR);
            }

            word raw_length;
            read(tfh, &raw_length, sizeof(raw_length));

            if(raw_length < sizeof(buf2))
            {
                read(tfh, buf2, raw_length);
                data.unpack(buf2);
            }
        }
        else
        {
            // Get status
            for(j = 1, i = 0; i < 4; j += j, i++)
            {
                if(v7data.nodeflags & j)
                {
                    strcpy(data.status, v7nodeflags[i]);
                }
            }
            *data.status = NUL;
            // Get baud
            sprintf(data.baud, "%lu", 300L * (long)v7data.baudrate);
            // Get flags
            *data.flags = NUL;
            char * ptr = data.flags;

            for(j = (1 << 4), i = 4; i < 16; j += j, i++)
            {
                if(v7data.nodeflags & j)
                {
                    ptr = stpcpy(ptr, v7nodeflags[i]);
                }
            }

            // Get modem types
            for(j = 1, i = 0; i < 8; j += j, i++)
            {
                if(v7data.modemtype & j)
                {
                    ptr = stpcpy(ptr, v7modemtype[i]);
                }
            }

            // Erase the trailing comma
            if(ptr != data.flags)
            {
                *(--ptr) = NUL;
            }
        }

        // Get address
        data.addr.zone  = v7data.zone;
        data.addr.net   = v7data.net;
        data.addr.node  = v7data.node;
        data.addr.point = (word)((v7data.nodeflags & V7_B_Point) ? v7data.hubnode : 0);
        data.addr.make_string(data.address);
    }
} // ftn_version7_nodelist_index::fetchdata

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::getindexkey()
{
    _V7IndxRef * ip = &(block.ndx.inodeblk.indxref[inode - 1]);

    keylength = ip->indxlen;
    memcpy(key, (char *)&block + ip->indxofs, keylength);
    key[keylength] = NUL;

  #ifdef DEBUG
    printf("`---%02d:%02d <%04ld> ",
           inode,
           block.ndx.inodeblk.indxcnt,
           block.ndx.inodeblk.indxref[inode - 1].indxptr);
  #endif
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::getleafkey()
{
    _V7LeafRef * lp = &(block.ndx.lnodeblk.leafref[node - 1]);

    keylength = lp->keylen;
    memcpy(key, (char *)&block + lp->keyofs, keylength);
    key[keylength] = NUL;

  #ifdef DEBUG
    printf("`---%02d:%02d ", node, block.ndx.lnodeblk.indxcnt);
  #endif
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::getblock()
{
    lseek(xfh, blockno * (long)ctl.ndx.ctlblk.ctlblksize, SEEK_SET);
    read(xfh, &block, sizeof(_V7Ndx));

  #ifdef DEBUG

    if(block.ndx.inodeblk.indxfirst != -1)
    {
        printf("INDEXINFO: Branch:%ld, Below:%ld, Left:%ld, Right:%ld.\n",
               blockno,
               block.ndx.inodeblk.indxfirst,
               block.ndx.inodeblk.indxblink,
               block.ndx.inodeblk.indxflink);
    }

  #endif
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::getleaf()
{
  #ifdef DEBUG
    printf("LEAF_INFO: Branch:%ld, Left:%ld, Right:%ld.\n",
           blockno,
           block.ndx.lnodeblk.indxblink,
           block.ndx.lnodeblk.indxflink);
  #endif
}

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::search()
{
    int diff          = 0;
    int prevdiff      = 0;
    int previndexdiff = 0;

    exactmatch = false;
    xfh        = namebrowse ? sfh : nfh;
    node       = 0;

    // Get CtlRec
    lseek(xfh, 0, SEEK_SET);
    read(xfh, &ctl, sizeof(_V7Ndx));
    // The guts of the matter -- walk from CtlRec to Leaf
    blockno = ctl.ndx.ctlblk.ctlroot;
    memset(&block, 0, sizeof(_V7Ndx));
    inode = 1;
    // Read the first Index node.
    getblock();

    // Follow the node tree until we either match a key right in the
    // index node, or locate the leaf node which must contain the data.
    while(block.ndx.inodeblk.indxfirst != -1)
    {
        if(block.ndx.inodeblk.indxcnt == 0)
        {
            exactmatch = false;
            return false;
        }

        for(inode = 1; inode <= block.ndx.inodeblk.indxcnt; inode++)
        {
            getindexkey();
            diff          = namebrowse ? namecmp() : addrcmp();
            previndexdiff = diff;

      #ifdef DEBUG
            printf("(%d)\n", diff);
      #endif

            if(diff <= 0)
            {
                break;
            }
        }

        if(inode == 1)
        {
            blockno = block.ndx.inodeblk.indxfirst;
        }
        else if(diff == 0)
        {
            blockno = block.ndx.inodeblk.indxref[inode - 1].indxptr;
        }
        else
        {
            blockno = block.ndx.inodeblk.indxref[(--inode) - 1].indxptr;
        }

        getblock();
    }

    // We can only get here if we've found the leafnode which must
    // contain our data. Find our guy here or die trying.
    if(block.ndx.lnodeblk.indxcnt != 0)
    {
        // Search for a higher key
        getleaf();

        for(node = 1; node <= block.ndx.lnodeblk.indxcnt; node++)
        {
            getleafkey();
            prevdiff = diff;
            diff     = namebrowse ? namecmp() : addrcmp();

      #ifdef DEBUG
            printf("(%d)\n", diff);
      #endif

            if(diff < 0)
            {
                break;
            }

            if(diff == 0)
            {
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
        }

        if((prevdiff > 0) and (diff < 0))
        {
            if(absolute(prevdiff) > absolute(diff))
            {
        #ifdef DEBUG
                printf("Begin lookup at previous blockno.\n");
        #endif
                prevnode();
            }
            else
            {
        #ifdef DEBUG
                printf("Begin lookup at current blockno.\n");
        #endif
            }
        }
        else
        {
            if(absolute(previndexdiff) > absolute(diff))
            {
        #ifdef DEBUG
                printf("Begin lookup at next blockno.\n");
        #endif
                nextnode();
            }
            else
            {
        #ifdef DEBUG
                printf("Begin lookup at this blockno.\n");
        #endif

                if(node > 1)
                {
                    node--;
                }
            }
        }

        fetchdata();
    }

    return exactmatch;
} // ftn_version7_nodelist_index::search

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::prevnode()
{
    if(node == 1)
    {
        // Reached first node in current leaf
        if(block.ndx.lnodeblk.indxblink == 0)
        {
            return false;
        }

        blockno = block.ndx.inodeblk.indxblink;
        getblock();
        getleaf();
        node = block.ndx.lnodeblk.indxcnt;
    }
    else
    {
        node--;
    }

    getleafkey();
    return true;
}

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::nextnode()
{
    if(node >= block.ndx.lnodeblk.indxcnt)
    {
        // Reached end of nodes in current leaf
        if(block.ndx.lnodeblk.indxflink == 0)
        {
            return false;
        }

        blockno = block.ndx.inodeblk.indxflink;
        getblock();
        getleaf();
        node = 1;
    }
    else
    {
        node++;
    }

    getleafkey();
    return true;
}

//  ------------------------------------------------------------------
ftn_version7_nodelist_index::ftn_version7_nodelist_index()
{
    nfh        = sfh = dfh = tfh = -1;
    use_v7plus = false;
    isopen     = false;
}

//  ------------------------------------------------------------------
ftn_version7_nodelist_index::~ftn_version7_nodelist_index()
{
    if(isopen)
    {
        close();
    }
}

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::open()
{
    if(isopen)
    {
        close();
    }

    nfh = ::sopen(AddPath(nlpath, "NODEX.NDX"), O_RDONLY | O_BINARY, SH_DENYNO, S_STDRD);
    sfh = ::sopen(AddPath(nlpath, "SYSOP.NDX"), O_RDONLY | O_BINARY, SH_DENYNO, S_STDRD);

    if(sfh == -1)
    {
        sfh = ::sopen(AddPath(nlpath, "NODEX.SDX"),
                      O_RDONLY | O_BINARY,
                      SH_DENYNO,
                      S_STDRD);
    }

    dfh = ::sopen(AddPath(nlpath, "NODEX.DAT"), O_RDONLY | O_BINARY, SH_DENYNO, S_STDRD);
    tfh = ::sopen(AddPath(nlpath, "NODEX.DTP"), O_RDONLY | O_BINARY, SH_DENYNO, S_STDRD);

    if((nfh == -1) or (sfh == -1) or (dfh == -1))
    {
        // Unable to open an index file
        close();
        return false;
    }

    if(tfh != -1)
    {
        read(tfh, &dtpctl, sizeof(_V7DTPCtl));
    }

    isopen = true;
    return true;
} // ftn_version7_nodelist_index::open

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::close()
{
    if(dfh != -1)
    {
        ::close(dfh);
    }

    dfh = -1;

    if(sfh != -1)
    {
        ::close(sfh);
    }

    sfh = -1;

    if(nfh != -1)
    {
        ::close(nfh);
    }

    nfh = -1;

    if(tfh != -1)
    {
        ::close(tfh);
    }

    tfh    = -1;
    isopen = false;
} // ftn_version7_nodelist_index::close

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::find(const char * lookup_name)
{
    namebrowse = true;
    char tmpname[80];
    strcpy(tmpname, lookup_name);
    strchg(tmpname, '.', ' ');
    struplow(strrevname(searchname, tmpname));
    return search();
}

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::find(const ftn_addr & addr)
{
    namebrowse = false;
    searchaddr = addr;
    return search();
}

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::previous()
{
    bool moved = prevnode();

    if(moved)
    {
        fetchdata();
        compare();
    }

    return moved;
}

//  ------------------------------------------------------------------
bool ftn_version7_nodelist_index::next()
{
    bool moved = nextnode();

    if(moved)
    {
        fetchdata();
        compare();
    }

    return moved;
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::first()
{
    if(namebrowse)
    {
        *searchname = NUL;
    }
    else
    {
        searchaddr.reset();
    }

    search();
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::last()
{
    if(namebrowse)
    {
        memset(searchname, 0xFF, sizeof(searchname));
        searchname[sizeof(searchname) - 1] = NUL;
    }
    else
    {
        searchaddr.set_all(0xFFFF);
    }

    search();
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::push_state()
{
    state.blockno = blockno;
    state.node    = node;
}

//  ------------------------------------------------------------------
void ftn_version7_nodelist_index::pop_state()
{
    blockno = state.blockno;
    node    = state.node;
    getblock();
    getleaf();
    getleafkey();
    fetchdata();
    compare();
}

//  ------------------------------------------------------------------
const char * ftn_version7_nodelist_index::index_name() const
{
    return namebrowse ? "SYSOP.NDX" : "NODEX.NDX";
}

//  ------------------------------------------------------------------
const char * ftn_version7_nodelist_index::nodelist_name() const
{
    return use_v7plus ? "NODEX.DTP" : (const char *)NULL;
}

//  ------------------------------------------------------------------
