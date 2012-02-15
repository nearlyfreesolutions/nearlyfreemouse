set PATH=c:\Borland\BCC55\Bin;%PATH%
bcc32 -W -Ic:\Borland\BCC55\Include -Lc:\Borland\BCC55\Lib main.c udp.c buffer.c xdo.c
move main.exe nearlyfreemouse.exe
