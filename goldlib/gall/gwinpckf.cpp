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
//  Allows user to select a file name.
//  ------------------------------------------------------------------

#include <gctype.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <gmemdbg.h>
#include <gfilutil.h>
#include <gwildmat.h>
#include <gwinall.h>
#include <gfilutil.h>
#include <gdirposx.h>
#include <gstrall.h>


//  ------------------------------------------------------------------

static bool path_in_title, case_sensitive;
static VfvCP open_function = NULL;
static char* cwdp;
static char* tcwdp;
static char* namextp;


//  ------------------------------------------------------------------
//  this function is the compare function for qsort()

static int compare(const char** str1, const char** str2) {

  // Sort with directories first
  bool dir1 = !!strchr(*str1, GOLD_SLASH_CHR);
  bool dir2 = !!strchr(*str2, GOLD_SLASH_CHR);
  int cmp = compare_two(dir2, dir1);
  if(cmp != 0)
    return cmp;

  Path p1, p2;
  strcpy(p1, *str1); if(dir1) { p1[strlen(p1)-1] = NUL; }
  strcpy(p2, *str2); if(dir2) { p2[strlen(p2)-1] = NUL; }

  if(case_sensitive)
    return strcmp(p1, p2);
  else {
    cmp = stricmp(p1, p2);
    if(cmp == 0)
      cmp = strcmp(p1, p2);
  }

  return cmp;
}


//  ------------------------------------------------------------------
//  this function displays the title on the pick window border

static void disp_title() {

  if(path_in_title) {
    char buf[sizeof(Path)+2];
    strcpy(buf, " ");
    PathCopy(buf+1, cwdp);
    strcat(buf, namextp);
    strcat(buf, " ");

    wtitle(buf, TCENTER, gwin.active->battr);
  }

  if(open_function)
    (*open_function)();
}


//  ------------------------------------------------------------------
//  this function frees all allocated strings in the array of pointers

static void free_strings(char** p, int numelems) {

  for(int i=0;i<numelems;i++)
    throw_xfree(p[i]);
}


//  ------------------------------------------------------------------

static void pre_exit(char** p, int numelems) {

  free_strings(p,numelems);
  throw_xfree(p);
  gchdir(tcwdp);
}


//  ------------------------------------------------------------------

bool wpickfile(int srow, int scol, int erow, int ecol, int btype, int bordattr, int winattr, int barattr, bool title, string &filespec, VfvCP open, bool casesens) {

  Path cwd, dir, namext, tcwd, path, spec;

  cwdp = cwd;
  tcwdp = tcwd;
  namextp = namext;

  // allocate space to hold array of char pointers
  int allocated = 100;
  char** p = (char**)throw_xmalloc(allocated*sizeof(char*));

  // set static variables
  open_function = open;
  path_in_title = title;
  case_sensitive = casesens;

  // save current working directory
  getcwd(tcwd, GMAXPATH);

  // if drive was specified, change to it
  char* q = strxcpy(spec, filespec.c_str(), sizeof(Path));

  // split up input filespec into its current
  // working directory and filename/extension
  char* r = strrchr(q, GOLD_SLASH_CHR);
  #if defined(__HAVE_DRIVES__)
  if(r == NULL)
    if(q[1] == ':')
      r = q + 1;
  #endif
  if(r == NULL) {
    *dir = NUL;
    strcpy(namext, q);
  }
  else {
    strins(" ", ++r, 0);
    *r++ = NUL;
    strcpy(dir, q);
    strcpy(namext, r);
  }

  bool finished;
  int picked, files;

  do {

    // if directory was specified, change to it
    if(*dir) {
      if(gchdir(dir)) {
        pre_exit(p, 0);
        return false;
      }
    }

    // get current working directory
    getcwd(cwd, GMAXPATH);
    strcpy(dir, cwd);

    // find all directories plus all files matching input filespec in
    // current directory, allocating an array element for each
    files = 0;
    picked = -1;
    gposixdir d(dir);
    const gdirentry *de;
    if(d.ok) {
      while((de = d.nextentry()) != NULL) {
        const char* name = NULL;
        if(de->is_directory()) {
          if(de->name != ".") {
            strxmerge(path, sizeof(Path), de->name.c_str(), GOLD_SLASH_STR, NULL);
            name = path;
          }
        }
        else if(de->is_file()) {
          if(case_sensitive ? gwildmat(de->name.c_str(), namext) : gwildmati(de->name.c_str(), namext))
            name = de->name.c_str();
        }
        if(name) {
          p[files] = throw_xstrdup(name);
          files++;
          if(files == allocated-1) {
            allocated *= 2;
            p = (char**)throw_xrealloc(p, allocated*sizeof(char*));
          }
        }
      }
    }
    p[files] = NULL;

    // sort files in array by swapping their pointers
    qsort(p, files, sizeof(char*), (StdCmpCP)compare);

    // let user pick file
    if(files)
      picked = wpickstr(srow, scol, erow, ecol, btype, bordattr, winattr, barattr, p, 0, disp_title);
    if(picked == -1 or files == 0) {
      pre_exit(p, files);
      return false;
    }

    // see if a directory was selected. if so save
    // directory name, otherwise build whole path name
    q = strchr(p[picked], GOLD_SLASH_CHR);
    if(q) {
      finished = false;
      strcpy(dir, p[picked]);
      r = strrchr(dir, GOLD_SLASH_CHR);
      if(r)
        *r = NUL;
      *q = NUL;
    }
    else {
      finished = true;
      PathCopy(filespec, cwd);
      filespec += p[picked];
    }

    // free allocated strings
    free_strings(p, files);

  // if a directory was selected, go back and do again
  } while(not finished);

  // change back to current drive and directory
  gchdir(tcwd);

  // free allocated char pointers
  throw_xfree(p);

  // return normally
  return true;
}


//  ------------------------------------------------------------------

