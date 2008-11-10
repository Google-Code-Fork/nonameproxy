# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CC     = g++

objects = test.o 

# to make plugins easy we always do a clean build

all: clean test

test: $(objects)
	$(CC)  $(CPPFLAGS)  -shared -o test.so $(objects)

test.o : test.cpp test.h 

clean:
	rm -f test.so $(objects)
