#
##
#

ARCH = $(shell uname)

APPNAME = nearlyfreemouse

FILESC = \
         main.c \
         udp.c \
         buffer.c \
         xdo.c

CFLAGS=-Wall

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(@D)/$(@F)

OBJS = $(FILESC:.c=.o) 

all: $(APPNAME)

dummy:


ifeq ($(ARCH),Linux)
$(APPNAME): $(OBJS) 
	$(CXX) $(CFLAGS) -o $(APPNAME) $(OBJS) -lxdo -s

$(APPNAME).static: $(OBJS) 
	$(CXX) $(CFLAGS) -o $(APPNAME) $(OBJS) -s libxdo.a -static \
        -static -L/usr/lib/ -lX11 -lxcb -lXtst -lXinerama -lXext -lXau -lXdmcp -lpthread -ldl
else
$(APPNAME): $(OBJS) 
	$(CXX) $(CFLAGS) -o $(APPNAME).exe $(OBJS) -s
endif

tilde:
	find ./ -name \*~ | xargs rm -f

clean: tilde
	find ./ -name core | xargs rm -f
	find ./ -name \*.o | xargs rm -f
	find ./ -name \*.obj | xargs rm -f
	find ./ -name \*.a | xargs rm -f
	find ./ -name \*.exe | xargs rm -f
	rm -f $(APPNAME)

install: $(APPNAME)
	install -D $(APPNAME) $(DESTDIR)/usr/bin/$(APPNAME)
	install -D $(APPNAME).desktop $(DESTDIR)/usr/share/doc/$(APPNAME)/$(APPNAME).desktop
	install -D $(APPNAME).autostart $(DESTDIR)/usr/share/doc/$(APPNAME)/$(APPNAME).autostart

win32: dummy
	~/.wine/drive_c/Borland/BCC55/Bin/bcc32.exe -wall -I/home/`whoami`/.wine/drive_c/Borland/BCC55/Include/ -c $(FILESC)

