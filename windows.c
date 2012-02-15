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

// dirty gnu getopt clone
#include "getopt.c"

#ifndef HWND_MESSAGE
#   define HWND_MESSAGE     0
#endif

void os_work(void);
void quit(void);
int done(void);
void w32init(HINSTANCE hInstance);
void w32destroy(void);

int CALLBACK WinMain
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    char **argv;
    int argc=mkargv(lpCmdLine,&argv);

    (void)hInstance;
    (void)hPrevInstance;
    (void)nCmdShow;
    w32init(hInstance);
    go(argc,argv);
    w32destroy();
    return(0);
}


typedef struct {
    int _done;
    HWND _window;
    NOTIFYICONDATA _nd;
    HICON _icon;
} Static;

Static _static;

int done(void)
{
    return(_static._done);
}

void quit(void)
{
    _static._done=1;
}

void os_work(void)
{
    MSG msg;

    if(PeekMessage(&msg,0,0,0,PM_REMOVE))
    {
        if(msg.message==WM_QUIT)
        {
            quit();
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

LRESULT CALLBACK w32wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_APP:
            if(lParam==WM_RBUTTONUP)
            {
                quit();
            }
            break;
    }
    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}


void w32init(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    BYTE *icon=(BYTE *)malloc(64*64);

    memset(&wndclass,0,sizeof(wndclass));
    memset(&_static._nd,0,sizeof(_static._nd));
    wndclass.lpfnWndProc=(WNDPROC)w32wndproc;
    wndclass.hInstance=hInstance;
    wndclass.lpszClassName="nearlyfreemouse";
    RegisterClass(&wndclass);

    _static._window=CreateWindow("nearlyfreemouse",
                                 "Nearly Free Mouse",
                                 WS_BORDER,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 HWND_MESSAGE,
                                 0,
                                 hInstance,
                                 0);

    _static._icon=CreateIcon(hInstance,64,64,1,8,icon,icon);
    free(icon);

    _static._nd.cbSize=sizeof(_static._nd);
    _static._nd.hWnd=_static._window;
    _static._nd.uID=1;
    _static._nd.hIcon=_static._icon;
    _static._nd.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
    _static._nd.uCallbackMessage=WM_APP;
    strcpy(_static._nd.szTip,"Nearly Free Mouse: Right click to exit.");
    Shell_NotifyIcon(NIM_ADD,&_static._nd);
}

void w32destroy(void)
{
    // clean up
    Shell_NotifyIcon(NIM_DELETE,&_static._nd);
    DestroyWindow(_static._window);
    DestroyIcon(_static._icon);
}

