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

#ifndef TYPES_H

#   define TYPES_H

//#   ifdef __linux

    // system types
#   include <stdint.h>

    //
    // types
    //
    typedef int8_t int8;
    typedef uint8_t uint8;
    typedef int16_t int16;
    typedef uint16_t uint16;
    typedef int32_t int32;
    typedef uint32_t uint32;
    typedef int64_t int64;
    typedef uint64_t uint64;
    typedef void *tnpointer;

#   define PACKSTRUCT       __attribute__((packed))
/*
#   elif defined(__WIN32__)

    typedef char int8;
    typedef unsigned char uint8;
    typedef short int16;
    typedef unsigned short uint16;
    typedef long int32;
    typedef unsigned long uint32;
    typedef int32 int64;
    typedef int64 uint64;
#   define const    

#   define PACKSTRUCT

#   endif
*/

#endif

