/*
 * This file is part of uudeview, the simple and friendly multi-part multi-
 * file uudecoder  program  (c) 1994-2001 by Frank Pilhofer. The author may
 * be contacted at fp@fpx.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Strings used in the library for easier translation etc.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef SYSTEM_WINDLL
#include <windows.h>
#endif
#ifdef SYSTEM_OS2
#include <os2.h>
#endif

#include <stdio.h>

#ifdef STDC_HEADERS
#include <stdlib.h>
#include <string.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#include <uudeview.h>
#include <uuint.h>
#include <uustring.h>

char * uustring_id = (char *)"$Id$";

typedef struct {
  int code;
  char * msg;
} stringmap;

/*
 * Map of messages. This table is not exported, the messages must
 * be retrieved via the below uustring() function.
 */

static stringmap messages[] = {
  /* I/O related errors/messages. Last parameter is strerror() */
  { S_NOT_OPEN_SOURCE,  (char *)"Could not open source file %s: %s" },
  { S_NOT_OPEN_TARGET,  (char *)"Could not open target file %s for writing: %s" },
  { S_NOT_OPEN_FILE,    (char *)"Could not open file %s: %s" },
  { S_NOT_STAT_FILE,    (char *)"Could not stat file %s: %s" },
  { S_SOURCE_READ_ERR,  (char *)"Read error on source file: %s" },
  { S_READ_ERROR,       (char *)"Error reading from %s: %s" },
  { S_IO_ERR_TARGET,    (char *)"I/O error on target file %s: %s" },
  { S_WR_ERR_TARGET,    (char *)"Write error on target file %s: %s" },
  { S_WR_ERR_TEMP,      (char *)"Write error on temp file: %s" },
  { S_TMP_NOT_REMOVED,  (char *)"Could not remove temp file %s: %s (ignored)" },

  /* some other problems */
  { S_OUT_OF_MEMORY,    (char *)"Out of memory allocating %d bytes" },
  { S_TARGET_EXISTS,    (char *)"Target file %s exists and overwriting is not allowed" },
  { S_NOT_RENAME,       (char *)"Could not change name of %s to %s" },
  { S_ERR_ENCODING,     (char *)"Error while encoding %s: %s" },
  { S_STAT_ONE_PART,    (char *)"Could not stat input, encoding to one part only" },
  { S_PARM_CHECK,       (char *)"Parameter check failed in %s" },
  { S_SHORT_BINHEX,     (char *)"BinHex encoded file %s ended prematurely (%ld bytes left)" },
  { S_DECODE_CANCEL,    (char *)"Decode operation canceled" },
  { S_ENCODE_CANCEL,    (char *)"Encode operation canceled" },
  { S_SCAN_CANCEL,      (char *)"Scanning canceled" },
  { S_SIZE_MISMATCH,    (char *)"%s: Decoded size (%ld) does not match expected size (%ld)" },
  { S_PSIZE_MISMATCH,   (char *)"%s part %d: Decoded size (%ld) does not match expected size (%ld)" },
  { S_CRC_MISMATCH,     (char *)"CRC32 mismatch in %s. Decoded file probably corrupt." },
  { S_PCRC_MISMATCH,    (char *)"PCRC32 mismatch in %s part %d. Decoded file probably corrupt." },

  /* informational messages */
  { S_LOADED_PART,      (char *)"Loaded from %s: '%s' (%s): %s part %d %s %s %s" },
  { S_NO_DATA_FOUND,    (char *)"No encoded data found in %s" },
  { S_NO_BIN_FILE,      (char *)"Oops, could not find decoded file?" },
  { S_STRIPPED_SETUID,  (char *)"Stripped setuid/setgid bits from target file %s mode %d" },
  { S_DATA_SUSPICIOUS,  (char *)"Data looks suspicious. Decoded file might be corrupt." },
  { S_NO_TEMP_NAME,     (char *)"Could not get name for temporary file" },
  { S_BINHEX_SIZES,     (char *)"BinHex file: data/resource fork sizes %ld/%ld" },
  { S_BINHEX_BOTH,      (char *)"BinHex file: both forks non-empty, decoding data fork" },
  { S_SMERGE_MERGED,    (char *)"Parts of '%s' merged with parts of '%s' (%d)" },
  
  /* MIME-related messages */
  { S_MIME_NO_BOUNDARY, (char *)"Multipart message without boundary ignored" },
  { S_MIME_B_NOT_FOUND, (char *)"Boundary expected on Multipart message but found EOF" },
  { S_MIME_MULTI_DEPTH, (char *)"Multipart message nested too deep" },
  { S_MIME_PART_MULTI,  (char *)"Handling partial multipart message as plain text" },

  { 0, (char *)"" }
};

/*
 * description of the return values UURET_*
 */

char *uuretcodes[] = {
  (char *)"OK",
  (char *)"File I/O Error",
  (char *)"Not Enough Memory",
  (char *)"Illegal Value",
  (char *)"No Data found",
  (char *)"Unexpected End of File",
  (char *)"Unsupported function",
  (char *)"File exists",
  (char *)"Continue -- no error",   /* only to be seen internally */
  (char *)"Operation Canceled"
};

/*
 * Names of encoding types
 */

char *codenames[8] = {
  (char *)"", (char *)"UUdata", (char *)"Base64", (char *)"XXdata", (char *)"Binhex", (char *)"Text", (char *)"Text", (char *)"yEnc"
};

/*
 * Message types
 */

char *msgnames[6] = {
  (char *)"", (char *)"Note: ", (char *)"Warning: ", (char *)"ERROR: ", (char *)"FATAL ERROR: ", (char *)"PANIC: "
};

/*
 * Retrieve one of the messages. We never return NULL
 * but instead escape to "oops".
 */

char *
uustring (int codeno)
{
  static char * faileddef = (char *)"oops";
  stringmap *ptr = messages;

  while (ptr->code) {
    if (ptr->code == codeno)
      return ptr->msg;
    ptr++;
  }

  UUMessage (uustring_id, __LINE__, UUMSG_ERROR,
         (char *)"Could not retrieve string no %d",
         codeno);

  return faileddef;
}
