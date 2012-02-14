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

#ifndef UDP_H

#   define UDP_H

#   include "types.h"
//#   if defined(linux) 
#       include <netdb.h>
/*
#   elif defined(__WIN32__)
#       include <winsock.h>
        typedef SOCKET UDPSocket;
        typedef int socklen_t;
#   endif
*/
    int udp_connect(uint16 port);
    int udp_bind(uint16 port,uint8 bnd);
    void udp_close(int socket);
    int udp_read(int socket);
    int udp_broadcast(int,uint16,const void *,uint32);
    void udp_get_sender(struct sockaddr_storage *sender_address,
                        socklen_t *sender_len);

#endif

