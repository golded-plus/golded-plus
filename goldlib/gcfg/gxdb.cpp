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
//  Read areas from D'Bridge
//  ------------------------------------------------------------------

#include <cstdlib>
#include <gmemdbg.h>
#include <gstrall.h>
#undef GCFG_NODB
#include <gedacfg.h>
#include <gs_db.h>


//  ------------------------------------------------------------------
//  Read D'Bridge 1.30

void gareafile::ReadDB130(char* tag, char* dbpath) {

  AreaCfg aa;
  DB130_AA1 AA1;
  DB130_AA2 AA2;
  FILE *fp1, *fp2;
  Path file1, file2;

  MakePathname(file1, dbpath, "dbridge.aa1");
  MakePathname(file2, dbpath, "dbridge.aa2");

  fp1 = fsopen(file1, "rb", sharemode);
  if(fp1) {
    setvbuf(fp1, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file1 << endl;

    fp2 = fsopen(file2, "rb", sharemode);
    if(fp2) {
      setvbuf(fp2, NULL, _IOFBF, 8192);

      if(not quiet)
        cout << "* Reading " << file2 << endl;

      while(fread(&AA1, sizeof(DB130_AA1), 1, fp1) == 1) {

        fread(&AA2, sizeof(DB130_AA2), 1, fp2);

        if(AA1.allocated and strchr("QFqf", AA2.msgbase)) {
          aa.reset();
          switch(toupper(AA2.msgbase)) {
            case 'Q':
              aa.msgbase = GMB_HUDSON;
              aa.board = AA2.board;
              break;
            case 'F':
              aa.msgbase = fidomsgtype;
              aa.setpath(STRNP2C(AA2.path));
              break;
          }
          switch(toupper(AA2.kind)) {
            case 'N':
              aa.type = GMB_NET;
              aa.attr = attribsnet;
              break;
            case 'E':
              aa.type = GMB_ECHO;
              aa.attr = attribsecho;
              break;
            default:
              aa.type = GMB_LOCAL;
              aa.attr = attribslocal;
          }
          STRNP2C(AA2.origin);
          aa.setorigin(AA2.origin);
          aa.attr.pvt(AA2.ispvt);
          aa.groupid = (char)AA2.group;
          aa.setechoid(STRNP2C(AA1.echoid));
          aa.setdesc(STRNP2C(AA2.desc));
          aa.aka = AA2.addr;
          AddNewArea(aa);
        }
      }

      fclose(fp2);
    }

    fclose(fp1);
  }
}


//  ------------------------------------------------------------------
//  Read D'Bridge B1046

void gareafile::ReadDB1046(char* file, char* tag) {

  AreaCfg aa;
  DB1046_ADF* ADF = (DB1046_ADF*)throw_calloc(1, sizeof(DB1046_ADF));

  FILE* fp = fsopen(file, "rb", sharemode);
  if(fp) {

    setvbuf(fp, NULL, _IOFBF, 8192);

    if(not quiet)
      cout << "* Reading " << file << endl;

    while(fread(ADF, sizeof(DB1046_ADF), 1, fp) == 1) {
      if(ADF->allocated and strchr("QFqf", ADF->msgbase)) {
        aa.reset();
        switch(toupper(ADF->msgbase)) {
          case 'Q':
            aa.msgbase = GMB_HUDSON;
            aa.board = ADF->board;
            break;
          case 'F':
            aa.msgbase = fidomsgtype;
            aa.setpath(STRNP2C(ADF->path));
            break;
        }
        switch(toupper(ADF->kind)) {
          case 'N':
            aa.type = GMB_NET;
            aa.attr = attribsnet;
            break;
          case 'E':
            aa.type = GMB_ECHO;
            aa.attr = attribsecho;
            break;
          default:
            aa.type = GMB_LOCAL;
            aa.attr = attribslocal;
        }
        STRNP2C(ADF->origin);
        aa.setorigin(ADF->origin);
        aa.attr.pvt(ADF->ispvt);
        aa.groupid = (char)ADF->group;
        aa.setechoid(STRNP2C(ADF->echoid));
        aa.setdesc(STRNP2C(ADF->desc));
        aa.aka = ADF->addr;
        AddNewArea(aa);
      }
    }

    fclose(fp);
  }

  throw_free(ADF);
}


//  ------------------------------------------------------------------
//  Read D'Bridge B1047.A22 and B1047.A27+

void gareafile::ReadDB1047A22(char* file, int reclen, char* tag) {

  AreaCfg aa;
  FILE* fp;
  DB1047A22_ADF* ADF;

  ADF = (DB1047A22_ADF *)throw_malloc(reclen);
  if(ADF) {

    fp = fsopen(file, "rb", sharemode);
    if(fp) {
      setvbuf(fp, NULL, _IOFBF, 8192);

      if(not quiet)
        cout << "* Reading " << file << endl;

      while(fread(ADF, reclen, 1, fp) == 1) {
        if(ADF->allocated and strchr("QFqf", ADF->msgbase)) {
          aa.reset();
          switch(toupper(ADF->msgbase)) {
            case 'Q':
              aa.msgbase = GMB_HUDSON;
              aa.board = ADF->board;
              break;
            case 'F':
              aa.msgbase = fidomsgtype;
              aa.setpath(STRNP2C(ADF->path));
              break;
          }
          switch(toupper(ADF->kind)) {
            case 'N':
              aa.type = GMB_NET;
              aa.attr = attribsnet;
              break;
            case 'E':
              aa.type = GMB_ECHO;
              aa.attr = attribsecho;
              break;
            default:
              aa.type = GMB_LOCAL;
              aa.attr = attribslocal;
          }
          STRNP2C(ADF->origin);
          aa.setorigin(ADF->origin);
          aa.attr.pvt(ADF->ispvt);
          aa.groupid = (char)ADF->group;
          aa.setechoid(STRNP2C(ADF->echoid));
          aa.setdesc(STRNP2C(ADF->desc));
          aa.aka = ADF->addr;
          AddNewArea(aa);
        }
      }

      fclose(fp);
    }

    throw_free(ADF);
  }
}


//  ------------------------------------------------------------------
//  Read D'Bridge B2011

void gareafile::ReadDB2011(char* file, int reclen, char* tag) {

  AreaCfg aa;
  FILE* fp;
  DB2011_ADF* ADF;

  ADF = (DB2011_ADF *)throw_malloc(reclen);
  if(ADF) {

    fp = fsopen(file, "rb", sharemode);
    if(fp) {
      setvbuf(fp, NULL, _IOFBF, 8192);

      if(not quiet)
        cout << "* Reading " << file << endl;

      while(fread(ADF, reclen, 1, fp) == 1) {
        if(ADF->allocated and strchr("QFqf", ADF->msgbase)) {
          aa.reset();
          switch(toupper(ADF->msgbase)) {
            case 'Q':
              if(ADF->board < 1 or ADF->board > 200)
                continue;   // Bad area number
              aa.msgbase = GMB_HUDSON;
              aa.board = ADF->board;
              break;
            case 'F':
              aa.msgbase = fidomsgtype;
              aa.setpath(STRNP2C(ADF->path));
              break;
          }
          switch(toupper(ADF->kind)) {
            case 'N':
              aa.type = GMB_NET;
              aa.attr = attribsnet;
              break;
            case 'E':
              aa.type = GMB_ECHO;
              aa.attr = attribsecho;
              break;
            default:
              aa.type = GMB_LOCAL;
              aa.attr = attribslocal;
          }
          STRNP2C(ADF->origin);
          aa.setorigin(ADF->origin);
          aa.attr.pvt(ADF->ispvt);
          aa.groupid = (char)ADF->group;
          aa.setechoid(STRNP2C(ADF->echoid));
          aa.setdesc(STRNP2C(ADF->desc));
          aa.aka = ADF->addr;
          AddNewArea(aa);
        }
      }

      fclose(fp);
    }

    throw_free(ADF);
  }
}


//  ------------------------------------------------------------------
//  Read D'Bridge areas, various versions...

void gareafile::ReadDBridge(char* tag) {

  AreaCfg aa;
  FILE* fp;
  char* ptr;
  int line;
  char buf[256], type, options[80];
  Path dbpath, file, badecho, netpath;

  *dbpath = NUL;
  strcpy(options, tag);

  ptr = strtok(tag, " \t");
  while(ptr) {
    if(*ptr != '-') {
      AddBackslash(strcpy(dbpath, ptr));
      break;
    }
    ptr = strtok(NULL, " \t");
  }
  if(*dbpath == NUL) {
    ptr = getenv("DBRIDGE");
    if(ptr == NULL)
      ptr = getenv("DB");
    if(ptr)
      AddBackslash(strcpy(dbpath, ptr));
  }
  if(*dbpath == NUL)
    strcpy(dbpath, areapath);

  MakePathname(file, dbpath, "dbridge.prm");

  fp = fsopen(file, "rt", sharemode);
  if(fp) {

    if(not quiet)
      cout << "* Reading " << file << endl;

    // Read netmail storage method etc
    for(line=1; line <= 2; line++)
      fgets(buf, 255, fp);
    type = (char)toupper(*buf);

    // Fido-style netmail path
    line++;
    fgets(buf, 255, fp);
    strtrim(buf);
    strcpy(netpath, buf);

    // BADECHO area
    for(; line <= 8; line++)
      fgets(buf, 255, fp);
    strtrim(buf);
    strcpy(badecho, buf);

    // Hudson path
    for(; line <= 11; line++)
      fgets(buf, 255, fp);
    strtrim(buf);
    CfgHudsonpath(buf);

    // Primary address
    for(; line <= 15; line++)
      fgets(buf, 255, fp);
    strtrim(buf);
    CfgAddress(buf);

    // Username
    for(; line <= 17; line++)
      fgets(buf, 255, fp);
    strtrim(buf);
    //CfgUsername(buf);

    // Hudson netmail board
    for(; line <= 20; line++)   // NOTE: was 17 in older versions
      fgets(buf, 255, fp);

    // Address/misc field
    if(type == 'F' or type == 'Q') {
      aa.reset();
      aa.type = GMB_NET;
      aa.attr = attribsnet;
      if(type == 'F') {
        aa.msgbase = fidomsgtype;
        aa.setpath(strbtrim(netpath));
      }
      else if(type == 'Q') {
        strtrim(buf);
        aa.msgbase = GMB_HUDSON;
        aa.board = atoi(buf+50);
      }
      aa.setdesc("D'Bridge Netmail");
      aa.setautoid("NETMAIL");
      AddNewArea(aa);
    }

    strbtrim(badecho);
    if(*badecho) {
      aa.reset();
      aa.msgbase = fidomsgtype;
      aa.type = GMB_ECHO;
      aa.attr = attribsecho;
      aa.setpath(badecho);
      aa.setdesc("D'Bridge Bad Messages");
      aa.setautoid("ECHO_BAD");
      AddNewArea(aa);
    }

    fclose(fp);
  }

  // Read the areafile from the correct version
  MakePathname(file, dbpath, "dbridge.adf");
  if(fexist(file)) {
    long size = GetFilesize(file);
    if(size != -1L) {
      if(striinc("-2011", options))
        ReadDB2011(file, sizeof(DB2011_ADF), options);
      else if(striinc("-1047A27", options))
        ReadDB1047A22(file, sizeof(DB1047A27_ADF), options);
      else if(striinc("-1047A22", options))
        ReadDB1047A22(file, sizeof(DB1047A22_ADF), options);
      else if(striinc("-1046", options))
        ReadDB1046(file, options);
      else if((size % sizeof(DB2011_ADF)) == 0)
        ReadDB2011(file, sizeof(DB2011_ADF), options);
      else if((size % sizeof(DB1047A27_ADF)) == 0)
        ReadDB1047A22(file, sizeof(DB1047A27_ADF), options);
      else if((size % sizeof(DB1047A22_ADF)) == 0)
        ReadDB1047A22(file, sizeof(DB1047A22_ADF), options);
      else if((size % sizeof(DB1046_ADF)) == 0)
        ReadDB1046(file, options);
    }
  }
  else if(fexist(AddPath(dbpath, "DBRIDGE.AA1"))) {
    ReadDB130(options, dbpath);
  }
}


//  ------------------------------------------------------------------
