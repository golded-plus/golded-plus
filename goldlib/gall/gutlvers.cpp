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
//  Get operating system version.
//  ------------------------------------------------------------------

#include <cstdio>
#include <gstrall.h>
#include <gutlmisc.h>

#if defined(__WIN32__)
#include <windows.h>
#elif defined(__GNUC__)
#include <sys/utsname.h>
#endif

#if defined(__BEOS__)
#include <File.h>
#include <AppFileInfo.h>
#endif

//  ------------------------------------------------------------------

char* ggetosstring(void) {

  static char osstring[256] = "";

  if(*osstring == NUL) {

    #if defined(__UNIX__) || defined(__DJGPP__) || defined(__EMX__)

    struct utsname info;

    if(uname(&info) != -1)
      #if defined(__EMX__)
      sprintf(osstring, "%s %s.%s %s", info.sysname, info.version, info.release, info.machine);
      #elif defined(__DJGPP__)
      sprintf(osstring, "%s %s.%s %s", info.sysname, info.release, info.version, info.machine);
      #elif defined(__BEOS__)
      {
      	BAppFileInfo appFileInfo;
		version_info sys_ver = {0};
		BFile file("/boot/beos/system/lib/libbe.so", B_READ_ONLY); 
		appFileInfo.SetTo(&file);
		appFileInfo.GetVersionInfo(&sys_ver, B_APP_VERSION_KIND);
        sprintf(osstring, "%s %s %s", info.sysname, sys_ver.short_info, info.machine);
	  }
	  #else
      sprintf(osstring, "%s %s %s", info.sysname, info.release, info.machine);
      #endif
    else
      strcpy(osstring, "unknown");

    #elif defined (__WIN32__)

    OSVERSIONINFO info;
    SYSTEM_INFO si;
    char ostype[16];
    char processor[16];

    GetSystemInfo(&si);
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if(GetVersionEx(&info)) {
      switch(info.dwPlatformId) {
        case VER_PLATFORM_WIN32_NT:
          strcpy(ostype, "WinNT");
          break;
        case VER_PLATFORM_WIN32_WINDOWS:
          strcpy(ostype, "Win9x");
          break;
        default:
          strcpy(ostype, "Win32s");
          break;
      }
      switch(*((WORD*)&si)) {
        case PROCESSOR_ARCHITECTURE_INTEL:
          {
            int cpu;

            if(info.dwPlatformId == VER_PLATFORM_WIN32_NT)
              cpu = si.wProcessorLevel;
            else {
              switch(si.dwProcessorType) {
                case PROCESSOR_INTEL_386:
                  cpu = 3;
                  break;
                case PROCESSOR_INTEL_486:
                  cpu = 4;
                  break;
                default:
                  cpu = 5;
                  break;
              }
            }
            sprintf(processor, "i%d86", cpu);
          }
          break;
        case PROCESSOR_ARCHITECTURE_MIPS:
          sprintf(processor, "mips%d000", si.wProcessorLevel);
          break;
        case PROCESSOR_ARCHITECTURE_ALPHA:
          sprintf(processor, "alpha%d", si.wProcessorLevel);
          break;
        case PROCESSOR_ARCHITECTURE_PPC:
          switch(si.wProcessorLevel) {
            case 1:
              strcpy(processor, "ppc601");
              break;
            case 3:
              strcpy(processor, "ppc603");
              break;
            case 4:
              strcpy(processor, "ppc604");
              break;
            case 6:
              strcpy(processor, "ppc603+");
              break;
            case 9:
              strcpy(processor, "ppc604+");
              break;
            case 20:
              strcpy(processor, "ppc620");
              break;
            default:
              strcpy(processor, "ppcXXX");
              break;
          }
          break;
        default:
          strcpy(processor, "unknown");
          break;
      }
      if(info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
        info.dwBuildNumber = info.dwBuildNumber & 0x0000ffffl;
      if(info.dwPlatformId == VER_PLATFORM_WIN32_NT and *info.szCSDVersion != NUL) {
        char _tmp[128];
        strcpy(_tmp, info.szCSDVersion);
        strchg(_tmp, ' ', '_');
        sprintf(osstring, "%s %ld.%ld.%ld-%s %s", ostype, info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber, _tmp, processor);
      }
      else
        sprintf(osstring, "%s %ld.%ld.%ld %s", ostype,info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber, processor);
    }
    else
      strcpy(osstring, "unknown");

    #else

    #if defined(__MSDOS__)
    const char* osname = "DOS";
    #elif defined(__OS2__)
    const char* osname = "OS/2";
    #endif

    sprintf(osstring, "%s %d.%02d ix86", osname, _osmajor, _osminor);

    #endif
  }

  return osstring;
}


// -------------------------------------------------------------------
