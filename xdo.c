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

#include "xdo.h"

#if defined(__CYGWIN__)||defined(__WIN32__)

#if defined(__CYGWIN__)
#   include <sys/cygwin.h>
#   include <winable.h>
#else
#   include <windows.h>

#if (__BORLANDC__ < 0x0550)
// fail on bc5
#define INPUT_KEYBOARD    1
#define INPUT_MOUSE       2
typedef struct {
    int type;
    union {
        struct Key { int wVk; int dwFlags; } ki;
        struct Mouse { int wVk; int dwFlags; } mi;
    };
} INPUT;
void SendInput(int i,INPUT *in,int l){(void)i;(void)in;(void)l;}
#endif
#endif


#include <stdio.h>

#include "keys.c"

void key(unsigned char);
void alt(unsigned char);

xdo_t xdo_new(void *xdo)
{
    (void)xdo;
#if defined(__CYGWIN__)
    cygwin_internal(CW_SYNC_WINENV);
#endif
    return((xdo_t)1);
}

void xdo_free(xdo_t xdo)
{
    (void)xdo;
}

void xdo_keysequence(xdo_t xdo,XDOWindow window,char *text,int delay)
{
    (void)xdo;
    (void)window;
    (void)delay;
    if(text)
    {
        if(!strcmpi(text,"Escape"))
        {
            key(VK_ESCAPE);
        }
        else if(!strcmpi(text,"Alt_L+Tab"))
        {
            alt(VK_TAB);
        }
        else if(!strcmpi(text,"Alt_L+F4"))
        {
            alt(VK_F4);
        }
        else
        {
            printf("[Error] Unknown key (%s)\n",text);
            fflush(0);
        }
    }
}

void send_key(unsigned char k,unsigned char up)
{
    INPUT i;

    memset(&i,0,sizeof(i));
    i.type=INPUT_KEYBOARD;
    i.ki.wVk=k;
    if(up)
    {
        i.ki.dwFlags=KEYEVENTF_KEYUP;
    }
    SendInput(1,&i,sizeof(i));
}

void key(unsigned char k)
{
    send_key(k,0);
    send_key(k,1);
}

void alt(unsigned char k)
{
    send_key(VK_MENU,0);
    key(k);
    send_key(VK_MENU,1);
}

void xdo_mousedown(xdo_t xdo,XDOWindow w,int button)
{
    POINT point;

    (void)xdo;
    (void)w;
    if(GetCursorPos(&point))
    {
        INPUT i;

        memset(&i,0,sizeof(i));
        i.type=INPUT_MOUSE;
        i.mi.dwFlags=button==1?MOUSEEVENTF_LEFTDOWN:MOUSEEVENTF_RIGHTDOWN;
        SendInput(1,&i,sizeof(i));
    }
}

void xdo_mouselocation(xdo_t xdo,int *x,int *y,int *screen)
{
    POINT point;

    (void)xdo;
    (void)screen;
    if(GetCursorPos(&point))
    {
        *x=(int)point.x;
        *y=(int)point.y;
    }
}

void xdo_mousemove(xdo_t xdo,int x,int y,int screen)
{
    (void)xdo;
    (void)screen;
    SetCursorPos(x,y);
}

void xdo_mouseup(xdo_t xdo,XDOWindow window,int button)
{
    POINT point;

    (void)xdo;
    (void)window;
    if(GetCursorPos(&point))
    {
        mouse_event(button==1?MOUSEEVENTF_LEFTUP:MOUSEEVENTF_RIGHTUP,
                    point.x,
                    point.y,
                    0,
                    0);
    }
}

void xdo_window_get_active(xdo_t xdo,XDOWindow *win)
{
    (void)xdo;
    *win=GetActiveWindow();
}

void xdo_desktop_size(double *w,double *h)
{
    *w=GetSystemMetrics(SM_CXSCREEN);
    *h=GetSystemMetrics(SM_CYSCREEN);
}

#endif

