//  This may look like C code, but it is really -*- C++ -*-

//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1992-1995 by Andreas Klein
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
//  IMail Definitions and Structures
//  ------------------------------------------------------------------

#ifndef __gs_im160_h
#define __gs_im160_h


//  ------------------------------------------------------------------

#include <gftnall.h>


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack(1)
#endif


//  ------------------------------------------------------------------

#define IMAIL_MAJ_VERSION     1
#define IMAIL_MIN_VERSION     75
#define IMAIL_MAJ_VERSION_STR "1"
#define IMAIL_MIN_VERSION_STR "80"
#define STRUCT_MAJ_VERSION    5
#define STRUCT_MIN_VERSION    14
#define IM_PRD_CODE           0x4B
#define BETA

//  Data type definitions

typedef unsigned char bit;

//  Internal limits

#define MAXAKAS              50       /* Max # of addresses */
#define MAXPACKERS           11       /* Max # of packer def */
#define MAXEXPORT           200       /* Max export defs */
#define MAXVIA               40       /* max nodes packed via */
#define MAXGROUPS           255       /* max nodes packed via */
#define MAXEXCEPT            10       /* max EXCEPT nodes */
#define MAXPACK              32       /* max default pack cmd */
#define MAXFWDLINK           15       /* max fwd link structs */
#define MAXNOIMPT            20       /* max # names for IMPORT */
#define MAXSYSNAME           20       /* max # names for PERSMAIL */
#define ZONESDOM             10       /* zones per domain entry */
#define MAXTAG               51       /* max areatag length */
#define MAXNAME              37       /* max namefield lenght */
#define MAXPACKNAME          50       /* max packer length */
#define MAXORIGIN            64       /* max origin length */

//  Message Base Types

#ifndef MSGTYPE_SDM
  #define MSGTYPE_SDM     0x01
#endif
#ifndef MSGTYPE_SQUISH
  #define MSGTYPE_SQUISH  0x02
#endif
#ifndef MSGTYPE_HUDSON
  #define MSGTYPE_HUDSON  0x03
#endif
#ifndef MSGTYPE_JAM
  #define MSGTYPE_JAM     0x04
#endif
#ifndef MSGTYPE_PASSTH
  #define MSGTYPE_PASSTH  0x0F
#endif
#ifndef MSGTYPE_ECHO
  #define MSGTYPE_ECHO    0x80
#endif
#ifndef MSGTYPE_LOCAL
  #define MSGTYPE_LOCAL   0x90
#endif
#ifndef MSGTYPE_NET
  #define MSGTYPE_NET     0xA0
#endif

#define BASEMASK          0x0F
#define TYPEMASK          0xF0

//  In case your compiler doesn't have these ...

#ifndef MAXPATH
  #define MAXPATH            80
#endif
#ifndef MAXDRIVE
  #define MAXDRIVE           3
#endif
#ifndef MAXDIR
  #define MAXDIR             66
#endif
#ifndef MAXFILE
  #define MAXFILE            9
#endif
#ifndef MAXEXT
  #define MAXEXT             5
#endif

//  Structs used in IMAIL Configuration files

struct eaddress {                           /* used in Area Manager */
  ftn_addr        dstn;                     /* node number */
  bit             exp_only:1;               /* export only flag */
  bit             imp_only:1;               /* import only flag */
  bit             paused:1;                 /* echo mail paused */
  bit             denied:1;                 /* access is denied */
  bit             uplink:1;                 /* uplink */
  bit             rsvd1:3;                  /* reserved */
};

struct fwd_link {                           /* used in Forward Manager */
  char         areasfile[MAXFILE+MAXEXT];   /* name of areas file */
  char         toprogram[10];               /* name of area manager */
  char         password[21];                /* area manager password */
  ftn_addr     uplink;                      /* address of uplink */
  byte         accessgroup;                 /* accessgroup for forwarding */
  byte         creategroup;                 /* creategroup for forwarding */
  char         filler[10];                  /* reserved */
};

struct dom {
  char           domain[21];                  /* name of domain */
  char           outbound[MAXPATH];           /* root outbound path */
  word           zones[ZONESDOM];             /* Zones in this domain */
  byte           akas[MAXAKAS];               /* =my= AKAs in this domain */
};

struct im_stats {
  unsigned long  th_day_nr;                   /* nr this day */
  unsigned long  la_day_nr;                   /* nr last day */
  unsigned long  th_week_nr;                  /* nr this week */
  unsigned long  la_week_nr;                  /* nr last week */
  unsigned long  th_month_nr;                 /* nr this month */
  unsigned long  la_month_nr;                 /* nr last month */
  unsigned long  th_year_nr;                  /* nr this year */
  unsigned long  la_year_nr;                  /* nr last year */
  unsigned long  th_day_size;                 /* amount this day */
  unsigned long  la_day_size;                 /* amount last day */
  unsigned long  th_week_size;                /* amount this week */
  unsigned long  la_week_size;                /* amount last week */
  unsigned long  th_month_size;               /* amount this month */
  unsigned long  la_month_size;               /* amount last month */
  unsigned long  th_year_size;                /* amount this year */
  unsigned long  la_year_size;                /* amount last year */
};


