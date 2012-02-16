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

#ifndef MAIN_H

#   define MAIN_H

#   include "xdo.h"
#   include "types.h"

    typedef struct {
        int _socket;
        xdo_t *_xdo;
        uint16 _port;
        uint32 _sleep;
        uint8 _test;
        uint8 _code[4];
        char *_display;
    } Options;

    extern uint8 parse_args(Options *,int argc,char **argv);
    extern void init(void);
    extern void run(void);
    extern void test(void);
    extern void ansleep(uint32);
    extern void buffer_listener(uint8 *data,uint32 len);
    extern void hexdump(uint8 *data,uint32 len);
    extern void no_dispatch(uint8 *data,uint32 len);
    extern void dispatch_whosthere(uint8 *data,uint32 len);
    extern void dispatch_ignore(uint8 *data,uint32 len){(void)data;(void)len;}
    extern void dispatch_mouse(uint8 *data,uint32 len);
    extern void dispatch_move(uint8 *data,uint32 len);
    extern void dispatch_click(uint8 *data,uint32 len,uint8 rclick);
    extern void dispatch_key(uint8 *data,uint32 len);
    extern void dispatch_key_sym(uint8 *data,uint32 len);
    extern void dispatch_ping(uint8 *data,uint32 len);
    extern void dispatch_mdown(uint8 *data,uint32 len,uint8 rclick);
    extern void dispatch_mup(uint8 *data,uint32 len,uint8 rclick);

#endif

