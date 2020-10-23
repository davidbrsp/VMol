LIBDIR = ../../lib
INCLUDE = ../../include

CC = gcc
CFLAGS = -g -I$(INCLUDE)

HEADERS = prim.h sphere.h ellipsoid.h
OBJECTS = prim.o sphere.o ellipsoid.o
LIBNAME = libprim.a
 

$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ $(LIBNAME)
