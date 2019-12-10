//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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

#ifndef __gs_db_h
#define __gs_db_h
//  ------------------------------------------------------------------

#include <gdefs.h>
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack(1)
#endif
//  ------------------------------------------------------------------
//  D'Bridge 1.30 structs
typedef struct
{
    byte allocated;
    char tag[17];
    char echoid[57];
} DB130_AA1;
typedef struct
{
    char     desc[41];
    short    group;
    char     msgbase;
    char     path[49];
    char     kind;
    short    board;
    byte     ispvt;
    byte     tinyseenbys;
    char     origin[57];
    char     defaultpriority;
    ftn_addr addr;
    short    purge;
    short    preserve;
    short    security;
    char     archive[57];
    char     forwardto[5][57];
} DB130_AA2;
//  ------------------------------------------------------------------
//  D'Bridge 1.30+ QBBS lastread records
typedef struct
{
    char name[37];
    struct
    {
        word msgno;
        word index;
    } lastread[200];
} DB130_LRQ;
//  ------------------------------------------------------------------
//  D'Bridge B1046 DBRIDGE.ADF struct
typedef struct
{
    byte     allocated;     // 000
    char     tag[17];       // 001
    char     echoid[57];    // 018
    char     desc[41];      // 075
    short    group;         // 116
    char     msgbase;       // 118
    char     path[49];      // 119
    char     kind;          // 168
    short    board;         // 169
    byte     ispvt;         // 171
    byte     tinyseenbys;   // 172
    char     origin[57];    // 173
    char     defaultpriority; // 230
    ftn_addr addr;          // 231
    short    purge;
    short    preserve;
    short    security;
    char     archive[57];
    char     forwardto[5][57];
} DB1046_ADF;               // 587 bytes
//  ------------------------------------------------------------------
//  D'Bridge B1047.A22 DBRIDGE.ADF struct
typedef struct
{
    byte     allocated;     // 000
    char     alertflag;     // 001
    char     tag[17];       // 002
    char     echoid[57];    // 019
    char     desc[41];      // 076
    short    group;         // 117
    char     msgbase;       // 119
    char     path[49];      // 120
    char     kind;          // 169
    short    board;         // 170
    byte     ispvt;         // 172
    char     tinyseenbys;   // 173
    char     origin[57];    // 174
    char     defaultpriority; // 231
    ftn_addr addr;          // 232
    byte     fill[18];
    short    purge;
    short    preserve;
    short    security;
    char     archive[57];
    char     forwardto[5][57];
} DB1047A22_ADF;            // 606 bytes
//  ------------------------------------------------------------------
//  D'Bridge B1047.A27+ DBRIDGE.ADF struct
typedef struct
{
    byte     allocated;     // 000
    char     alertflag;     // 001
    char     tag[17];       // 002
    char     echoid[57];    // 019
    char     desc[41];      // 076
    short    group;         // 117
    char     msgbase;       // 119
    char     path[49];      // 120
    char     kind;          // 169
    short    board;         // 170
    byte     ispvt;         // 172
    char     tinyseenbys;   // 173
    char     origin[57];    // 174
    char     defaultpriority; // 231
    ftn_addr addr;          // 232
    byte     fill[13];      // 240
    short    purge;         // 253
    short    preserve;      // 255
    short    security;      // 257
    char     archive[57];   // 259
    char     forwardto[5][57]; // 316
} DB1047A27_ADF;            // 601 bytes
//  ------------------------------------------------------------------
//  D'Bridge B2011+ DBRIDGE.ADF struct
typedef struct
{
    byte     allocated;     // 000
    char     alertflag;     // 001
    char     tag[17];       // 002
    char     echoid[57];    // 019
    char     desc[41];      // 076
    short    group;         // 117
    char     msgbase;       // 119
    char     path[49];      // 120
    char     kind;          // 169
    short    board;         // 170
    byte     ispvt;         // 172
    char     tinyseenbys;   // 173
    char     origin[57];    // 174
    char     defaultpriority; // 231
    ftn_addr addr;          // 232
    byte     fill[13];      // 240
    short    purge;         // 253
    short    preserve;      // 255
    short    security;      // 257
    char     archive[57];   // 259
    char     forwardto[19][77]; // 316
} DB2011_ADF;               // 1779 bytes
//  ------------------------------------------------------------------

#if defined (GOLD_CANPACK)
#pragma pack()
#endif
//  ------------------------------------------------------------------

#endif // ifndef __gs_db_h
//  ------------------------------------------------------------------
