# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CC     = g++

objects = console.o 

# to make plugins easy we always do a clean build

all: clean console

console: $(objects)
	$(CC)  $(CPPFLAGS)  -shared -o console.so $(objects)

console.o : console.cpp console.h 

clean:
	rm -f console.so $(objects)
