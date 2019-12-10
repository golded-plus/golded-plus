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
//  Sound class.
//  ------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <gutlos.h>

#if defined (__MSDOS__)
#include <gmemi86.h>
#include <gmemdbg.h>
#endif

#include <gstrall.h>
#include <gasmamis.h>
#include <gsndall.h>
#include <gfilutil.h>
//  ------------------------------------------------------------------

#ifdef DEBUG
int debug = false;
#endif
//  ------------------------------------------------------------------
gsnd::gsnd()
{
  #if defined (__MSDOS__)
    mpx = -1;
  #if (defined (__WATCOMC__) && defined (__386__)) || defined (__DJGPP__)
    buffer = -1;
  #else
    buffer = NULL;
  #endif
    data = NULL;
  #endif

    api_open  = false;
    file_open = false;
}

//  ------------------------------------------------------------------
gsnd::~gsnd()
{
    if(api_open)
    {
        close_api();
    }

    free_buffer();
}

//  ------------------------------------------------------------------

#if defined (__MSDOS__)
int gsnd::call_api(uint al, uint bx)
{
    if(api_open)
    {
        i86 cpu;
        cpu.al(al);
        cpu.ah(mpx);
        cpu.bx(bx);
        cpu.genint(0x2D);
        return (int)cpu.ax();
    }

    return 0;
}

#endif
//  ------------------------------------------------------------------
void gsnd::free_buffer()
{
  #if defined (__MSDOS__)
  #if defined (__DJGPP__) || (defined (__WATCOMC__) && defined (__386__))

    if(buffer != -1)
    {
        __dpmi_free_dos_memory(buffer);
        buffer = -1;
    }

  #else

    if(buffer)
    {
        farfree(buffer);
        buffer = NULL;
    }

  #endif
  #endif
}

//  ------------------------------------------------------------------
int gsnd::open_api()
{
  #if defined (__MSDOS__)

    i86 cpu;

    // Installation check
    for(mpx = 0; mpx < 256; mpx++)
    {
        cpu.ah(mpx);
        cpu.al(GSAPI_INSTALL_CHECK);
        cpu.genint(0x2D);

        if(cpu.al() == 0xFF)
        {
      #ifdef DEBUG

            if(debug)
            {
                printf("gsnd: examining signature at %04X:%04X for mpx %u\n",
                       cpu.dx(),
                       cpu.di(),
                       mpx);
            }

      #endif
            amis_signature * signature = (amis_signature *)gmkfp(cpu.dx(), cpu.di());
      #ifdef DEBUG

            if(debug)
            {
                printf("gsnd: version is %04X\n", cpu.cx());
            }

            if(debug)
            {
                printf("gsnd: manufacturer is \"%8.8s\"\n", signature->manufacturer);
            }

            if(debug)
            {
                printf("gsnd: product name is \"%8.8s\"\n", signature->product_name);
            }

            if(debug)
            {
                printf("gsnd: description  is \"%s\"\n", signature->product_description);
            }

      #endif

            if((cpu.cx() >= 0x0100) and (cpu.cx() < 0x0200))
            {
                if(strnieql(signature->product_name, "GoldSAPI", 8))
                {
                    break;
                }
            }
        }
    }

    // Return if not installed
    if(mpx >= 256)
    {
        mpx = -1;
        return -1;
    }

    // Call the open api function
    cpu.al(GSAPI_OPEN_API);
    cpu.ah(mpx);
    cpu.genint(0x2D);

    if(cpu.al())
    {
    #ifdef DEBUG

        if(debug)
        {
            printf("gsnd: open api returned %02Xh\n", cpu.al());
        }

    #endif
        return cpu.al();
    }

  #ifdef DEBUG

    if(debug)
    {
        printf("gsnd: using data at %04X:%04X, length %u\n", cpu.dx(), cpu.di(),
               cpu.cx());
    }

  #endif
    data      = (gsapidata *)gmkfp(cpu.dx(), cpu.di());
    key_value = cpu.bx();
    api_open  = true;
    return 0;

  #elif defined (GUTLOS_FUNCS)

    api_open = true;
    return 0;

  #else // if defined (__MSDOS__)
    api_open = false;
    return -1;

  #endif // if defined (__MSDOS__)
} // gsnd::open_api

//  ------------------------------------------------------------------
int gsnd::close_api()
{
  #if defined (__MSDOS__)

    return call_api(GSAPI_CLOSE_API, key_value);

  #elif defined (GUTLOS_FUNCS)

    if(api_open)
    {
        if(file_open)
        {
            close();
        }

        api_open = false;
    }

    return 0;

  #else

    return -1;

  #endif
}

