#
# Makefile for the malloc lab driver
#
# Regular compiler
CC = gcc
# Compiler for mm.c
CLANG = clang
LLVM_PATH = /usr/local/depot/llvm-7.0/bin/

# Additional flags used to compile mdriver-dbg
# You can edit these freely to change how your debug binary compiles.
COPT_DBG = -Og
CFLAGS_DBG = -DDEBUG=1

# Flags used to compile normally
COPT = -O3
CFLAGS = -Wall -Wextra -Werror $(COPT) -g -DDRIVER -Wno-unused-function -Wno-unused-parameter

# Build configuration
FILES = mdriver mdriver-dbg mdriver-2 mdriver-2-dbg
LDLIBS = -lm -lrt
COBJS = memlib.o fcyc.o clock.o stree.o
MDRIVER_HEADERS = fcyc.h clock.h memlib.h config.h mm.h stree.h

MC = ./macro-check.pl
MCHECK = $(MC) -i dbg_

# Default rule
all: $(FILES)

# Regular driver
mdriver: mdriver.o mm-native.o $(COBJS)
	$(CC) -o $@ $^ $(LDLIBS)

mdriver-2: mdriver.o mm-2-native.o $(COBJS)
	$(CC) -o $@ $^ $(LDLIBS)

# Debug driver
mdriver-dbg: COPT = $(COPT_DBG)
mdriver-dbg: CFLAGS += $(CFLAGS_DBG)
mdriver-dbg: mdriver.o mm-native-dbg.o $(COBJS)
	$(CC) -o $@ $^ $(LDLIBS)

mdriver-2-dbg: COPT = $(COPT_DBG)
mdriver-2-dbg: CFLAGS += $(CFLAGS_DBG)
mdriver-2-dbg: mdriver.o mm-2-native-dbg.o $(COBJS)
	$(CC) -o $@ $^ $(LDLIBS)

mm-native.o: mm.c mm.h memlib.h $(MC)
	$(MCHECK) -f $<
	$(LLVM_PATH)$(CLANG) $(CFLAGS) -c -o $@ $<

mm-2-native.o: mm-2.c mm.h memlib.h $(MC)
	$(MCHECK) -f $<
	$(LLVM_PATH)$(CLANG) $(CFLAGS) -c -o $@ $<

mm-native-dbg.o: mm.c mm.h memlib.h $(MC)
	$(LLVM_PATH)$(CLANG) $(CFLAGS) -c -o $@ $<

mm-2-native-dbg.o: mm-2.c mm.h memlib.h $(MC)
	$(LLVM_PATH)$(CLANG) $(CFLAGS) -c -o $@ $<

mdriver.o: mdriver.c $(MDRIVER_HEADERS)
memlib.o: memlib.c memlib.h
mm.o: mm.c mm.h memlib.h
fcyc.o: fcyc.c fcyc.h
ftimer.o: ftimer.c ftimer.h config.h
clock.o: clock.c clock.h
stree.o: stree.c stree.h

clean:
	rm -f *~ *.o *.bc *.ll
	rm -f $(FILES)

.PHONY: all clean
