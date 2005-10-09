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

#define _MAX_VNAME_LEN  13
#define _MAX_MNAME_LEN  30


//  ------------------------------------------------------------------

struct gcpu_info
{
  char v_name[_MAX_VNAME_LEN+1];  // vendor name
  char m_name[_MAX_MNAME_LEN+1];  // model name
  int  family;
  int  model;
  int  stepping;
};


//  ------------------------------------------------------------------

static bool HaveCPUID()
{
//  TO_PORT_TAG: CPUID
#if defined(_MSC_VER)
  __try
  {
    __asm
    {
      xor eax, eax
      cpuid
    }
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
    return false;
  }

  return true;
#else
  return false;
#endif
}


//  ------------------------------------------------------------------

static void cpuname(int family, int model, const char *v_name, char *m_name)
{
  if (!strcmp("AuthenticAMD", v_name))
  {
    switch (family)
    {
    case 4:
      strcpy(m_name, "AMD_486");
      break;

    case 5:
      switch (model)
      {
      case 0:
      case 1:
      case 2:
      case 3:
        strcpy(m_name, "AMD_K5");
        break;
      case 6:
      case 7:
        strcpy(m_name, "AMD_K6");
        break;
      case 8:
        strcpy(m_name, "AMD_K6-2");
        break;
      case 9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
        strcpy(m_name, "AMD_K6-3");
        break;
      default:
        sprintf(m_name, "AMD_F%dM%d", family, model);
      }
      break;

    case 6:
      strcpy(m_name, "AMD_K7");
      break;

    case 15:
      strcpy(m_name, "AMD_K8");
      break;

    default:
      sprintf(m_name, "AMD_F%dM%d", family, model);
    }
  }
  else if (!strcmp("GenuineIntel", v_name))
  {
    switch (family)
    {
    case 4:
      switch (model)
      {
      case 0:
      case 1:
        strcpy(m_name, "i486DX");
        break;
      case 2:
        strcpy(m_name, "i486SX");
        break;
      case 3:
        strcpy(m_name, "i486DX2");
        break;
      case 4:
        strcpy(m_name, "i486SL");
        break;
      case 5:
        strcpy(m_name, "i486SX2");
        break;
      case 7:
        strcpy(m_name, "i486DX2E");
        break;
      case 8:
        strcpy(m_name, "i486DX4");
        break;
      default:
        sprintf(m_name, "iF%dM%d", family, model);
      }
      break;

    case 5:
      switch (model)
      {
      case 1:
      case 2:
      case 3:
        strcpy(m_name, "iPentium");
        break;
      case 4:
        strcpy(m_name, "iP-MMX");
        break;
      default:
        sprintf(m_name, "iF%dM%d", family, model);
      }
      break;

    case 6:
      switch (model)
      {
      case 1:
        strcpy(m_name, "iP-Pro");
        break;
      case 3:
      case 5:
        strcpy(m_name, "iP-II");
        break;
      case 6:
        strcpy(m_name, "iCeleron");
        break;
      case 7:
      case 8:
      case 11:
        strcpy(m_name, "iP-III");
        break;
      default:
        sprintf(m_name, "iF%dM%d", family, model);
      }
      break;

      case 15:
        strcpy(m_name, "iP-4");
        break;

      default:
        sprintf(m_name, "iF%dM%d", family, model);
    }
  }
  else if (!strcmp("CyrixInstead", v_name))
    sprintf(m_name, "CyrF%dM%d", family, model);
  else if (!strcmp("CentaurHauls", v_name))
    sprintf(m_name, "CenF%dM%d", family, model);
  else
    sprintf(m_name, "%3sF%dM%d", v_name, family, model);
}


//  ------------------------------------------------------------------

void gcpuid(gcpu_info *pinfo)
{
  union
  {
    char buff[_MAX_VNAME_LEN];
    struct
    {
      DWORD dw0;
      DWORD dw1;
      DWORD dw2;
    } dw;
  } vendor;

  DWORD standard = 0;
  vendor.buff[_MAX_VNAME_LEN-1] = 0;

//  TO_PORT_TAG: CPUID
#if defined(_MSC_VER)
  __asm
  {
    // get the vendor string
    xor eax, eax
    cpuid
    mov vendor.dw.dw0, ebx
    mov vendor.dw.dw1, edx
    mov vendor.dw.dw2, ecx

    // get the standard bits
    mov eax, 1
    cpuid
    mov standard, eax
  }
#else
  strcpy(vendor.buff, "UNKNOUN_CPU!");
#endif

  pinfo->family = (standard >> 8) & 0xF;  // retriving family
  pinfo->model = (standard >> 4) & 0xF;   // retriving model
  pinfo->stepping = standard & 0xF;       // retriving stepping

  strncpy(pinfo->v_name, vendor.buff, _MAX_VNAME_LEN);
  cpuname(pinfo->family, pinfo->model, pinfo->v_name, pinfo->m_name);
}


//  ------------------------------------------------------------------

char* ggetosstring(void) {

  static char osstring[256] = "";

  if(*osstring == NUL) {

    #if defined(__UNIX__) || defined(__DJGPP__) || defined(__EMX__)

    struct utsname info;

    if(uname(&info) != -1) {
      #if defined(__EMX__)
      sprintf(osstring, "%s %s.%s %s", info.sysname, info.version, info.release, info.machine);
      #elif defined(__DJGPP__)
      sprintf(osstring, "%s %s.%s %s", info.sysname, info.release, info.version, info.machine);
      #elif defined(__BEOS__)
      BAppFileInfo appFileInfo;
      version_info sys_ver = {0};
      BFile file("/boot/beos/system/lib/libbe.so", B_READ_ONLY);
      appFileInfo.SetTo(&file);
      appFileInfo.GetVersionInfo(&sys_ver, B_APP_VERSION_KIND);
      sprintf(osstring, "%s %s %s", info.sysname, sys_ver.short_info, info.machine);
      #else
      sprintf(osstring, "%s %s %s", info.sysname, info.release, info.machine);
      #endif
    }
    else
      strcpy(osstring, "unknown");

    #elif defined (__WIN32__)

    OSVERSIONINFO info;
    SYSTEM_INFO si;
    char ostype[16];
    char processor[_MAX_MNAME_LEN];

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
            if (HaveCPUID())
            {
              gcpu_info pinfo;
              gcpuid(&pinfo);
              strcpy(processor, pinfo.m_name);
            }
            else
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
                case PROCESSOR_INTEL_PENTIUM:
                  cpu = 5;
                case 15:   /* Pentium 4 */
                  cpu = 7;
                default:
                  cpu = 6;
                  break;
                }
              }
              switch(cpu) {
              case 15:
                sprintf(processor, "i786");
                break;
              default:
                if( cpu>9 ) cpu= cpu%10+int(cpu/10)+1;
                sprintf(processor, "i%d86", cpu);
              }
            }
          }
          break;
        case PROCESSOR_ARCHITECTURE_IA64:
          sprintf(processor, "IA64-%d", si.wProcessorLevel);
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
