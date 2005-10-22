/* This is modifyed part of HUSKY project */
/*
 *  SMAPI; Modified Squish MSGAPI
 *
 *  Squish MSGAPI0 is copyright 1991 by Scott J. Dudley.  All rights reserved.
 *  Modifications released to the public domain.
 *
 *  Use of this file is subject to the restrictions contain in the Squish
 *  MSGAPI0 licence agreement.  Please refer to licence.txt for complete
 *  details of the licencing restrictions.  If you do not find the text
 *  of this agreement in licence.txt, or if you do not have this file,
 *  you should contact Scott Dudley at FidoNet node 1:249/106 or Internet
 *  e-mail Scott.Dudley@f106.n249.z1.fidonet.org.
 *
 *  In no event should you proceed to use any of the source files in this
 *  archive without having accepted the terms of the MSGAPI0 licensing
 *  agreement, or such other agreement as you are able to reach with the
 *  author.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hsksupp.h"

#if defined(__TURBOC__) || defined(__DJGPP__)
#include <dir.h>
#endif

#if !defined( __IBMC__) && !defined(UNIX) && !defined(__MINGW32__) && !(defined(_MSC_VER) && (_MSC_VER >= 1200))
#include <dos.h>
#endif

#include "ffind.h"

#ifdef OS2
#define INCL_NOPM
#define INCL_DOS
#include <os2.h>
#if defined(__386__) || defined(__FLAT__)
#undef DosQPathInfo
#define DosQPathInfo(a,b,c,d,e)  DosQueryPathInfo(a,b,c,d)
#endif
#endif

/*#ifdef UNIX
#include "patmat.h"
#endif*/

/*
 *  FFindOpen;  Use like MSDOS "find first" function,  except be sure to
 *  release allocated system resources by caling FFindClose() with the
 *  handle returned by this function.
 *
 *  Returns: NULL == File not found.
 */

