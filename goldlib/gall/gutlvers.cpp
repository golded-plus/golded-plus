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
    #if __VISUAL_C_NOT_LESS(14,0)
        //#if defined(_MSC_VER) && (_MSC_VER >= 1400)
        #include <intrin.h>
    #endif
#elif defined(__GNUC__)
    #include <sys/utsname.h>
#endif

#if defined(__BEOS__)
    #include <File.h>
    #include <AppFileInfo.h>
#endif


//  ------------------------------------------------------------------

#define _MAX_VNAME_LEN  12
#define _MAX_MNAME_LEN  30

#ifdef GCFG_NO_CPUID
# define gcpuid(pstr) (pstr)
# define HaveCPUID()  (0)
#else
//  ------------------------------------------------------------------

#if __VISUAL_C_LESS(14,0)
//#if defined(_MSC_VER) && (_MSC_VER < 1400)
static void __cpuid(int CPUInfo[4], int cpuidfun)
{
    __asm
    {
        mov eax, cpuidfun
        cpuid

        mov esi, CPUInfo
        mov [esi + 0], eax
        mov [esi + 4], ebx
        mov [esi + 8], ecx
        mov [esi + 12], edx
    }
}
#endif


//  ------------------------------------------------------------------

inline static bool HaveCPUID()
{
//  TO_PORT_TAG: CPUID
#if defined(_MSC_VER)
    __try
    {
        int CPUInfo[4];
        __cpuid(CPUInfo, 0);
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        return false;
    }

    return true;
#elif defined(__GNUC__)
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
            switch (model)
            {
            case 3:
            case 7:
                strcpy(m_name, "AMD486DX2");
                break;
            case 8:
            case 9:
                strcpy(m_name, "AMD486DX4");
                break;
            case 14:
            case 15:
                strcpy(m_name, "AMD5x86");
                break;
            default:
                sprintf(m_name, "AMD486_M%d", model);

            }
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
                strcpy(m_name, "AMD_K6-3");
                break;
            case 13:
            case 14:
            case 15:
                strcpy(m_name, "AMD_K6-3+");
                break;
            default:
                sprintf(m_name, "AMD_F%dM%d", family, model);
            }
            break;

        case 6:
            switch (model)
            {
            /* need full F/M/S/Rev for identification
                  case 1:
                  case 2:
                  case 4:
                  case 6:
                  case 8:
                    strcpy(m_name, "AMD_Athlon");   // (S:0) & (S:1; Rev:80)
                    strcpy(m_name, "AMD_Sempron");  // (S:1; Rev:B0)
                    break;
                  case 3:
                  case 7:
                    strcpy(m_name, "AMD_Duron");
                    break;
            */
            default:
                sprintf(m_name, "AMD_K7_M%u", model);
            }
            break;

        case 15:
            switch (model)
            {
            /* need full F/M/S/Rev for identification
                  case 12:
                    strcpy(m_name, "AMD_Sempron");
                    break;
            */
            default:
                sprintf(m_name, "AMD_K8_M%u", model);
            }
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
                strcpy(m_name, "i486SX2O");
                break;
            case 7:
                strcpy(m_name, "i486DX2E");
                break;
            case 8:
                strcpy(m_name, "i486DX4");
                break;
            default:
                sprintf(m_name, "i486_M%d", model);
            }
            break;

        case 5:
            switch (model)
            {
            case 1:
                strcpy(m_name, "iP");
                break;
            case 2:
                strcpy(m_name, "iP54C");
                break;
            case 3:
                strcpy(m_name, "iP_OverDrive");
                break;
            case 4:
                strcpy(m_name, "iP55C");
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
            case 13:
                strcpy(m_name, "iP-M");  // Pentium M "Centrino" (Pentium Mobile)
                break;
            default:
                sprintf(m_name, "iF%dM%d", family, model);
            }
            break;

        case 15:
            switch (model)
            {
            /*      case 2:  // 15-2-7, 15-4-1
                    strcpy(m_name, "iXeon");
                    break;
            */
            default:
                strcpy(m_name, "iP-IV");
                break;
            }
            break;

        default:
            sprintf(m_name, "iF%dM%d", family, model);
        }
    }
    else if (!strcmp("GenuineTMx86", v_name))
    {
        switch (family)
        {
        case 15:
            switch(model)
            {
            case 2:       // Transmeta Efficeon(tm) Processor TM8000
                strcpy(m_name, "TM8000");
                break;
            default:
                sprintf(m_name, "TM F%dM%d", family, model);
            }
            break;
        default:
            sprintf(m_name, "TM F%dM%d", family, model);
        }
    }
    else if (!strcmp("CyrixInstead", v_name))
        sprintf(m_name, "CyrF%dM%d", family, model);
    else if (!strcmp("CentaurHauls", v_name))
    {
        switch (family)
        {
        case 6:  //  VIA C3 Nehemiah = F6M9; VIA C3 Samuel 2 = F6M7
            strcpy(m_name, "VIA_C3");
            break;
        default:
            sprintf(m_name, "VIA F%dM%d", family, model);
        }
    }
    else
    {
        if (model)
        {
            sprintf(m_name, "CPU %3s-F%dM%d", v_name, family, model);
        }
        else
        {
            switch (family)
            {
            case 0:
                sprintf(m_name, "CPU %s", v_name);
                break;
            case 3:
            case 4:
                sprintf(m_name, "%s%s%u86", v_name, v_name[0]?"-":"", family);
                break;
            default:
                sprintf(m_name, "CPU %3s-F%dM%d", v_name, family, model);
            }
        }
    }
}


