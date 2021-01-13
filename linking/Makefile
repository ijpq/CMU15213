CC = gcc
CFLAGS = -Wall -Og

PROGS = global \
	mismatch \
	prog\
	prog2c\
	prog2l\
	prog2r\
	static-local.o \
	symbols \
	puzzles

# Do nothing
all: $(PROGS)

static-local.o: static-local.c
	$(CC) $(CFLAGS) -c static-local.c

# example program 1 
prog: main.o sum.o
	$(CC) $(CFLAGS) -o prog main.o sum.o
	$(CC) $(CFLAGS) -S main.c
	objdump -dx main.o          > main-relo.d
	objdump -dx -j .data main.o > maindata-relo.d
	readelf -s main.o	    > mainsym.d	
	objdump -dx sum.o           > sum-relo.d
	objdump -dx -j .data sum.o  > sumdata-relo.d
	objdump -dx prog            > prog-exe.d
	objdump -dx -j .data prog   > progdata-exe.d

# Type mismatch example
mismatch: mismatch-main.c mismatch-variable.c
	$(CC) $(CFLAGS) -o mismatch mismatch-main.c mismatch-variable.c

# Use of extern specifier
global: global-c1.c global-c2.c global.h
	$(CC) $(CFLAGS) -o global global-c1.c global-c2.c

# Static library linked at compile time
prog2c: main2.o addvec.o multvec.o
	$(CC) $(CFLAGS) -c main2.c addvec.c multvec.c
	ar rcs libvector.a addvec.o multvec.o
	$(CC) $(CFLAGS) -static -o prog2c main2.o -L. -lvector
#       Alternative for above line
#	$(CC) $(CFLAGS) -static -o prog2c main2.o libvector.a
	objdump -d prog2c > prog2c-exe.d


# Shared library linked at load-time
prog2l: main2.o libvector.so
	$(CC) $(CFLAGS) -o prog2l main2.o ./libvector.so
	objdump -dx main2.o          > main2-relo.d
	objdump -dx prog2l > prog2l-exe.d
	objdump -xs -j .data -j .got prog2l > prog2ldata-exe.d

# Shared library linked at run time
prog2r: dll.c
	$(CC) $(CFLAGS) -rdynamic -o prog2r dll.c -ldl

libvector.so: addvec.c multvec.c	
	$(CC) $(CFLAGS) -shared -fpic -o libvector.so addvec.c multvec.c
	objdump -xd libvector.so > libvector-relo.d
	objdump -xRr -j .data -j .got -j .got.plt -j rela.dyn -j rela.plt libvector.so > libvectordata-relo.d



symbols.o: symbols.c
	$(CC) $(CFLAGS) -c symbols.c

symbols: symbols.o
	$(CC) $(CFLAGS) -o symbols symbols.o

puzzles: puzzles-main.c puzzles-1.c puzzles-2.c
	$(CC) $(CFLAGS) -o puzzles puzzles-main.c puzzles-1.c puzzles-2.c

clean:
	rm -f $(PROGS) *.o *~ *.d *.so *.a *.s





