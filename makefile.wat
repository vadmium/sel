# Makefile for "sel" program, for Watcom
# By Martin Panter

CC = wcc386
CFLAGS = -bc -bt=NT -wx -6r -fp6 -fpi87 -oikl+nr -q
LINK = wlink
LINKFLAGS =

EXENAME = sel
#~ LIBS =
OBJS = main.obj

!ifdef __LOADDLL__
!  loaddll wpp386 wppd386.dll
!  loaddll wlink wlink.dll
!endif
 
all: .SYMBOLIC $(EXENAME).exe
exec: .SYMBOLIC $(EXENAME).exe
	$(EXENAME).exe

$(EXENAME).exe: $(OBJS)
	$(LINK) SYS NT F {$(OBJS)} $(LINKFLAGS) N $@
# L {$(LIBS)}

CC_CMD = $(CC) $[@ $(CFLAGS) -fo=$@
main.obj: main.c
	$(CC_CMD)

clean: .PROCEDURE
	FOR %o IN ($(OBJS)) DO @del %o
	del $(EXENAME).exe