/*
 *
 *  IMAIL.CF structure
 *
 */

struct im_config_type {
  byte           im_ver_maj;                    /* Major Version */
  byte           im_ver_min;                    /* Minor Version */
  byte           struct_maj;                    /* reserved */
  byte           struct_min;                    /* reserved */
  char           sysop[MAXNAME];                /* name of sysop */
  ftn_addr        aka[MAXAKAS];                 /* the AKAs */
  struct dom     domains[MAXAKAS];              /* domain names & zones */
  byte           rsvd1[10];                     /* reserved */
  char           netmail[MAXPATH];              /* net mail subdirectory */
  char           sec_inbound[MAXPATH];          /* secure inbound files */
  char           in_pkt[MAXPATH];               /* Directory for inbound PKTs */
  char           out_pkt[MAXPATH];              /* Directory for outbound PKTs */
  char           outbound[MAXPATH];             /* outbound directory */
  char           quickbbs[MAXPATH];             /* QuickBBS system directory */
  char           unsec_inbound[MAXPATH];        /* Unsecure inbound files */
  char           echotoss[MAXPATH];             /* name of echotoss.log */
  char           dupebase[MAXPATH];             /* dupe data base directory */
  char           semaphor[MAXPATH];             /* Semaphor directory */
  char           logfilename[MAXPATH];          /* Log file name */
  char           before_toss[MAXPATH];          /* call before proc. a PKT */
  char           semaphor_net[MAXFILE+MAXEXT];  /* Netmail rescan semaphor file */
  char           alnk_help[MAXFILE+MAXEXT];     /* AreaLink help text */
  char           maint_help[MAXFILE+MAXEXT];    /* Alnk Remote Maint. Helptext */
  char           rsvd2[MAXFILE+MAXEXT];         /* reserved */
  char           dflt_origin[MAXORIGIN];        /* default origin line */
  bit            rtnrecpt:1;                    /* True if to send rtn recpt */
  bit            del_empty_msg:1;               /* delete empty netmails (TOSS) */
  bit            ARCmail06:1;                   /* ARCmail 0.6 compatibility */
  bit            use_crc_names:1;               /* use crc-names for auto-areas */
  bit            req_all_allowed:1;             /* allow arealink +* command */
  bit            multi_tasking:1;               /* true if multi-tasking */
  bit            ignore_unknown:1;              /* ALNK ignores unknown systems */
  bit            singleextract:1;               /* extract 1 bundle at a time */
  bit            trunc_sent:1;                  /* 1 = Trunc 0 = Delete */
  bit            keep_alnk_answ:1;              /* keep arealink answer */
  bit            prod_names:1;                  /* use the FTSC product list */
  bit            swap_ems:1;                    /* swap to EMS */
  bit            swap_ext:1;                    /* swap to extended memory */
  bit            forward_everything:1;          /* forward req. not in fwd-lists */
  bit            direct_video:1;                /* use direct screen writing */
  bit            close_at_end:1;                /* close graphic window at end */
  bit            compr_after_pkt:1;             /* compress after each PKT? */
  bit            delete_bases:1;                /* when removing an area, */
                                                /* delete also squish/msg-base */
  bit            quiet_packers:1;               /* send packer output >NUL */
  bit            use_imcomp:1;                  /* call IMCOMP in case of tight */
                                                /* diskspace or abort at once */
  bit            sort_alnk_lists:1;             /* sort ALNK lists by areatag */
  bit            ulnk_hudson_passth:1;          /* unlinked Hudson areas passth */
  bit            compr_before_unpack:1;         /* compress before unpacking */
  bit            rsvd3:1;                       /* reserved */
  time32_t       last_run;                      /* last maintenance run */
  word           rsvd4;                         /* reserved */
  byte           rsvd5;                         /* reserved */
  byte           rsvd6;                         /* reserved */
  word           max_arcmail_size;              /* max size of arcmail bundles */
  word           pwd_expire_days;               /* days before pwd expr'd */
  word           max_pkt_size;                  /* max size of pkt to create */
  byte           max_add_pkt;                   /* PKTs to compress in one run */
  byte           pkt_not_for_us;                /* how to handle PKTs not for us */
  byte           environment;                   /* FroDo, Binkley or Intermail */
  byte           max_msg_size;                  /* max size of netmail (split) */
  byte           via_line;                      /* add Via Line to netmails */
  byte           dupe_ring;                     /* Check for possible d-rings */
  byte           cpd_check;                     /* circular path detection */
  byte           pers_mail;                     /* use personal mail feature */
  byte           unlink_req;                    /* Unlink areas without dlink */
  byte           keep_alnk_req;                 /* keep arealink request */
  byte           rsvd7;                         /* reserved */
  unsigned long  max_dupes;                     /* max dupes kept in dbase */
  word           max_files_per_dir;             /* max. nr files when autocreate */
  byte           deadlink_days;                 /* nr of days for a dealink req */
  byte           rsvd8;                         /* reserved */
  char           bbs_system;                    /* BBS software used */
  char           new_areas[MAXPATH];            /* name of file for new areas */
  word           sp_before_unpack;              /* min. diskspace required */
  word           sp_before_toss;                /* before decompress, toss */
  word           sp_before_compress;            /* and compress (in MB). */
  char           kill_dead;                     /* Kill Dead Selection */
  word           prod[20];                      /* Type2+ product codes */
  long           setup_pwd_crc;                 /* Setup password (CRC) */
  char           rule_path[MAXPATH];            /* path to the rule-files */
  char           local_inbound[MAXPATH];        /* path to local PKTs (protected) */
  char           rsvd9[556];                    /* reserved */
  struct fwd_link fwd[MAXFWDLINK];              /* forward link requests */
  char           echojam[MAXPATH];              /* path to ECHOMAIL.JAM */
  char           before_toss_ii[MAXPATH];       /* call before proc. the PKTs */
  char           userbase[MAXPATH];             /* path to the userbase */
  unsigned long  stoptossmsgs;                  /* stop tossing after xxxxx msgs */
  unsigned long  stoptossnetmsgs;               /* stop tossing after xxxxx net */
                                                /* msgs within a PKT or at all */
  char           ignorelist[MAXPATH];           /* list of areas to suppress */
  char           db_queue[MAXPATH];             /* D'Bridge queue directory */
  long           log_level;                     /* logging level */
  char           att_status;                    /* Def. status of attach msg */
  char           msg_status;                    /* Def. status of Alnk msgs */
  char           filler[278];                   /* reserved */
};


