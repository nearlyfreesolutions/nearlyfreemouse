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
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "msgs.h"
#include "udp.h"
#include "buffer.h"

buffer_callback_fn _dispatch[0x10];
Options _options;
int _done = 0;

void go(int argc,char **argv);
int done(void);
void quit(void);

#if !defined(__WIN32__)

int main(int argc,char **argv)
{
    go(argc,argv);
    return(0);
}

void os_work(void)
{
}

int done(void)
{
    return(_done);
}

void quit(void)
{
    _done=1;
}

#else

// include the windows mess..
#include "windows.c"

#endif

void init(void)
{
    uint32 i;

    _options._socket=-1;
    _options._port=5840;
    _options._test=0;
    _options._sleep=5;
    _options._display=0;
    memcpy(_options._code,"NFS!",sizeof(_options._code));
    buffer_callback(buffer_listener);
    for(i=0;i<sizeof(_dispatch)/sizeof(*_dispatch);++i)
    {
        *(_dispatch+i)=no_dispatch;
    }
    *(_dispatch+MSG_SERVER-0xf0)=dispatch_ignore;       // ignore our response
    *(_dispatch+MSG_MOUSE-0xf0)=dispatch_mouse;
    *(_dispatch+MSG_KEY-0xf0)=dispatch_key;
    *(_dispatch+MSG_PING-0xf0)=dispatch_ping;
    *(_dispatch+MSG_WHOSTHERE-0xf0)=dispatch_whosthere;
}

void go(int argc,char **argv)
{
    init();
    if(parse_args(&_options,argc,argv))
    {
        if((_options._socket=udp_connect(_options._port))!=-1)
        {
            if(!_options._test)
            {
                run();
            }
            else
            {
                test();
            }
            udp_closesocket(_options._socket);
        }
        else
        {
            printf("Unable to connect to port %u\n",(unsigned)_options._port);
        }
    }
    else
    {
        printf("Usage: nearlyfreemouse [-p port] [-c code] [-d display]\n");
    }
}

void run(void)
{
    int n;

    _options._xdo=xdo_new(_options._display);
    if(_options._xdo)
    {
        while(!done())
        {
            os_work();
            if((n=udp_read(_options._socket))<0)
            {
                quit();
            }
            if(!n)
            {
                ansleep(_options._sleep);
            }
        }
        xdo_free(_options._xdo);
    }
}

void test(void)
{
    uint8 done=0;
    uint32 i,n=5;
    uint8 who=0xff;
    MsgMouseMove move;

    //buffer_test();
    for(i=0;i<sizeof(_dispatch)/sizeof(*_dispatch);++i)
    {
        *(_dispatch+i)=hexdump;
    }

    // who's there?
    udp_broadcast(_options._socket,_options._port,&who,1);

    memset(&move,0,sizeof(move));
    move._hdr._type=MSG_MOUSE;
    move._type=MSG_MOUSE_MOVE;
    memcpy(move._hdr._code,_options._code,sizeof(move._hdr._code));
    move._term._term=0xff;
    for(i=0;i<n;++i)
    {
        ansleep(25);
        move._x=move._y=htons(((uint16)i));
        move._width=move._height=htons(((uint16)n));
        udp_broadcast(_options._socket,_options._port,&move,sizeof(move));
    }

    while(!done)
    {
        done=(uint8)(udp_read(_options._socket)<0);
        ansleep(25);
    }
}

void getdim(double *w,double *h,int screen)
{
#if defined(linux)
    *w=DisplayWidth(_options._xdo->xdpy,screen);
    *h=DisplayHeight(_options._xdo->xdpy,screen);
#else
    (void)screen;
    xdo_desktop_size(w,h);
#endif
}

uint8 parse_args(Options *options,int argc,char **argv)
{
    uint8 ok=1;
    int c;

    while(ok&&((c=getopt(argc,argv,"tp:c:d:"))!=-1))
    {
        switch(c)
        {
            case 't':
            {
                options->_test=1;
                break;
            }
            case 'c':
            {
                if(optarg)
                {
                    memset(options->_code,0,sizeof(options->_code));
                    strncpy((char *)options->_code,
                            optarg,
                            sizeof(options->_code));
                }
                else
                {
                    printf("Invalid code\n");
                    ok=0;
                }
                break;
            }
            case 'p':
            {
                unsigned short u;
                if(optarg&&sscanf(optarg,"%hu",&u)==1)
                {
                    options->_port=u;
                }
                else
                {
                    printf("Invalid port %s\n",optarg);
                    ok=0;
                }
                break;
            }
            case 'd':
            {
                options->_display=optarg;
                break;
            }
            case '?':
            {
                ok=0;
                break;
            }
        }
    }
    return(ok);
}

