# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CC     = g++

objects = map.o localmap.o

# to make plugins easy we always do a clean build

all: clean map

map: $(objects)
	$(CC)  $(CPPFLAGS)  -shared -o map.so $(objects)

map.o : map.cpp map.h 

localmap.o : localmap.cpp localmap.h map.h

clean:
	rm -f map.so $(objects)
