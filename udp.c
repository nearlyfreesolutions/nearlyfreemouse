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

#include "udp.h"
#include "buffer.h"

#include <string.h>

#   if defined(linux) || defined(__CYGWIN__)
#       include <unistd.h>
#       include <netdb.h>
#       include <sys/socket.h>
#       include <sys/time.h>
#       include <netinet/in.h>
#	    include <arpa/inet.h>
#	    include <netinet/tcp.h>
#	    include <ctype.h>
#       define UDPError            -1
#       define UDPInvalidSocket    -1
#       define UDPioctl            ioctl
#       define UDPClose            tnclose
        typedef int UDPSocket;

#   elif defined(__WIN32__)

#       include <winsock.h>
#       define UDPError             SOCKET_ERROR
#       define UDPInvalidSocket     INVALID_SOCKET
#       define UDPioctl             ioctlsocket
#       define UDPClose             closesocket

#   endif

static struct sockaddr_storage _sender_address;
socklen_t _sender_len = 0;
void os_init(void);
void udp_init(void);

int udp_connect(uint16 port)
{
    return(udp_bind(port,1));
}

int udp_bind(uint16 port,uint8 bnd)
{
    int fd;
    struct sockaddr_in server;

    udp_init();
    if((fd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))!=UDPError)
    {
        if(bnd)
        {
            memset(&server,0,sizeof(server));
            server.sin_family=AF_INET;
            server.sin_port=htons(port);
            server.sin_addr.s_addr=htonl(INADDR_ANY);
            if(!bind(fd,(struct sockaddr *)&server,sizeof(server)))
            {
            }
            else
            {
                udp_closesocket(fd);
                fd=UDPInvalidSocket;
            }
        }
        if(fd!=UDPInvalidSocket)
        {
            int on=1;
            if(setsockopt(fd,
                          SOL_SOCKET,
                          SO_BROADCAST,
                          (void *)&on,
                          sizeof(on))!=UDPError)
            {
            }
            else
            {
                udp_closesocket(fd);
                fd=UDPInvalidSocket;
            }
        }
    }
    return(fd);
}

void udp_closesocket(int socket)
{
#if defined(__linux)||defined(__CYGWIN__)
    close(socket);
#else
    closesocket(socket);
#endif
}

int udp_read(int socket)
{
    struct timeval to={0,0};
    fd_set fds;
    int i=-1;

    if(socket!=-1)
    {
        // check for available data
        FD_ZERO(&fds);
        FD_SET(socket,&fds);
        i=select(socket+1,&fds,NULL,NULL,&to);
        switch(i)
        {
            case -1: // error
            {
                break;
            }
            case 0: // no data
            {
                i=0;
                break;
            }
            default:  // data available, so read it
            {
                uint8 buffer[256];
		        _sender_len=sizeof(_sender_address);
                switch(i=recvfrom(socket,
                                  (void *)buffer,
                                  sizeof(buffer),
                                  0,
                                  (struct sockaddr *)&_sender_address,
                                  &_sender_len))
                {
                    case -1:  // error
                    case 0:   // error
                    {
                        i=-1;
                        break;
                    }
                    default: // wooo! data!
                    {
                        // write it to the buffer
                        buffer_write(buffer,i);
                        // process the messages
                        while(buffer_check());
                        break;
                    }
                }
                break;
            }
        }
    }
    return(i);
}

int udp_broadcast(int socket,uint16 port,const void *data,uint32 len)
{
    int sent=-1;

    if(data&&len)
    {
        struct sockaddr_in server;

        memset(&server,0,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_port=htons(port);
        server.sin_addr.s_addr=htonl(INADDR_BROADCAST);
        //memset(&server.sin_zero,0,sizeof(server.sin_zero));
        sent=sendto(socket,
                    data,
                    len,
                    0,
                    (struct sockaddr *)&server,
                    sizeof(server));
    }
    return(sent);
}

void udp_get_sender(struct sockaddr_storage *sender_address,
                    socklen_t *sender_len)
{
    if(sender_address&&sender_len)
    {
        memcpy(sender_address,&_sender_address,sizeof(sender_address));
        *sender_len=_sender_len;
    }
}

void udp_init(void)
{
    static int _init = 0;

    if(!_init)
    {
        os_init();
    }
}

#if defined(linux) || defined(__CYGWIN__)

void os_init()
{
}

#else

void os_init()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 1, 1 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        MessageBox(NULL,"Cannot Setup Winsock.dll!","Error.",MB_OK);
        PostQuitMessage(0);
        return;
    }

    if ( LOBYTE( wsaData.wVersion ) != 1 ||
          HIBYTE( wsaData.wVersion ) != 1 ) {
        WSACleanup();
        MessageBox(NULL,"Cannot Setup Winsock.dll!","Error.",MB_OK);
        PostQuitMessage(0);
        return;
    }
}

#endif

