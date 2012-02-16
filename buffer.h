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

#ifndef BUFFER_H

#   define BUFFER_H

#   include "types.h"

    typedef void (*buffer_callback_fn)(uint8 *,uint32);

    uint32 buffer_write(uint8 *data,uint32 len);
    uint32 buffer_check(void);
    uint32 buffer_unstuff(uint8 *buffer,uint32 len);
    void buffer_dispatch(uint8 *data,uint32 len);
    void buffer_test(void);
    void buffer_callback(buffer_callback_fn);

#endif

