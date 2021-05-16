###########################################################################
EXE       =  rh
#DEBUGFL  =  -DDEBUG -O0 -g
64BIT     =  -D_FILE_OFFSET_BITS=64
FTIME     =  -DHAVE_FTIME=1
CDEFS     =  -DBSD -DSUNOS_4 $(FTIME) $(64BIT) $(DEBUGFL)
CFLAGS   +=  -O $(CDEFS)
CC       ?=  cc
RM       ?=  rm -f
STRIP    ?=  strip -s
###########################################################################
OBJS      = alloc.o errmsg.o getdate.o resname.o rh.o rhcmds.o rhdata.o \
            rhdir.o rhgram.o rhlex.o strtol.o strtoul.o sysinfo.o \
            wildmat.o dump.o ftime.o
###########################################################################
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)
###########################################################################
.PHONY: clean
clean:
	$(RM) $(OBJS) $(EXE)
###########################################################################
.PHONY: shrink
shrink: $(EXE)
	$(STRIP) $(EXE)
###########################################################################
.PHONY: strip
strip: shrink
###########################################################################
alloc.o: alloc.c
alloc.o: errmsg.h
###########################################################################
dump.o: dump.c
dump.o: getdate.h
dump.o: rh.h
dump.o: rhcmds.h
dump.o: rhdata.h
###########################################################################
errmsg.o: errmsg.c
###########################################################################
ftime.o: ftime.c
###########################################################################
getdate.o: getdate.c
getdate.o: getdate.h
###########################################################################
resname.o: alloc.h
resname.o: resname.c
###########################################################################
rh.o: alloc.h
rh.o: errmsg.h
rh.o: getdate.h
rh.o: rh.c
rh.o: rh.h
rh.o: rhcmds.h
rh.o: rhdata.h
rh.o: rhgram.h
###########################################################################
rhcmds.o: errmsg.h
rhcmds.o: getdate.h
rhcmds.o: rh.h
rhcmds.o: rhcmds.c
rhcmds.o: rhdata.h
rhcmds.o: sysinfo.h
###########################################################################
rhdata.o: errmsg.h
rhdata.o: getdate.h
rhdata.o: rh.h
rhdata.o: rhcmds.h
rhdata.o: rhdata.c
rhdata.o: rhgram.h
###########################################################################
rhdir.o: errmsg.h
rhdir.o: getdate.h
rhdir.o: rh.h
rhdir.o: rhdata.h
rhdir.o: rhdir.c
rhdir.o: sysinfo.h
###########################################################################
rhgram.o: alloc.h
rhgram.o: errmsg.h
rhgram.o: getdate.h
rhgram.o: rh.h
rhgram.o: rhcmds.h
rhgram.o: rhdata.h
rhgram.o: rhgram.c
###########################################################################
rh.h: getdate.h
###########################################################################
rhlex.o: alloc.h
rhlex.o: errmsg.h
rhlex.o: getdate.h
rhlex.o: rh.h
rhlex.o: rhdata.h
rhlex.o: rhgram.h
rhlex.o: rhlex.c
rhlex.o: sysinfo.h
###########################################################################
strtol.o: strtol.c
###########################################################################
strtoul.o: strtoul.c
###########################################################################
sysinfo.o: alloc.h
sysinfo.o: sysinfo.c
sysinfo.o: sysinfo.h
###########################################################################
wildmat.o: wildmat.c
###########################################################################
