# Project: vmol
# Makefile created by David

LIBDIR = lib
INCLUDE = include
BINDIR=bin
CODE=code
CC = gcc
CFLAGS = -g -I$(INCLUDE) -lm -mwindows -mconsole
MACHO = -Wall -pedantic -ansi
SOURCES = $(CODE)/vmol.c $(CODE)/tk.c $(CODE)/imagem.c $(CODE)/camera.c\
           $(CODE)/prim.c $(CODE)/ellipsoid.c $(CODE)/sphere.c
BIN = vmol.exe
RM = rm -f
LIBS = $(LIBDIR)/libmap.a $(LIBDIR)/libgp.a\
        $(LIBDIR)/librad.a $(LIBDIR)/libray.a \
        $(LIBDIR)/libvisib.a $(LIBDIR)/libhier.a \
        $(LIBDIR)/libscene.a $(LIBDIR)/libshade.a \
        $(LIBDIR)/libraster.a $(LIBDIR)/libclip.a \
        $(LIBDIR)/libview.a $(LIBDIR)/libgutil.a \
        $(LIBDIR)/libcsg.a \
        $(LIBDIR)/libpoly.a $(LIBDIR)/liblang.a \
        $(LIBDIR)/libimg.a $(LIBDIR)/libcolor.a \
        $(LIBDIR)/libgeom.a $(LIBDIR)/libaux.a
#        $(LIBDIR)/libprim.a 

.PHONY: all all-before all-after clean clean-custom

all: all-before vmol.exe all-after

$(BIN): $(SOURCES)
	$(CC) $(SOURCES) -o $(BINDIR)/$(BIN) $(LIBS) $(CFLAGS)
# Seja macho: comente a linha de cima e descomente essa
#	$(CC) $(SOURCES) -o $(BINDIR)/$(BIN) $(LIBS) $(CFLAGS) $(MACHO)

clean:
	${RM} $(BINDIR)/$(BIN)
