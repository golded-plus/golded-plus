/*
 * fptools.c, some helper functions for getcgi.c and uu(en|de)view
 *
 * Distributed by the GNU General Public License. Use and be happy.
 * Read http://www.uni-frankfurt.de/~fp/Tools/Getcgi.html for more
 * information.                     fp@informatik.uni-frankfurt.de
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

#ifndef _ANSI_ARGS_
#ifdef PROTOTYPES
#define _ANSI_ARGS_(c)	c
#else
#define _ANSI_ARGS_(c)	()
#endif
#endif

#ifndef TOOLEXPORT
#define TOOLEXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

void	TOOLEXPORT	_FP_free	_ANSI_ARGS_((void *));
char *	TOOLEXPORT	_FP_strdup	_ANSI_ARGS_((char *));
char *	TOOLEXPORT	_FP_strncpy	_ANSI_ARGS_((char *, char *, int));
void *	TOOLEXPORT	_FP_memdup	_ANSI_ARGS_((void *, int));
int 	TOOLEXPORT	_FP_stricmp	_ANSI_ARGS_((char *, char *));
int 	TOOLEXPORT	_FP_strnicmp	_ANSI_ARGS_((char *, char *, int));
char *	TOOLEXPORT	_FP_strrstr	_ANSI_ARGS_((char *, char *));
char *	TOOLEXPORT	_FP_stoupper	_ANSI_ARGS_((char *));
char *	TOOLEXPORT	_FP_stolower	_ANSI_ARGS_((char *));
int 	TOOLEXPORT	_FP_strmatch	_ANSI_ARGS_((char *, char *));
char *	TOOLEXPORT	_FP_strstr	_ANSI_ARGS_((char *, char *));
char *	TOOLEXPORT	_FP_stristr	_ANSI_ARGS_((char *, char *));
char *	TOOLEXPORT	_FP_strirstr	_ANSI_ARGS_((char *, char *));
char *	TOOLEXPORT	_FP_strrchr	_ANSI_ARGS_((char *, int));
char *	TOOLEXPORT	_FP_fgets	_ANSI_ARGS_((char *, int, FILE *));
char *	TOOLEXPORT	_FP_strpbrk	_ANSI_ARGS_((char *, char *));
char *	TOOLEXPORT	_FP_strtok	_ANSI_ARGS_((char *, char *));
char *	TOOLEXPORT	_FP_cutdir	_ANSI_ARGS_((char *));
char *	TOOLEXPORT	_FP_strerror	_ANSI_ARGS_((int));
char *	TOOLEXPORT	_FP_tempnam	_ANSI_ARGS_((char *, char *));

#ifdef __cplusplus
}
#endif
#endif
