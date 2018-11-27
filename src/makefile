# Luis Maya
# CS789
# Makefile for linkedList


OBJS 	= linktest.o
CC		= g++ -Wall -pedantic -g -std=c++11 -pthread
DEPS	= coarseList.h fineList.h optimisticList.h lazyList.h

all: linktest

linktest: linktest.cpp $(OBJS)
	$(CC) -o linktest $(OBJS)

linktest.o: linktest.cpp $(DEPS)
	$(CC) -c linktest.cpp

# -----
# clean by removing object files.

clean:
	rm	$(OBJS) linktest
