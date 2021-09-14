# Luis Maya
# CS789
# Makefile for linkedList


OBJS 	= heavytest.o
CC		= g++ -Wall -pedantic -g -std=c++11 -pthread
DEPS	= CoarseGrainedLL.h FineGrainedLL.h OptimisticLL.h LazyLL.h LockFreeLL.h

all: heavytest

heavytest: heavytest.cpp $(OBJS)
	$(CC) -o heavytest $(OBJS)

heavytest.o: heavytest.cpp $(DEPS)
	$(CC) -c heavytest.cpp

# -----
# clean by removing object files.

clean:
	rm	$(OBJS) heavytest