//  ------------------------------------------------------------------

char *gcpuid(char *_cpuname)
{
#if defined(__GNUC__) && defined(__i386__) || defined(_MSC_VER)
    static struct scpuid_t
    {
        dword         cpu;           /* x86, where x=cpu */
        dword         cpu_high;      /* highest CPUID capability */
#if defined(_MSC_VER)
        union
        {
            char vendor[_MAX_VNAME_LEN+1];
            struct
            {
                dword dw0;
                dword dw1;
                dword dw2;
            } dw;
        };
#else
        char          vendor[3*sizeof(dword)+1]; /* CPU vendor string 12 bytes, 13th byte is zero */
#endif
        uint8_t family;        /* CPU stepping number, 4 bits */
        uint8_t model;         /* CPU model number, 4 bits */
        uint8_t stepping;      /* CPU stepping value, 4 bits */
        //  unsigned cpu_id;        /* stepping ID, 12 bits: 0x0FMS */
        //  unsigned features;      /* CPU features info */
    } scpuid;  /* ISO C: static variabled is initialised with 0 */
#endif

#if defined(_MSC_VER)

    int CPUInfo[4];

    // get the vendor string
    __cpuid(CPUInfo, 0);
    scpuid.dw.dw0 = CPUInfo[1];
    scpuid.dw.dw1 = CPUInfo[3];
    scpuid.dw.dw2 = CPUInfo[2];

    // get the CPU family, model, stepping, features bits
    __cpuid(CPUInfo, 1);
    scpuid.stepping = CPUInfo[0] & 0x0F;
    scpuid.model = (CPUInfo[0] >> 4) & 0x0F;
    scpuid.family = (CPUInfo[0] >> 8) & 0x0F;

    cpuname(scpuid.family, scpuid.model, scpuid.vendor, _cpuname);

#elif defined(__GNUC__) && defined(__i386__)

    asm(  /* assembler code is based on code of FreeBSD kernel sources */
        /* uses AT&T assembler notation */

        /* Step 1. Try to toggle alignment check flag; does not exist on 386. */
        "pushfl\n\t"
        "popl %%eax\n\t"
        "movl %%eax,%%ecx\n\t"
        "orl  $0x00040000,%%eax\n\t"   /* sets a alignment check flag */
        "pushl    %%eax\n\t"
        "popfl\n\t"
        "pushfl\n\t"
        "popl %%eax\n\t"
        "xorl %%ecx,%%eax\n\t"
        "andl $0x00040000,%%eax\n\t"   /* test alignment check flag */
        "pushl    %%ecx\n\t"
        "popfl\n\t"

        "testl    %%eax,%%eax\n\t"         /* alignment check flag is set? */
        "jnz  try486\n\t"

        /* NexGen CPU does not have aligment check flag. */
        "pushfl\n\t"
        "movl $0x5555, %%eax\n\t"
        "xorl %%edx, %%edx\n\t"
        "movl $2, %%ecx\n\t"
        "clc\n\t"
        "divl %%ecx\n\t"
        "jz   nexgen\n\t"
        "popfl\n\t"
        "movl $3,%0\n\t"              /* CPU 386 */
        "jmp  end\n"

        "nexgen:"
        "popfl\n\t"
        "movl $5,%0\n\t"              /* CPU NX586 */
        "movl $0x4778654e,%1\n\t"     /* store vendor string */
        "movl $0x72446e65,%1+4\n\t"   /* "NexGenDriven"      */
        "movl $0x6e657669,%1+8\n\t"
//      "movl   $0,%1+12\n\t"           // vendor is zero-filled already
        "jmp  end\n"

        /* Step2. Try to toggle identification flag; does not exist on early 486s.*/
        "try486:"
        "pushfl\n\t"
        "popl %%eax\n\t"
        "movl %%eax,%%ecx\n\t"
        "xorl $0x00200000,%%eax\n\t" /* sets a identification bit */
        "pushl    %%eax\n\t"
        "popfl\n\t"
        "pushfl\n\t"
        "popl %%eax\n\t"
        "xorl %%ecx,%%eax\n\t"
        "andl $0x00200000,%%eax\n\t"  /* test identification bit */
        "pushl    %%ecx\n\t"
        "popfl\n\t"

        "testl    %%eax,%%eax\n\t"        /* if identification flag is set then cpuid CPU's command may be used */
        "jnz  trycpuid\n\t"
        "movl $4,%0\n\t"              /* CPU 486 */

        /*
         * Check Cyrix CPU
         * Cyrix CPUs do not change the undefined flags following
         * execution of the divide instruction which divides 5 by 2.
         *
         * Note: CPUID is enabled on M2, so it passes another way.
         */
        "pushfl\n\t"
        "movl $0x5555, %%eax\n\t"
        "xorl %%edx, %%edx\n\t"
        "movl $2, %%ecx\n\t"
        "clc\n\t"
        "divl %%ecx\n\t"
        "jnc  trycyrix\n\t"
        "popfl\n\t"
        "jmp  end\n"                  /* You may use Intel CPU */

        "trycyrix:"
        "popfl\n\t"
        /*
         * IBM Bluelighting CPU also doesn't change the undefined flags.
         * Because IBM doesn't disclose the information for Bluelighting
         * CPU, we couldn't distinguish it from Cyrix's (including IBM
         * brand of Cyrix CPUs).
         */
        "movl $0x69727943,%1\n\t"     /* store vendor string */
        "movl $0x736e4978,%1+4\n\t"   /* "CyrixInstead"      */
        "movl $0x64616574,%1+8\n\t"
        "jmp  end\n"

        /* Step 3. Use the `cpuid' instruction. */
        "trycpuid:"
        "xorl %%eax,%%eax\n\t"
        ".byte    0x0f,0xa2\n\t"      /* cpuid 0 */
        "movl %%eax,%2\n\t"       /* "cpuid 1" capability */
        "movl %%ebx,%1\n\t"       /* store vendor string */
        "movl %%edx,%1+4\n\t"
        "movl %%ecx,%1+8\n\t"
//      "movb   $0,%1+12\n\t"   // vendor is zero-filled already

        "andl     %%eax,%%eax\n\t"        /* "cpuid 1" is allowed? (eax==1?) */
        "jz       i586\n\t"               /* no, skip "cpuid 1"  */

        "movl $1,%%eax\n\t"
        ".byte    0x0f,0xa2\n\t"      // cpuid 1
//      "movl   %%eax,%6\n\t"       // store cpu_id
//      "movl   %%edx,%7\n\t"       // store cpu_feature

        "movb %%al,%%bl\n\t"
        "shrb $4,%%bl\n\t"        // extract CPU model
        "movb %%bl,%4\n\t"        // store model

        "andl $0x0F0F,%%eax\n\t"      // extract CPU family type and stepping
        "movb %%al,%5\n\t"        // store stepping
        "movb %%ah,%3\n\t"        // store family
        "cmpb $5,%%ah\n\t"
        "jae  i586\n\t"

        /* less than Pentium; must be 486 */
        "movl $4,%0\n\t"    /* CPU 486 */
        "jmp  end\n"
        "i586:\n\t"  /* Pentium and greater. Store family type into CPU type var */
        "movb %%ah,%0\n"
        "end:\n\t"
        "nop\n\t"

        : /* output */
        "=m" (scpuid.cpu)         /* %0 */
        ,"=m" (scpuid.vendor)      /* %1 */
        ,"=m" (scpuid.cpu_high)    /* %2 */
        ,"=m" (scpuid.family)      /* %3 */
        ,"=m" (scpuid.model)       /* %4 */
        ,"=m" (scpuid.stepping)    /* %5 */
//       ,"=m" (scpuid.cpu_id)      /* %6 */
//       ,"=m" (scpuid.features)    /* %7 */
        : /* no input */
        : /* modified registers */
        "eax", "ebx", "ecx", "edx", "esp"
    );

    cpuname(scpuid.family?scpuid.family:scpuid.cpu, scpuid.model, scpuid.vendor, _cpuname);

#else
#if defined(MSDOS) || defined(DOS) || defined(__MSDOS__) || defined(_DOS) \
   || defined(WIN32) || defined(__WIN32__) || defined(_WIN) || defined(WINNT) \
   || defined(__OS2__) || defined(OS2)
    cpuname(0, 0, "x86", _cpuname);
#else
    cpuname(0, 0, "UNKNOWN", _cpuname);
#endif
#endif

    return _cpuname;
}
#endif // ifdef GCFG_NO_CPUID

