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

all: leda.o
	$(CC) $? -o run_this $(LIBS) $(LFLAGS)

leda.o: leda.cc
	$(CC) $(WARN) $(OPT) -o $@ -c $< $(LIBS) $(LFLAGS)

clean:
	rm -f run_this a.out *.o

.PHONY: clean
