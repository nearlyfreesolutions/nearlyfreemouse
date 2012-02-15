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

#include "buffer.h"
#include <stdio.h>
#include <string.h>

#define _buffer_max 512
static uint8 _buffer[_buffer_max];
static uint32 _buffer_length = 0;

extern void hexdump(uint8 *,uint32);

buffer_callback_fn _buffer_callback = 0;

uint32 buffer_write(uint8 *data,uint32 len)
{
    if(data&&len)
    {
        // if the data fits in the remaining buffer
        if((len+_buffer_length)<=_buffer_max)
        {
            // save it...
            memcpy(_buffer+_buffer_length,data,len);
            _buffer_length+=len;
        }
        else
        {
            // failed, reset the buffer
            len=_buffer_length=0;
        }
    }
    else
    {
        len=0;
    }
    return(len);
}

uint32 buffer_check(void)
{
    uint8 msg[_buffer_max];
    uint32 n_msgs=0,pos=0,msg_len;
    uint16 state=0;

    while(pos<_buffer_length)
    {
        switch(state)
        {
            case 0: // scanning for 0xf?
            {
                for(;pos<_buffer_length&&*(_buffer+pos)<0xf1;++pos);
                if(pos<_buffer_length)
                {
                    // move the buffer contents to the start
                    memcpy(_buffer,_buffer+pos,_buffer_length-pos);
                    _buffer_length-=pos;
                    pos=0;
                    // next state..
                    ++state;
                }
                break;
            }
            case 1: // scan for 0xff
            {
                for(;pos<_buffer_length&&*(_buffer+pos)<0xff;++pos);
                if(pos<_buffer_length)
                {
                    ++state;
                }
                break;
            }
            case 2: // found a message
            {
                ++n_msgs;
                ++pos;
                memcpy(msg,_buffer,msg_len=pos);
                memcpy(_buffer,_buffer+pos,_buffer_length-pos);
                _buffer_length-=pos;
                state=0;
                msg_len=buffer_unstuff(msg,msg_len);
                if(msg_len)
                {
                    buffer_dispatch(msg,msg_len);
                }
                else
                {
                    printf("[Error] Stuffing error.\n");
                }
            }
        }
    }
    return(n_msgs);
}

uint32 buffer_unstuff(uint8 *data,uint32 len)
{
    if(data&&len)
    {
        uint32 i;
        for(i=0;i<len;++i)
        {
            if(*(data+i)==0xf0)
            {
                if(*(data+i+1)<0x10)
                {
                    *(data+i)|=*(data+i+1);
                    memcpy(data+i+1,data+i+2,len-(i+1));
                    --len;
                }
                else
                {
                    // failure
                    len=0;
                }
            }
        }
    }
    else
    {
        len=0;
    }
    return(len);
}
    
void buffer_callback(buffer_callback_fn cb)
{
    _buffer_callback=cb;
}

void buffer_dispatch(uint8 *data,uint32 len)
{
    if(_buffer_callback)
    {
        _buffer_callback(data,len);
    }
}

void buffer_test(void)
{
    uint8 d1[] = { 1,2,3,4,5,0xf0,5,6,0xff,0xf0,0xf1,0xf0,0x02,0xff,4,5};
    uint8 d2[] = { 1,2,3,4,5,0xf0,5,6,0xff,0xf0,0xf2,0x0,0x02,0xff,4,5};
    uint8 d3[] = { 1,2,3,4,5,0xf0,5,6,0xff,0xf3,0x0,0x02,0xf0,4,5};
    uint8 d4[] = { 1,2,3,4,5,0xf0,5,6,0xff,0xf4,0x0,0x02,0xff,4,5};

    buffer_write(d1,sizeof(d1)); while(buffer_check());
    buffer_write(d2,sizeof(d2)); while(buffer_check());
    buffer_write(d3,sizeof(d3)); while(buffer_check());
    buffer_write(d4,sizeof(d4)); while(buffer_check());
}


