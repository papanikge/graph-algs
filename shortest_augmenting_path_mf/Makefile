#
# George 'papanikge' Papanikolaou
# CEID Advance Algorithm Desing Course 2014
# Boost & LEDA benchmarking
# This makefile has the required paths for LEDA hardcoded
#

CC=g++
WARN=-Wall -Wextra
FLAG=-fno-strict-aliasing
LEDA=/usr/local/LEDA
LIBS=-I$(LEDA)/incl -L$(LEDA) -L/usr/X11R6/lib64/
LFLAGS=-lleda -lX11 -lm

all: main.o aux.o
	$(CC) $? -o run_this $(LIBS) $(LFLAGS)

main.o: main.cc boost-types.h
	$(CC) $(WARN) $(FLAG) -O2 -o $@ -c $< $(LIBS) $(LFLAGS)

aux.o: aux.cc boost-types.h
	$(CC) $(WARN) $(FLAG) -O2 -o $@ -c $< $(LIBS) $(LFLAGS)

# Boost-only file so we don't need the LEDA dependencies
sap.o: sap.cc boost-types.h
	$(CC) $(WARN) $(FLAG) -O3 -o $@ -c $<

clean:
	rm -f run_this a.out *.o

.PHONY: clean
