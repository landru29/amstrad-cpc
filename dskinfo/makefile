# Makefile for addhead utility

CC = gcc
BIND = gcc
RM = rm

#   CFLAGS    flags for C compile
#   LFLAGS1   flags after output file spec, before obj file list
#   LFLAGS2   flags after obj file list (libraries, etc)

CFLAGS = -O2 -O3 -DUNIX
LFLAGS1 =
LFLAGS2 = -s 

DSKINFO_O=	dskinfo.o

DSKINFO: $(DSKINFO_O)
	$(BIND)  $(DSKINFO_O) -o dskinfo $(LFLAGS1) $(LFLAGS2) $(LIBS)

