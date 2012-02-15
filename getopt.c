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

#if defined(__WIN32__) && !defined(__CYGWIN__)

char *optarg = 0;
static int _argc = 1;

int mkargv(char *ptr,char ***argv)
{
    int i;
    int argc=0;
    char *p;
    char **a;

    *argv=0;
    if(ptr&&*ptr)
    {
        // we will loose this.. that's the price you pay for win32
        p=(char *)malloc(1+strlen(ptr));
        strcpy(p,ptr);
        ptr=p;
        for(argc=1,p=ptr;*p;++p)
        {
            if(*p==' ')
            {
                ++argc;
            }
        }
        a=(char **)(malloc(argc*sizeof(char *)));
        *argv=a;
        for(i=0;*ptr;++i)
        {
            *(a+i)=ptr;
            for(;*ptr&&*ptr!=' ';++ptr);
            if(*ptr)
            {
                *ptr++=0;
            }
        }
    }
    return(argc);
}

int is_opt(char c,char *opts)
{
    int result=0;

    while(*opts&&!result)
    {
        if(c==*opts++)
        {
            if(*opts==':')
            {
                result=2;
            }
            else
            {
                result=1;
            }
        }
    }
    return(result);
}

int getopt(int argc,char **argv,char *opts)
{
    int result=-1;

    optarg=0;
    if(_argc<argc)
    {
        if(**(argv+_argc)=='-')
        {
            switch(is_opt(*(*(argv+_argc)+1),opts))
            {
                case 1:
                {
                    result=*(*(argv+_argc)+1);
                    break;
                }
                case 2:
                {
                    if((_argc+1)<argc)
                    {
                        result=*(*(argv+_argc)+1);
                        ++_argc;
                        optarg=*(argv+_argc);
                    }
                    break;
                }
                default:break;
            }
        }
        ++_argc;
    }
    return(result);
}

#endif
