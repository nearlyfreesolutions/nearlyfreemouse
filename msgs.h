/*
This is the Nearly Free Mouse server.
Copyright (C) 2012 nearlyfreesolutions@gmail.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
Boston, MA  02110-1301, USA.
*/

#ifndef MSGS_H

#   define MSGS_H

#   include "types.h"

#   define MSG_SERVER           0xf1

#   define MSG_MOUSE            0xf2
#       define MSG_MOUSE_MOVE       0x00
#       define MSG_MOUSE_CLICK      0x01
#       define MSG_MOUSE_RCLICK     0x02
#       define MSG_MOUSE_LDOWN      0x03
#       define MSG_MOUSE_LUP        0x04

#   define MSG_KEY              0xf3
#       define MSG_KEY_SYM          0x00

#   define MSG_PING             0xf4

#   define MSG_WHOSTHERE        0xff

#   ifdef __BORLANDC__
#       pragma pack(push)
#       pragma pack(1)
#   endif

    typedef struct {
        uint8 _type;
        uint8 _code[4];
    } PACKSTRUCT MsgHdr;

    typedef struct {
        uint8 _term;
    } PACKSTRUCT MsgTerm;

    typedef struct {
        MsgHdr _hdr;
        MsgTerm _term;
    } PACKSTRUCT MsgServer;

    typedef struct {
        MsgHdr _hdr;
        MsgTerm _term;
    } PACKSTRUCT MsgPing;

    typedef struct {
        MsgHdr _hdr;
        uint8 _type;
        uint16 _x;
        uint16 _width;
        uint16 _y;
        uint16 _height;
        MsgTerm _term;
    } PACKSTRUCT MsgMouseMove;

    typedef struct {
        MsgHdr _hdr;
        uint8 _type;
        uint16 _x;
        uint16 _width;
        uint16 _y;
        uint16 _height;
        MsgTerm _term;
    } PACKSTRUCT MsgMouseClick;

    typedef struct {
        MsgHdr _hdr;
        uint8 _type;
        uint16 _x;
        uint16 _width;
        uint16 _y;
        uint16 _height;
        MsgTerm _term;
    } PACKSTRUCT MsgMouseDown;

    typedef struct {
        MsgHdr _hdr;
        uint8 _type;
        uint16 _x;
        uint16 _width;
        uint16 _y;
        uint16 _height;
        MsgTerm _term;
    } PACKSTRUCT MsgMouseUp;

    typedef struct {
        MsgHdr _hdr;
        uint8 _type;
        char  _sym[1];
    } PACKSTRUCT MsgKeySym;

#   ifdef __BORLANDC__
#       pragma pack(pop)
#   endif

#endif

