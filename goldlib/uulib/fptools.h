/*
 * fptools.c, some helper functions for getcgi.c and uu(en|de)view
 *
 * Distributed under the terms of the GNU General Public License.
 * Use and be happy.
 */

/*
 * Some handy, nonstandard functions. Note that the original may
 * be both faster and better. ``better'', if your compiler allows
 * cleaner use of such functions by proper use of ``const''.
 *
 * $Id$
 */

#ifndef __FPTOOLS_H__
#define __FPTOOLS_H__

#ifndef TOOLEXPORT
    #define TOOLEXPORT
#endif

void    TOOLEXPORT  _FP_free    (void *);
char *  TOOLEXPORT  _FP_strdup  (char *);
char *  TOOLEXPORT  _FP_strncpy (char *, char *, int);
void *  TOOLEXPORT  _FP_memdup  (void *, int);
int     TOOLEXPORT  _FP_stricmp (char *, char *);
int     TOOLEXPORT  _FP_strnicmp(char *, char *, int);
char *  TOOLEXPORT  _FP_strrstr (char *, char *);
char *  TOOLEXPORT  _FP_stoupper(char *);
char *  TOOLEXPORT  _FP_stolower(char *);
int     TOOLEXPORT  _FP_strmatch(char *, char *);
char *  TOOLEXPORT  _FP_strstr  (char *, char *);
char *  TOOLEXPORT  _FP_stristr (char *, char *);
char *  TOOLEXPORT  _FP_strirstr(char *, char *);
char *  TOOLEXPORT  _FP_strrchr (char *, int);
char *  TOOLEXPORT  _FP_fgets   (char *, int, FILE *);
char *  TOOLEXPORT  _FP_strpbrk (char *, char *);
char *  TOOLEXPORT  _FP_strtok  (char *, char *);
char *  TOOLEXPORT  _FP_cutdir  (char *);
#if 0
    char *  TOOLEXPORT  _FP_strerror(int);
    char *  TOOLEXPORT  _FP_tempnam (char *, char *);
#endif

#endif