FFIND *FFindOpen(const char *filespec, uint16_t attribute)
{
    FFIND *ff;

    ff = malloc(sizeof(FFIND));

    if (ff != NULL)
    {
#if defined(__TURBOC__) || defined(__DJGPP__)

        if (findfirst(filespec, &(ff->ffbuf), attribute) != 0)
        {
            free(ff);
            ff = NULL;
        }
        else
        {
            ff->ff_attrib = ff->ffbuf.ff_attrib;
            ff->ff_ftime  = ff->ffbuf.ff_ftime;
            ff->ff_fdate  = ff->ffbuf.ff_fdate;
            ff->ff_fsize  = ff->ffbuf.ff_fsize;
            memcpy(ff->ff_name, ff->ffbuf.ff_name, sizeof(ff->ff_name));
            ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
        }

#elif (defined(_MSC_VER) && (_MSC_VER < 1200)) || defined(__WATCOMC__)

        if (_dos_findfirst(filespec, attribute, &(ff->ffbuf)) != 0)
        {
            free(ff);
            ff = NULL;
        }
        else
        {
            ff->ff_attrib = ff->ffbuf.attrib;
            ff->ff_ftime  = ff->ffbuf.wr_time;
            ff->ff_fdate  = ff->ffbuf.wr_date;
            ff->ff_fsize  = ff->ffbuf.size;
            memcpy(ff->ff_name, ff->ffbuf.name, sizeof(ff->ff_name));
            ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
        }

#elif defined(OS2)

#if defined(__386__) || defined(__FLAT__)
        ULONG SearchCount = 1;
        FILEFINDBUF3 findbuf;
#else
        USHORT SearchCount = 1;
        FILEFINDBUF findbuf;
#endif

        ff->hdir = HDIR_CREATE;

        if (!DosFindFirst((PBYTE) filespec, &ff->hdir, attribute, &findbuf, sizeof(findbuf), &SearchCount, 1L))
        {
            ff->ff_attrib = (char)findbuf.attrFile;
            ff->ff_fsize = findbuf.cbFile;

            ff->ff_ftime = *((USHORT *) & findbuf.ftimeLastWrite);
            ff->ff_fdate = *((USHORT *) & findbuf.fdateLastWrite);

            strncpy(ff->ff_name, findbuf.achName, sizeof(ff->ff_name));
            ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#elif defined(UNIX)

        char *p;
        int fin = 0;
        struct dirent *de;

        p = strrchr(filespec, '/');
        if (p == NULL)
        {
            strcpy(ff->firstbit, ".");
            strcpy(ff->lastbit, filespec);
        }
        else if (p == filespec)
	{
	    strcpy(ff->firstbit, "/");
	    strcpy(ff->lastbit, filespec+1);
	}
	else
        {
            memcpy(ff->firstbit, filespec, p - filespec);
            ff->firstbit[p - filespec] = '\0';
            strcpy(ff->lastbit, p + 1);
        }
        ff->dir = opendir(ff->firstbit);
        if (ff->dir != NULL)
        {
            while (!fin)
            {
                de = readdir(ff->dir);
                if (de == NULL)
                {
                    closedir(ff->dir);
                    free(ff);
                    ff = NULL;
                    fin = 1;
                }
                else
                {
                    if (patmat(de->d_name, ff->lastbit))
                    {
                        strncpy(ff->ff_name, de->d_name, sizeof ff->ff_name);
                        ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
			ff->ff_fsize = -1L; /* All who wants to know it's size 
					     * must read it by himself
					     */
                        fin = 1;
                    }
                }
            }
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#elif defined(SASC)

        char *temp;
        int error;

        temp = strrchr(filespec, '/');
        if (temp == NULL)
        {
            temp = strrchr(filespec, '\\');
        }
        if (temp == NULL)
        {
            temp = strrchr(filespec, ':');
        }
        if (temp == NULL)
        {
            strcpy(ff->prefix, "");
        }
        else
        {
            memcpy(ff->prefix, filespec, temp - filespec + 1);
            *(ff->prefix + (temp - filespec + 1)) = '\0';
        }
        error = dfind(&ff->info, filespec, 0);
        if (error == 0)
        {
            strcpy(ff->ff_name, ff->prefix);
            strcat(ff->ff_name, ff->info.fib_FileName);
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#elif defined(__RSXNT__) || defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 1200))

        ff->hDirA = FindFirstFile(filespec, &(ff->InfoBuf));
        ff->attrib_srch = (char)attribute;
        while (ff->hDirA != INVALID_HANDLE_VALUE)
        {
            if (strlen(ff->InfoBuf.cFileName) < sizeof(ff->ff_name))
            {
                if ((!(ff->InfoBuf.dwFileAttributes &
                       FILE_ATTRIBUTE_DIRECTORY)) ||
                    (ff->attrib_srch & MSDOS_SUBDIR))
                {
                    break;
                }
            }
            /* skip file for some reason */
            if (!FindNextFile(ff->hDirA, &(ff->InfoBuf)))
            {
                if (ff->hDirA != INVALID_HANDLE_VALUE)
                {
                    FindClose(ff->hDirA);
                }
                ff->hDirA = INVALID_HANDLE_VALUE;
            }
        }
        if (ff->hDirA != INVALID_HANDLE_VALUE)
        {
            strcpy(ff->ff_name, ff->InfoBuf.cFileName);
            ff->ff_fsize = ff->InfoBuf.nFileSizeLow;
            ff->ff_attrib = 0;
            if (ff->InfoBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                ff->ff_attrib |= MSDOS_SUBDIR;
            }
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#else
#error Unknown compiler!
#endif

    }

    return ff;
}

/*
 *  FFindNext: Returns 0 if next file was found, non-zero if it was not.
 */

int FFindNext(FFIND * ff)
{
    int rc = -1;

    if (ff != NULL)
    {
#if defined(__TURBOC__) || defined(__DJGPP__)

        rc = findnext(&(ff->ffbuf));

        ff->ff_attrib = ff->ffbuf.ff_attrib;
        ff->ff_ftime  = ff->ffbuf.ff_ftime;
        ff->ff_fdate  = ff->ffbuf.ff_fdate;
        ff->ff_fsize  = ff->ffbuf.ff_fsize;
        memcpy(ff->ff_name, ff->ffbuf.ff_name, sizeof(ff->ff_name));
        ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';

#elif (defined(_MSC_VER) && (_MSC_VER < 1200)) || defined(__WATCOMC__)

        rc = _dos_findnext(&(ff->ffbuf));

        ff->ff_attrib = ff->ffbuf.attrib;
        ff->ff_ftime  = ff->ffbuf.wr_time;
        ff->ff_fdate  = ff->ffbuf.wr_date;
        ff->ff_fsize  = ff->ffbuf.size;
        memcpy(ff->ff_name, ff->ffbuf.name, sizeof(ff->ff_name));
        ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';

#elif defined(OS2)

#if defined(__386__) || defined(__FLAT__)
        ULONG SearchCount = 1;
        FILEFINDBUF3 findbuf;
#else
        USHORT SearchCount = 1;
        FILEFINDBUF findbuf;
#endif

        if (ff->hdir && !DosFindNext(ff->hdir, &findbuf, sizeof(findbuf),
          &SearchCount))
        {
            ff->ff_attrib = (char)findbuf.attrFile;
            ff->ff_ftime = *((USHORT *) & findbuf.ftimeLastWrite);
            ff->ff_fdate = *((USHORT *) & findbuf.fdateLastWrite);
            ff->ff_fsize = findbuf.cbFile;
            strncpy(ff->ff_name, findbuf.achName, sizeof(ff->ff_name));
            ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
            rc = 0;
        }

#elif defined(UNIX)

        int fin = 0;
        struct dirent *de;

        while (!fin)
        {
            de = readdir(ff->dir);
            if (de == NULL)
            {
               closedir(ff->dir);
               ff->dir = NULL;
               fin = 1;
            }
            else
            {
                if (patmat(de->d_name, ff->lastbit))
                {
                    strncpy(ff->ff_name, de->d_name, sizeof ff->ff_name);
                    ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
		    ff->ff_fsize = -1L; /* All who wants to know it's size 
					 * must read it by himself
					 */
		    fin = 1;
                    rc = 0;
                }
            }
        }

#elif defined(SASC)
        int error = 0;

        error = dnext(&ff->info);
        if (error == 0)
        {
            strcpy(ff->ff_name, ff->prefix);
            strcat(ff->ff_name, ff->info.fib_FileName);
            rc = 0;
        }
#elif defined(__RSXNT__) || defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 1200))

        do
        {
            if (!FindNextFile(ff->hDirA, &(ff->InfoBuf)))
            {
                if (ff->hDirA != INVALID_HANDLE_VALUE)
                {
                    FindClose(ff->hDirA);
                }
                ff->hDirA = INVALID_HANDLE_VALUE;
            }
            else
            {

                if (strlen(ff->InfoBuf.cFileName) < sizeof(ff->ff_name))
                {
                    if ((!(ff->InfoBuf.dwFileAttributes &
                          FILE_ATTRIBUTE_DIRECTORY)) ||
                        (ff->attrib_srch & MSDOS_SUBDIR))
                    {
                        break;
                    }
                }
            }
        } while (ff->hDirA != INVALID_HANDLE_VALUE);

        if (ff->hDirA != INVALID_HANDLE_VALUE)
        {

            strcpy(ff->ff_name, ff->InfoBuf.cFileName);
            ff->ff_fsize = ff->InfoBuf.nFileSizeLow;
            ff->ff_attrib = 0;
            if (ff->InfoBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                ff->ff_attrib |= MSDOS_SUBDIR;
            }
            rc = 0;
        }

#else
#error Unable to determine compiler and target operating system!
#endif
    }

    return rc;
}

/*
 *  FFindClose: End a directory search.  Failure to call this function
 *  will result in unclosed file handles under OS/2, and unreleased
 *  memory in both DOS and OS/2.
 */

void FFindClose(FFIND * ff)
{
    if (ff != NULL)
    {
#if defined(__TURBOC__) || defined(__DJGPP__)
#elif (defined(__WATCOMC__) && defined (__NT__)) || (defined(_MSC_VER) && (_MSC_VER < 1200) && !defined(MSDOS))
        _dos_findclose(&(ff->ffbuf));
#elif defined(OS2)
        if (ff->hdir)
        {
            DosFindClose(ff->hdir);
        }
#elif defined(UNIX)
        if (ff->dir)
        {
            closedir(ff->dir);
        }
#elif defined(SASC)
#elif defined(__RSXNT__) || defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 1200))
        if (ff->hDirA != INVALID_HANDLE_VALUE)
        {
            FindClose(ff->hDirA);
        }
#endif
        free(ff);
    }
}

/*
 *  FindInfo: This function was added because it is SIGNIFICANTLY faster
 *  under OS/2 to call DosQPathInfo() rather than DosFindFirst() if all
 *  you are interested in is getting a specific file's date/time/size.
 *
 *  PLF Thu  10-17-1991  18:12:37
 */

FFIND *FindInfo(const char *filespec)
{
#if !defined(OS2) && !defined(__OS2__)
    return FFindOpen(filespec, 0);
#else
    FFIND *ff;
    FILESTATUS fs;
    const char *f;

    ff = malloc(sizeof *ff);
    if (ff == NULL)
    {
        return NULL;
    }

    memset(ff, 0, sizeof *ff);
    if (!DosQPathInfo((PBYTE) filespec, FIL_STANDARD, (PBYTE) &fs, sizeof fs, 0L))
    {
        ff->ff_attrib = (char)fs.attrFile;
        ff->ff_ftime = *((USHORT *) & fs.ftimeLastWrite);
        ff->ff_fdate = *((USHORT *) & fs.fdateLastWrite);
        ff->ff_fsize = fs.cbFile;

        /* isolate file name */
        f = strrchr(filespec, '\\');
        if (f == NULL)
        {
            f = filespec;
        }
        else
        {
            f++;
        }
        strncpy(ff->ff_name, f, sizeof(ff->ff_name));
        ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
    }
    else
    {
        free(ff);
        return NULL;
    }
    return ff;
#endif
}