void ansleep(uint32 l)
{
#if defined(__linux)||defined(__CYGWIN__)
    struct timespec spec,rem;

    spec.tv_sec=l/1000;
    spec.tv_nsec=(l-(spec.tv_sec*1000))*1000000;
    nanosleep(&spec,&rem);
#else
    Sleep(l);
#endif
}

void hexdump(uint8 *data,uint32 len)
{
    if(data&&len)
    {
        uint32 i;
        for(i=0;i<len;++i)
        {
            printf("0x%02x ",*(data+i));
        }
        printf("\n");
    }
}

void buffer_listener(uint8 *data,uint32 len)
{
    if(data&&len)
    {
        uint8 fn=*data;
        if(fn>=0xf0)
        {
            uint8 valid=(uint8)(fn==0xff);
            if(!valid)
            {
                // check the code..
                if(len>=sizeof(MsgHdr))
                {
                    MsgHdr *hdr=(MsgHdr *)data;
                    valid=(uint8)(!memcmp(hdr->_code,_options._code,sizeof(hdr->_code)));
                }
            }
            if(valid)
            {
                fn-=(uint8)0xf0;
                (*(_dispatch+fn))(data,len);
            }
            else
            {
                printf("Invalid msg 0x%02x/%u",(unsigned)*data,(unsigned)len);
            }
        }
        else
        {
            printf("Invalid message..\n");
        }
    }
}

void no_dispatch(uint8 *data,uint32 len)
{
    if(data&&len)
    {
        printf("No dispatch for fn 0x%02x\n",*data);
    }
}

void dispatch_whosthere(uint8 *data,uint32 len)
{
    static MsgServer weare;

    (void)data;
    (void)len;
    memset(&weare,0,sizeof(MsgServer));
    weare._hdr._type=MSG_SERVER;
    weare._term._term=0xff;
    memcpy(weare._hdr._code,_options._code,sizeof(weare._hdr._code));
    // who's there? We are!!
    udp_broadcast(_options._socket,_options._port,(void *)&weare,sizeof(weare));
}

void dispatch_ping(uint8 *data,uint32 len)
{
    static MsgPing ping;

    (void)data;
    (void)len;
    memset(&ping,0,sizeof(MsgPing));
    ping._hdr._type=MSG_PING;
    ping._term._term=0xff;
    memcpy(ping._hdr._code,_options._code,sizeof(ping._hdr._code));
    // ping.. this is a broadcast.. probably not the best idea..
    udp_broadcast(_options._socket,_options._port,(void *)&ping,sizeof(ping));
}

void dispatch_mouse(uint8 *data,uint32 len)
{
    if(data&&len)
    {
        switch(*(data+sizeof(MsgHdr)))
        {
            case MSG_MOUSE_MOVE:
            {
                if(len==sizeof(MsgMouseMove))
                {
                    dispatch_move(data,len);
                }
                break;
            }
            case MSG_MOUSE_CLICK:
            {
                if(len==sizeof(MsgMouseClick))
                {
                    dispatch_click(data,len,0);
                }
                break;
            }
            case MSG_MOUSE_RCLICK:
            {
                if(len==sizeof(MsgMouseClick))
                {
                    dispatch_click(data,len,1);
                }
                break;
            }
            case MSG_MOUSE_LDOWN:
            {
                if(len==sizeof(MsgMouseDown))
                {
                    dispatch_mdown(data,len,0);
                }
                break;
            }
            case MSG_MOUSE_LUP:
            {
                if(len==sizeof(MsgMouseUp))
                {
                    dispatch_mup(data,len,0);
                }
                break;
            }
            default:
            {
                // unknown
                printf("[Error] Unknown message.");
                hexdump(data,len);
                break;
            }
        }
    }
}

