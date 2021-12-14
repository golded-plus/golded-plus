/*

BeLock.h - interface to Fido-style message bases lock BeOS library.

This code is written by Siarzhuk Zharski <imker@gmx.li> for free and
can be used for any purposes in any form without any warranties.

*/

/*
 COMMENT: Many of Fidonet message bases uses file locks for theirs work in
 concurrent environment. The Be Operating System has not such possibility.
 Existing BeOS inplementations of locking allows to block only "nodes" from
 any access. This blocking cannot prevent from some "entry" modification
 attempts - for example, you can delete such file without problems or move
 it to somewhere. The node is still locked but it is not that we wait for.

 After long discussion on beos developer mail-lists I have not found usable
 solution for this problem. That's why I decide to implement some alternative
 locking technology. The key idea of my locking is using some BeOS-specific
 kernel objects. At this time I found only single one to be accessible from
 many processes - the BeOS kernel ports. See BeBook->Kernel Kit->Ports for
 details. We can create port with some name and we can look for port with
 such name in the system. Than, the locking algorithm is below:

 1) Get the device  and node IDs for file handle we going to lock:

 fstat(file_handle, &stat_info);

 2) Create the name for the port to be used as locking object:

 sprintf(name, "FidoLock:%08X:%08X", stat_info.st_dev, stat_info.st_ino);

 3) Look for existing port with this name:

 pid = find_port(name);
 if(pid == B_NAME_NOT_FOUND)
 {

   4) Create such port.

    pid = create_port(1, name);

 }
 else
 {
    5) If such port exists - the file is locked - we must not touch it now -
       say calling program about failed lock.
 }

 To unlock locked file - just delete the port

 delete_port(pid);

 This technique is also not perfect, but, in my opinion it is more safe than
 node locking and more safe that work without any locking at al.
*/

#ifndef __BeLock_H_
#define __BeLock_H_

#include <OS.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* be[os]_[f][un]lock() - acquires/releases file lock for file handle or stream
     parameters:
       file_handle - hanlde to file you want to [un]lock
       file - opened stream for file you want to [un]lock
     returns:
       be_* - set of functions:
         0  - on success
         -1 - on fails. See errno for error code and B_* - codes below.
       beos_* - set of functions:
         B_OK
         [0x00000000] - on success

         other codes on error:

         B_IO_ERROR (EIO)
         [0x80000001] - input/output error.
                        For example fstat or fileno on file handles failed.
         B_NAME_NOT_FOUND
         [0x80000007] - port_name doesn't name an existing port.
                        Try to unlock non-existing filelock.
         B_BAD_VALUE (EINVAL)
         [0x80000005] - port's queue_length is too big or less than zero.
                        Error creating new port.
         B_NO_MORE_PORTS
         [0x80001201] - the system couldn't allocate another port.
                        Error creating new port.
         B_BUSY (EBUSY)
         [0x8000000E] - file is already locked - port exists.

         B_BAD_PORT_ID
         [0x80001200] - port doesn't identify an open port.

$Id$

*/

int be_lock(int file_handle);
int be_unlock(int file_handle);
int be_flock(FILE *file);
int be_funlock(FILE *file);

status_t beos_lock(int file_handle);
status_t beos_unlock(int file_handle);
status_t beos_flock(FILE *file);
status_t beos_funlock(FILE *file);

#ifdef __cplusplus
} //extern "C"
#endif //__cplusplus

#endif // __BeLock_H_