//  ------------------------------------------------------------------
int gsnd::open(const char * file)
{
  #if defined (__MSDOS__)

    strcpy(data->parameters, file);
try_again: int retval = call_api(GSAPI_OPEN_AND_LOAD_FILE);

    switch(retval)
    {
        case 0x01:
            close();
            goto try_again;

        case 0x02:
            free_buffer();
      #if defined (__DJGPP__) || (defined (__WATCOMC__) && defined (__386__))
            int seg =
                __dpmi_allocate_dos_memory((data->buffer_length >> 4) + 1, &buffer);

            if(seg == -1)
            {
                return -1;
            }

            data->buffer_segment = seg & 0xffff;
            data->buffer_offset  = 0;
      #else
            buffer = (char *)farmalloc(data->buffer_length);

            if(buffer == NULL)
            {
                return false;
            }

            data->buffer_segment = FP_SEG(buffer);
            data->buffer_offset  = FP_OFF(buffer);
      #endif
            retval = call_api(GSAPI_OPEN_AND_LOAD_FILE);
    } // switch

    if(retval == 0)
    {
        file_open = true;
    }

  #ifdef DEBUG

    if(debug)
    {
        printf("gsnd::open: retval = %04X\n", retval);
    }

  #endif

    return retval;

  #elif defined (GUTLOS_FUNCS)

    if(file_open)
    {
        close();
    }

    char buf[GMAXPATH + sizeof("open  alias noise wait")];
    sprintf(buf, "open %s alias noise wait", file);

    if(g_send_mci_string(buf, NULL))
    {
        file_open = true;
        return 0;
    }

    return -1;

  #else // if defined (__MSDOS__)
    NW(file);
    return -1;

  #endif // if defined (__MSDOS__)
} // gsnd::open

//  ------------------------------------------------------------------
int gsnd::close()
{
  #if defined (__MSDOS__)

    if(file_open)
    {
        file_open = false;
        return call_api(GSAPI_CLOSE_FILE);
    }

    return 1;

  #elif defined (GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("close noise wait", NULL);
        file_open = false;
        return 0;
    }

    return 0xFFFF;

  #else

    return 0xFFFF;

  #endif
} // gsnd::close

//  ------------------------------------------------------------------
int gsnd::play(uint sample_rate)
{
  #if defined (__MSDOS__)

    return file_open ? call_api(GSAPI_PLAY, sample_rate) : 1;

  #elif defined (GUTLOS_FUNCS)

    NW(sample_rate);

    if(file_open)
    {
        g_send_mci_string("seek noise to start", NULL);
        g_send_mci_string("play noise", NULL);
        return 0;
    }

    return 1;

  #else

    NW(sample_rate);
    return 1;

  #endif
}

//  ------------------------------------------------------------------
int gsnd::stop()
{
  #if defined (__MSDOS__)

    return file_open ? call_api(GSAPI_STOP) : 1;

  #elif defined (GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("stop noise wait", NULL);
        return 0;
    }

    return 1;

  #else

    return 1;

  #endif
}

//  ------------------------------------------------------------------
int gsnd::pause()
{
  #if defined (__MSDOS__)

    return file_open ? call_api(GSAPI_PAUSE) : 1;

  #elif defined (GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("pause noise", NULL);
        return 0;
    }

    return 1;

  #else

    return 1;

  #endif
}

//  ------------------------------------------------------------------
int gsnd::resume()
{
  #if defined (__MSDOS__)

    return file_open ? call_api(GSAPI_RESUME) : 1;

  #elif defined (GUTLOS_FUNCS)

    if(file_open)
    {
        g_send_mci_string("resume noise", NULL);
        return 0;
    }

    return 1;

  #else

    return 1;

  #endif
}

//  ------------------------------------------------------------------
int gsnd::break_loop(int method)
{
  #if defined (__MSDOS__)

    return file_open ? call_api(GSAPI_BREAK_LOOP, method) : 1;

  #elif defined (GUTLOS_FUNCS)

    NW(method);
    return 0xFFFF;

  #else

    NW(method);
    return 0xFFFF;

  #endif
}

//  ------------------------------------------------------------------
void gsnd::speaker(int onoff)
{
  #if defined (__MSDOS__)

    call_api(GSAPI_SPEAKER_ON_OFF, onoff ? 1 : 0);

  #elif defined (GUTLOS_FUNCS)
    // Not implemented
    NW(onoff);

  #else

    NW(onoff);

  #endif
}

//  ------------------------------------------------------------------
uint gsnd::get_sample_rate()
{
  #if defined (__MSDOS__)

    return file_open ? data->sample_rate : 0;

  #elif defined (GUTLOS_FUNCS)

    return 0;

  #else

    return 0;

  #endif
}

//  ------------------------------------------------------------------
int gsnd::is_playing()
{
  #if defined (__MSDOS__)

    if(file_open)
    {
        return make_bool_not(data->status == 0);
    }

    return false;

  #elif defined (GUTLOS_FUNCS)

    char return_buffer[BUFFERSIZE];

    if(file_open)
    {
        if(g_send_mci_string("status noise mode wait", return_buffer))
        {
            return strieql(return_buffer, "playing") or strieql(return_buffer,
                                                                "seeking");
        }
    }

    return 0;

  #else

    return 0;

  #endif
} // gsnd::is_playing

//  ------------------------------------------------------------------
