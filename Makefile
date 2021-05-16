###########################################################################
EXE      = rh
#DEBUGFL = -DDEBUG -O0 -g
64BIT    = -D_FILE_OFFSET_BITS=64
FTIME    = -DHAVE_FTIME=1
CDEFS    = -DBSD -DSUNOS_4 $(FTIME) $(64BIT) $(DEBUGFL)
CC      ?= cc
RM      ?= rm -f
CFLAGS  += -O $(CDEFS)
###########################################################################
OBJS     = alloc.o errmsg.o getdate.o resname.o rh.o rhcmds.o rhdata.o \
           rhdir.o rhgram.o rhlex.o strtol.o strtoul.o sysinfo.o \
           wildmat.o dump.o ftime.o
###########################################################################
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)
###########################################################################
clean:
	$(RM) $(OBJS) $(EXE)
###########################################################################
