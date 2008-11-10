# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CC     = g++

objects = info.o 

# to make plugins easy we always do a clean build

all: clean info

info: $(objects)
	$(CC)  $(CPPFLAGS)  -shared -o info.so $(objects)

info.o : info.cpp info.h 

clean:
	rm -f info.so $(objects)
