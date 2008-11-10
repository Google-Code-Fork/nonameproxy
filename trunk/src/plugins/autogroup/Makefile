# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CC     = g++

objects = autogroup.o 

# to make plugins easy we always do a clean build

all: clean autogroup

autogroup: $(objects)
	$(CC)  $(CPPFLAGS)  -shared -o autogroup.so $(objects)

autogroup.o : autogroup.cpp autogroup.h 

clean:
	rm -f autogroup.so $(objects)