/*
 *
 *  IMAIL.AR structure
 *
 */

struct areas_record_type
{
  char           aname[MAXTAG];              /* area name */
  char           comment[61];                /* area comment */
  char           origin[MAXORIGIN];          /* origin line to use */
  byte           grp;                        /* area group */
  char           o_addr;                     /* address for origin */
  char           use_akas[MAXAKAS];          /* addresses for seen-bys */
  byte           msg_base_type;              /* message base type */
  byte           brd;                        /* board number */
  char           msg_path[MAXPATH];          /* MSG/Squish path */
  bit            active:1;                   /* flag area active */
  bit            zone_gate:1;                /* Zone-gate stripping */
  bit            tiny_seen:1;                /* tiny seen-by flag */
  bit            secure:1;                   /* secure flag */
  bit            import_seen:1;              /* import seen-by into base */
  bit            deleted:1;                  /* flag deleted area */
  bit            auto_added:1;               /* flag auto-added record */
  bit            mandatory:1;                /* area is mandatory */
  bit            read_only:1;                /* area is read only */
  bit            unlinked:1;                 /* area has been unlinked */
  bit            ulnk_req:1;                 /* perform unlinked requests? */
  bit            hidden:1;                   /* area is hidden */
  bit            to_link:1;                  /* should by processed by LINK */
  bit            check_dup:1;                /* check for dupes in this area? */
  bit            no_pause:1;                 /* %PAUSE not allowed in this echo? */
  bit            hide_seen:1;                /* Hide seens when importing */
  bit            manual:1;                   /* No changes via Arealink */
  bit            fwdreq_pending:1;           /* Requested but yet not arrived */
  bit            sqkillfly:1;                /* Squish 'Kill on the fly' */
  bit            dupe_msgid:1;               /* Dupecheck on MSGID only? */
  bit            deadlink_req:1;             /* Deadlink request has been sent */
  bit            packing_pending:1;          /* areas contains msgs deleted by KILL */
  bit            rsvd:1;                     /* reserved */
  byte           user_bits;                  /* 8 user-available bits */
  byte           days;                       /* days to keep messages */
  word           msgs;                       /* num messages to keep */
  struct im_stats stats;                     /* statistics */
  time32_t       creation;                   /* date/time of statistic start */
  time32_t       update;                     /* last update by midnight update */
  time32_t       marked;                     /* used by kill dead */
  byte           kill_dead;                  /* kill echos without traffic */
  word           read_sec;                   /* Security level for read access */
  word           write_sec;                  /* Security level for write access */
  char           rulename[MAXFILE+MAXEXT];   /* filename for the rule file */
  long           msg_counter;                /* Msg Counter only reset by PACK */
  char           filler[12];
  struct eaddress exp[MAXEXPORT];         /* export list */
};


//  ------------------------------------------------------------------

#if defined(GOLD_CANPACK)
#pragma pack()
#endif


//  ------------------------------------------------------------------

#endif

//  ------------------------------------------------------------------