void dispatch_key(uint8 *data,uint32 len)
{
    if(data&&len)
    {
        switch(*(data+sizeof(MsgHdr)))
        {
            case MSG_KEY_SYM:
            {
                if(len>sizeof(MsgKeySym))
                {
                    dispatch_key_sym(data,len);
                }
                else printf("m len fail %d/%d\n",len,sizeof(MsgMouseMove));
                break;
            }
            default:
            {
                // unknown
                printf("[Error] Unknown message.");
                hexdump(data,len);
                break;
            }
        }
    }
}

void dispatch_move(uint8 *data,uint32 len)
{
    double w,h;

    if(data&&len==sizeof(MsgMouseMove))
    {
        MsgMouseMove *mouse=(MsgMouseMove *)data;

        int screen,cx,cy;
        // get current screen
        xdo_mouselocation(_options._xdo,&cx,&cy,&screen);
        // get w/h
        getdim(&w,&h,screen);
        // get message data
        mouse->_x=ntohs(mouse->_x);
        mouse->_width=ntohs(mouse->_width);
        mouse->_y=ntohs(mouse->_y);
        mouse->_height=ntohs(mouse->_height);

        // scale from android to screen
        cx=w*mouse->_x/mouse->_width;
        cy=h*mouse->_y/mouse->_height;

        // printf("Move: [%d,%d]  [%d,%d] of [%d,%d]\n",
        //        cx,cy,mouse->_x,mouse->_y,mouse->_width,mouse->_height);

        // move the mouse
        xdo_mousemove(_options._xdo,cx,cy,screen);
    }
}

void dispatch_click(uint8 *data,uint32 len,uint8 rclick)
{
    double w,h;

    if(data&&len==sizeof(MsgMouseClick))
    {
        MsgMouseClick *mouse=(MsgMouseClick *)data;

        int screen,cx,cy;
        // get current screen
        xdo_mouselocation(_options._xdo,&cx,&cy,&screen);
        // get w/h
        getdim(&w,&h,screen);
        // get message data
        mouse->_x=ntohs(mouse->_x);
        mouse->_width=ntohs(mouse->_width);
        mouse->_y=ntohs(mouse->_y);
        mouse->_height=ntohs(mouse->_height);

        // scale from android to screen
        cx=w*mouse->_x/mouse->_width;
        cy=h*mouse->_y/mouse->_height;
        // move the mouse
        xdo_mousemove(_options._xdo,cx,cy,screen);
        xdo_mousedown(_options._xdo,CURRENTWINDOW,rclick?3:1);
        xdo_mouseup(_options._xdo,CURRENTWINDOW,rclick?3:1);
    }
}

void dispatch_key_sym(uint8 *data,uint32 len)
{
    if(data&&len>sizeof(MsgKeySym))
    {
        MsgKeySym *keysym=(MsgKeySym *)data;

        // send sym
        xdo_keysequence(_options._xdo,CURRENTWINDOW,keysym->_sym,0);
        //printf("Key [%s]\n",keysym->_sym);
    }
}

void dispatch_mdown(uint8 *data,uint32 len,uint8 rclick)
{
    double w,h;

    if(data&&len==sizeof(MsgMouseClick))
    {
        MsgMouseDown *mouse=(MsgMouseDown *)data;

        int screen,cx,cy;
        // get current screen
        xdo_mouselocation(_options._xdo,&cx,&cy,&screen);
        // get w/h
        getdim(&w,&h,screen);
        // get message data
        mouse->_x=ntohs(mouse->_x);
        mouse->_width=ntohs(mouse->_width);
        mouse->_y=ntohs(mouse->_y);
        mouse->_height=ntohs(mouse->_height);

        // scale from android to screen
        cx=w*mouse->_x/mouse->_width;
        cy=h*mouse->_y/mouse->_height;
        // move the mouse
        xdo_mousemove(_options._xdo,cx,cy,screen);
        xdo_mousedown(_options._xdo,CURRENTWINDOW,rclick?3:1);
    }
}

void dispatch_mup(uint8 *data,uint32 len,uint8 rclick)
{
    if(data&&len==sizeof(MsgMouseClick))
    {
        xdo_mouseup(_options._xdo,CURRENTWINDOW,rclick?3:1);
    }
}

