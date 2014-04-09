#
# George 'papanikge' Papanikolaou
# CEID Advance Algorithm Desing Course 2014
# Library of Efficient Data types and Algorithms (LEDA) testing
#

CC=g++
WARN=-Wall -Wextra -g
OPT=-O2 -fno-strict-aliasing
LEDA=/usr/local/LEDA
LIBS=-I$(LEDA)/incl -L$(LEDA) -L/usr/X11R6/lib64/
LFLAGS=-lleda -lX11 -lm

all: main.o strong_comp.o
	$(CC) $? -o run_this $(LIBS) $(LFLAGS)

main.o: main.cc basic.h
	$(CC) $(WARN) $(OPT) -o $@ -c $< $(LIBS) $(LFLAGS)

strong_comp.o: strong_comp.cc basic.h
	$(CC) $(WARN) $(OPT) -o $@ -c $< $(LIBS) $(LFLAGS)

clean:
	rm -f run_this a.out *.o

.PHONY: clean
