//  This may look like C code, but it is really -*- C++ -*-
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
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
//  Text -> Paragraph conversion.
//  ------------------------------------------------------------------

#include <gstrall.h>
#include <gtxtpara.h>
//  ------------------------------------------------------------------
int GParagraph::ConvertText(char * _tptr, uint _tlen)
{
    // Text position counter
    uint _tpos = 0;

    // Main conversion loop
    while(_tpos < _tlen)
    {
        // Skip SOFTCR, NUL and LF characters at the start of a paragraph
        while(_tpos < _tlen)
        {
            switch(*_tptr)
            {
                case SOFTCR:

                    if(softcr != LF)
                    {
                        break;            // Allow soft-cr to be a real char
                    }

                case NUL:
                case LF:
                    _tptr++;
                    _tpos++;
                    continue;
            }
            break;
        }

        // Break out of loop at end of text
        if(_tpos >= _tlen)
        {
            break;
        }

        // Paragraph start pointer and position
        char * _pptr = _tptr;
        uint _ppos   = _tpos;

        // Find next CR or end of text
        while((*_tptr != CR) and (_tpos < _tlen))
        {
            _tptr++, _tpos++;
        }
        // NUL-terminate paragraph (overwriting the CR if any)
        *_tptr = NUL;
        // Add paragraph
        AddParagraph(_pptr, _tpos - _ppos);
    }
    return lines;
} // GParagraph::ConvertText

//  ------------------------------------------------------------------
uint GParagraph::CheckCtrlLines()
{
    // Non-ctrl text size including a terminating CR
    uint _length = 0;

    if(paraidx)
    {
        // Flags
        int _got_tearline = false;
        int _got_origin   = false;
        // Point to one item past the last item
        GParaData * _pdptr = &paraidx[lines];
        int _line          = lines;

        // Check paragraphs in bottom-up order
        do
        {
            // Previous paragraph
            _line--;
            _pdptr--;
            // Shortcut alias
            char * _ptr = _pdptr->text;

            // Switch the CHR4 value
            switch(*(dword *)_ptr)
            {
                // AREA: echo identifier
                case CHR4_AREA:

                    // Mark all occurrences. Check in a separate loop afterwards
                    if(strneql(_ptr, "AREA:", 5))
                    {
                        _pdptr->control = CTRL_AREA;
                    }

                    break;

                // --- Tearline
                case CHR4_TEARLINENUL:
                case CHR4_TEARLINESPACE:

                    // Only the last tearline is valid
                    if(not _got_tearline)
                    {
                        if(strneql(_ptr, "---\0", 4) or strneql(_ptr, "--- ", 4))
                        {
                            _pdptr->control = CTRL_TEARLINE, _got_tearline = true;
                        }
                    }

                    break;

                // * Origin:
                case CHR4_ORIGIN:

                    // Only the last origin is valid and only if before any tearline
                    if(not _got_tearline and not _got_origin)
                    {
                        if(strneql(_ptr, " * Origin: ", 11))
                        {
                            _pdptr->control = CTRL_ORIGIN, _got_origin = true;
                        }
                    }

                    break;

                // SEEN-BY: (non-kludge version)
                case CHR4_SEENBY:

                    // Non-kludge version is only valid after tearline/origin
                    if(not _got_tearline and not _got_origin)
                    {
                        if(strneql(_ptr, "SEEN-BY: ", 9))
                        {
                            _pdptr->control = CTRL_SEENBY;
                        }
                    }

                    break;

                // Standard known kludges
                case CHR4_INTL:

                    if(strneql(_ptr, "\001INTL ", 6))
                    {
                        _pdptr->control = CTRL_INTL;
                    }

                    break;

                case CHR4_FMPT:

                    if(strneql(_ptr, "\001FMPT ", 6))
                    {
                        _pdptr->control = CTRL_FMPT;
                    }

                    break;

                case CHR4_TOPT:

                    if(strneql(_ptr, "\001TOPT ", 6))
                    {
                        _pdptr->control = CTRL_TOPT;
                    }

                    break;

                case CHR4_MSGID:

                    if(strneql(_ptr, "\001MSGID: ", 8))
                    {
                        _pdptr->control = CTRL_MSGID;
                    }

                    break;

                case CHR4_REPLY:

                    if(strneql(_ptr, "\001REPLY: ", 8))
                    {
                        _pdptr->control = CTRL_REPLY;
                    }

                    break;

                case CHR4_PID:

                    if(strneql(_ptr, "\001PID: ", 6))
                    {
                        _pdptr->control = CTRL_PID;
                    }

                    break;

                case CHR4_SEENBY1:

                    if(strneql(_ptr, "\001SEEN-BY: ", 10))
                    {
                        _pdptr->control = CTRL_SEENBY1;
                    }

                    break;

                case CHR4_PATH:

                    if(strneql(_ptr, "\001PATH: ", 7))
                    {
                        _pdptr->control = CTRL_PATH;
                    }

                    break;

                case CHR4_CHARSET:

                    if(strneql(_ptr, "\001CHARSET: ", 10))
                    {
                        _pdptr->control = CTRL_CHARSET;
                    }

                    break;

                case CHR4_CHRS:

                    if(strneql(_ptr, "\001CHRS: ", 7))
                    {
                        _pdptr->control = CTRL_CHRS;
                    }

                    break;

                case CHR4_TZUTC:

                    if(strneql(_ptr, "\001TZUTC: ", 8))
                    {
                        _pdptr->control = CTRL_TZUTC;
                    }

                    break;

                case CHR4_VIA:

                    if(strneql(_ptr, "\001Via ", 5))
                    {
                        _pdptr->control = CTRL_VIA;
                    }

                    break;

                case CHR4_FLAGS:

                    if(strneql(_ptr, "\001FLAGS ", 7))
                    {
                        _pdptr->control = CTRL_FLAGS;
                    }

                    break;
            } // switch

            // Mark unknown kludges
            if(not _pdptr->control and (*_ptr == CTRL_A))
            {
                _pdptr->control = CTRL_UNKNOWNKLUDGE;
            }

            // Calculate non-ctrl text size
            if(_pdptr->control < CTRL_KLUDGE)
            {
                _length += _pdptr->length + 1;
            }
        }
        while(_line);
        // Flags
        int _got_area    = false;
        int _got_nonctrl = false;

        // Loop to check AREA: occurrences
        while(_line < lines)
        {
            // Is this a control line?
            if(_pdptr->control)
            {
                // Eliminate AREA: duplicates and any occurrence after first non-ctrl line
                if(_pdptr->control == CTRL_AREA)
                {
                    if(_got_area or _got_nonctrl)
                    {
                        _pdptr->control = 0;
                    }

                    _got_area = true;
                }
            }
            else
            {
                // Flag first occurrence of a non-ctrl line
                _got_nonctrl = true;
            }

            // Next paragraph
            _pdptr++;
            _line++;
        }
    }

    // Return total non-ctrl text size
    return _length;
} // GParagraph::CheckCtrlLines

//  ------------------------------------------------------------------
