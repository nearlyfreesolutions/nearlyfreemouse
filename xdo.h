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

#ifndef NFM_XDO_H

#   define NFM_XDO_H

#   if defined(linux)

#       include <xdo.h>
        typedef Window XDOWindow;

#   elif defined(__CYGWIN__) || defined(__WIN32__)

#   ifdef __CYGWIN__
#       include <w32api/windows.h>
#       include <unistd.h>
#   else
#       include <windows.h>
        extern int getopt(int argc,char **argv,char *);
        extern char *optarg;
#   endif

#      define CURRENTWINDOW 0

        typedef void *xdo_t;
        typedef HWND XDOWindow;

        xdo_t xdo_new(void *);
        void xdo_free(xdo_t);
        void xdo_keysequence(xdo_t,XDOWindow,char *,int);
        void xdo_mousedown(xdo_t,XDOWindow,int);
        void xdo_mouselocation(xdo_t,int *,int *,int *);
        void xdo_mousemove(xdo_t,int,int,int);
        void xdo_mouseup(xdo_t,XDOWindow,int);
        void xdo_window_get_active(xdo_t,XDOWindow *);

        void xdo_desktop_size(double *w,double *h);

#   endif

#endif

