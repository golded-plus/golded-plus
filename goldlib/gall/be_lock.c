/*

BeLock.c - implementation of Fido-style message bases lock library.

Copyright 2001-2003 Siarzhuk Zharski, imker@gmx.li
                                      All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
4. Neither the names of the authors nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/
#include <stdio.h>
#include <errno.h>

#include "be_lock.h"

static const char rcsid[]="$Id$";

static status_t look_for_port(int file_handle, bool b_create, port_id *port_id_ptr)
{
  status_t status = B_OK;
  struct stat st = {0};
  char name[B_OS_NAME_LENGTH] = {0};
  
  if(fstat(file_handle, &st))
    return B_IO_ERROR;

  sprintf(name, "FidoLock:%08X:%08X", st.st_dev, st.st_ino);
  
  *port_id_ptr = find_port(name);
  if(*port_id_ptr == B_NAME_NOT_FOUND)
  {
    if(b_create)
    {
      *port_id_ptr = create_port(1, name);
      if(*port_id_ptr == B_BAD_VALUE || *port_id_ptr == B_NO_MORE_PORTS)
        status = *port_id_ptr;
    }
    else
      status = B_NAME_NOT_FOUND;
  }
  else // already have such lock ...
    status = B_BUSY;
  return status; 
}
 
status_t beos_lock(int file_handle)
{
  port_id pid = 0;
  return look_for_port(file_handle, true, &pid);
}

status_t beos_unlock(int file_handle)
{
  port_id pid = 0;
  status_t status = look_for_port(file_handle, false, &pid);
  if(status == B_BUSY)
  	status = delete_port(pid);
  return status;
}

#define IMPLEMENT_BEOS_FUNC(_FUNC_NAME, _FUNC)\
status_t _FUNC_NAME(FILE *handle)             \
{                                             \
  int file_handle = fileno(handle);           \
  if(file_handle == -1)                       \
    return B_IO_ERROR;                        \
  return _FUNC(file_handle);                  \
}                                             \

IMPLEMENT_BEOS_FUNC(beos_flock, beos_lock)
IMPLEMENT_BEOS_FUNC(beos_funlock, beos_unlock)

#define IMPLEMENT_BE_FUNC(_FUNC_NAME, _HANDLE_TYPE, _FUNC)\
int _FUNC_NAME(_HANDLE_TYPE handle)\
{                                           \
  int ret_value = 0;                        \
  status_t status = _FUNC(handle);          \
  if(status != B_OK)                        \
  {                                         \
    errno = status;                         \
    ret_value = -1;                         \
  }                                         \
  return ret_value;                         \
}                                           \
 
IMPLEMENT_BE_FUNC(be_lock, int, beos_lock)
IMPLEMENT_BE_FUNC(be_unlock, int, beos_unlock)
IMPLEMENT_BE_FUNC(be_flock, FILE *, beos_flock)
IMPLEMENT_BE_FUNC(be_funlock, FILE *, beos_funlock)

