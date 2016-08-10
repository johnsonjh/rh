CDEFS=-DBSD -D_FILE_OFFSET_BITS=64 -DSUNOS_4 -DDEBUG
CC=gcc
CFLAGS=$(CDEFS) -O

OBJS=alloc.o errmsg.o getdate.o resname.o rh.o rhcmds.o rhdata.o \
    rhdir.o rhgram.o rhlex.o strtol.o strtoul.o sysinfo.o \
    wildmat.o dump.o
rh: $(OBJS)
	$(CC) $(CFLAGS) -o rh $(OBJS)
