# rcsid: $Id: Makefile,v 1.3 1994/01/26 03:17:29 rick Exp $

PROGRAM = rh

CFLAGS=-g
CC=gcc

# Possible C preprocessor flags:
#	-DBSD			    BSD directory handling
#	-DSUNOS_4		    compiling on SunOS 4.1.x
#	-DPOSIX_DIRECTORY_LIBRARY   POSIX directory handling
#
# If neither -DBSD or -DPOSIX_DIRECTORY_LIBRARY is specified then System V
# directory handling will be used.
#

CPPFLAGS= -DBSD -DSUNOS_4 $(DEBUG_CPPFLAGS)
CPP=gcc -E

YACC=byacc

LEX = flex
LFLAGS = $(DEBUG_LFLAGS) -s

DEPENDS = $(PROGRAM).depends

SRCS =			\
	alloc.c		\
	errmsg.c	\
	getdate.c	\
	resname.c	\
	rh.c		\
	rhcmds.c	\
	rhdata.c	\
	rhdir.c		\
	rhgram.c	\
	rhlex.c		\
	strtol.c	\
	strtoul.c	\
	sysinfo.c	\
	wildmat.c	\
	$(DEBUG_SRCS)

# strtol.o and strtoul.o can be removed from the object files list
# if they work correctly on your system
# NOTE: strtol() is broken under SunOS. It doesn't always set the
# 'end' pointer. Also, strtoul() does not exist under SunOS 4.1.x.

OBJS =			\
	alloc.o		\
	errmsg.o	\
	getdate.o	\
	resname.o	\
	rh.o		\
	rhcmds.o	\
	rhdata.o	\
	rhdir.o		\
	rhgram.o	\
	rhlex.o		\
	strtol.o	\
	strtoul.o	\
	sysinfo.o	\
	wildmat.o	\
	$(DEBUG_OBJS)

all: debug

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM)

insight:
	$(MAKE) DEBUG_YFLAGS=-t DEBUG_LFLAGS=-d		\
		DEBUG_SRCS=dump.c DEBUG_OBJS=dump.o	\
		DEBUG_CPPFLAGS="-DDEBUG"		\
		CC=insight $(PROGRAM)

purify: $(OBJS)
	purify -logfile=purify.out		\
	       -inuse-at-exit			\
	       -chain-length=200		\
	       -free-queue-length=1000		\
	    $(CC) $(OBJS) -o $(PROGRAM).p

tc: $(OBJS)
	proof $(CC) $(OBJS) -o $(PROGRAM).tc

debug:
	$(MAKE) DEBUG_YFLAGS=-t DEBUG_LFLAGS=-d		\
		DEBUG_SRCS=dump.c DEBUG_OBJS=dump.o	\
		DEBUG_CPPFLAGS="-DDEBUG" $(PROGRAM)

debug_p:
	$(MAKE) DEBUG_YFLAGS=-t DEBUG_LFLAGS=-d		\
		DEBUG_SRCS=dump.c DEBUG_OBJS=dump.o	\
		DEBUG_CPPFLAGS="-DDEBUG" purify

rhgram.c rhgram.h: rhgram.y
	$(YACC) $(DEBUG_YFLAGS) -d -p rh_ rhgram.y
	mv y.tab.c rhgram.c
	mv y.tab.h rhgram.h
	# $(YACC) $(DEBUG_YFLAGS) -d -B -p rh_ rhgram.y

getdate.c: getdate.y
	$(YACC) $(DEBUG_YFLAGS) -p gd_ getdate.y
	mv y.tab.c getdate.c
	# $(YACC) $(DEBUG_YFLAGS) B -p gd_ getdate.y

cc_src: $(SRCS)
	#load $(CPPFLAGS) $(SRCS)
	#link

clean:
	$(RM) *.o core *~ *.output
	$(RM) purify.out .purifyversion lex.backtrack *.pure_* .pure
	$(RM) *.bak *.BAK rhgram.tab.[ch] getdate.tab.[ch]
	$(RM) -r TC.Cache rh.tc*
	$(RM) tca.map tca.log *.out *.ins.c

clobber: clean
	$(RM) rhgram.[ch] rhlex.c getdate.c
	$(RM) $(PROGRAM) $(PROGRAM).p $(PROGRAM).tc

depend: $(SRCS) dump.c
	./makedep -r -e /usr/include/ -e /usr/local/gnu/	\
		-m $(DEPENDS) -o '$(CPPFLAGS)' $(SRCS) dump.c

include $(DEPENDS)