//  ------------------------------------------------------------------

char* ggetosstring(void)
{

    static char osstring[256] = "";

    if(*osstring == NUL)
    {

        char processor[_MAX_MNAME_LEN] = "";

#if defined(__UNIX__) || defined(__DJGPP__) || defined(__EMX__)

        struct utsname info;

        gcpuid(processor);

        if(uname(&info) != -1)
        {
            if(!processor[0])
                strcpy(processor,info.machine);

#if defined(__EMX__)
            sprintf(osstring, "%s %s.%s %s", info.sysname, info.version, info.release, processor);
#elif defined(__DJGPP__)
            sprintf(osstring, "%s %s.%s %s", info.sysname, info.release, info.version, processor);
#elif defined(__BEOS__)
            BAppFileInfo appFileInfo;
            version_info sys_ver = {0};
            BFile file("/boot/beos/system/lib/libbe.so", B_READ_ONLY);
            appFileInfo.SetTo(&file);
            appFileInfo.GetVersionInfo(&sys_ver, B_APP_VERSION_KIND);
            sprintf(osstring, "%s %s %s", info.sysname, sys_ver.short_info, processor);
#else
            sprintf(osstring, "%s %s %s", info.sysname, info.release, processor);
#endif
        }
        else
            strcpy(osstring, "unknown");

#elif defined (__WIN32__)

        OSVERSIONINFO info;
        SYSTEM_INFO si;
        char ostype[16];

        GetSystemInfo(&si);
        info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(GetVersionEx(&info))
        {
            switch(info.dwPlatformId)
            {
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
            switch(*((WORD*)&si))
            {
            case PROCESSOR_ARCHITECTURE_INTEL:
            {
                if (HaveCPUID())
                {
                    gcpuid(processor);
                }
                else
                {
                    int cpu;
                    if( (info.dwPlatformId == VER_PLATFORM_WIN32_NT &&
                            info.dwMajorVersion > 3)
#ifdef VER_PLATFORM_WIN32_CE
                            || info.dwPlatformId == VER_PLATFORM_WIN32_CE
#endif
                      )
                        cpu = si.wProcessorLevel;
                    else
                    {
                        switch(si.dwProcessorType)   /* Windows NT 3.5 and earlier */
                        {
                        case PROCESSOR_INTEL_386:
                            cpu = 3;
                            break;
                        case PROCESSOR_INTEL_486:
                            cpu = 4;
                            break;
                        case PROCESSOR_INTEL_PENTIUM:
                            cpu = 5;
                            break;
                        case 6:   /* Pentium Pro or Pentim II */
                            cpu = 6;
                        case 15:   /* Pentium 4 */
                            cpu = 8;
                        default:
                            cpu = 7;
                            break;
                        }
                    }
                    switch(cpu)
                    {
                    case 15:
                        sprintf(processor, "i886");
                        break;
                    default:
                        if( cpu>9 ) cpu= cpu%10+int(cpu/10)+2;
                        sprintf(processor, "i%d86", cpu);
                    }
                }
            }
            break;
            case PROCESSOR_ARCHITECTURE_IA64:
                sprintf(processor, "IA64-%d", si.wProcessorLevel);
                break;
#ifdef PROCESSOR_ARCHITECTURE_AMD64
            case PROCESSOR_ARCHITECTURE_AMD64:
                if (HaveCPUID())
                    gcpuid(processor);
                else
                    sprintf(processor, "AMD64-%d", si.wProcessorLevel);
                break;
#endif
            case PROCESSOR_ARCHITECTURE_MIPS:
                /* si.wProcessorLevel is of the form 00xx, where xx is an 8-bit
                   implementation number (bits 8-15 of the PRId register). */
                sprintf(processor, "MIPS R%u000", si.wProcessorLevel);
                break;
            case PROCESSOR_ARCHITECTURE_ALPHA:
                /* si.wProcessorLevel is of the form xxxx, where xxxx is a 16-bit
                   processor version number (the low-order 16 bits of a version
                   number from the firmware). */
                sprintf(processor, "Alpha%d", si.wProcessorLevel);
                break;
            case PROCESSOR_ARCHITECTURE_ALPHA64:
                sprintf(processor, "Alpha%d", si.wProcessorLevel);
                break;
            case PROCESSOR_ARCHITECTURE_PPC:
                /* si.wProcessorLevel is of the form xxxx, where xxxx is a 16-bit
                   processor version number (the high-order 16 bits of the Processor
                   Version Register). */
                switch(si.wProcessorLevel)
                {
                case 1:
                    strcpy(processor, "PPC601");
                    break;
                case 3:
                    strcpy(processor, "PPC603");
                    break;
                case 4:
                    strcpy(processor, "PPC604");
                    break;
                case 6:
                    strcpy(processor, "PPC603+");
                    break;
                case 9:
                    strcpy(processor, "PPC604+");
                    break;
                case 20:
                    strcpy(processor, "PPC620");
                    break;
                default:
                    sprintf(processor, "PPC l%u", si.wProcessorLevel);
                    break;
                }
                break;
#ifdef PROCESSOR_ARCHITECTURE_SHX
            case PROCESSOR_ARCHITECTURE_SHX:
                sprintf(processor, "SH-%d", si.wProcessorLevel);
                break;
#endif
#ifdef PROCESSOR_ARCHITECTURE_ARM
            case PROCESSOR_ARCHITECTURE_ARM:
                sprintf(processor, "ARM-%d", si.wProcessorLevel);
                break;
#endif
            default:
                strcpy(processor, "CPU-unknown");
                break;
            }
            if(info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
                info.dwBuildNumber = info.dwBuildNumber & 0x0000ffffl;
            if(info.dwPlatformId == VER_PLATFORM_WIN32_NT and *info.szCSDVersion != NUL)
            {
                char _tmp[128];
                strcpy(_tmp, info.szCSDVersion);
                strchg(_tmp, ' ', '_');
                strisrep(_tmp, "Service_Pack_", "SP");
                sprintf(osstring, "%s %u.%u.%u-%s %s", ostype, unsigned(info.dwMajorVersion), unsigned(info.dwMinorVersion), unsigned(info.dwBuildNumber), _tmp, processor);
            }
            else
                sprintf(osstring, "%s %u.%u.%u %s", ostype, unsigned(info.dwMajorVersion), unsigned(info.dwMinorVersion), unsigned(info.dwBuildNumber), processor);
        }
        else
            strcpy(osstring, "Win32-unknown");

#else

#if defined(__MSDOS__)
        const char* osname = "DOS";
#elif defined(__OS2__)
        const char* osname = "OS/2";
#endif

        sprintf(osstring, "%s %d.%02d %s", osname, _osmajor, _osminor, gcpuid(processor));

#endif
    }

    return osstring;
}


// -------------------------------------------------------------------
