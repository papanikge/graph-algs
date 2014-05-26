#
# George 'papanikge' Papanikolaou
# CEID Advance Algorithm Desing Course 2014
# Boost & LEDA benchmarking
# This makefile has the required paths for LEDA hardcoded
#

CC=g++
WARN=-Wall -Wextra -g -Wno-deprecated
FLAG=-fno-strict-aliasing
LEDA=/usr/local/LEDA
LIBS=-I$(LEDA)/incl -L$(LEDA) -L/usr/X11R6/lib64/
LFLAGS=-lleda -lX11 -lm

all: main.o kruskal.o graph.o
	$(CC) $? -o run_this $(LIBS) $(LFLAGS)

main.o: main.cc
	$(CC) $(WARN) $(FLAG) -O2 -o $@ -c $< $(LIBS) $(LFLAGS)

kruskal.o: kruskal.cc
	$(CC) $(WARN) $(FLAG) -O3 -o $@ -c $<

graph.o: graph.cc
	$(CC) $(WARN) $(FLAG) -O2 -o $@ -c $< $(LIBS) $(LFLAGS)

clean:
	rm -f run_this a.out *.o

.PHONY: clean
